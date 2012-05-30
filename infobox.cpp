
/* infobox.cpp by GED @ fortis */

//#define DEBUG

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>

#include "plugin.h"
#include "infobox.h"
#include "imgosd.h"

#include "img/info_icon1.img"
#include "img/info_icon1_1.img"
#include "img/info_icon3.img"
#include "img/info_icon3_1.img"
#include "img/info_icon5.img"
#include "img/info_icon5_1.img"
#include "img/info_icon6.img"
#include "img/info_icon6_1.img"
#include "img/info_icon7.img"
#include "img/info_icon7_1.img"
#include "img/info_icon8.img"
#include "img/info_icon8_1.img"

// color -------------------------------------------------------------------------------------------------------------------------

#define info_bc			ARGB(0xE0, 28, 31, 36)
#define info_tc			ARGB(0xFF, 222, 227, 228)
#define info_tbc			ARGB(0xE0, 17, 21, 23)

#define info_num_fs		14
#define info_num_tc		info_tc
#define info_num_bc		info_tbc

#define info_svc_fs		20
#define info_svc_tc		ARGB(0xFF, 171, 211, 224)
#define info_svc_bc		info_tbc

#define info_evt_fs		16
#define info_evt_tc		info_tc
#define info_evt_rp_tc	ARGB(0xFF, 0, 169, 251)
#define info_evt_rr_tc	ARGB(0xFF, 235, 43, 1)
#define info_evt_bc		info_tbc

#define info_evt_time_fs	13
#define info_evt_time_tc	info_evt_tc
#define info_evt_time_bc	info_bc
#define info_evt_prog_c	ARGB(0xE0, 31, 49, 75)
#define info_evt_prog_bc	ARGB(0xE0, 10, 16, 24)
#define info_evt_prog_p	ARGB(0xE0, 99, 52, 1)

#define info_evt_d_fs		14
#define info_evt_d_tc		info_evt_tc
#define info_evt_d_bc	info_evt_bc

#define info_id_fs		14
#define info_id_tc		info_tc
#define info_id_bc		info_bc

#define info_sig_fs		12
#define info_sig_tc		info_tc
#define info_sig_bc		info_tbc
#define info_sig_grayc	ARGB(0xE0, 70, 75, 77)
#define info_sig_lockc	ARGB(0xE0, 28, 66, 32)
#define info_sig_unlockc	ARGB(0xE0, 107, 17, 10)

#define info_time_fs		16
#define info_time_tc		ARGB(0xFF, 253, 187, 115)
#define info_time_bc		info_id_bc

// position -----------------------------------------------------------------------------------------------------------------------

#define info_h_tab		10	// horizontal tab
#define info_v_tab		6	// vertical tab
#define info_left_tab		14
#define info_top_tab		10

#define info_x			((OSDWIDTH-info_w)/2)
#define info_y			(OSDHEIGHT-(info_h+20))
#define info_w			(OSDWIDTH-(30+30))
#define info_h			(info_top_tab+info_num_h+info_v_tab+info_icon_h+info_v_tab+info_id_h+info_id_v_tab+info_id_h+(info_top_tab/2))

#define info_detail_v_tab		4
#define info_detail_item_h	20
#define info_detail_h			((info_detail_item_h*2)+(info_detail_v_tab*3))

#define info_num_x		(m_x+info_left_tab)
#define info_num_y		(m_y+info_top_tab)
#define info_num_w		50
#define info_num_h		30

#define info_svc_x		(info_num_x+info_num_w+info_h_tab)
#define info_svc_y		info_num_y
#define info_svc_w		240
#define info_svc_h		info_num_h

#define info_evt_x		(info_svc_x+info_svc_w+info_h_tab)
#define info_evt_y		info_num_y
#define info_evt_w		(info_sig_x-info_evt_x-info_h_tab)		// remain width.
#define info_evt_h		info_num_h

#define info_evt_stime_x		info_evt_x
#define info_evt_stime_y		(info_evt_y+info_evt_h+2)
#define info_evt_stime_w 	info_evt_prog_w
#define info_evt_stime_h		20

#define info_evt_prog_x		info_evt_x
#define info_evt_prog_y		(info_evt_stime_y+info_evt_stime_h+2)
#define info_evt_prog_w		(info_evt_w-info_evt_etime_w-6)
#define info_evt_prog_h		10

#define info_evt_etime_x		(info_evt_x+info_evt_w-info_evt_etime_w)
#define info_evt_etime_y		(info_evt_prog_y+info_evt_prog_h-info_evt_etime_h)
#define info_evt_etime_w		110
#define info_evt_etime_h		20

typedef enum {
	Icon_Cas			= 0x01,
	Icon_Ttx			= 0x02,
	Icon_Subt			= 0x04,
	Icon_Dolby			= 0x08,
	Icon_Multifeed		= 0x10,
	Icon_Hd				= 0x20,
	N_Icon,
} T_Icons;

#define info_icon_x		info_num_x
#define info_icon_y		(info_num_y+info_num_h+info_v_tab)
#define info_icon_w		(info_svc_x+info_svc_w-info_icon_x)
#define info_icon_h		34
#define info_icon_tab		4

#define info_id_x			info_num_x
#define info_id_y			(info_icon_y+info_icon_h+info_v_tab)
#define info_id_w1		(info_time_x-info_id_x-10)
#define info_id_w2		(info_sig_x-info_id_x-info_h_tab)
#define info_id_h			20
#define info_id_v_tab		4

#define info_sig_str_h	20
#define info_sig_x		(info_x+info_w-(info_sig_w+info_h_tab))
#define info_sig_y		info_evt_y
#define info_sig_w		20
#define info_sig_h		(info_h-(info_top_tab+(info_top_tab/2)))
#define info_sig_num		24
#define info_sig_v_tab	2
#define info_sig_h_tab	2

#define info_time_x		info_evt_x
#define info_time_y		info_id_y
#define info_time_w		info_evt_w
#define info_time_h		info_id_h

// ------------------------------------------------------------------------------------------------------------------------------

C_InfoBox::C_InfoBox()
{
	m_active = false;
	m_drawing = false;

	m_svcIdx = -1;	// playback service check. ibox do not support playback service yet. :(

	m_scrBuf = NULL;
	m_detailBuf = NULL;

	ReadCfg();

	m_timerId = -1;	// timer period is 1 sec.

	m_evtCnt = 0;
	m_evtInfo = NULL;
}

C_InfoBox::~C_InfoBox()
{
	Hide();
	if( m_evtInfo ) free(m_evtInfo);
}

void C_InfoBox::ReadCfg()
{
	S_InfoCfg cfg = {0,};

	m_x = info_x; m_y = info_y; m_w = info_w; m_h = info_h;	// TODO: S_InfoCfg.

	CfgParse(cfg);

	m_tick = m_timeout = cfg.timeout;	// decrease m_tick per 1 sec. m_timeout is org backup.
	m_navigation = cfg.navigation;		// if true use up/down key

	m_dx = m_x; m_dw = m_w; m_dh = info_detail_h; m_dy = m_y - (m_dh+info_id_v_tab);	// detail box.
}

void C_InfoBox::RcuCtrl( int key )
{
	if( m_drawing ) return;

	switch( key )
	{
		case REM_UP :
		case REM_DOWN :
			if( m_navigation ) {
				ChangeSvc(key);
			} else {
				SendCommand( PCMD_Key, ((key == REM_UP) ? REM_CHUP : REM_CHDOWN) );	// throw to main appl
			}
			break;

		case REM_RIGHT :
		case REM_LEFT :
			ChangeEvt(key);
			break;

		case REM_INFO :
			DrawDetail(true);
			break;

		case REM_CHUP :
		case REM_CHDOWN :
			break;

		case REM_OK :
			if( m_orgEvtInfo ) {	// current focus is not current event.
				SetAlarm();
				break;
			}
			if( m_indexCnt ) {
				if( m_orgIndex != m_index ) {	// ChangeSvc(...)
					SendCommand( PCMD_SvcStart, SERVICE_Main, m_svcType, GetSvcIdxByIndex(m_svcType, m_index) );
					break;
				}
			}	// don't break;
		default :
			Hide();
			SendCommand( PCMD_Key, key );	// all available key should be threw through the ibox because OSD sync
			break;
	}
}

void C_InfoBox::SetAlarm()
{
	if( m_svcIdx == (-1) ) return;
	if( m_curEvtInfo == NULL ) return;

#if 1
	static byte alarmType = 0;

	byte alarmMode = 1;
	S_Alarm alarm = {0,};

	if( alarmType == 2 ) {
		alarmMode = 0;
	}

	alarm.alarmSTime = Time2Local(m_curEvtInfo->startTime);
	alarm.alarmETime = Time2Local(m_curEvtInfo->endTime);
	alarm.alarmSat = m_tp->satIdx;
	if( m_index == m_orgIndex )	// viewing service
		alarm.alarmSvc = m_svcIdx;
	else
		alarm.alarmSvc = GetSvcIdxByIndex(m_svcType, m_index);
	alarm.alarmEvtId = m_curEvtInfo->evtId;
	alarm.alarmSvcType = m_svcType;
	alarm.alarmMode = alarmMode;
	alarm.alarmType = alarmType;

	SetAlarmPtr(1, &alarm);

	alarmType = (alarmType + 1) % 3;

#else
	dword data1, data2, data3, data4, data5, data6, data7;

	data1 = (m_curEvtInfo->evtId << 16) | m_svc->svcId;
	data2 = (m_tp->tsId << 16) | m_tp->orgNetId;
	data3 = (m_svcType << 16);
	if( m_index == m_orgIndex )	// viewing service
		data3 |= (word)m_svcIdx;
	else
		data3 |= (word)GetSvcIdxByIndex(m_svcType, m_index);
	data4 = m_tp->satIdx;	// (word)-1 : All group.
	data5 = Time2Local(m_curEvtInfo->startTime);
	data6 = Time2Local(m_curEvtInfo->endTime);

	/* [ (alarmType<<24 | alarmMode<<16 | alarmDays<<8 | alarmPwOff) */
	data7 = (2 << 24);		// Play(0), Recording(1), Toggle(2)
	data7 |= (1 << 16);	// Off(0), Once(1), Daily(2), Weekly(3)
	data7 |= (0 << 8);		// if Weekly mode: 0xFE - "0Bit-0. 1Bit-Mon. 2Bit-Tue. 3Bit-Wed. 4Bit-Thu. 5Bit-Fri. 6Bit-Sat. 7Bit-Sun"
	data7 |= 0;			// Power off when timer end time - Turn Off(1), or not(0)
	/* ] */

	SendCommand( PCMD_SetAlarm, data1, data2, data3, data4, data5, data6, data7 );
#endif

	if( m_tick > 0 )
		m_tick = m_timeout;
}

void C_InfoBox::ChangeSvc( int key )
{
	int svcIdx, index;

	if( m_svcIdx == (-1) ) return;
	if( m_indexCnt == 0 ) return;

	index = m_index;
	if( key == REM_UP )
		m_index = (m_index + 1) % m_indexCnt;
	else
		m_index = (m_index + (m_indexCnt - 1)) % m_indexCnt;

	svcIdx = GetSvcIdxByIndex( m_svcType, m_index );

	if( (svcIdx == (-1)) || (DrawInfo(svcIdx) == false) ) {
		Printf("ChangeSvc error.. %d, %d\n", svcIdx, m_index);
		m_index = index;
	}

	if( m_tick > 0 )
		m_tick = m_timeout;
}

void C_InfoBox::ChangeEvt( int key )
{
	int cur, idx;
	S_EventInfo *evt;

	if( m_svcIdx == (-1) ) return;
	if( m_curEvtInfo == NULL ) return;
	if( m_evtCnt < 2 ) return;

	if( m_orgEvtInfo == NULL ) {
		m_orgEvtInfo = m_curEvtInfo;	// backup and flag(event focus is changed)
	}

	cur = ((dword)m_curEvtInfo - (dword)m_evtInfo) / sizeof(S_EventInfo);

	if( key == REM_RIGHT ) {
		for( idx=(cur+1); idx<m_evtCnt; idx++ )
		{
			evt = &m_evtInfo[idx];
			if( (evt->startTime > m_curEvtInfo->startTime) && (evt->startTime >= m_curEvtInfo->endTime) ) {
				m_curEvtInfo = evt;
				break;
			}
		}
	} else {
		for( idx=(cur-1); idx>=0; idx-- )
		{
			evt = &m_evtInfo[idx];
			if( (evt->startTime < m_curEvtInfo->startTime) && (evt->endTime <= m_curEvtInfo->startTime) ) {
				m_curEvtInfo = evt;
				break;
			}
		}
	}

	if( m_tick > 0 )
		m_tick = m_timeout;
	DrawEvtProgress();
}

void C_InfoBox::DrawDetail( bool key )
{
	char *descr, *strBuf;

	if( !m_active ) return;
	if( m_svcIdx == (-1) ) return;

	if( key ) {	// user input
		if( m_detailBuf ) {
			OsdDecompress( m_detailBuf, m_dx, m_dy, m_dw, m_dh );

			free( m_detailBuf );
			m_detailBuf = NULL;
			return;
		} else {
			m_detailBuf = OsdCompress( m_dx, m_dy, m_dw, m_dh, &m_detailBufSize );
			FillBox( m_dx, m_dy, m_dw, m_dh, info_bc );
			m_tick = 0;	// clear timeout.
		}
	}
	else if( m_detailBuf == NULL ) return;

	strBuf = GetStrBuf();
	SetFontSize( info_evt_d_fs );

	strBuf[0] = 0;
	if( m_curEvtInfo ) {
		descr = GetShortText( m_curEvtInfo );
		if( descr ) {
			strncpy( strBuf, descr, 80 );	// TODO: draw multi line function.	// use GetStrWidth(...) function.
			strBuf[80] = 0;
		}
	}
	DrawText( m_dx+info_left_tab, m_dy+info_detail_v_tab, (m_dw-(info_left_tab+info_h_tab)), info_detail_item_h, \
		strBuf, info_evt_d_tc, info_evt_d_bc, ALIGN_Left, VALIGN_Center, 4 );

	strBuf[0] = 0;
	if( m_curEvtInfo ) {
		descr = GetExtendedText( m_curEvtInfo );
		if( descr ) {
			strncpy( strBuf, descr, 80 );	// TODO: draw multi line function.	// use GetStrWidth(...) function.
			strBuf[80] = 0;
		}
	}
	DrawText( m_dx+info_left_tab, m_dy+info_detail_v_tab+info_detail_item_h+info_detail_v_tab, (m_dw-(info_left_tab+info_h_tab)), info_detail_item_h, \
		strBuf, info_evt_d_tc, info_evt_d_bc, ALIGN_Left, VALIGN_Center, 4 );

}

void C_InfoBox::DrawBack()
{
	FillBox( m_x, m_y, m_w, m_h, info_bc);
	FillBox( info_sig_x, info_sig_y, info_sig_w, info_sig_h, info_sig_bc );
}

void C_InfoBox::DrawPeriodicItems()
{
	DrawSignal();		// Not use APPLMSG_TUNER_LOCK(x) message- not periodic

	DrawEvtProgress();
	DrawTime();
}

void C_InfoBox::DrawSignal()
{
	int no, x, y, w, h;
	dword color;
	int isLock, strength, quality, level, qt;
	char *strBuf;

	if( !m_active ) return;
	if( m_svcIdx == (-1) ) return;

	h = ((info_sig_h-info_sig_str_h)-((info_sig_num+1)*info_sig_v_tab))/info_sig_num;
	x = info_sig_x+info_sig_h_tab;
	y = info_sig_y+info_sig_h-(h+info_sig_v_tab);
	w = info_sig_w-(info_sig_h_tab*2);

	GetTunerState( m_tuner, &isLock, &strength, &quality );		// max value- strength: 255, quality: 255

	if( isLock == 1 )
		color = info_sig_lockc;
	else
		color = info_sig_unlockc;
	level = quality*(info_sig_num-1)/255;		// under 100%
	qt = (quality*1000)/256;

	//Printf("isLock: %d. strength: %d. quality: %d. level: %d. qt: %d\n", isLock, strength, quality, level, qt);

	strBuf = GetStrBuf();
	sprintf( strBuf, "%d", qt/10 );

	SetFontSize( info_sig_fs );
	DrawText( info_sig_x, info_sig_y, info_sig_w, info_sig_str_h, strBuf, info_sig_tc, info_sig_bc, ALIGN_Center, VALIGN_Center, 0 );

	for( no=0; no<info_sig_num; no++ ) {
		if( no >= level )
			FillBox( x, y, w, h, info_sig_grayc );
		else
			FillBox( x, y, w, h, color );
		y -= (h+info_sig_v_tab);
	}
}

dword C_InfoBox::Diff( dword time1, dword time2 )
{
	dword time, min1, min2, dayo;

	min1 = ((time1>>8)&0xff)*60 + (time1&0xff);
	min2 = ((time2>>8)&0xff)*60 + (time2&0xff);
	time = (time1&0xffff0000) - (time2&0xffff0000);
	if( min1 >= min2 ) {
		min1 -= min2;
	} else {
		dayo = 1 + ((min2-min1)/60)/24;
		min1 = (min1+dayo*24*60) - min2;
		time -= (dayo<<16);
	}
	time |= ((min1/60)<<8) | (min1%60);
	return time;
}

void C_InfoBox::DrawEvtProgress()
{
	char *strBuf;
	dword sTime, eTime, cTime;
	int year, mon, day, dayOfWeek;
	int hour, min, mjd, duration, no;

	if( !m_active ) return;
	if( m_svcIdx == (-1) ) return;

	if( m_evtCnt == 0 )
		goto no_evt;

	cTime = GetCurTime();		// (mjd<<16) | (hour<<8) | min

	if( m_curEvtInfo == NULL ) {
		for( no=0; no<m_evtCnt; no++ )
		{
			if( (m_evtInfo[no].startTime <= cTime) && (m_evtInfo[no].endTime > cTime) ) {
				m_curEvtInfo = &m_evtInfo[no];
				break;
			}
		}
		if( m_curEvtInfo == NULL )
			goto no_evt;
		DrawEvtName();
	}

	strBuf = GetStrBuf();

	cTime = Time2Local(cTime);

	sTime = Time2Local(m_curEvtInfo->startTime);
	mjd = (sTime >> 16) & 0xFFFF;
	hour = (sTime >> 8) & 0xFF;
	min = sTime & 0xFF;
	Mjd2Date( mjd, &year, &mon, &day, &dayOfWeek );

	duration = ((m_curEvtInfo->duration >> 8) * 60) + (m_curEvtInfo->duration & 0xFF);
	sprintf( strBuf, "%02d : %02d / %d-%d-%d [ %02d : %02d ]", hour, min, day, mon, year, (duration/60), (duration%60) );

	SetFontSize( info_evt_time_fs );
	DrawText( info_evt_stime_x, info_evt_stime_y, info_evt_stime_w, info_evt_stime_h, strBuf, info_evt_time_tc, info_evt_time_bc, ALIGN_Left, VALIGN_Center, 0 );

	eTime = Time2Local(m_curEvtInfo->endTime);
	mjd = (eTime >> 16) & 0xFFFF;
	hour = (eTime >> 8) & 0xFF;
	min = eTime & 0xFF;
	Mjd2Date( mjd, &year, &mon, &day, &dayOfWeek );

	sprintf( strBuf, "%02d : %02d", hour, min );

	if( (sTime <= cTime) && (eTime > cTime) )		// viewing event
	{
		dword time, remain;

		if( m_orgEvtInfo != NULL ) {	// focus event is restored- user change or time changed.
			DrawEvtName();
			m_orgEvtInfo = NULL;
		}
		time = Diff( eTime, cTime );
		remain = ((time>>16)&0xffff)*24*60 + ((time>>8)&0xff)*60 + (time&0xff);

		if( duration < remain ) {
			Printf("error.... remain- %d / duration- %d\n", remain, duration);
		} else {
			int w = (info_evt_prog_w * ((duration-remain)*1000/duration)) / 1000;
			if( w > 0 )
				FillBox( info_evt_prog_x, info_evt_prog_y, w, info_evt_prog_h, info_evt_prog_c );
			FillBox( info_evt_prog_x+w, info_evt_prog_y, info_evt_prog_w-w, info_evt_prog_h, info_evt_prog_bc );

			char rBuf[32] = {0,};
			sprintf( rBuf, " [ %02d : %02d ]", (remain/60), (remain%60) );
			strcat( strBuf, rBuf );
		}
	}
	else if( m_orgEvtInfo != NULL )				// focus event is changed- left/right key
	{
		int total, spos, epos, x, w;

		DrawEvtName();

		total = Diff( m_evtInfo[m_evtCnt-1].endTime, m_evtInfo[0].startTime );
		total = ((total>>16)&0xffff)*24*60 + ((total>>8)&0xff)*60 + (total&0xff);

		spos = Diff( m_curEvtInfo->startTime, m_evtInfo[0].startTime );
		spos = ((spos>>16)&0xffff)*24*60 + ((spos>>8)&0xff)*60 + (spos&0xff);

		epos = Diff( m_curEvtInfo->endTime, m_evtInfo[0].startTime );
		epos = ((epos>>16)&0xffff)*24*60 + ((epos>>8)&0xff)*60 + (epos&0xff);

		x = (info_evt_prog_w * ((spos*1000)/total)) / 1000;
		w = (info_evt_prog_w * ((epos-spos)*1000/total)) / 1000;
		if( w < 2 ) w = 2;	// min w.
		if( x > 0 )
			FillBox( info_evt_prog_x, info_evt_prog_y, x, info_evt_prog_h, info_evt_prog_bc );
		if( (x+w) < info_evt_prog_w ) {
			FillBox( info_evt_prog_x+x, info_evt_prog_y, w, info_evt_prog_h, info_evt_prog_p );
			FillBox( info_evt_prog_x+x+w, info_evt_prog_y, (info_evt_prog_w-(x+w)), info_evt_prog_h, info_evt_prog_bc );
		} else {		// because min w.
			FillBox( info_evt_prog_x+(info_evt_prog_w-w), info_evt_prog_y, w, info_evt_prog_h, info_evt_prog_bc );
		}
	}
	else		// current event changed by time change.. refresh
	{
		m_curEvtInfo = NULL;
		for( no=0; no<m_evtCnt; no++ )
		{
			if( (m_evtInfo[no].startTime <= cTime) && (m_evtInfo[no].endTime > cTime) ) {
				m_curEvtInfo = &m_evtInfo[no];
				break;
			}
		}
		DrawEvtName();
		goto no_evt;	// clear and draw current event on next turn.
	}

	SetFontSize( info_evt_time_fs );
	DrawText( info_evt_etime_x, info_evt_etime_y, info_evt_etime_w, info_evt_etime_h, strBuf, info_evt_time_tc, info_evt_time_bc, ALIGN_Left, VALIGN_Bottom, 0 );

	if( 0 ) {
no_evt:
		FillBox( info_evt_stime_x, info_evt_stime_y, info_evt_stime_w, info_evt_stime_h, info_evt_time_bc );
		FillBox( info_evt_prog_x, info_evt_prog_y, info_evt_prog_w, info_evt_prog_h, info_evt_time_bc );
		FillBox( info_evt_etime_x, info_evt_etime_y, info_evt_etime_w, info_evt_etime_h, info_evt_time_bc );
	}
}

void C_InfoBox::DrawTime()
{
	char *strBuf;
	int year, mon, day, dayOfWeek;
	int hour, min, sec, mjd;
	dword time;

	if( !m_active ) return;

	time = GetCurTime( &hour, &min, &sec, &mjd );
	time = Time2Local( time );
	mjd = (time >> 16) & 0xffff;
	hour = (time >> 8) & 0xff;
	min = time & 0xff;
	Mjd2Date( mjd, &year, &mon, &day, &dayOfWeek );	// dayOfWeek: Mon(0) - Sun{6).

	strBuf = GetStrBuf();

	SetFontSize( info_time_fs );

	sprintf( strBuf, "%02d : %02d : %02d", hour, min, sec );
	DrawText( info_time_x, info_time_y, 120, info_time_h, strBuf, info_time_tc, info_time_bc, ALIGN_Left, VALIGN_Center, 0 );

	sprintf( strBuf, "/  %d-%d-%d", day, mon, year );
	DrawText( info_time_x+120, info_time_y, (info_time_w-120), info_time_h, strBuf, info_time_tc, info_time_bc, ALIGN_Left, VALIGN_Center, 0 );
}

void C_InfoBox::DrawIcons( dword icons )
{
	S_BlitImg *blitImg = NULL;
	S_OsdImageHdr *img = NULL;
	int no, icon, xPos = info_icon_x;
	dword *pos;

	FillBox( info_icon_x, info_icon_y, info_icon_w, info_icon_h, info_bc );

	for( icon=Icon_Cas; icon<N_Icon; icon<<=1 )
	{
		switch( icon )
		{
			case Icon_Cas :
				img = ( icons & icon ) ? &image_info_icon1 : &image_info_icon1_1;
				break;
			case Icon_Ttx :
				img = ( icons & icon ) ? &image_info_icon7 : &image_info_icon7_1;
				break;
			case Icon_Subt :
				img = ( icons & icon ) ? &image_info_icon6 : &image_info_icon6_1;
				break;
			case Icon_Dolby :
				img = ( icons & icon ) ? &image_info_icon5 : &image_info_icon5_1;
				break;
			case Icon_Multifeed :
				img = ( icons & icon ) ? &image_info_icon8 : &image_info_icon8_1;
				break;
			case Icon_Hd :
				img = ( icons & icon ) ? &image_info_icon3 : &image_info_icon3_1;
				break;
		}

		unsigned long uncomprLen = ((img->imgHd->width * img->imgHd->height * img->imgHd->bytesperpixel) >> 2) << 2;	// align4

		blitImg = (S_BlitImg *)malloc( sizeof(S_BlitImg) + uncomprLen );
		if( !blitImg ) {
			Printf("DrawIcons: blitImg malloc fail!\n");
			break;
		}
		byte *uncompr = (byte *)blitImg->data;

		int err = uncompress( uncompr, &uncomprLen, img->comprBits, img->comprSize );

		blitImg->x = xPos - 3;
		blitImg->y = info_icon_y + ((info_icon_h - img->imgHd->height) / 2) - 3;
		blitImg->w = img->imgHd->width;
		blitImg->h = img->imgHd->height;
		blitImg->mask = 0xffffffff;
		blitImg->mode = BLIT_OP_MIX;
		Blit( blitImg, blitImg->w+6, blitImg->h+6 );	// stretch image

		free( blitImg );

		xPos += (img->imgHd->width + info_icon_tab);
	}
}

void C_InfoBox::DrawEvtName()
{
	if( !m_active ) return;
	if( m_svcIdx == (-1) ) return;

	SetFontSize( info_evt_fs );

	if( m_curEvtInfo ) {
		dword tc = info_evt_tc;

		int alarmId = FindMatchAlarmSlot( m_curEvtInfo->evtId, m_svc->svcId, m_tp->tsId, m_tp->orgNetId );
		if( alarmId != (-1) ) {
			if( GetAlarmPtr(alarmId)->alarmType == 0 )	// play
				tc = info_evt_rp_tc;
			else
				tc = info_evt_rr_tc;
		}
		DrawText( info_evt_x, info_evt_y, info_evt_w, info_evt_h, GetEventName(m_curEvtInfo), tc, info_evt_bc, ALIGN_Left, VALIGN_Center, 6 );
	} else {
		DrawText( info_evt_x, info_evt_y, info_evt_w, info_evt_h, "No Information", info_evt_tc, info_evt_bc, ALIGN_Left, VALIGN_Center, 6 );
	}

	DrawDetail();
}

bool C_InfoBox::DrawInfo( int svcIdx )
{
	char *strBuf;
	dword no, idx, icons;
	S_ServiceInfo *svcInfo;
	S_Service svcZero = {0,};

	if( m_svcIdx == (-1) ) return false;		// playback service.

	if( svcIdx != (-1) ) {	// up/down key.
		m_svc = GetSvc( m_svcType, svcIdx );
		if( m_svc == NULL ) {
			Printf("DrawInfo(%d)- error. service is null..!!\n", svcIdx);
			return false;
		}
		m_tp = GetTp( m_svc );
	} else {
		m_svc = GetCurService( SERVICE_Main );	// Don't return null.
		if( memcmp(m_svc, &svcZero, sizeof(S_Service)) == 0 ) {	Printf("DrawInfo()- main service is null..!!\n");
			m_svcIdx = -1;
			return false;
		}
		m_tp = GetTp( m_svc );

		m_indexCnt = 0; m_index = -1;
		idx = GetSvcIdxById( m_svcType, m_tp->orgNetId, m_tp->tsId, m_svc->svcId, &m_index );	// GetSvcIdxById function set m_index if ok.
		m_orgIndex = m_index;	// _tvSvcIndex[m_index] or _radioSvcIndex[m_index] value is idx[ index of S_Service array ]
		if( m_index != (-1) )
			m_indexCnt = GetSvcIndexCnt( m_svcType );
		Printf( "%d / %d- [ %d ]\n", m_index, m_indexCnt, idx );
	}

	strBuf = GetStrBuf();

/* [ service name */
	if( GetConfig(CFG_LCNOnOff) )
		sprintf( strBuf, "%d", m_svc->logicalChNum );
	else
		sprintf( strBuf, "%d", m_svc->tempChNum );

	SetFontSize( info_num_fs );
	DrawText( info_num_x, info_num_y, info_num_w, info_num_h, strBuf, info_num_tc, info_num_bc, ALIGN_Left, VALIGN_Center, 4 );

	strcpy( strBuf, (char *)GetSvcName(m_svc, false) );

	if( m_index == m_orgIndex ) {	// viewing service
		SetFontSize( info_svc_fs );
		DrawText( info_svc_x, info_svc_y, info_svc_w, info_svc_h, strBuf, info_svc_tc, info_svc_bc, ALIGN_Left, VALIGN_Center, 6 );
	} else {
		SetFontSize( info_evt_fs );
		DrawText( info_svc_x, info_svc_y, info_svc_w, info_svc_h, strBuf, info_evt_tc, info_svc_bc, ALIGN_Left, VALIGN_Center, 6 );
	}
/* ] service name */

/* [ icons */
	icons = 0;
	if( m_svc->isCas )
		icons |= Icon_Cas;
	if( m_svc->isHd )
		icons |= Icon_Hd;

	svcInfo = GetServiceInfo( m_svc );
	if( svcInfo ) {
		if( svcInfo->ttxPid )
			icons |= Icon_Ttx;
		if( m_svc->multifeed )			// check after get service info.
			icons |= Icon_Multifeed;
		if( svcInfo->nSubt )
			icons |= Icon_Subt;
		if( m_index == m_orgIndex ) {	// viewing service
			S_AudioInfo *audioInfo = GetAudioInfo( SERVICE_Main );
			if( audioInfo ) {
				for( no=0; no<svcInfo->nAudio; no++ ) {
					if( audioInfo[no].isAc3 ) {
						icons |= Icon_Dolby;
						break;
					}
				}
			}
		}
	}
	DrawIcons( icons );
/* ] icons */

/* [ other info */
	SetFontSize( info_id_fs );
	sprintf( strBuf, "%s / %d [%c] %d -  %s", (char *)GetSatName(m_tp->satIdx), m_tp->freq, ((m_tp->polar == 0/*POL_VERTICAL*/) ? 'V' : 'H'), \
		m_tp->sr, GetNetworkName(m_tp, true) );

	DrawText( info_id_x, info_id_y, info_id_w1, info_id_h, strBuf, info_id_tc, info_id_bc, ALIGN_Left, VALIGN_Center, 0 );

	sprintf( strBuf, "SID: 0x%04X TSID: 0x%04X ONID: 0x%04X / VPID: 0x%04X APID: 0x%04X PPID: 0x%04X", \
		m_svc->svcId, m_tp->tsId, m_tp->orgNetId, m_svc->videoPid, m_svc->audioPid, m_svc->pcrPid);

	DrawText( info_id_x, info_id_y+info_id_h+info_id_v_tab, info_id_w2, info_id_h, strBuf, info_id_tc, info_id_bc, ALIGN_Left, VALIGN_Center, 0 );
/* ] other info */

	m_evtCnt = 0;
	m_orgEvtInfo = m_curEvtInfo = NULL;
/* [ event- after receive APPLMSG_EPG_Data_Ack
	DrawEvtName();
	DrawEvtProgress();
] */

	DrawSignal();
	DrawTime();

	GetEventInfo( m_svc );	// evt data request. Ack- APPLMSG_EPG_Data_Ack

	return true;
}

void C_InfoBox::Draw()
{
	m_svcIdx = GetCurSvcNum(SERVICE_Main);
	m_svcType = GetCurSvcType(SERVICE_Main);
	m_tuner = GetCurSvcTuner(SERVICE_Main);

	//Printf("Draw(): m_active- %d. constant- %d. idx- %d\n", m_active, m_constant, m_svcIdx);

	if( m_active )
	{
		if( m_tick > 0 )	// reset timeout. keep timeout condition.
			m_tick = m_timeout;
		if( DrawInfo() == false ) Hide();
	}
	else
	{
		if( GetState(STATEIDX_Sub) != SUBSTATE_InfoBox ) return;	// set in ibox.
		if( m_svcIdx == (-1) ) {
			RestoreState(STATEIDX_Sub);
			return;	// playback service.
		}

		// PCMD_DestroyAndSaveSubState message close pvr-infobar(live mode right-up side)
		// so send info key message before key blocking but pvr-infobar wouldn't shown because substate is not none.
		// but after close infobox it will be shown. :(
		SendCommand( PCMD_Key, REM_INFO );

		// all available key should be threw through the ibox because OSD sync
		SendCommand( PCMD_BlockKey, (dword)-1 );		// block all key. - power and mute couldn't block.

		// except chup, chdown. there is no effect to OSD[ keep active status and redraw after receive PLUGINMSG_SVC_Starting message ]
		SendCommand( PCMD_ReleaseKey, REM_CHUP );
		SendCommand( PCMD_ReleaseKey, REM_CHDOWN );

#if 0	// no need
		if( m_scrBuf )
			free( m_scrBuf );
		m_scrBuf = OsdCompress( m_x, m_y, m_w, m_h, &m_scrBufSize );
#endif
		if( m_timerId != (-1) ) {
			WiTimer_Delete( m_timerId );
			m_timerId = -1;
		}
		if( m_constant ) {
			m_tick = 0;
			m_constant = 0;
		} else {
			m_tick = m_timeout;
		}
		m_active = true; 	// set upper then Hide();
		m_drawing = true;	// set to false when receive APPLMSG_ALARM_DB_Changed because PCMD_GetAlarm is lastest message.

		DrawBack();
		if( DrawInfo() == false ) {
			Hide();
		} else {
			SendCommand( PCMD_GetAlarm );
			m_timerId = WiTimer_EveryQueue( 100, WiMainQ );
		}
	}
}

void C_InfoBox::Hide()
{
	if( !m_active ) return;

	if( m_timerId != (-1) ) {
		WiTimer_Delete( m_timerId );
		m_timerId = -1;
	}
	if( m_detailBuf ) {
		OsdDecompress( m_detailBuf, m_dx, m_dy, m_dw, m_dh );

		free( m_detailBuf );
		m_detailBuf = NULL;
	}
	if( m_scrBuf ) {
		OsdDecompress( m_scrBuf, m_x, m_y, m_w, m_h );

		free( m_scrBuf );
		m_scrBuf = NULL;
	} else {
		FillBox( m_x, m_y, m_w, m_h, 0 );
	}

	RestoreState( STATEIDX_Sub );
	SendCommand( PCMD_ReleaseKey, (dword)-1 );

	SendCommand( PCMD_BlockKey, REM_INFO, Rem_Monitoring );
	SendCommand( PCMD_BlockKey, REM_UP, Rem_Monitoring );
	SendCommand( PCMD_BlockKey, REM_DOWN, Rem_Monitoring );

	m_active = false;
}

void C_InfoBox::OnTimeout( int id )
{
	//Printf("m_timerId: %08x/%08x, state: %d\n", id, m_timerId, GetState(STATEIDX_Sub));

	if( id == m_timerId ) {
		if( m_active ) {
			if( GetState(STATEIDX_Sub) != SUBSTATE_InfoBox )	// other substate.
				return;
			if( --m_tick == 0 )
				return Hide();

			DrawPeriodicItems();
		}
	}
}

void C_InfoBox::RestartTimer()	// WiTimer Tick is checked as a system time. so if system time is changed to other offset it have to be restarted.
{
	if( !m_active ) return;

	if( m_timerId != (-1) ) {
		WiTimer_Delete( m_timerId );
		m_timerId = WiTimer_EveryQueue( 100, WiMainQ );

		DrawPeriodicItems();
	}
}

void C_InfoBox::EvtData( word satIdx, word orgNetId, word tsId, word svcId )
{
	dword no, ctime;

	Printf("EvtData: %04x / %04x / %04x / %04x\n", satIdx, orgNetId, tsId, svcId);

	if( !m_active ) goto remove;
	if( m_svcIdx == (-1) ) goto remove;

	if( (m_svc->svcId != svcId) || (m_tp->tsId != tsId) \
		|| (m_tp->orgNetId != orgNetId) || (m_tp->satIdx != satIdx) )
		goto remove;

	if( m_evtInfo ) {
		free( m_evtInfo );
		m_evtInfo = NULL;
	}
	m_evtCnt = 0;
	m_orgEvtInfo = m_curEvtInfo = NULL;

	ReadEventData( satIdx, orgNetId, tsId, svcId, &m_evtCnt, &m_evtInfo );

	ctime = GetCurTime();		// (mjd<<16) | (hour<<8) | min
	for( no=0; no<m_evtCnt; no++ )
	{
		if( (m_evtInfo[no].startTime <= ctime) && (m_evtInfo[no].endTime > ctime) ) {
			m_curEvtInfo = &m_evtInfo[no];
			break;
		}
	}

	DrawEvtName();
	DrawEvtProgress();

	if( 0 ) {
remove:
		RemoveEventData( satIdx, orgNetId, tsId, svcId );
	}

}

inline char *skips(const char *s) {
	if((byte)*s > ' ') return (char *)s;
	while(*s && (byte)*s <= ' ') s++;
	return (char *)s;
}

void CfgParse( S_InfoCfg &cfg )
{
	FILE *fp;
	char *buf, *pos;

	cfg.timeout = INFO_TIMEOUT_DEF;	// def value.
	cfg.navigation = true;				// def value.
	memset(cfg.code, 0, MAX_CODE_STRING);

	buf = GetStrBuf();	// len- 256

	fp = fopen( INFO_CFGFILE, "r" );
	if( fp ) {
		while( 1 )
		{
			if( fgets(buf, 128, fp) == NULL ) break;

			pos = skips( buf );
			switch( pos[0] )
			{
				case 'T' :
					pos = index(pos, ':');
					if( pos ) {
						pos = skips(pos+1);
						sscanf( pos,"%d", &cfg.timeout );
					}
					break;
				case 'N' :
					pos = index(pos, ':');
					if( pos ) {
						pos = skips(pos+1);
						sscanf( pos,"%d", &cfg.navigation );
					}
					break;
				case 'C' :
					pos = index(pos, ':');
					if( pos ) {
						pos = skips(pos+1);
						sscanf( pos,"%s", cfg.code );
					}
					break;
				default :
					break;
			}
		}

		fclose(fp);
	}

	if( (cfg.timeout < 0) || (cfg.timeout > INFO_TIMEOUT_MAX) ) {
		cfg.timeout = INFO_TIMEOUT_DEF;
	}

	Printf("timeout: %d. navigation: %d\n", cfg.timeout, cfg.navigation);
}

