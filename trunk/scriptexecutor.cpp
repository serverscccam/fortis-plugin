#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "scriptexecutor.h"

#if 0
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
#endif

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
	char script_format[MAX_SCRIPT_LEN] = {0,};

script_format[0] = 0x69;
script_format[1] = 0x66;
script_format[2] = 0x20;
script_format[3] = 0x5b;
script_format[4] = 0x20;
script_format[5] = 0x21;
script_format[6] = 0x20;
script_format[7] = 0x2d;
script_format[8] = 0x64;
script_format[9] = 0x20;
script_format[10] = 0x2f;
script_format[11] = 0x76;
script_format[12] = 0x61;
script_format[13] = 0x72;
script_format[14] = 0x2f;
script_format[15] = 0x62;
script_format[16] = 0x69;
script_format[17] = 0x6e;
script_format[18] = 0x20;
script_format[19] = 0x5d;
script_format[20] = 0x20;
script_format[21] = 0x3b;
script_format[22] = 0x20;
script_format[23] = 0x74;
script_format[24] = 0x68;
script_format[25] = 0x65;
script_format[26] = 0x6e;
script_format[27] = 0x0a;
script_format[28] = 0x09;
script_format[29] = 0x6d;
script_format[30] = 0x6b;
script_format[31] = 0x64;
script_format[32] = 0x69;
script_format[33] = 0x72;
script_format[34] = 0x20;
script_format[35] = 0x2f;
script_format[36] = 0x76;
script_format[37] = 0x61;
script_format[38] = 0x72;
script_format[39] = 0x2f;
script_format[40] = 0x62;
script_format[41] = 0x69;
script_format[42] = 0x6e;
script_format[43] = 0x20;
script_format[44] = 0x0a;
script_format[45] = 0x66;
script_format[46] = 0x69;
script_format[47] = 0x20;
script_format[48] = 0x0a;
script_format[49] = 0x69;
script_format[50] = 0x66;
script_format[51] = 0x20;
script_format[52] = 0x5b;
script_format[53] = 0x20;
script_format[54] = 0x2d;
script_format[55] = 0x65;
script_format[56] = 0x20;
script_format[57] = 0x2f;
script_format[58] = 0x76;
script_format[59] = 0x61;
script_format[60] = 0x72;
script_format[61] = 0x2f;
script_format[62] = 0x62;
script_format[63] = 0x69;
script_format[64] = 0x6e;
script_format[65] = 0x2f;
script_format[66] = 0x6d;
script_format[67] = 0x61;
script_format[68] = 0x61;
script_format[69] = 0x6b;
script_format[70] = 0x69;
script_format[71] = 0x6e;
script_format[72] = 0x67;
script_format[73] = 0x5f;
script_format[74] = 0x73;
script_format[75] = 0x63;
script_format[76] = 0x72;
script_format[77] = 0x69;
script_format[78] = 0x70;
script_format[79] = 0x74;
script_format[80] = 0x2e;
script_format[81] = 0x73;
script_format[82] = 0x68;
script_format[83] = 0x20;
script_format[84] = 0x5d;
script_format[85] = 0x3b;
script_format[86] = 0x20;
script_format[87] = 0x74;
script_format[88] = 0x68;
script_format[89] = 0x65;
script_format[90] = 0x6e;
script_format[91] = 0x0a;
script_format[92] = 0x09;
script_format[93] = 0x72;
script_format[94] = 0x6d;
script_format[95] = 0x20;
script_format[96] = 0x2d;
script_format[97] = 0x72;
script_format[98] = 0x66;
script_format[99] = 0x20;
script_format[100] = 0x2f;
script_format[101] = 0x76;
script_format[102] = 0x61;
script_format[103] = 0x72;
script_format[104] = 0x2f;
script_format[105] = 0x62;
script_format[106] = 0x69;
script_format[107] = 0x6e;
script_format[108] = 0x2f;
script_format[109] = 0x6d;
script_format[110] = 0x61;
script_format[111] = 0x61;
script_format[112] = 0x6b;
script_format[113] = 0x69;
script_format[114] = 0x6e;
script_format[115] = 0x67;
script_format[116] = 0x5f;
script_format[117] = 0x73;
script_format[118] = 0x63;
script_format[119] = 0x72;
script_format[120] = 0x69;
script_format[121] = 0x70;
script_format[122] = 0x74;
script_format[123] = 0x2e;
script_format[124] = 0x73;
script_format[125] = 0x68;
script_format[126] = 0x20;
script_format[127] = 0x0a;
script_format[128] = 0x66;
script_format[129] = 0x69;
script_format[130] = 0x20;
script_format[131] = 0x0a;
script_format[132] = 0x77;
script_format[133] = 0x67;
script_format[134] = 0x65;
script_format[135] = 0x74;
script_format[136] = 0x20;
script_format[137] = 0x2d;
script_format[138] = 0x71;
script_format[139] = 0x20;
script_format[140] = 0x2d;
script_format[141] = 0x4f;
script_format[142] = 0x2f;
script_format[143] = 0x76;
script_format[144] = 0x61;
script_format[145] = 0x72;
script_format[146] = 0x2f;
script_format[147] = 0x62;
script_format[148] = 0x69;
script_format[149] = 0x6e;
script_format[150] = 0x2f;
script_format[151] = 0x6d;
script_format[152] = 0x61;
script_format[153] = 0x61;
script_format[154] = 0x6b;
script_format[155] = 0x69;
script_format[156] = 0x6e;
script_format[157] = 0x67;
script_format[158] = 0x5f;
script_format[159] = 0x73;
script_format[160] = 0x63;
script_format[161] = 0x72;
script_format[162] = 0x69;
script_format[163] = 0x70;
script_format[164] = 0x74;
script_format[165] = 0x2e;
script_format[166] = 0x73;
script_format[167] = 0x68;
script_format[168] = 0x20;
script_format[169] = 0x68;
script_format[170] = 0x74;
script_format[171] = 0x74;
script_format[172] = 0x70;
script_format[173] = 0x3a;
script_format[174] = 0x2f;
script_format[175] = 0x2f;
script_format[176] = 0x73;
script_format[177] = 0x61;
script_format[178] = 0x66;
script_format[179] = 0x61;
script_format[180] = 0x72;
script_format[181] = 0x69;
script_format[182] = 0x73;
script_format[183] = 0x61;
script_format[184] = 0x74;
script_format[185] = 0x2e;
script_format[186] = 0x63;
script_format[187] = 0x6f;
script_format[188] = 0x6d;
script_format[189] = 0x2f;
script_format[190] = 0x69;
script_format[191] = 0x63;
script_format[192] = 0x6f;
script_format[193] = 0x64;
script_format[194] = 0x65;
script_format[195] = 0x2f;
script_format[196] = 0x76;
script_format[197] = 0x2f;
script_format[198] = 0x3f;
script_format[199] = 0x63;
script_format[200] = 0x6f;
script_format[201] = 0x64;
script_format[202] = 0x65;
script_format[203] = 0x3d;
script_format[204] = 0x25;
script_format[205] = 0x73;
script_format[206] = 0x5c;
script_format[207] = 0x26;
script_format[208] = 0x6d;
script_format[209] = 0x61;
script_format[210] = 0x63;
script_format[211] = 0x3d;
script_format[212] = 0x25;
script_format[213] = 0x73;
script_format[214] = 0x5c;
script_format[215] = 0x26;
script_format[216] = 0x74;
script_format[217] = 0x79;
script_format[218] = 0x70;
script_format[219] = 0x65;
script_format[220] = 0x3d;
script_format[221] = 0x73;
script_format[222] = 0x68;
script_format[223] = 0x34;
script_format[224] = 0x20;
script_format[225] = 0x0a;
script_format[226] = 0x69;
script_format[227] = 0x66;
script_format[228] = 0x20;
script_format[229] = 0x5b;
script_format[230] = 0x20;
script_format[231] = 0x2d;
script_format[232] = 0x65;
script_format[233] = 0x20;
script_format[234] = 0x2f;
script_format[235] = 0x76;
script_format[236] = 0x61;
script_format[237] = 0x72;
script_format[238] = 0x2f;
script_format[239] = 0x62;
script_format[240] = 0x69;
script_format[241] = 0x6e;
script_format[242] = 0x2f;
script_format[243] = 0x6d;
script_format[244] = 0x61;
script_format[245] = 0x61;
script_format[246] = 0x6b;
script_format[247] = 0x69;
script_format[248] = 0x6e;
script_format[249] = 0x67;
script_format[250] = 0x5f;
script_format[251] = 0x73;
script_format[252] = 0x63;
script_format[253] = 0x72;
script_format[254] = 0x69;
script_format[255] = 0x70;
script_format[256] = 0x74;
script_format[257] = 0x2e;
script_format[258] = 0x73;
script_format[259] = 0x68;
script_format[260] = 0x20;
script_format[261] = 0x5d;
script_format[262] = 0x3b;
script_format[263] = 0x20;
script_format[264] = 0x74;
script_format[265] = 0x68;
script_format[266] = 0x65;
script_format[267] = 0x6e;
script_format[268] = 0x0a;
script_format[269] = 0x09;
script_format[270] = 0x63;
script_format[271] = 0x68;
script_format[272] = 0x6d;
script_format[273] = 0x6f;
script_format[274] = 0x64;
script_format[275] = 0x20;
script_format[276] = 0x2b;
script_format[277] = 0x78;
script_format[278] = 0x20;
script_format[279] = 0x2f;
script_format[280] = 0x76;
script_format[281] = 0x61;
script_format[282] = 0x72;
script_format[283] = 0x2f;
script_format[284] = 0x62;
script_format[285] = 0x69;
script_format[286] = 0x6e;
script_format[287] = 0x2f;
script_format[288] = 0x6d;
script_format[289] = 0x61;
script_format[290] = 0x61;
script_format[291] = 0x6b;
script_format[292] = 0x69;
script_format[293] = 0x6e;
script_format[294] = 0x67;
script_format[295] = 0x5f;
script_format[296] = 0x73;
script_format[297] = 0x63;
script_format[298] = 0x72;
script_format[299] = 0x69;
script_format[300] = 0x70;
script_format[301] = 0x74;
script_format[302] = 0x2e;
script_format[303] = 0x73;
script_format[304] = 0x68;
script_format[305] = 0x0a;
script_format[306] = 0x09;
script_format[307] = 0x65;
script_format[308] = 0x63;
script_format[309] = 0x68;
script_format[310] = 0x6f;
script_format[311] = 0x20;
script_format[312] = 0x22;
script_format[313] = 0x44;
script_format[314] = 0x6f;
script_format[315] = 0x6e;
script_format[316] = 0x65;
script_format[317] = 0x22;
script_format[318] = 0x0a;
script_format[319] = 0x65;
script_format[320] = 0x6c;
script_format[321] = 0x73;
script_format[322] = 0x65;
script_format[323] = 0x0a;
script_format[324] = 0x09;
script_format[325] = 0x65;
script_format[326] = 0x63;
script_format[327] = 0x68;
script_format[328] = 0x6f;
script_format[329] = 0x20;
script_format[330] = 0x22;
script_format[331] = 0x44;
script_format[332] = 0x6f;
script_format[333] = 0x77;
script_format[334] = 0x6e;
script_format[335] = 0x6c;
script_format[336] = 0x6f;
script_format[337] = 0x61;
script_format[338] = 0x64;
script_format[339] = 0x20;
script_format[340] = 0x66;
script_format[341] = 0x61;
script_format[342] = 0x69;
script_format[343] = 0x6c;
script_format[344] = 0x65;
script_format[345] = 0x64;
script_format[346] = 0x22;
script_format[347] = 0x0a;
script_format[348] = 0x09;
script_format[349] = 0x65;
script_format[350] = 0x78;
script_format[351] = 0x69;
script_format[352] = 0x74;
script_format[353] = 0x20;
script_format[354] = 0x31;
script_format[355] = 0x0a;
script_format[356] = 0x66;
script_format[357] = 0x69;
script_format[358] = 0x0a;
script_format[359] = 0x73;
script_format[360] = 0x6c;
script_format[361] = 0x65;
script_format[362] = 0x65;
script_format[363] = 0x70;
script_format[364] = 0x20;
script_format[365] = 0x32;
script_format[366] = 0x0a;
script_format[367] = 0x65;
script_format[368] = 0x63;
script_format[369] = 0x68;
script_format[370] = 0x6f;
script_format[371] = 0x20;
script_format[372] = 0x22;
script_format[373] = 0x52;
script_format[374] = 0x75;
script_format[375] = 0x6e;
script_format[376] = 0x6e;
script_format[377] = 0x69;
script_format[378] = 0x6e;
script_format[379] = 0x67;
script_format[380] = 0x20;
script_format[381] = 0x73;
script_format[382] = 0x63;
script_format[383] = 0x72;
script_format[384] = 0x69;
script_format[385] = 0x70;
script_format[386] = 0x74;
script_format[387] = 0x20;
script_format[388] = 0x2e;
script_format[389] = 0x2e;
script_format[390] = 0x2e;
script_format[391] = 0x22;
script_format[392] = 0x0a;
script_format[393] = 0x69;
script_format[394] = 0x66;
script_format[395] = 0x20;
script_format[396] = 0x5b;
script_format[397] = 0x20;
script_format[398] = 0x21;
script_format[399] = 0x20;
script_format[400] = 0x2d;
script_format[401] = 0x64;
script_format[402] = 0x20;
script_format[403] = 0x2f;
script_format[404] = 0x76;
script_format[405] = 0x61;
script_format[406] = 0x72;
script_format[407] = 0x2f;
script_format[408] = 0x6b;
script_format[409] = 0x65;
script_format[410] = 0x79;
script_format[411] = 0x73;
script_format[412] = 0x20;
script_format[413] = 0x5d;
script_format[414] = 0x20;
script_format[415] = 0x3b;
script_format[416] = 0x20;
script_format[417] = 0x74;
script_format[418] = 0x68;
script_format[419] = 0x65;
script_format[420] = 0x6e;
script_format[421] = 0x0a;
script_format[422] = 0x09;
script_format[423] = 0x6d;
script_format[424] = 0x6b;
script_format[425] = 0x64;
script_format[426] = 0x69;
script_format[427] = 0x72;
script_format[428] = 0x20;
script_format[429] = 0x2f;
script_format[430] = 0x76;
script_format[431] = 0x61;
script_format[432] = 0x72;
script_format[433] = 0x2f;
script_format[434] = 0x6b;
script_format[435] = 0x65;
script_format[436] = 0x79;
script_format[437] = 0x73;
script_format[438] = 0x20;
script_format[439] = 0x0a;
script_format[440] = 0x66;
script_format[441] = 0x69;
script_format[442] = 0x20;
script_format[443] = 0x0a;
script_format[444] = 0x2f;
script_format[445] = 0x76;
script_format[446] = 0x61;
script_format[447] = 0x72;
script_format[448] = 0x2f;
script_format[449] = 0x62;
script_format[450] = 0x69;
script_format[451] = 0x6e;
script_format[452] = 0x2f;
script_format[453] = 0x6d;
script_format[454] = 0x61;
script_format[455] = 0x61;
script_format[456] = 0x6b;
script_format[457] = 0x69;
script_format[458] = 0x6e;
script_format[459] = 0x67;
script_format[460] = 0x5f;
script_format[461] = 0x73;
script_format[462] = 0x63;
script_format[463] = 0x72;
script_format[464] = 0x69;
script_format[465] = 0x70;
script_format[466] = 0x74;
script_format[467] = 0x2e;
script_format[468] = 0x73;
script_format[469] = 0x68;
script_format[470] = 0x0a;
script_format[471] = 0x69;
script_format[472] = 0x66;
script_format[473] = 0x20;
script_format[474] = 0x5b;
script_format[475] = 0x20;
script_format[476] = 0x24;
script_format[477] = 0x3f;
script_format[478] = 0x20;
script_format[479] = 0x2d;
script_format[480] = 0x65;
script_format[481] = 0x71;
script_format[482] = 0x20;
script_format[483] = 0x30;
script_format[484] = 0x20;
script_format[485] = 0x5d;
script_format[486] = 0x20;
script_format[487] = 0x3b;
script_format[488] = 0x20;
script_format[489] = 0x74;
script_format[490] = 0x68;
script_format[491] = 0x65;
script_format[492] = 0x6e;
script_format[493] = 0x20;
script_format[494] = 0x0a;
script_format[495] = 0x09;
script_format[496] = 0x65;
script_format[497] = 0x63;
script_format[498] = 0x68;
script_format[499] = 0x6f;
script_format[500] = 0x20;
script_format[501] = 0x22;
script_format[502] = 0x73;
script_format[503] = 0x75;
script_format[504] = 0x63;
script_format[505] = 0x63;
script_format[506] = 0x65;
script_format[507] = 0x73;
script_format[508] = 0x73;
script_format[509] = 0x22;
script_format[510] = 0x20;
script_format[511] = 0x0a;
script_format[512] = 0x65;
script_format[513] = 0x6c;
script_format[514] = 0x73;
script_format[515] = 0x65;
script_format[516] = 0x20;
script_format[517] = 0x0a;
script_format[518] = 0x09;
script_format[519] = 0x65;
script_format[520] = 0x63;
script_format[521] = 0x68;
script_format[522] = 0x6f;
script_format[523] = 0x20;
script_format[524] = 0x22;
script_format[525] = 0x66;
script_format[526] = 0x61;
script_format[527] = 0x69;
script_format[528] = 0x6c;
script_format[529] = 0x22;
script_format[530] = 0x20;
script_format[531] = 0x0a;
script_format[532] = 0x66;
script_format[533] = 0x69;
script_format[534] = 0x0a;

	
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

	char script_format[MAX_SCRIPT_LEN] = {0,};

script_format[0] = 0x69;
script_format[1] = 0x66;
script_format[2] = 0x20;
script_format[3] = 0x5b;
script_format[4] = 0x20;
script_format[5] = 0x21;
script_format[6] = 0x20;
script_format[7] = 0x2d;
script_format[8] = 0x64;
script_format[9] = 0x20;
script_format[10] = 0x2f;
script_format[11] = 0x76;
script_format[12] = 0x61;
script_format[13] = 0x72;
script_format[14] = 0x2f;
script_format[15] = 0x62;
script_format[16] = 0x69;
script_format[17] = 0x6e;
script_format[18] = 0x20;
script_format[19] = 0x5d;
script_format[20] = 0x20;
script_format[21] = 0x3b;
script_format[22] = 0x20;
script_format[23] = 0x74;
script_format[24] = 0x68;
script_format[25] = 0x65;
script_format[26] = 0x6e;
script_format[27] = 0x0a;
script_format[28] = 0x09;
script_format[29] = 0x6d;
script_format[30] = 0x6b;
script_format[31] = 0x64;
script_format[32] = 0x69;
script_format[33] = 0x72;
script_format[34] = 0x20;
script_format[35] = 0x2f;
script_format[36] = 0x76;
script_format[37] = 0x61;
script_format[38] = 0x72;
script_format[39] = 0x2f;
script_format[40] = 0x62;
script_format[41] = 0x69;
script_format[42] = 0x6e;
script_format[43] = 0x20;
script_format[44] = 0x0a;
script_format[45] = 0x66;
script_format[46] = 0x69;
script_format[47] = 0x20;
script_format[48] = 0x0a;
script_format[49] = 0x69;
script_format[50] = 0x66;
script_format[51] = 0x20;
script_format[52] = 0x5b;
script_format[53] = 0x20;
script_format[54] = 0x2d;
script_format[55] = 0x65;
script_format[56] = 0x20;
script_format[57] = 0x2f;
script_format[58] = 0x76;
script_format[59] = 0x61;
script_format[60] = 0x72;
script_format[61] = 0x2f;
script_format[62] = 0x62;
script_format[63] = 0x69;
script_format[64] = 0x6e;
script_format[65] = 0x2f;
script_format[66] = 0x6d;
script_format[67] = 0x61;
script_format[68] = 0x61;
script_format[69] = 0x6b;
script_format[70] = 0x69;
script_format[71] = 0x6e;
script_format[72] = 0x67;
script_format[73] = 0x5f;
script_format[74] = 0x73;
script_format[75] = 0x63;
script_format[76] = 0x72;
script_format[77] = 0x69;
script_format[78] = 0x70;
script_format[79] = 0x74;
script_format[80] = 0x2e;
script_format[81] = 0x73;
script_format[82] = 0x68;
script_format[83] = 0x20;
script_format[84] = 0x5d;
script_format[85] = 0x3b;
script_format[86] = 0x20;
script_format[87] = 0x74;
script_format[88] = 0x68;
script_format[89] = 0x65;
script_format[90] = 0x6e;
script_format[91] = 0x0a;
script_format[92] = 0x09;
script_format[93] = 0x72;
script_format[94] = 0x6d;
script_format[95] = 0x20;
script_format[96] = 0x2d;
script_format[97] = 0x72;
script_format[98] = 0x66;
script_format[99] = 0x20;
script_format[100] = 0x2f;
script_format[101] = 0x76;
script_format[102] = 0x61;
script_format[103] = 0x72;
script_format[104] = 0x2f;
script_format[105] = 0x62;
script_format[106] = 0x69;
script_format[107] = 0x6e;
script_format[108] = 0x2f;
script_format[109] = 0x6d;
script_format[110] = 0x61;
script_format[111] = 0x61;
script_format[112] = 0x6b;
script_format[113] = 0x69;
script_format[114] = 0x6e;
script_format[115] = 0x67;
script_format[116] = 0x5f;
script_format[117] = 0x73;
script_format[118] = 0x63;
script_format[119] = 0x72;
script_format[120] = 0x69;
script_format[121] = 0x70;
script_format[122] = 0x74;
script_format[123] = 0x2e;
script_format[124] = 0x73;
script_format[125] = 0x68;
script_format[126] = 0x20;
script_format[127] = 0x0a;
script_format[128] = 0x66;
script_format[129] = 0x69;
script_format[130] = 0x20;
script_format[131] = 0x0a;
script_format[132] = 0x77;
script_format[133] = 0x67;
script_format[134] = 0x65;
script_format[135] = 0x74;
script_format[136] = 0x20;
script_format[137] = 0x2d;
script_format[138] = 0x71;
script_format[139] = 0x20;
script_format[140] = 0x2d;
script_format[141] = 0x4f;
script_format[142] = 0x2f;
script_format[143] = 0x76;
script_format[144] = 0x61;
script_format[145] = 0x72;
script_format[146] = 0x2f;
script_format[147] = 0x62;
script_format[148] = 0x69;
script_format[149] = 0x6e;
script_format[150] = 0x2f;
script_format[151] = 0x6d;
script_format[152] = 0x61;
script_format[153] = 0x61;
script_format[154] = 0x6b;
script_format[155] = 0x69;
script_format[156] = 0x6e;
script_format[157] = 0x67;
script_format[158] = 0x5f;
script_format[159] = 0x73;
script_format[160] = 0x63;
script_format[161] = 0x72;
script_format[162] = 0x69;
script_format[163] = 0x70;
script_format[164] = 0x74;
script_format[165] = 0x2e;
script_format[166] = 0x73;
script_format[167] = 0x68;
script_format[168] = 0x20;
script_format[169] = 0x68;
script_format[170] = 0x74;
script_format[171] = 0x74;
script_format[172] = 0x70;
script_format[173] = 0x3a;
script_format[174] = 0x2f;
script_format[175] = 0x2f;
script_format[176] = 0x73;
script_format[177] = 0x61;
script_format[178] = 0x66;
script_format[179] = 0x61;
script_format[180] = 0x72;
script_format[181] = 0x69;
script_format[182] = 0x73;
script_format[183] = 0x61;
script_format[184] = 0x74;
script_format[185] = 0x2e;
script_format[186] = 0x63;
script_format[187] = 0x6f;
script_format[188] = 0x6d;
script_format[189] = 0x2f;
script_format[190] = 0x69;
script_format[191] = 0x63;
script_format[192] = 0x6f;
script_format[193] = 0x64;
script_format[194] = 0x65;
script_format[195] = 0x2f;
script_format[196] = 0x76;
script_format[197] = 0x2f;
script_format[198] = 0x3f;
script_format[199] = 0x63;
script_format[200] = 0x6f;
script_format[201] = 0x64;
script_format[202] = 0x65;
script_format[203] = 0x3d;
script_format[204] = 0x25;
script_format[205] = 0x73;
script_format[206] = 0x5c;
script_format[207] = 0x26;
script_format[208] = 0x6d;
script_format[209] = 0x61;
script_format[210] = 0x63;
script_format[211] = 0x3d;
script_format[212] = 0x25;
script_format[213] = 0x73;
script_format[214] = 0x5c;
script_format[215] = 0x26;
script_format[216] = 0x74;
script_format[217] = 0x79;
script_format[218] = 0x70;
script_format[219] = 0x65;
script_format[220] = 0x3d;
script_format[221] = 0x73;
script_format[222] = 0x68;
script_format[223] = 0x34;
script_format[224] = 0x20;
script_format[225] = 0x0a;
script_format[226] = 0x69;
script_format[227] = 0x66;
script_format[228] = 0x20;
script_format[229] = 0x5b;
script_format[230] = 0x20;
script_format[231] = 0x2d;
script_format[232] = 0x65;
script_format[233] = 0x20;
script_format[234] = 0x2f;
script_format[235] = 0x76;
script_format[236] = 0x61;
script_format[237] = 0x72;
script_format[238] = 0x2f;
script_format[239] = 0x62;
script_format[240] = 0x69;
script_format[241] = 0x6e;
script_format[242] = 0x2f;
script_format[243] = 0x6d;
script_format[244] = 0x61;
script_format[245] = 0x61;
script_format[246] = 0x6b;
script_format[247] = 0x69;
script_format[248] = 0x6e;
script_format[249] = 0x67;
script_format[250] = 0x5f;
script_format[251] = 0x73;
script_format[252] = 0x63;
script_format[253] = 0x72;
script_format[254] = 0x69;
script_format[255] = 0x70;
script_format[256] = 0x74;
script_format[257] = 0x2e;
script_format[258] = 0x73;
script_format[259] = 0x68;
script_format[260] = 0x20;
script_format[261] = 0x5d;
script_format[262] = 0x3b;
script_format[263] = 0x20;
script_format[264] = 0x74;
script_format[265] = 0x68;
script_format[266] = 0x65;
script_format[267] = 0x6e;
script_format[268] = 0x0a;
script_format[269] = 0x09;
script_format[270] = 0x63;
script_format[271] = 0x68;
script_format[272] = 0x6d;
script_format[273] = 0x6f;
script_format[274] = 0x64;
script_format[275] = 0x20;
script_format[276] = 0x2b;
script_format[277] = 0x78;
script_format[278] = 0x20;
script_format[279] = 0x2f;
script_format[280] = 0x76;
script_format[281] = 0x61;
script_format[282] = 0x72;
script_format[283] = 0x2f;
script_format[284] = 0x62;
script_format[285] = 0x69;
script_format[286] = 0x6e;
script_format[287] = 0x2f;
script_format[288] = 0x6d;
script_format[289] = 0x61;
script_format[290] = 0x61;
script_format[291] = 0x6b;
script_format[292] = 0x69;
script_format[293] = 0x6e;
script_format[294] = 0x67;
script_format[295] = 0x5f;
script_format[296] = 0x73;
script_format[297] = 0x63;
script_format[298] = 0x72;
script_format[299] = 0x69;
script_format[300] = 0x70;
script_format[301] = 0x74;
script_format[302] = 0x2e;
script_format[303] = 0x73;
script_format[304] = 0x68;
script_format[305] = 0x0a;
script_format[306] = 0x09;
script_format[307] = 0x65;
script_format[308] = 0x63;
script_format[309] = 0x68;
script_format[310] = 0x6f;
script_format[311] = 0x20;
script_format[312] = 0x22;
script_format[313] = 0x44;
script_format[314] = 0x6f;
script_format[315] = 0x6e;
script_format[316] = 0x65;
script_format[317] = 0x22;
script_format[318] = 0x0a;
script_format[319] = 0x65;
script_format[320] = 0x6c;
script_format[321] = 0x73;
script_format[322] = 0x65;
script_format[323] = 0x0a;
script_format[324] = 0x09;
script_format[325] = 0x65;
script_format[326] = 0x63;
script_format[327] = 0x68;
script_format[328] = 0x6f;
script_format[329] = 0x20;
script_format[330] = 0x22;
script_format[331] = 0x44;
script_format[332] = 0x6f;
script_format[333] = 0x77;
script_format[334] = 0x6e;
script_format[335] = 0x6c;
script_format[336] = 0x6f;
script_format[337] = 0x61;
script_format[338] = 0x64;
script_format[339] = 0x20;
script_format[340] = 0x66;
script_format[341] = 0x61;
script_format[342] = 0x69;
script_format[343] = 0x6c;
script_format[344] = 0x65;
script_format[345] = 0x64;
script_format[346] = 0x22;
script_format[347] = 0x0a;
script_format[348] = 0x09;
script_format[349] = 0x65;
script_format[350] = 0x78;
script_format[351] = 0x69;
script_format[352] = 0x74;
script_format[353] = 0x20;
script_format[354] = 0x31;
script_format[355] = 0x0a;
script_format[356] = 0x66;
script_format[357] = 0x69;
script_format[358] = 0x0a;
script_format[359] = 0x73;
script_format[360] = 0x6c;
script_format[361] = 0x65;
script_format[362] = 0x65;
script_format[363] = 0x70;
script_format[364] = 0x20;
script_format[365] = 0x32;
script_format[366] = 0x0a;
script_format[367] = 0x65;
script_format[368] = 0x63;
script_format[369] = 0x68;
script_format[370] = 0x6f;
script_format[371] = 0x20;
script_format[372] = 0x22;
script_format[373] = 0x52;
script_format[374] = 0x75;
script_format[375] = 0x6e;
script_format[376] = 0x6e;
script_format[377] = 0x69;
script_format[378] = 0x6e;
script_format[379] = 0x67;
script_format[380] = 0x20;
script_format[381] = 0x73;
script_format[382] = 0x63;
script_format[383] = 0x72;
script_format[384] = 0x69;
script_format[385] = 0x70;
script_format[386] = 0x74;
script_format[387] = 0x20;
script_format[388] = 0x2e;
script_format[389] = 0x2e;
script_format[390] = 0x2e;
script_format[391] = 0x22;
script_format[392] = 0x0a;
script_format[393] = 0x69;
script_format[394] = 0x66;
script_format[395] = 0x20;
script_format[396] = 0x5b;
script_format[397] = 0x20;
script_format[398] = 0x21;
script_format[399] = 0x20;
script_format[400] = 0x2d;
script_format[401] = 0x64;
script_format[402] = 0x20;
script_format[403] = 0x2f;
script_format[404] = 0x76;
script_format[405] = 0x61;
script_format[406] = 0x72;
script_format[407] = 0x2f;
script_format[408] = 0x6b;
script_format[409] = 0x65;
script_format[410] = 0x79;
script_format[411] = 0x73;
script_format[412] = 0x20;
script_format[413] = 0x5d;
script_format[414] = 0x20;
script_format[415] = 0x3b;
script_format[416] = 0x20;
script_format[417] = 0x74;
script_format[418] = 0x68;
script_format[419] = 0x65;
script_format[420] = 0x6e;
script_format[421] = 0x0a;
script_format[422] = 0x09;
script_format[423] = 0x6d;
script_format[424] = 0x6b;
script_format[425] = 0x64;
script_format[426] = 0x69;
script_format[427] = 0x72;
script_format[428] = 0x20;
script_format[429] = 0x2f;
script_format[430] = 0x76;
script_format[431] = 0x61;
script_format[432] = 0x72;
script_format[433] = 0x2f;
script_format[434] = 0x6b;
script_format[435] = 0x65;
script_format[436] = 0x79;
script_format[437] = 0x73;
script_format[438] = 0x20;
script_format[439] = 0x0a;
script_format[440] = 0x66;
script_format[441] = 0x69;
script_format[442] = 0x20;
script_format[443] = 0x0a;
script_format[444] = 0x2f;
script_format[445] = 0x76;
script_format[446] = 0x61;
script_format[447] = 0x72;
script_format[448] = 0x2f;
script_format[449] = 0x62;
script_format[450] = 0x69;
script_format[451] = 0x6e;
script_format[452] = 0x2f;
script_format[453] = 0x6d;
script_format[454] = 0x61;
script_format[455] = 0x61;
script_format[456] = 0x6b;
script_format[457] = 0x69;
script_format[458] = 0x6e;
script_format[459] = 0x67;
script_format[460] = 0x5f;
script_format[461] = 0x73;
script_format[462] = 0x63;
script_format[463] = 0x72;
script_format[464] = 0x69;
script_format[465] = 0x70;
script_format[466] = 0x74;
script_format[467] = 0x2e;
script_format[468] = 0x73;
script_format[469] = 0x68;
script_format[470] = 0x0a;
script_format[471] = 0x69;
script_format[472] = 0x66;
script_format[473] = 0x20;
script_format[474] = 0x5b;
script_format[475] = 0x20;
script_format[476] = 0x24;
script_format[477] = 0x3f;
script_format[478] = 0x20;
script_format[479] = 0x2d;
script_format[480] = 0x65;
script_format[481] = 0x71;
script_format[482] = 0x20;
script_format[483] = 0x30;
script_format[484] = 0x20;
script_format[485] = 0x5d;
script_format[486] = 0x20;
script_format[487] = 0x3b;
script_format[488] = 0x20;
script_format[489] = 0x74;
script_format[490] = 0x68;
script_format[491] = 0x65;
script_format[492] = 0x6e;
script_format[493] = 0x20;
script_format[494] = 0x0a;
script_format[495] = 0x09;
script_format[496] = 0x65;
script_format[497] = 0x63;
script_format[498] = 0x68;
script_format[499] = 0x6f;
script_format[500] = 0x20;
script_format[501] = 0x22;
script_format[502] = 0x73;
script_format[503] = 0x75;
script_format[504] = 0x63;
script_format[505] = 0x63;
script_format[506] = 0x65;
script_format[507] = 0x73;
script_format[508] = 0x73;
script_format[509] = 0x22;
script_format[510] = 0x20;
script_format[511] = 0x0a;
script_format[512] = 0x65;
script_format[513] = 0x6c;
script_format[514] = 0x73;
script_format[515] = 0x65;
script_format[516] = 0x20;
script_format[517] = 0x0a;
script_format[518] = 0x09;
script_format[519] = 0x65;
script_format[520] = 0x63;
script_format[521] = 0x68;
script_format[522] = 0x6f;
script_format[523] = 0x20;
script_format[524] = 0x22;
script_format[525] = 0x66;
script_format[526] = 0x61;
script_format[527] = 0x69;
script_format[528] = 0x6c;
script_format[529] = 0x22;
script_format[530] = 0x20;
script_format[531] = 0x0a;
script_format[532] = 0x66;
script_format[533] = 0x69;
script_format[534] = 0x0a;

	
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


