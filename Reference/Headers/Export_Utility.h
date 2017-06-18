/*!
 * \file Export_Utility.h
 * \date 2015/04/03 1:44
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Frustum.h"
#include "LightMgr.h"
#include "Management.h"
#include "NavMgr.h"
#include "QuadTree.h"
#include "Renderer.h"


BEGIN(Engine)

inline CFrustum* Get_Frustum(void);
inline CLightMgr* Get_LightMgr(void);
inline CManagement* Get_Management(void);
inline CNavMgr* Get_NavMgr(void);
inline CQuadTree* Get_Quadtree(void);
inline CRenderer* Get_Renderer(void);

#include "Export_Utility.inl"

END


#endif // Export_Utility_h__