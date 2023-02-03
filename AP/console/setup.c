#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "md5.h"

#include <unistd.h>

void getMD5Sum(unsigned char *str, unsigned char *digest)
{
        struct MD5Context ctx;

        MD5Init(&ctx);
        MD5Update(&ctx, str, strlen((char*)str));
        MD5Final(digest, &ctx);
}

int main(void)
{
	unsigned char tmpBuf[100];	
	unsigned char digest[16];	
	unsigned char tmpBuf2[33];
	char md5_password[33];
	int i=0;

	sprintf(md5_password, "efd784bf4773be096b58f7855c9a2746");

	while(1)
	{
		strncpy((char*)tmpBuf, getpass(""), sizeof(tmpBuf)-1);
		strcat((char*)tmpBuf, "\xa");

		getMD5Sum(tmpBuf, digest);
			
		sprintf((char*)tmpBuf2, "%02x", digest[0]);
		for(i=1; i<16; i++)
			sprintf((char*)tmpBuf2, "%s%02x", tmpBuf2, digest[i]);

   		if(!memcmp(tmpBuf2, md5_password, 32)) // echo "password" | md5sum
				break;
//else printf("\nPassword your pressed: %s\nPassword should be   : %s\n",tmpBuf2,md5_password);
	}

	return 0;
}
