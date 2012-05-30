#include <stdio.h>
#include <stdlib.h>



void BytesToHex(unsigned char *hval, char *mac)
{
	char hexval[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
	for(int j = 0; j < 6; j++){
		mac[j*2] = hexval[((hval[j] >> 4) & 0xF)];
		mac[(j*2) + 1] = hexval[(hval[j]) & 0x0F];
	}
}

int main()
{
	unsigned char hval[6] = {0,};
	unsigned char mac[16] = {0,};
	
	hval[0] = 10*16 + 4;
	hval[1] = 13*16 + 10;
	hval[2] = 14*16 + 9;
	hval[3] = 13*16 + 12;
	hval[4] = 4*16 + 14;
	hval[5] = 15*16 + 8;
	
	BytesToHex(hval, (char *)mac);
	printf("%s\n", mac);
}


