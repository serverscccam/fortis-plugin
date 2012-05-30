
/* ibox.cpp by GED @ fortis */

//#define DEBUG

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "plugin.h"
#include "setupbox.h"
#include "infobox.h"

int _ibox_pid = 0;

static C_InfoBox *_infobox = NULL;
static C_SetupBox *_setupbox = NULL;

void PluginClose()
{
	delete _infobox;

	SendCommand( PCMD_NormalUnit, 0, UNIT_InfoBox );		// release org infobox.
	SendCommand( PCMD_NormalUnit, 0, UNIT_DirectBar );		// release simple svclist.

	WiTask_Sleep(200);

	FontClose();
	WiFront_Term();
}

int PluginMain( int argc, char *argv[] )
{
	int key, mainState = STATE_None;
	dword evt[4] = {0,};

	WiFront_Init();
	FontInit( "sfam.ttf" );

	SendCommand( PCMD_NormalUnit, 1, UNIT_InfoBox );		// block org infobox.
	SendCommand( PCMD_NormalUnit, 1, UNIT_DirectBar );		// block simple svclist.

	_infobox = new C_InfoBox();

	mainState = GetState(STATEIDX_Main);

	if( mainState == STATE_Normal ) {	// live mode
		SendCommand( PCMD_NotifyMsg, APPLMSG_Destroy );
	}

	_ibox_pid = getpid();

	SendCommand( PCMD_BlockKey, REM_INFO, Rem_Monitoring );
	SendCommand( PCMD_BlockKey, REM_UP, Rem_Monitoring );
	SendCommand( PCMD_BlockKey, REM_DOWN, Rem_Monitoring );

	while( 1 )
	{
		if( WiQueue_Receive(WiMainQ, (u_int8 *)evt, 16, -1) != 0 ) continue;

		if( (evt[0] & MSG_BASE_MASK) == MSG_BASE_REMOCON ) {

			key = evt[1] & MSG_MASK_KEY_CODE;

			if( _setupbox != NULL ) {
				if( _setupbox->RcuCtrl(key) ) {
					delete _setupbox;
					_setupbox = NULL;

					_infobox->ReadCfg();
				}
			} else {
				if( _infobox->IsActive() ) {
					_infobox->RcuCtrl(key);
				} else {
					if( (mainState == STATE_Normal) && (GetState(STATEIDX_Sub) == SUBSTATE_None) \
						&& ((key == REM_INFO) || (key == REM_UP) || (key == REM_DOWN)) \
						) {
						_infobox->SetConstant();	// no timeout
						/* close all osd popup items and set substate to SUBSTATE_InfoBox (to avoid osd collision). ex. subtitle */
						SendCommand( PCMD_DestroyAndSaveSubState, SUBSTATE_InfoBox );
					}
				}
			}
		}
		else if( evt[0] == PLUGINMSG_StateChangeStart ) {

			if( mainState != evt[1] )
			{
				if( evt[1] != STATE_Normal ) {
					_infobox->Hide();
				}
			}
		}
		else if( evt[0] == PLUGINMSG_StateChanged ) {

			if( mainState != evt[1] )
			{
				mainState = evt[1];
				if( mainState == STATE_Normal ) {	// live mode
					SendCommand( PCMD_DestroyAndSaveSubState, SUBSTATE_InfoBox );
				}
			} else {	// substate is changed.
				if( evt[2] == SUBSTATE_InfoBox )
				{
					if( mainState == STATE_Normal ) {
						_infobox->Draw();
					} else {
						RestoreState(STATEIDX_Sub);	// exception control
					}
				}
			}
		}
		else if( evt[0] == PLUGINMSG_SVC_Starting ) {

			if( (mainState == STATE_Normal) && (evt[1] == SERVICE_Main) ) {	// live mode
				if( _infobox->IsActive() )
					_infobox->Draw();
				else
					SendCommand( PCMD_DestroyAndSaveSubState, SUBSTATE_InfoBox );
			}
		}
		else if( evt[0] == APPLMSG_ALARM_DB_Changed ) {
			_infobox->DrawEvtName();
			_infobox->DrawIsDone();
		}
		else if( evt[0] == APPLMSG_ALARM_Set_Fail ) {
			;	// PCMD_SetAlarm ack.
		}
		else if( evt[0] == APPLMSG_EPG_Data_Ack ) {
			_infobox->EvtData( (evt[1]>>16)&0xFFFF, evt[1]&0xFFFF, (evt[2]>>16)&0xFFFF, evt[2]&0xFFFF );
		}
		else if( evt[0] == APPLMSG_PluginSelectedCmd ) {	// press ok on started plugin item in plugin menu.

			if( evt[1] == _ibox_pid ) {
				if( (GetState(STATEIDX_Sub) == SUBSTATE_None) && (_setupbox == NULL) ) {
					_setupbox = new C_SetupBox();
				}
			}
		}
		else if( evt[0] == APPLMSG_Destroy ) {	// when alarm confirm box open or firmware download box open.. and so on.

			if( _setupbox ) {
				delete _setupbox;
				_setupbox = NULL;
			}
			_infobox->Hide();	// for release key.
		}
		else if( evt[0] == APPLMSG_SI_TDT )
		{
			_infobox->RestartTimer();
		}
		else if( (evt[0] & MSG_BASE_MASK) == MSG_BASE_TIMER )	// _infobox->Draw();
		{
			_infobox->OnTimeout( evt[0] );
		}
#if 0	// use timer- C_InfoBox::DrawPeriodicItems()
		else if( (evt[0] == APPLMSG_TUNER_LOCK(0)) || (evt[0] == APPLMSG_TUNER_LOCK(1)) || (evt[0] == APPLMSG_TUNER_UNLOCK(0)) || (evt[0] == APPLMSG_TUNER_UNLOCK(1)) )
		{
			_infobox->DrawSignal();
		}
#endif
	}
}


