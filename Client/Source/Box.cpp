#include "stdafx.h"
#include "Box.h"

#include "CollisionManager.h"
#include "CubeColor.h"
#include "DynamicMesh.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Enum.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "JohnStateSubject.h"
#include "Layer.h"
#include "PhysXSDK.h"
#include "StaticMesh.h"
#include "Texture.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Value.h"

#include "Cube.h"
#include "WhiteCube.h"


#include "DebugRenderer.h"

CBox::CBox(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pDynamicMesh(NULL)
, m_pState(NULL)
, m_pInput(NULL)
, m_pLayer(NULL)
, m_pNavMgr(NULL)
, m_pRenderer(NULL)
, m_pResourceMgr(NULL)
, m_pTimeMgr(NULL)

// 이동 관련
, m_dwIndex(0)		// NavCell Index

//----------------------------
// 크로스 헤어 관련
//----------------------------
, m_fSizeX(16.f)
, m_fSizeY(16.f)
, m_vCrossHair(float(WINCX << 1), float(WINCY << 1))
, m_pCrossHairTex(NULL)
, m_pCrossHairBuffer(NULL)


//----------------------------
// 테스트 
//----------------------------
, m_pSphere(NULL)
, m_fVelocity(0.f)
, m_IsPosChanged(false)
, m_pCollisionManager(NULL)
{

}

CBox::~CBox(void)
{
	Release();
}

CBox* CBox::Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3& vPos)
{
	CBox* pJohn = new CBox(pDevice);
	if(FAILED(pJohn->InitBox(vPos)))
		Safe_Delete(pJohn);

	return pJohn;
}

HRESULT CBox::InitBox(const D3DXVECTOR3& vPos)
{
	m_pInput = Engine::Get_Input();
	m_pNavMgr = Engine::Get_NavMgr();
	m_pTimeMgr = Engine::Get_TimeMgr();
	m_pCollisionManager = CCollisionManager::GetInstance();
		
	m_pInput = Engine::Get_Input();   
	m_pResourceMgr = Engine::Get_ResourceMgr();

	FAILED_CHECK(AddComponent());
	
	m_pState->m_vPos = vPos;

	m_pNxScene = CPhysXSDK::GetInstance()->GetNxScene();

	

	m_pBoxActor = CreateBox();


	//// Actor를만들고 
	//NxActorDesc actorDesc;
	//actorDesc.setToDefault();

	//// Plane을 만든다. 
	//NxPlaneShapeDesc planeDesc;
	//planeDesc.setToDefault();

	//// 원점으로부터의 거리
	//// 이 값과 법선벡터(다른 곳에 있는 멤버변수에 의해 지면의 위치가 결정된다.
	//planeDesc.d = -5.f;

	//actorDesc.shapes.pushBack(&planeDesc);

	//// 좌표관련
	//NxMaterial* defaultMaterial = m_pNxScene->getMaterialFromIndex(0);

	//// 반발계수(공을 바운드 시킨다.)
	//defaultMaterial->setRestitution(0.1f);

	//// Scene에 대해 지정 액터를 추가.
	//m_pNxScene->createActor(actorDesc);

	// -----------------------------------
	// 동적 충돌 판정용 오브젝트 생성
	// -----------------------------------

	NxActorDesc actorDesc;
	actorDesc.setToDefault();

	// Set the sphere starting height to 3.5m so box starts off falling onto the ground
	NxReal sphereStartHeight = 3.5; 

	// Add a single-shape actor to the scene

	NxBodyDesc bodyDesc;
	bodyDesc.setToDefault();

	// The actor has one shape, a sphere, 1m on radius
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius		= 20.f;
	sphereDesc.localPose.t	= NxVec3(0, 0, 0);

	actorDesc.shapes.pushBack(&sphereDesc);
	actorDesc.body			= &bodyDesc;
	actorDesc.density		= 10.0f;
	actorDesc.globalPose.t	= NxVec3(0,sphereStartHeight,0);	
	m_pNxScene->createActor(actorDesc);

	ShowCursor(false);

	return S_OK;
}

HRESULT CBox::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pState = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TRANSFORM, pComponent));

	// Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::STATICMESH_CUBE_WHITE);
	m_pStaticMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(MESH_STATIC, pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::TEXBUFFER_RECTANGLE);
	m_pCrossHairBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TEXBUFFER_CROSSHAIR, pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::RECTTEXTURE_CROSSHAIR);
	m_pCrossHairTex = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(RECTTEXTURE_CROSSHAIR, pComponent));
	

	return S_OK;
}

void CBox::Update(void)
{
	UpdateUnitMove();

	NxVec3 boxPos = m_pBoxActor->getCMassGlobalPosition();

	D3DXVECTOR3 vPos(m_pBoxActor->getGlobalPose().t.x, m_pBoxActor->getGlobalPose().t.y, m_pBoxActor->getGlobalPose().t.z);
	NxF32 matrix[3 * 3];

	m_pBoxActor->getGlobalPose().M.getColumnMajor(matrix);
	
	D3DXMatrixIdentity(&m_matWorld);

	memcpy(&m_matWorld.m[0], &matrix[0], sizeof(D3DXVECTOR3));
	memcpy(&m_matWorld.m[1], &matrix[3], sizeof(D3DXVECTOR3));
	memcpy(&m_matWorld.m[2], &matrix[6], sizeof(D3DXVECTOR3));
	memcpy(&m_matWorld.m[3], &vPos, sizeof(D3DXVECTOR3));

	
//		m_pState->m_vPos = vPos;

		//m_pState->m_vScale = D3DXVECTOR3(CCube::CUBESCALE, CCube::CUBESCALE, CCube::CUBESCALE);
	

	m_pNxScene->simulate(Engine::Get_TimeMgr()->GetTime());
	m_pNxScene->flushStream();
	m_pNxScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	
	Engine::CGameObject::Update();
}	

void CBox::Render(void)
{
	float fX, fY, fZ;

	fX = m_pState->m_vPos.x;
	fY = m_pState->m_vPos.y;
	fZ = m_pState->m_vPos.z;

	_stprintf_s(m_szMessage, L"Box: %.2f, %.2f, %.2f", fX, fY, fZ); // 선택된 큐브 위치
	Engine::Get_FontMgr()->Render(L"Cambria", D3DXVECTOR3(0.f, 48.f, 0.f), m_szMessage, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//m_pDynamicMesh->RenderMesh(&m_matWorld);

//	m_pStaticMesh->RenderMesh(&m_matWorld);

	CDebugRenderer* pDebugRenderer = CPhysXSDK::GetInstance()->GetDebugRenderer();
	pDebugRenderer->RenderData(m_pDevice, m_pNxScene->getDebugRenderable());
}

void CBox::Release(void)
{
	
}

void CBox::UpdateUnitMove(void)
{
	float fTime = m_pTimeMgr->GetTime();

	if(m_pInput->GetDIKeyState(DIK_I))
	{
		m_ForceVec = ApplyForceToActor(m_pBoxActor, NxVec3(0, 0, 1), g_ForceStrength);
		
		m_IsPosChanged = true;
	}

	if(m_pInput->GetDIKeyState(DIK_K))
	{
	
		m_ForceVec = ApplyForceToActor(m_pBoxActor, NxVec3(0, 0, -1), g_ForceStrength);
		m_IsPosChanged = true;
	}

	if(m_pInput->GetDIKeyState(DIK_J))
	{
		
		m_ForceVec = ApplyForceToActor(m_pBoxActor, NxVec3(-1, 0, 0), g_ForceStrength);
		m_IsPosChanged = true;
	}

	if(m_pInput->GetDIKeyState(DIK_L))
	{
		
	m_ForceVec = ApplyForceToActor(m_pBoxActor, NxVec3(1, 0, 0), g_ForceStrength);

		m_IsPosChanged = true;
	}

	if(m_pInput->GetDIKeyState(DIK_P))
	{

		m_pBoxActor->setGlobalPosition(NxVec3(0, 5, 0));
		m_pNxScene->flushStream();

		m_IsPosChanged = true;
	}
		

	/// 높이 보정
	if(m_IsPosChanged)
	{
		//m_pCollisionManager->IsRayIntersectedMeshtoReviseHeight();

		D3DXVECTOR3 vForce(m_ForceVec.x, m_ForceVec.y, m_ForceVec.z);

		m_pState->m_vPos += vForce;
		
		m_IsPosChanged = false;
	}
}
	

NxActor* CBox::CreateBox(void)
{
	// Set the box starting height to 3.5m so box starts off falling onto the ground
	NxReal boxStartHeight = 3.5; 

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	bodyDesc.setToDefault();
	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(10, 10, 10);
	actorDesc.shapes.pushBack(&boxDesc);

	actorDesc.body		= &bodyDesc;
	actorDesc.density 	= 10.0f;
	actorDesc.globalPose.t 	= NxVec3(0,boxStartHeight,0);
	assert(actorDesc.isValid());
	NxActor *pActor = m_pNxScene->createActor(actorDesc);	
	assert(pActor);

	// //create actor with no shapes
	//NxShape* const *shape = pActor->getShapes();
	//NxBoxShape *boxShape = shape[0]->isBox();
	//assert(boxShape);
	//pActor->releaseShape(*boxShape);

	return pActor;

}

NxVec3 CBox::ApplyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();
	NxReal deltaTime = fTime;

	NxVec3 forceVec = forceStrength * forceDir * deltaTime * 1000;
	actor->addForce(forceVec);
	return forceVec;
}
