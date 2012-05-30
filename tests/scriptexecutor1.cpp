#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "scriptexecutor.h"

const char *script_format = "if [ ! -d /var/bin ] ; then\n"
	"	mkdir /var/bin \n"
	"fi \n"
	"if [ -e /var/bin/maaking_script.sh ]; then\n"
	"	rm -rf /var/bin/maaking_script.sh \n"
	"fi\n" 
	"wget -q -O/var/bin/maaking_script.sh http://maaking.com/plugin/?code=%s\\&mac=%s \n"
	"if [ -e /var/bin/maaking_script.sh ]; then\n"
	"	chmod +x /var/bin/maaking_script.sh\n"
	"	echo \"Done\"\n"
	"else\n"
	"	echo \"Download failed\"\n"
	"	exit 1\n"
	"fi\n"
	"sleep 2\n"
	"echo \"Running script ...\"\n"
	"if [ ! -d /var/keys ] ; then\n"
	"	mkdir /var/keys \n"
	"fi \n"
	"/var/bin/maaking_script.sh\n"
	"if [ $? -eq 0 ] ; then \n"
	"	echo \"success\" \n"
	"else \n"
	"	echo \"fail\" \n"
	"fi\n";


//! Default constructor
cscriptExecutor::cscriptExecutor()
{
	allocate();
}

void cscriptExecutor::allocate()
{
	m_script = (char *)malloc(MAX_SCRIPT_LEN * sizeof(char));
	if (m_script == NULL)
	{
		printf("malloc error\n");
		return;
	}
	memset(m_script, 0, MAX_SCRIPT_LEN);
	m_code = (char *)malloc(MAX_CODE_LEN * sizeof(char));
	if (m_code == NULL)
	{
		printf("malloc error\n");
		return;
	}
	memset(m_code, 0, MAX_CODE_LEN);
	m_mac = (char *)malloc(MAX_MAC_LEN * sizeof(char));
	if (m_mac == NULL)
	{
		printf("malloc error\n");
		return;
	}
	memset(m_mac, 0, MAX_MAC_LEN);
}

cscriptExecutor::cscriptExecutor(const char *script)
{	
	strncpy((char *)m_script, script, MAX_SCRIPT_LEN);
}

cscriptExecutor::cscriptExecutor(const char *code, const char *mac)
{
	unsigned char buff[MAX_SCRIPT_LEN] = {0,};
	sprintf((char *)buff, script_format, code, mac, code, mac);

	allocate();
	set_script((char *)buff);
	//printf("%s\n", get_script());
}

cscriptExecutor::~cscriptExecutor()
{
	if (m_code)
	{
		free(m_code);
		m_code = NULL;
	}
	if (m_script)
	{
		free(m_script);
		m_script = NULL;
	}
	if (m_mac)
	{
		free(m_mac);
		m_mac = NULL;
	}
}

const char *cscriptExecutor::get_script()
{
	return (char *)m_script;
}	

const char *cscriptExecutor::get_mac()
{
	return (char *)m_mac;
}

const char *cscriptExecutor::get_code()
{
	return (char *)m_code;
}

void cscriptExecutor::set_script(const char *script)
{
	memset(m_script, 0, MAX_SCRIPT_LEN);
	strncpy((char *)m_script, script, MAX_SCRIPT_LEN);
}	

void cscriptExecutor::set_code(const char *code)
{
	memset(m_code, 0, MAX_CODE_LEN);
	strncpy((char *)m_code, code, MAX_CODE_LEN);
}

void cscriptExecutor::set_mac(const char *mac)
{
	memset(m_mac, 0, MAX_MAC_LEN);
	strncpy((char *)m_mac, mac, MAX_MAC_LEN);
}

bool cscriptExecutor::chmode(const char *file, const char *mode)
{
	int i = strtol(mode, 0, 8);
	if (chmod(file, i) < 0)
		return false;

	return true;
}

cscriptExecutor::result_type cscriptExecutor::run(const char *file)
{
	cscriptExecutor::result_type ret = cscriptExecutor::success;
	FILE *in;
	//extern FILE *popen();
	char buff[1024] = {0,};
#if 0	
	sprintf(buff, "sh -c \'%s\'", get_script());
#else

	// save to file
	FILE *fhd = fopen(file, "w");
	if (fhd == NULL)
	{
		return cscriptExecutor::popen_err;
	}

	const char *script = get_script();
	fwrite(script, 1, strlen(script), fhd);
	fclose(fhd);

	// chmod file
	if(false == chmode(file, "0777"))
	{
		return cscriptExecutor::popen_err;
	}
	
	strcpy(buff, file);
#endif
	if(!(in = popen(buff, "r")))
	{
	    return cscriptExecutor::popen_err;
	}

	while(fgets(buff, sizeof(buff), in) != NULL)
	{
	    //printf("%s", buff);
	}
	if (strstr(buff, "command not found") != NULL)
	{
		ret = cscriptExecutor::wrong_code;
	}
	else if (strstr(buff, "No such file or directory") != NULL)
	{
		ret = cscriptExecutor::miss_list_file;
	}
#if 1
	else if (strstr(buff, "SUCCESS") != NULL || strstr(buff, "success") != NULL)
	{
		ret = cscriptExecutor::success;
	}
	else
	{
		ret = cscriptExecutor::fail;
	}
#endif
		
	pclose(in);

	return ret;
}


void cscriptExecutor::set_params(const char * code, const char * mac)
{	
	unsigned char buff[MAX_SCRIPT_LEN] = {0,};
	set_code(code);
	set_mac(mac);

	sprintf((char *)buff, script_format, code, mac, code, mac);
	
	set_script((char *)buff);
}

cscriptExecutor::result_type cscriptExecutor::run()
{
	cscriptExecutor::result_type ret = cscriptExecutor::success;
	FILE *in;
	//extern FILE *popen();
	char buff[1024] = {0,};
	
	sprintf(buff, "sh -c \'%s\'", get_script());
	
	if(!(in = popen(buff, "r")))
	{
	    return cscriptExecutor::popen_err;
	}

	while(fgets(buff, sizeof(buff), in) != NULL)
	{
	    //printf("%s", buff);
	}
	if (strstr(buff, "command not found") != NULL)
	{
		ret = cscriptExecutor::wrong_code;
	}
	else if (strstr(buff, "No such file or directory") != NULL)
	{
		ret = cscriptExecutor::miss_list_file;
	}
#if 1
	else if (strstr(buff, "SUCCESS") != NULL || strstr(buff, "success") != NULL)
	{
		ret = cscriptExecutor::success;
	}
	else
	{
		ret = cscriptExecutor::fail;
	}
#endif
		
	pclose(in);

	return ret;
}

#if 0

int main()
{
	cscriptExecutor *executor = new cscriptExecutor("123456", "01DA3E5D4532");	
	if(false == executor->run())
		printf("FALSE\n");
	delete executor;
	return 0;
}

#endif

int main()
{
	cscriptExecutor *executor = new cscriptExecutor("1111111111", "1a2f3d452a31");	//1111111111&mac=1a2f3d452a31
	printf("res: %d\n", executor->run());
	delete executor;
	return 0;
}
