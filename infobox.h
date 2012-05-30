
#ifndef __INFO_BOX_H__
#define __INFO_BOX_H__

#define INFO_CFGFILE		"/var/etc/ibox.cfg"
#define INFO_TIMEOUT_DEF	5	// 	5 sec
#define INFO_TIMEOUT_MAX	10
#define MAX_CODE_STRING		16

extern int _ibox_pid;

#define SUBSTATE_InfoBox		(SUBSTATE_PluginPop + SUBSTATE_Offset + _ibox_pid)	// for unique substate index
#define SUBSTATE_InfoBoxSetup	(SUBSTATE_PluginPop + (SUBSTATE_Offset*2) + _ibox_pid)	// for unique substate index

//#define SUBSTATE_InfoEtc		(SUBSTATE_PluginPop + (SUBSTATE_Offset*3) + _ibox_pid)	// for unique substate index

typedef struct
{
	int timeout;
	bool navigation;
	unsigned char code[MAX_CODE_STRING];
} S_InfoCfg;

extern void CfgParse( S_InfoCfg &cfg );

class C_InfoBox
{
public :
	C_InfoBox();
	virtual ~C_InfoBox();

	void ReadCfg();
	void RcuCtrl( int key );

	void DrawSignal();
	void DrawEvtName();

	void Draw();
	void Hide();
	void OnTimeout( int id );
	void RestartTimer();
	void EvtData( word satIdx, word orgNetId, word tsId, word svcId );

	bool IsActive() { return m_active; }
	void SetConstant() { m_constant=1; }
	void DrawIsDone() { m_drawing=false; }

private :
	bool m_active;	// shown state
	bool m_drawing;	// key input block because OSD broken.

	int m_svcIdx;	// slot num of _tvSvc[N_Tv] or _radioSvc[N_Radio]
	int m_svcType;	// SVCTYPE_Tv(0) or SVCTYPE_Radio(1)
	int m_tuner;

	int m_indexCnt;	// slot count of _tvSvcIndex or _radioSvcIndex.
	int m_index;		// slot num of _tvSvcIndex or _radioSvcIndex [ _xxSvcIndex is index array of _tvSvc or _radioSvc: same with live mode service list ]
	int m_orgIndex;	// m_index backup: up/down key.

	byte *m_scrBuf;		// infobox background backup buf- not used.
	int m_scrBufSize;		// backup buf size

	int m_x, m_y, m_w, m_h;

	byte *m_detailBuf;	// detail box background backup buf.
	int m_detailBufSize;

	int m_dx, m_dy, m_dw, m_dh;

	int m_timerId;
	int m_timeout;
	int m_tick;			// decrease m_tick per 1 sec
	bool m_navigation;	// use up/down key

	int m_constant;		// don't apply timeout when user press info key. - SetConstant()

	S_Service *m_svc;	// current displayed service on infobox.
	S_Tp *m_tp;			// tp of m_svc

	int m_evtCnt;			// number of S_EventInfo
	S_EventInfo *m_evtInfo;

	S_EventInfo *m_curEvtInfo;	// current displayed event on infobox.
	S_EventInfo *m_orgEvtInfo;	// backup of m_curEvtInfo- left/right key

	void SetAlarm();
	void ChangeSvc( int key );
	void ChangeEvt( int key );

	void DrawDetail( bool key=false );
	void DrawBack();
	void DrawPeriodicItems();

	dword Diff( dword time1, dword time2 );
	void DrawEvtProgress();

	void DrawTime();
	void DrawIcons( dword icons );
	bool DrawInfo( int svcIdx=-1 );

};

#endif // __INFO_BOX_H__

