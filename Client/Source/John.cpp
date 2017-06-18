#include "stdafx.h"
#include "John.h"

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
#include "Texture.h"
#include "Transform.h"
#include "SoundMgr.h"
#include "VIBuffer.h"
#include "Value.h"

CJohn::CJohn(LPDIRECT3DDEVICE9 pDevice)
: Engine::CCamera(pDevice)
, m_pDynamicMesh(NULL)
, m_pState(NULL)
, m_pInput(NULL)
, m_pLayer(NULL)
, m_pNavMgr(NULL)
, m_pRenderer(NULL)
, m_pResourceMgr(NULL)
, m_pTimeMgr(NULL)
, m_pJohnStateSubject(NULL)

// 이동 관련
, m_dwIndex(0)
, m_fSpeed(60.f)
, m_IsJumped(false)
, m_IsFlew(false)
, m_IsPosChanged(false)


, m_vVelocity(0.f, 0.f, 0.f)
, m_fVelocity(120.f)
, m_fReviseVelocity(120.f)
, m_fPtime(0.f)

// 점프 관련
, m_fJumpTime(0.f)
, m_fJumpVelocity(26.f)


// 충돌 관련
, m_pCollisionManager(NULL)

// 게임 시작 관련
, m_IsStart(true)
, m_iAniIdx(9)
, m_IsSHow(true)

{

}

CJohn::~CJohn(void)
{
	Release();
}

CJohn* CJohn::Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt)
{
	CJohn* pJohn = new CJohn(pDevice);
	if(FAILED(pJohn->InitJohn(vEye, vAt)))
		Safe_Delete(pJohn);

	return pJohn;
}

HRESULT CJohn::InitJohn(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt)
{
	m_pInput = Engine::Get_Input();
	m_pNavMgr = Engine::Get_NavMgr();
	m_pTimeMgr = Engine::Get_TimeMgr();
	m_pCollisionManager = CCollisionManager::GetInstance();

	
	m_vEye = vEye;
	m_vAt = vAt;
	Invalidate_ViewMatrix();

	m_fAspect = float(WINCX) / WINCY;
	m_fFovY = D3DXToRadian(60.f);
	m_fNear = 0.1f;
	m_fFar = 1000.f;
	Invalidate_ProjMatrix();

	m_pInput = Engine::Get_Input();   
	m_pResourceMgr = Engine::Get_ResourceMgr();

	FAILED_CHECK(AddComponent());

	m_pState->m_vScale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	m_pState->m_fAngle[Engine::ANGLE_Y] = 3.14f;
	m_pState->m_vPos = m_vEye;
	m_pState->m_vDir = m_vAt;
	m_pState->Update();

	m_StdVtx3._min = D3DXVECTOR3(-2.f, 0.f, -2.f);
	m_StdVtx3._max = D3DXVECTOR3(2.f, JOHNHEIGHT, 2.f);

	m_pJohnStateSubject = CJohnStateSubject::GetInstance();
	m_pJohnStateSubject->AddData(CJohnStateSubject::DATAMESSAGE_POS, CJohnStateSubject::DATATYPE_FLOAT, sizeof(D3DXVECTOR3), 100);

	ShowCursor(false);

	return S_OK;
}

HRESULT CJohn::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pState = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TRANSFORM, pComponent));

	// Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::DYNAMICMESH_GLOVE);
	m_pDynamicMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(MESH_DYNAMIC, pComponent));

	return S_OK;
}

void CJohn::Update(void)
{

	//if(m_pState->m_vPos.z > 220.f)
	//	m_pCollisionManager->Delete220();

	UpdateUnitMove();
	UpdateUnitRotate();

	

	UpdateMeshAnimation();

	// Collision
	if(m_pInput->GetDIMouseState(Engine::CInput::DIM_LBUTTON))
	{
		m_IsSHow = false;
		CSoundMgr::GetInstance()->PlaySkillSound(L"Turn.ogg");
		m_pCollisionManager->CollisionActiveCube();
	}

	if(m_pInput->GetDIMouseState(Engine::CInput::DIM_RBUTTON))
	{
		CSoundMgr::GetInstance()->PlaySkillSound(L"Turn.ogg");
	
		
		m_pCollisionManager->CollisionDeactiveCube();
	}
	m_pState->m_vPos = m_vEye;


	Engine::CCamera::Update();

	Engine::CGameObject::Update();

	D3DXMATRIX matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	if(m_pCollisionManager->CollisionWithBlueCube(m_StdVtx3, m_pState->m_matWorld, &m_pState->m_vDir))
	{	
		TCHAR szName[512];
		int iTrack = (rand() % 5) + 1;
		wsprintf(szName, L"BlueJump_0%d.ogg", iTrack);
		CSoundMgr::GetInstance()->PlaySkillSound(szName);
		memcpy(&m_vOriginPos, &matWorld.m[3][0], sizeof(D3DXVECTOR3));
		m_vParabolaDir = m_pState->m_vDir;
		D3DXVec3Normalize(&m_vParabolaDir, &m_vParabolaDir);
		m_fPtime = 0.f;
		m_fCacheParabolaHeight = 0.f;   
		m_IsFlew = true;
	}

	m_pJohnStateSubject->NotifyJohnState(CJohnStateSubject::DATAMESSAGE_POS, (void*)&m_pState->m_vPos, sizeof(D3DXVECTOR3));

}

void CJohn::Render(void)
{
	//float fX, fY, fZ;

	//fX = m_pState->m_vPos.x;
	//fY = m_pState->m_vPos.y;
	//fZ = m_pState->m_vPos.z;

	//_stprintf_s(m_szMessage, L"John: %.2f, %.2f, %.2f", fX, fY, fZ); // 선택된 큐브 위치
	//Engine::Get_FontMgr()->Render(L"Cambria", D3DXVECTOR3(0.f, 24.f, 0.f), m_szMessage, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	if(m_IsSHow)
	{
		m_pDynamicMesh->FrameMove(Engine::Get_TimeMgr()->GetTime());
		m_pDynamicMesh->RenderMesh(&m_pState->m_matWorld);

	}
	
}

void CJohn::Release(void)
{
	
}

void CJohn::UpdateUnitMove(void)
{
	float fTime = m_pTimeMgr->GetTime();

	if(m_IsFlew)
	{
		UpdateParabolaMovement(fTime);
		return;
	}

	D3DXVECTOR3 vEye = m_vEye; 
	D3DXVECTOR3 vAt = m_vAt;

	if(m_pInput->GetDIKeyState(DIK_W))
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		// m_dwIndex = m_pNavMgr->MoveOnNavMesh(&vEye, &vAt, &(vLook  * m_fSpeed * fTime), m_dwIndex);

		vEye += vLook * m_fSpeed * fTime;
		vAt += vLook * m_fSpeed * fTime;

		m_IsPosChanged = true;
	}

	if(m_pInput->GetDIKeyState(DIK_S))
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		// m_dwIndex = m_pNavMgr->MoveOnNavMesh(&vEye, &vAt, &(vLook  * m_fSpeed * fTime * -1.f), m_dwIndex);

		vEye -= vLook * m_fSpeed * fTime;
		vAt -= vLook * m_fSpeed * fTime;


		m_IsPosChanged = true;
	}

	if(m_pInput->GetDIKeyState(DIK_A))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		//m_dwIndex = m_pNavMgr->MoveOnNavMesh(&vEye, &vAt, &(vRight  * m_fSpeed * fTime * -1.f), m_dwIndex);


		vEye -= vRight * m_fSpeed * fTime;
		vAt -= vRight * m_fSpeed * fTime;


		m_IsPosChanged = true;
	}

	if(m_pInput->GetDIKeyState(DIK_D))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		//m_dwIndex = m_pNavMgr->MoveOnNavMesh(&vEye, &vAt, &(vRight  * m_fSpeed * fTime), m_dwIndex);

		vEye += vRight * m_fSpeed * fTime;
		vAt += vRight * m_fSpeed * fTime;


		m_IsPosChanged = true;
	}

	if(m_pInput->GetDIKeyState(DIK_U))
	{
		m_iAniIdx = 10;
	}

	if(m_pInput->GetDIKeyState(DIK_SPACE))
	{
		if(!m_IsJumped)
		{
			D3DXMATRIX matWorld;
			D3DXMatrixInverse(&matWorld, NULL, &m_matView);
			memcpy(&m_vOriginPos, &matWorld.m[3][0], sizeof(D3DXVECTOR3));

			m_IsJumped = !m_IsJumped;
			m_fCacheJumpHeight = 0.f;
			m_fOriginHeight = vEye.y;
		}
	}

	if(m_IsJumped)
	{
		float fGravity = (0.5f * 14.f * m_fJumpTime * m_fJumpTime);
		vEye.y = (m_fJumpVelocity * m_fJumpTime * 1.f) + m_fOriginHeight - fGravity;
		m_fJumpVelocity -= fGravity * 0.02f;
		m_fJumpTime += fTime;

		if(m_pCollisionManager->CollisionWithBlueCube(m_StdVtx3, m_pState->m_matWorld, &m_pState->m_vDir))
		{
			TCHAR szName[512];
			int iTrack = (rand() % 5) + 1;
			wsprintf(szName, L"BlueJump_0%d.ogg", iTrack);
			CSoundMgr::GetInstance()->PlaySkillSound(szName);
			D3DXMATRIX matWorld;
			D3DXMatrixInverse(&matWorld, NULL, &m_matView);
			memcpy(&m_vOriginPos, &matWorld.m[3][0], sizeof(D3DXVECTOR3));

			D3DXVec3Normalize(&m_pState->m_vDir, &m_pState->m_vDir);
			m_vParabolaDir = m_pState->m_vDir;
			
			m_fCacheParabolaHeight = 0.f;
			m_IsFlew = true;
			m_IsJumped = !m_IsJumped;
			m_fJumpVelocity = 26.f;
			m_fJumpTime = 0.f;
			return;
		}

		float fResivedPoint = 3.f;

		if(m_fOriginHeight + fResivedPoint > vEye.y + fResivedPoint) 
		{
			 vEye.y += 7.f;
			if(m_pCollisionManager->IsRayIntersectedMeshtoReviseHeight(&vEye))
			{
				D3DXMATRIX matWorld;
				D3DXMatrixInverse(&matWorld, NULL, &m_matView);
				memcpy(&m_vOriginPos, &matWorld.m[3][0], sizeof(D3DXVECTOR3));

				m_IsJumped = !m_IsJumped;
				m_fJumpVelocity = 26.f;
				m_fJumpTime = 0.f;
				m_fCacheJumpHeight = 0.f;
				vEye.y += m_fCacheJumpHeight + 7.f;
				return;
			}
		}
		m_fCacheJumpHeight = vEye.y;

		if(0.f + fResivedPoint > vEye.y + fResivedPoint)
		{
			m_IsJumped = !m_IsJumped;
			m_fJumpVelocity = 26.f;
			m_fJumpTime = 0.f;
			m_fCacheJumpHeight = 0.f;
			vEye.y += m_fCacheJumpHeight + 7.f;
			return;
		}
	}
	else
	{
		if(m_IsPosChanged)
		{
			ReviseUnitPos(&vEye);
		}
	}
	m_vEye = vEye;
	m_vAt = vAt;

}

void CJohn::UpdateUnitRotate(void)
{
	int		iDistance = 0;



	if(iDistance = m_pInput->GetDIMouseMove(Engine::CInput::DIM_X))
	{
		D3DXMATRIX		matAxis;
		float fAngle = D3DXToRadian(iDistance * 0.1f);
		m_pState->m_fAngle[Engine::ANGLE_Y] += fAngle;
		D3DXMatrixRotationAxis(&matAxis, &m_vUp, fAngle);

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;

		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}

	if(iDistance = m_pInput->GetDIMouseMove(Engine::CInput::DIM_Y))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXMATRIX		matAxis;
		float fAngle = D3DXToRadian(iDistance * 0.1f);
		m_pState->m_fAngle[Engine::ANGLE_X] += -fAngle;

		// 마우스 상하 회전 막음
		if(m_pState->m_fAngle[Engine::ANGLE_X] < -1.5f || m_pState->m_fAngle[Engine::ANGLE_X] > 1.5f)
		{
			m_pState->m_fAngle[Engine::ANGLE_X] += fAngle;
			return;
		}

		D3DXMatrixRotationAxis(&matAxis, &vRight, fAngle);

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;

		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}
}

void CJohn::ReviseUnitPos(D3DXVECTOR3* pEye)
{
	/// 높이 보정
	if(m_IsPosChanged && !m_IsJumped && !m_IsFlew)
	{
	//	TCHAR szName[512];
	//	int iTrack = (rand() % 5) + 1;
	//	wsprintf(szName, L"Footsteps_Metal0%d.ogg", iTrack);
	//	CSoundMgr::GetInstance()->PlaySkillSound(szName);

		m_pCollisionManager->IsRayIntersectedMeshtoReviseHeight(pEye);

		m_IsPosChanged = false;
	}  
}



void CJohn::UpdateParabolaMovement(const float& fTime)
{
	m_fPtime += fTime;

	float fGravity = (0.5f * 9.8f * m_fPtime * m_fPtime);

	m_fCacheParabolaHeight = m_vEye.y;

	m_vEye.x = (m_fVelocity * m_vParabolaDir.x * m_fPtime) + m_vOriginPos.x;
	m_vEye.y = (m_fReviseVelocity * m_vParabolaDir.y * m_fPtime) + m_vOriginPos.y - fGravity;
	m_vEye.z = (m_fVelocity * m_vParabolaDir.z * m_fPtime) + m_vOriginPos.z;

	m_fReviseVelocity -= fGravity * 0.02f;
	


	if(m_fCacheParabolaHeight > m_vEye.y && m_pCollisionManager->IsRayIntersectedMeshtoReviseHeight(&m_vEye))
	{
		D3DXMATRIX matWorld;
		D3DXMatrixInverse(&matWorld, NULL, &m_matView);
		memcpy(&m_vOriginPos, &matWorld.m[3][0], sizeof(D3DXVECTOR3));

		m_fVelocity = m_fReviseVelocity = 120.f;
		m_IsFlew = false;
		m_fPtime = 0.f;
		ReviseUnitPos(&m_vEye);
		m_fCacheParabolaHeight = 0.f;
		return;
	}

		/*if(m_vEye.y < m_vOriginPos.y)
	{
		m_fVelocity = m_fReviseVelocity = 120.f;
		m_IsFlew = false;
		m_vOriginPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_fPtime = 0.f;

		m_vEye.y += 18.f;
		ReviseUnitPos(&m_vEye);
		m_vEye.z += 1.f;
		m_fCacheParabolaHeight = 0.f;
		return;
	}*/

}

void CJohn::UpdateMeshAnimation(void)
{
	if(m_pDynamicMesh->GetPeriod() <= m_pDynamicMesh->GetTrackPosition())
	{	
		m_pDynamicMesh->SetAnimationSet(m_iAniIdx, 0.5);
		m_pDynamicMesh->SetTrackPos(0.0);
	}
	else
	{
		if(m_IsStart)
		{
			m_IsStart = !m_IsStart;
			m_iAniIdx = 16;
		}
		
	}
}