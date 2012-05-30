#ifndef __CSCRIPT_EXECUTOR_H__
#define __CSCRIPT_EXECUTOR_H__

#define MAX_SCRIPT_LEN 2048
#define MAX_MAC_LEN 16
#define MAX_CODE_LEN 128

class cscriptExecutor
{
public :
	typedef enum {
		wrong_code,
		wrong_mac,	
		miss_list_file,
		popen_err,
		fail,
		success
	} result_type;
		
	result_type run();
	result_type run(const char *file);
	cscriptExecutor();
	cscriptExecutor(const char *code, const char *mac);
	cscriptExecutor(const char *script);
	~cscriptExecutor();
	const char *get_script();
 	const char *get_mac();
	const char *get_code();
	void set_script ( const char *script );
 	void set_mac ( const char *mac );
	void set_code ( const char *code );
	void set_params( const char *code, const char *mac );

private :
	bool chmode(const char *file, const char *mode);
	void allocate();
	
private :
	char *m_script;
	char *m_mac;
	char *m_code;
};

#endif

