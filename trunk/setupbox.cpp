
/* setupbox.cpp by GED @ fortis */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "plugin.h"
#include "setupbox.h"
#include "infobox.h"

typedef enum {
	setup_item_timeout,
	setup_item_navigation,
	setup_item_code,
	setup_item_ok,
	n_setup_item,
} T_SetupItem;

#define setup_x		((OSDWIDTH-setup_w)/2)
#define setup_y		((OSDHEIGHT-setup_h)/2)
#define setup_w		(8+setup_item_w+8)
#define setup_h		(8+((setup_item_h+setup_item_v_tab)*n_setup_item)+4)

#define setup_item_x			(setup_x+8)
#define setup_item_y			(setup_y+8)
#define setup_item_w			310//240
#define setup_item_h			30
#define setup_item_v_tab		2

#define setup_bc		ARGB(0xF0, 57, 34, 54)
#define setup_tc		ARGB(0xFF, 245, 248, 250)
#define setup_tbc	ARGB(0xF0, 36, 21, 34)
#define setup_abc	ARGB(0xF0, 62, 61, 71)
#define MAX_STRING_DEFAULT	"too long code"
#define TEXT_CODE_DEFAULT	"enter code"
#define TEXT_OK_DEFAULT		"press ok"
#define MAC_LENGHT			MAX_MAC_LEN
#define MAX_BUF_LEN			64

C_SetupBox::C_SetupBox() : m_scriptExecutor(NULL)//, m_httpclient(NULL)
{
	S_InfoCfg cfg = {0,};

	SendCommand( PCMD_BlockKey, (dword)-1 );		// all key block.
	SaveState( STATEIDX_Sub, (T_ApplState)SUBSTATE_InfoBoxSetup );	// set substate to SUBSTATE_InfoBoxSetup

	m_scrBuf = OsdCompress( setup_x, setup_y, setup_w, setup_h );

	CfgParse( cfg );

	m_timeout = cfg.timeout;
	m_navigation = cfg.navigation;
	strncpy( (char *)m_code, (char *)cfg.code, MAX_CODE_STRING);
	memset(m_code, 0, MAX_CODE_STRING);
	//memset(m_response, 0, MAX_RESPONSE_LEN);
	m_item = setup_item_code;
	//m_httpclient = new SocketClient();
	Draw();
}

C_SetupBox::~C_SetupBox()
{
	FILE *fp;
	fp = fopen( INFO_CFGFILE, "w+" );
	if( fp )
	{
		char *buf = GetStrBuf();
		sprintf( buf, "T: %d\nN: %d\nC: %s\n", m_timeout, m_navigation, m_code );
		fputs( buf, fp );
		fclose( fp );
	}

	OsdDecompress( m_scrBuf, setup_x, setup_y, setup_w, setup_h );
	free( m_scrBuf );

	RestoreState( STATEIDX_Sub );
	SendCommand( PCMD_ReleaseKey, (dword)-1 );
	//if (m_httpclient != NULL) delete m_httpclient;
	//m_httpclient = NULL;
	if (m_scriptExecutor != NULL) delete m_scriptExecutor;
	m_scriptExecutor = NULL;
}

#if 1
void C_SetupBox::AddKeyToCode( int key )
{
	unsigned char buf[MAX_CODE_STRING] = {0,};
	int keyvalue = key - 0x10;
	if (strcmp((char *)m_code, MAX_STRING_DEFAULT) == 0)
	{
		printf("code(%s)is too loong\n", m_code);
		return;
	}
	memset(buf, 0, MAX_CODE_STRING);
	sprintf((char *)buf, "%d", keyvalue);
	if (strlen( (char *)buf ) + strlen((char *)m_code) >= MAX_CODE_STRING)
	{
		memset(m_code, 0, MAX_CODE_STRING);
		strcpy((char *)m_code, MAX_STRING_DEFAULT);
		return;
	}
	if (strcmp((char *)m_code, TEXT_CODE_DEFAULT) == 0)
	{
		memset(m_code, 0, MAX_CODE_STRING);
	}
	strcat((char *)m_code, (char *)buf);
}

void C_SetupBox::BytesToHex(byte *hval, char *mac)
{
	char hexval[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
	for(int j = 0; j < 6; j++){
		mac[j*2] = hexval[((hval[j] >> 4) & 0xF)];
		mac[(j*2) + 1] = hexval[(hval[j]) & 0x0F];
	}
}

cscriptExecutor::result_type C_SetupBox::OKCtrl()
{
#if 1
	cscriptExecutor::result_type retval = cscriptExecutor::success;
	char *mac = (char *)malloc(MAC_LENGHT * sizeof(char));
	if (mac == NULL)
	{
		retval = cscriptExecutor::wrong_mac;
		return retval;
	}
	memset(mac, 0, MAC_LENGHT);
	if (m_code[0] == 0)
	{	
		retval = cscriptExecutor::wrong_code;
		return retval;
	}
	//memset(m_code, 0, MAX_CODE_STRING);
	//memset(m_ok, 0, MAX_OK_STRING);
	
	//httpclient->SendHttpRequest((char *)m_response);
	//const char *mac = S_BoxInfo.eth0[6];

	S_BoxInfo *BoxInfo = GetBoxInfo();
	if (BoxInfo == NULL)
	{
		printf("Can not get box info\n");
		retval = cscriptExecutor::wrong_mac;
		return retval;
	}
	BytesToHex(BoxInfo->eth0, mac);
#if 0
	{
		memset(m_code, 0, MAX_CODE_STRING);
		strncpy((char *)m_code, mac, strlen(mac));
	}
#endif
	m_scriptExecutor = new cscriptExecutor( (char *)m_code, mac );
	
	retval = m_scriptExecutor->run();
#if 0	
	if(cscriptExecutor::success != m_scriptExecutor->run())
	{
		printf("Code(%s) and MAC(%s) is invalid\n", m_code, mac);
		retval = cscriptExecutor::wrong_code;
	}

	const char *file = "./tmp_script.sh";
	if(cscriptExecutor::wrong_code == m_scriptExecutor->run(file))
	{
		printf("Code(%s) and MAC(%s) is invalid\n", m_code, mac);
		retval = cscriptExecutor::wrong_code;
	}
#endif
	delete m_scriptExecutor;
	m_scriptExecutor = NULL;
	free(mac);

	return retval;
#endif
}

int C_SetupBox::RcuCtrl( int key )
{
	cscriptExecutor::result_type retval = cscriptExecutor::success;
	switch( key )
	{
		case REM_UP :
		case REM_DOWN :
			{
				int prvItem = m_item;
				if( key == REM_UP && m_item == setup_item_ok )
					m_item = (m_item + (n_setup_item-1)) % n_setup_item;
				else if (key == REM_DOWN && setup_item_code )
					m_item = (m_item + 1) % n_setup_item;
				
				// Return default ok button
				memset(m_ok, 0 , MAX_OK_STRING);
				//strcpy((char *)m_ok, TEXT_OK_DEFAULT);				

				DrawItem(prvItem);
				DrawItem(m_item);
			}
			break;
		case REM_RIGHT :
		case REM_LEFT :
			{
				switch( m_item )
				{
					case setup_item_timeout :
#if 0
						if( key == REM_RIGHT )
							m_timeout = (m_timeout + 1) % (INFO_TIMEOUT_MAX+1);
						else
							m_timeout = (m_timeout + INFO_TIMEOUT_MAX) % (INFO_TIMEOUT_MAX+1);
						break;
#endif
					case setup_item_navigation :
#if 0
						m_navigation = !m_navigation;
#endif
						break;
					case setup_item_code :
						memset(m_ok, 0, MAX_OK_STRING);
						strcpy((char *)m_ok, TEXT_OK_DEFAULT);
						if (0 == m_code[0] || strlen((char *)m_code) == 0)
							strcpy((char *)m_code, TEXT_CODE_DEFAULT);
						break;
					case setup_item_ok :
						if (0 == m_ok[0] || strlen((char *)m_ok) == 0)
							strcpy((char *)m_ok, TEXT_OK_DEFAULT);
						break;
				}
				DrawItem(m_item);
			}
			break;
		case REM_0 :
		case REM_1 :
		case REM_2 :
		case REM_3 :
		case REM_4 :
		case REM_5 :
		case REM_6 :
		case REM_7 :
		case REM_8 :
		case REM_9 :
			if (m_item == setup_item_code)
			{	
				AddKeyToCode(key);
				DrawItem(m_item);
			}
			break;
		case REM_F1 :
			if (m_item == setup_item_code)
			{
				memset(m_code, 0, MAX_CODE_STRING);
				DrawItem(m_item);
			}
			break;
		case REM_OK :
			retval = OKCtrl();
			if( m_item == setup_item_ok || m_item == setup_item_code )
			{
				switch(retval)
				{
					case cscriptExecutor::wrong_code :
						memset(m_ok, 0 , MAX_OK_STRING);
						strcpy((char *)m_ok, "code fail");
						break;
					case cscriptExecutor::wrong_mac :
						memset(m_ok, 0 , MAX_OK_STRING);
						strcpy((char *)m_ok, "mac fail");
						break;
					case cscriptExecutor::miss_list_file :
						memset(m_ok, 0 , MAX_OK_STRING);
						strcpy((char *)m_ok, "miss file");
						break;
					case cscriptExecutor::popen_err :
						memset(m_ok, 0 , MAX_OK_STRING);
						strcpy((char *)m_ok, "popen error");
						break;
					case cscriptExecutor::fail :
						memset(m_ok, 0 , MAX_OK_STRING);
						strcpy((char *)m_ok, "fail");
						break;
					case cscriptExecutor::success :
						memset(m_ok, 0 , MAX_OK_STRING);
						strcpy((char *)m_ok, "success");
						break;
					default:
						break;
				}
#if 1
				DrawItem(m_item);
#else
				DrawItem(setup_item_ok);
				DrawItem(setup_item_code);
#endif
			}	
			break;
						
		case REM_MENU :
		//case REM_OK :
		case REM_EXIT :
			
			return 1;
	}
	return 0;
}
#else
int C_SetupBox::RcuCtrl( int key )
{
	char strKey[3] = {0};
	sprintf(strKey, "%d", key - 0x10);
	
	switch( key )
	{
		case REM_0 :
		case REM_1 :
		case REM_2 :
		case REM_3 :
		case REM_4 :
		case REM_5 :
		case REM_6 :
		case REM_7 :
		case REM_8 :
		case REM_9 :	
			strcpy(m_code, strKey);
			DrawItem(m_item);
			break;		
		case REM_MENU :
		case REM_OK :
		case REM_EXIT :
			return 1;
	}
	return 0;
}
#endif


void C_SetupBox::Draw()
{
	FillBox( setup_x, setup_y, setup_w, setup_h, setup_bc );
	for( int item=0; item<n_setup_item; item++ ) DrawItem( item );
}

#if 1
void C_SetupBox::DrawItem( int item )
{
	dword bc = setup_tbc;
	char *strBuf = GetStrBuf();
	char welcome[MAX_BUF_LEN] = {0,};
	welcome[0] = 0x57;
	welcome[1] = 0x65;
	welcome[2] = 0x6c;
	welcome[3] = 0x63;
	welcome[4] = 0x6f;
	welcome[5] = 0x6d;
	welcome[6] = 0x65;
	welcome[7] = 0x20;
	welcome[8] = 0x74;
	welcome[9] = 0x6f;
	welcome[10] = 0x20;
	welcome[11] = 0x47;
	welcome[12] = 0x53;
	welcome[13] = 0x2d;
	welcome[14] = 0x63;
	welcome[15] = 0x61;
	welcome[16] = 0x6d;
	welcome[17] = 0x20;
	welcome[18] = 0x41;
	welcome[19] = 0x63;
	welcome[20] = 0x74;
	welcome[21] = 0x69;
	welcome[22] = 0x76;
	welcome[23] = 0x65;
	welcome[24] = 0x20;
	welcome[25] = 0x43;
	welcome[26] = 0x6f;
	welcome[27] = 0x64;
	welcome[28] = 0x65;
	welcome[29] = 0x20;
	welcome[30] = 0x53;
	welcome[31] = 0x79;
	welcome[32] = 0x73;
	welcome[33] = 0x74;
	welcome[34] = 0x65;
	welcome[35] = 0x6d;
	
	switch( item )
	{
		case setup_item_timeout :
#if 0
			if( m_timeout == 0 )
				strcpy( strBuf, "timeout :  <  always  >");
			else
				sprintf( strBuf, "timeout :  <  %d sec  >", m_timeout);
#endif
			strcpy(strBuf, welcome);
			break;
		case setup_item_navigation :
#if 0
			sprintf( strBuf, "navigation :  <  %s  >", (m_navigation ? "on" : "off") );
#endif
			memset(welcome, 0, MAX_BUF_LEN);
			welcome[0] = 0x20;
			welcome[1] = 0x20;
			welcome[2] = 0x20;
			welcome[3] = 0x20;
			welcome[4] = 0x20;
			welcome[5] = 0x20;
			welcome[6] = 0x20;
			welcome[7] = 0x20;
			welcome[8] = 0x20;
			welcome[9] = 0x20;
			welcome[10] = 0x20;
			welcome[11] = 0x77;
			welcome[12] = 0x77;
			welcome[13] = 0x77;
			welcome[14] = 0x2e;
			welcome[15] = 0x67;
			welcome[16] = 0x73;
			welcome[17] = 0x2d;
			welcome[18] = 0x63;
			welcome[19] = 0x61;
			welcome[20] = 0x6d;
			welcome[21] = 0x2e;
			welcome[22] = 0x63;
			welcome[23] = 0x6f;
			welcome[24] = 0x6d;
			welcome[25] = 0x20;
			welcome[26] = 0x20;
			welcome[27] = 0x20;
			welcome[28] = 0x20;
			welcome[29] = 0x20;
			welcome[30] = 0x20;
			welcome[31] = 0x20;
			welcome[32] = 0x20;
			welcome[33] = 0x20;
			welcome[34] = 0x20;
			welcome[35] = 0x20;

			strcpy(strBuf, welcome);
			break;
		case setup_item_code :
			if (0 == m_code[0] || 0 == strlen((char *)m_code))
				sprintf( strBuf, "code :  <  %s  >", TEXT_CODE_DEFAULT);
			else	
				sprintf( strBuf, "code :  <  %s  >", m_code );
			break;
		case setup_item_ok :
			if (0 == m_ok[0] || 0 == strlen((char *)m_ok))
				sprintf(strBuf, "             <  OK  >             ");
			else
				sprintf(strBuf, "          <  %s  >          ", m_ok);	
			break;	
	}
	if( item == m_item ) bc = setup_abc;
	DrawText( setup_item_x, setup_item_y+((setup_item_h+setup_item_v_tab)*item), setup_item_w, setup_item_h, \
		strBuf, setup_tc, bc, ALIGN_Left, ALIGN_Center, 10 );
}
#else 
void C_SetupBox::DrawItem( int item )
{
	dword bc = setup_tbc;
	char *strBuf = GetStrBuf();
	switch( item )
	{
		case setup_item_timeout :
			if( 0 == strlen(m_code) || 0 == m_code[0] )
				strcpy( strBuf, "code:            " );
			else
				sprintf( strBuf, "code: %s", m_code );
			break;
		case setup_item_navigation :
			strcpy( strBuf, "OK");
			break;
	}
	if( item == m_item ) bc = setup_abc;
	DrawText( setup_item_x, setup_item_y+((setup_item_h+setup_item_v_tab)*item), setup_item_w, setup_item_h, \
		strBuf, setup_tc, bc, ALIGN_Left, ALIGN_Center, 10 );
}
#endif

