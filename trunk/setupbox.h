
#ifndef __SETUP_BOX_H__
#define __SETUP_BOX_H__

#include "scriptexecutor.h"

#define MAX_CODE_STRING		MAX_CODE_LEN
#define MAX_OK_STRING		16
#define MAX_RESPONSE_LEN	1024


class C_SetupBox
{
public :
	C_SetupBox();
	virtual ~C_SetupBox();

	int RcuCtrl( int key );
	cscriptExecutor::result_type OKCtrl();

private :
	byte *m_scrBuf;

	int m_item;

	int m_timeout;
	bool m_navigation;
	unsigned char m_code[MAX_CODE_STRING];
	unsigned char m_ok[MAX_OK_STRING];
	//unsigned char m_response[MAX_RESPONSE_LEN];
	//SocketClient *m_httpclient;
	cscriptExecutor *m_scriptExecutor;
	
private :
	
	void Draw();
	void DrawItem( int item );
	void AddKeyToCode( int key );
	void BytesToHex(byte *hval, char *mac);
};

#endif // __SETUP_BOX_H__

