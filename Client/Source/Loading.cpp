#include "stdafx.h"
#include "Loading.h"

#include <process.h> // for using Thread

#include "Shlwapi.h" // for using PathRemoveFileSpecW
#pragma comment(lib, "Shlwapi.lib")

#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Include.h"
#include "Engine_Struct.h"
#include "Enum.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Layer.h"

// Cube 
#include "WhiteCube.h"
#include "GreenCube.h"
#include "PurpleCube.h"
#include "OrangeCube.h"
#include "BlueCube.h"
#include "RedCube.h"
#include "YellowCube.h"
#include "GrayCube.h"
#include "CubeWall.h"

CLoading::CLoading(void)
: m_pDevice(NULL)
, m_hThread(NULL)
, m_bComplete(false)
, m_pCubeLayer(NULL)
{
	ZeroMemory(&m_CSKey, sizeof(CRITICAL_SECTION));
	ZeroMemory(m_szLoadingMessage, sizeof(TCHAR) * 128);
}

CLoading::~CLoading(void)
{
	Release();
}

CLoading* CLoading::Create(void)
{
	CLoading* pLoad = new CLoading;

	if(FAILED(pLoad->InitLoading()))
		Engine::Safe_Delete(pLoad);

	return pLoad;
}

HRESULT CLoading::InitLoading(void)
{
	InitializeCriticalSection(&m_CSKey);

	m_pDevice = Engine::Get_GraphicDev()->GetDevice();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, LoadingFunction, this, 0, NULL);

	if(m_hThread == NULL)
		return E_FAIL;

	return S_OK;
}


void CLoading::Release(void)
{
	DeleteCriticalSection(&m_CSKey);
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
}


UINT WINAPI CLoading::LoadingFunction(void* pArg)
{   
	CLoading*		pLoading = (CLoading*)pArg;

	// TODO: 스레드가 증가할 경우 키를 사용할 것
	//EnterCriticalSection(&pLoading->m_CSKey);

	pLoading->LoadResources();

	//LeaveCriticalSection(&pLoading->m_CSKey);

	return 0;
}

void CLoading::LoadResources(void)
{
	HRESULT hr = NULL;

	//  Loading Buffer
	lstrcpy(m_szLoadingMessage, L"Loading Buffer...");

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice
		, Engine::BUFFER_SPHERECOL
		, RESOURCEID::COLBUFFER_SPHERE
		, CCube::CUBERADIUS * 1.73f);
	FAILED_CHECK_RETURN_MSG(hr, , L"Sphere");

	// Texture
	lstrcpy(m_szLoadingMessage, L"Loading Texture...");
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, Engine::TEXTURE_NORMAL
		, RESOURCEID::RECTTEXTURE_CROSSHAIR
		, L"../../Data/Resources/Texture/Crosshair.tga"
		, 1);		
	FAILED_CHECK_RETURN_MSG(hr,  , L"Failed to create Logo.tga");

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, Engine::TEXTURE_NORMAL
		, RESOURCEID::RECTTEXTURE_PICKINGPARTICLE
		, L"../../Data/Resources/Texture/PickingParticle/Picking_%d.tga"
		, 19);		
	FAILED_CHECK_RETURN_MSG(hr,  , L"Failed to create Logo.tga");

	// Static Mesh
	lstrcpy(m_szLoadingMessage, L"Loading Static Mesh...");

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_WHITE
		, L"../../Data/Resources/Mesh/StaticMesh/Cube/WhiteCube/"
		, L"WhiteCube.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_WHITE")

		/*
		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_GREEN
		, L"../../Data/Resources/Mesh/StaticMesh/Cube/GreenCube/"
		, L"GreenCube.X");		
		FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_GREEN")*/


		/* = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_PURPLE
		, L"../../Data/Resources/Mesh/StaticMesh/Cube/PurpleCube/"
		, L"PurpleCube.X");		
		FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_PURPLE")*/

		/*hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_ORANGE
		, L"../../Data/Resources/Mesh/StaticMesh/Cube/OrangeCube/"
		, L"OrangeCube.X");		
		FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_ORANGE")*/

		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_BLUE
		, L"../../Data/Resources/Mesh/StaticMesh/Cube/BlueCube/"
		, L"BlueCube.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_BLUE")

		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_RED	
		, L"../../Data/Resources/Mesh/StaticMesh/Cube/RedCube/"
		, L"RedCube.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_RED")

		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_YELLOW
		, L"../../Data/Resources/Mesh/StaticMesh/Cube/YellowCube/"
		, L"YellowCube.X");		
		FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_YELLOW")

		/*hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_YELLOW
		, L"../../Data/Resources/Mesh/StaticMesh/Cube/GrayCube/"
		, L"GrayCube.X");		
		FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_GRAY)
		*/

		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_WALL
		, L"../../Data/Resources/Mesh/StaticMesh/Cube/CubeWall/"
		, L"CubeWall.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_WALL")

		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_SKYDORM
		, L"../../Data/Resources/Mesh/StaticMesh/SkyDorm/"
		, L"SkyDorm.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_SKYDORM")

		// Create Cube
		lstrcpy(m_szLoadingMessage, L"Creating Cube Object...");

	TCHAR szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);
	SetCurrentDirectory(szDirPath);
	PathRemoveFileSpecW(szDirPath);
	PathRemoveFileSpecW(szDirPath);

	TCHAR szCubeDataDirPath[MAX_PATH] = L"";
	lstrcpy(szCubeDataDirPath, szDirPath);
	lstrcat(szCubeDataDirPath, L"\\Data\\Cube.dat");

	HANDLE	hFile = CreateFile(szCubeDataDirPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwByte = 0;

	typedef std::vector<Engine::CGameObject*>	OBJECTLIST;
	typedef std::map<DWORD, OBJECTLIST>	OBJECTLISTMAP;
	OBJECTLISTMAP mapObjectlist;
	Engine::CUBEDATA CubeData;
	LPDIRECT3DDEVICE9 pDevice = Engine::Get_GraphicDev()->GetDevice();

	while (true)
	{   
		// 경로 설정 수정하면 됨 

		ZeroMemory(&CubeData, sizeof(Engine::CUBEDATA));
		ReadFile(hFile, &CubeData, sizeof(Engine::CUBEDATA), &dwByte, NULL);

		if(dwByte == 0)
			break;

		CCube* pCube = NULL;

		switch(CubeData.dwCubeType)
		{
		case GAMEOBJECTID::CUBE_WHITE:
			pCube = CWhiteCube::Create(pDevice, &CubeData);
			break;

			/*case GAMEOBJECTID::CUBE_GREEN:
			pCube = CGreenCube::Create(pDevice, CubeData);
			break;
			break;*/

			/*case GAMEOBJECTID::CUBE_PURPLE:
			pCube = CPurpleCube::Create(pDevice, CubeData);
			break;*/

			/*case GAMEOBJECTID::CUBE_ORANGE:
			pCube = COrangeCube::Create(pDevice, CubeData);
			break;*/

		case GAMEOBJECTID::CUBE_BLUE:
			pCube = CBlueCube::Create(pDevice, &CubeData);
			break;

		case GAMEOBJECTID::CUBE_RED:
			pCube = CRedCube::Create(pDevice, &CubeData);
			break;

		case GAMEOBJECTID::CUBE_YELLOW:
			pCube = CYellowCube::Create(pDevice, &CubeData);
			break;

			/*case GAMEOBJECTID::CUBE_GRAY:
			pCube = CRedCube::Create(pDevice, CubeData);
			break;*/

		case GAMEOBJECTID::CUBE_WALL:
			pCube = CCubeWall::Create(pDevice, &CubeData);
			break;
		}

		if(mapObjectlist.count(CubeData.dwCubeType) == NULL)
			mapObjectlist[CubeData.dwCubeType] = OBJECTLIST();

		Engine::CGameObject* pGameObject = static_cast<Engine::CGameObject*>(pCube);
		NULL_CHECK_MSG(pGameObject, L"Failed to load an object");

		mapObjectlist[CubeData.dwCubeType].push_back(pGameObject);

	} // while

	CloseHandle(hFile);

	m_pCubeLayer = Engine::CLayer::Create(pDevice);

	m_pCubeLayer->SetMapObjectlist(&mapObjectlist);

	// NavMesh
	lstrcpy(m_szLoadingMessage, L"Loading NavMesh...");

	TCHAR szNavMeshDirPath[MAX_PATH] = L"";
	lstrcpy(szNavMeshDirPath, szDirPath);
	lstrcat(szNavMeshDirPath, L"\\Data\\NavMesh.dat");

	hFile = CreateFile(szNavMeshDirPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	Engine::CNavMgr* pNavMgr = Engine::Get_NavMgr();

	dwByte = 0;

	D3DXVECTOR3 vNavPoint[3];

	while (true)
	{
		ZeroMemory(vNavPoint, sizeof(D3DXVECTOR3) * 3);
		ReadFile(hFile, vNavPoint, sizeof(D3DXVECTOR3) * 3, &dwByte, NULL);

		if(dwByte == 0)
			break;

		pNavMgr->AddCell(vNavPoint, vNavPoint + 1, vNavPoint + 2);
	}

	pNavMgr->LinkCell();

	CloseHandle(hFile);

	// Dynamic Mesh
	lstrcpy(m_szLoadingMessage, L"Loading DynamicMesh...");

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
	, Engine::MESH_DYNAMIC
	, RESOURCEID::DYNAMICMESH_GLOVE
	, L"../../Data/Resources/Mesh/DynamicMesh/"
	, L"Glove.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"DYNAMICMESH_GLOVE")

	lstrcpy(m_szLoadingMessage, L"Loading Complete");


	m_bComplete = true;
}

const TCHAR* CLoading::GetLoadingMessage(void)
{
	return m_szLoadingMessage;
}

bool CLoading::GetComplete(void)
{
	return m_bComplete;
}

Engine::CLayer* CLoading::GetCubeLayer(void)
{
	return m_pCubeLayer;
}
