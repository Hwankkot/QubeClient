CFrustum* Get_Frustum(void)
{
	return CFrustum::GetInstance();
}

CLightMgr* Get_LightMgr(void)
{
	return CLightMgr::GetInstance();
}

CManagement* Get_Management(void)
{
	return CManagement::GetInstance();
}

CNavMgr* Get_NavMgr(void)
{
	return CNavMgr::GetInstance();
}

CQuadTree* Get_Quadtree(void)
{
	return CQuadTree::GetInstance();
}

CRenderer* Get_Renderer(void)
{
	return CRenderer::GetInstance();
}