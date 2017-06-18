#ifndef Enum_h__
#define Enum_h__

namespace GAMEOBJECTID
{
	enum UI
	{
		UI_PLAYUNIT = 0,
		UI_CROSSHAIR,
		UI_SKYDORM,
		UI_PARTICLE,
	};

	enum CUBE
	{
		CUBE_WHITE = 10,
		CUBE_GREEN,
		CUBE_PURPLE,
		CUBE_ORANGE,
		CUBE_BLUE,
		CUBE_RED,
		CUBE_YELLOW,
		CUBE_GRAY,
		CUBE_WALL,
		CUBE_END
	};

	enum ENVIRONMENT
	{
		ENV_SCENERY = 100,
	};
};

namespace RESOURCEID
{
	enum STATICMESH
	{
		STATICMESH_CUBE_WHITE = 3000,
		STATICMESH_CUBE_GREEN,
		STATICMESH_CUBE_PURPLE,
		STATICMESH_CUBE_ORANGE,
		STATICMESH_CUBE_BLUE,
		STATICMESH_CUBE_RED,
		STATICMESH_CUBE_YELLOW,
		STATICMESH_CUBE_GRAY,
		STATICMESH_CUBE_WALL,
		STATICMESH_SKYDORM,
	};

	enum DYNAMICMESH
	{
		DYNAMICMESH_GLOVE = 4000,
	};

	enum TEXBUFFER
	{
		TEXBUFFER_RECTANGLE = 5000,
	};

	enum COLBUFFER
	{
		COLBUFFER_SPHERE = 6000,
	};

	enum TEXTURE
	{
		RECTTEXTURE_LOGO = 9000,
		RECTTEXTURE_CROSSHAIR,
		RECTTEXTURE_PICKINGPARTICLE,	
	};
}


// TODO: def. ComponentID. Reference names
	//enum COMPONENTID 
	//{ 
	//	// 이동 관련
	//	TRANSFORM, 

	//	// 충돌 관련
	//	COLLISION_RAY,

	//	// 버퍼 관련
	//	TEXBUFFER_RECTANGLE,
	//	COLBUFFER_CUBE,
	//	COLBUFFER_SPHERE,

	//	// 텍스처 관련
	//	TEXTURE_RECTANGLE,

	//	// 메시 관련
	//	MESH_STATIC,
	//	MESH_DYNAMIC,

	//	COMPONENTID_END
	//};



#endif // Enum_h__
