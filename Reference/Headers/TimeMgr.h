/*!
 * \file TimeMgr.h
 * \date 2016/04/05 0:55
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

#pragma once

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class ENGINE_DLL CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	explicit CTimeMgr(void);

public:
	~CTimeMgr(void);

public:
	/// Set FrameTime, FixTime, LasTime.
	/// Get CpuTick.
	void InitTime(void);
	void SetTime(void);

public:
	/// It calls several times in one second.
	/// So, it's inline method.
	inline float GetTime(void);


private:
	/// Save a time in long union structure.
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;

	float m_fTime;
};

END