/*!
 * \file AniCtrl.h
 * \date 2016/04/08 16:08
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

#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl
{
public:
	CAniCtrl(void);
	CAniCtrl(const CAniCtrl& rhs);
	~CAniCtrl(void);

public:
	LPD3DXANIMATIONCONTROLLER* GetAniCtrl(void);
	DOUBLE GetTrackPosition(void);
	DOUBLE GetPeriod(void);

public:
	void SetAnimationSet(UINT iAniIdx , DOUBLE dAccTime);
	void SetTrackPos(DOUBLE dPosition);
	void FrameMove(const float& fTimePerSec);

private:
	void Release(void);

private:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl;

private:
	UINT		m_iMaxAniSet;
	UINT		m_iCurrentTrack;
	UINT		m_iNewTrack;
	DOUBLE		m_dAccTime;
	DOUBLE		m_dPeriod;
};

END

#endif // AniCtrl_h__