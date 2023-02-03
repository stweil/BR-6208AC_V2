/*
 *      Tool to convert ELF image to be the AP downloadable binary
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: cvimg.c,v 1.1.1.1 2005/03/09 04:52:51 rex Exp $
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#undef _RDC3210_
#include "apmib.h"
//#include "../../../define/C_DEFINE.h"


#define DEFAULT_START_ADDR	0x80300000

#ifdef _NETINDEX_FW_CHECK_ //vance 2009.02.24 for NetIndex
static unsigned char *VersionToHex(unsigned char *input);
#endif

static unsigned short calculateChecksum(char *buf, int len);

/////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	char inFile[80]={0}, outFile[80]={0}, imageType[80]={0};
#if defined (_RDC3210_)
     
  char initFile[80]={0};

#endif
	int fh, size;
	struct stat status;
	char *buf;
	IMG_HEADER_Tp pHeader;
#ifdef _NETINDEX_FW_CHECK_ //vance 2009.02.24 for NetIndex
	//unsigned char magicNo[2]={0},SWVersion[2]={0},uHWVersion[2]={0},lHWVersion[2]={0},temp[5]={0};
	unsigned char *magicNo=NULL,*SWVersion=NULL,*uHWVersion=NULL,*lHWVersion=NULL,temp[5]={0};
#else
	unsigned long startAddr;
   unsigned long burnAddr;
#endif
	unsigned short checksum;
	int i, is_rootfs = 0;

	// parse input arguments
#ifdef _NETINDEX_FW_CHECK_ //vance 2009.02.24 for NetIndex
	if ( argc != 8) {
		printf("Usage: cvimg linux input-filename output-filename Software-Version \n");
		printf("Magic-No, upper Hardware-Version, lower Hardware-Version\n");
		exit(1);
	}
#else
	if ( argc != 5 && argc != 6) {
		printf("Usage: cvimg linux input-filename output-filename start-addr burn-addr\n");
		printf("        \"start-addr\" is optional. If not given, it will use 0x%x as default.\n",
				DEFAULT_START_ADDR);
		exit(1);
	}
#endif
	
	sscanf(argv[1], "%s", imageType);
	sscanf(argv[2], "%s", inFile);
	sscanf(argv[3], "%s", outFile);
#if defined (_NETINDEX_FW_CHECK_) //vance 2009.02.24 for NetIndex
	if ( argc == 8 ) {
		sscanf(argv[4], "%s", &temp);
		SWVersion=VersionToHex(temp);
		sscanf(argv[5], "%s", &temp);
		magicNo=VersionToHex(temp);		
		sscanf(argv[6], "%s", &temp);
		uHWVersion=VersionToHex(temp);
		sscanf(argv[7], "%s", &temp);
		lHWVersion=VersionToHex(temp);
		//printf("sw=%s,no=%x%x\n",temp,lHWVersion[0],lHWVersion[1]);
	}
#else
	if ( argc == 5 ) {
#if defined (_RDC3210_)
		sscanf(argv[4], "%s", initFile);
		if ( stat(initFile, &status) < 0){
			printf("Can't get initFile length\n");
			exit(1);				
		}
		startAddr = status.st_size;   
		printf("initrd length:%ld, hex:0x%08x\n", startAddr,startAddr);
#else 
		sscanf(argv[4], "%x", &startAddr);
#endif
      }

	else
		startAddr = DEFAULT_START_ADDR;

	if ( argc == 6 ) {
		sscanf(argv[5], "%x", &burnAddr);
     
#if defined (_RDC3210_)
		sscanf(argv[4], "%s", initFile);
		if ( stat(initFile, &status) < 0){
			printf("Can't get initFile length\n");
			exit(1);				
		}
		startAddr = status.st_size;
		printf("%ld\n", startAddr);
#else 
		sscanf(argv[4], "%x", &startAddr);
#endif
        }
	else
		burnAddr = DEFAULT_START_ADDR;

#endif
	
	// check input file and allocate buffer
	if ( stat(inFile, &status) < 0 ) {
		printf("Can't stat file! [%s]\n", inFile );
		exit(1);
	}
	size = status.st_size + sizeof(IMG_HEADER_T) + sizeof(checksum);
	
	
#if defined (_RDC3210_)
	if (size%4) {
	printf("number of pending byte to kernel image: %d\n", 4 - (size%4) );
		size += (4 - (size%4));	//padding
		
	}	
#else 		
	if (size%2)
		size++;	// padding	
#endif

	buf = malloc(size);
	if (buf == NULL) {
		printf("Malloc buffer failed!\n");
		exit(1);
	}
	memset(buf, '\0', size);
	pHeader = (IMG_HEADER_Tp)buf;
	buf += sizeof(IMG_HEADER_T);

	// Read data and generate header
	fh = open(inFile, O_RDONLY);
	if ( fh == -1 ) {
		printf("Open input file error!\n");
		free( pHeader );
		exit(1);
	}
	lseek(fh, 0L, SEEK_SET);
	if ( read(fh, buf, status.st_size) != status.st_size) {
		printf("Read file error!\n");
		close(fh);
		free(pHeader);
		exit(1);
	}
	close(fh);

	if(!strcmp( imageType, "boot"))
		memcpy(pHeader->signature, "BOOT", SIGNATURE_LEN);
//EDX shakim start
	else if(!strcmp( imageType, "anfg"))
		memcpy(pHeader->signature, "ANFG", SIGNATURE_LEN);
//EDX shakim end
	else if(!strcmp( imageType, "adsl"))
		memcpy(pHeader->signature, "ADSL", SIGNATURE_LEN);
	else if(!strcmp( imageType, "rsdk"))
		memcpy(pHeader->signature, "RSDK", SIGNATURE_LEN);
	else if (!strcmp (imageType, "root"))	//special case for 2.6 kernel
		is_rootfs = 1;
	else
		memcpy(pHeader->signature, FW_HEADER, SIGNATURE_LEN);
	
#ifdef _NETINDEX_FW_CHECK_ //vance 2009.02.24 for NetIndex
	memcpy(pHeader->SWVersion, SWVersion, VERSION_LEN);
	memcpy(pHeader->magincNo, magicNo, VERSION_LEN);
	memcpy(pHeader->upperHWVersion, uHWVersion, VERSION_LEN);
	memcpy(pHeader->lowerHWVersion, lHWVersion, VERSION_LEN);
#else
	memcpy(pHeader->modTag, _WEB_HEADER_, SIGNATURE_LEN);	// Erwin
#endif
	
#if defined (_RDC3210_)	

       	pHeader->len = DWORD_SWAP(size-sizeof(IMG_HEADER_T));
	pHeader->startAddr = DWORD_SWAP(size-sizeof(IMG_HEADER_T));  // kernel length strored in startAddr field
        pHeader->burnAddr =  DWORD_SWAP(startAddr);  // initrd length strored in brunAddr field
    

#else 
	pHeader->len = DWORD_SWAP(size-sizeof(IMG_HEADER_T));
	//printf("%x\n",pHeader->len);
//	pHeader->len = DWORD_SWAP(0xc317);
#ifndef _NETINDEX_FW_CHECK_ //vance 2009.02.24 for NetIndex
#ifndef _ADM5120_ 
	pHeader->startAddr = DWORD_SWAP(startAddr);
    pHeader->burnAddr = DWORD_SWAP(burnAddr);
#endif
#endif
#endif
	if (is_rootfs){
		#define SIZE_OF_SQFS_SUPER_BLOCK 640
		unsigned long fs_len;

		fs_len = DWORD_SWAP((size - sizeof(IMG_HEADER_T) - sizeof(checksum) - SIZE_OF_SQFS_SUPER_BLOCK));
		memcpy(buf + 8, &fs_len, 4);

		fh = open(outFile, O_RDWR|O_CREAT|O_TRUNC);
		if (fh == -1) {
			printf("Create output file error! [%s]\n", outFile);
			free(pHeader);
			exit(1);
		}
		write(fh, buf, size - sizeof(IMG_HEADER_T) - sizeof(checksum));
	}else{
//EDX Rex		checksum = WORD_SWAP(calculateChecksum(buf, status.st_size));
		checksum = WORD_SWAP(calculateChecksum(buf, size-sizeof(IMG_HEADER_T)-sizeof(checksum))); //EDX Rex
		*((unsigned short *)&buf[size-sizeof(IMG_HEADER_T)-sizeof(checksum)]) = checksum;

		// Write image to output file
		fh = open(outFile, O_RDWR|O_CREAT|O_TRUNC);
		if ( fh == -1 ) {
			printf("Create output file error! [%s]\n", outFile);
			free(pHeader);
			exit(1);
		}
		write(fh, pHeader, size);
	}
	close(fh);
	
	printf("Generate kernel image successfully, length=%d (0x%08x), checksum=0x%x\n", DWORD_SWAP(pHeader->len),DWORD_SWAP(pHeader->len),checksum);


	free(pHeader);
}

#ifdef _NETINDEX_FW_CHECK_ //vance 2009.02.24 for NetIndex
//convert version number to hex
static unsigned char *VersionToHex(unsigned char *input)
{
	unsigned char *value;
	char *result = NULL;
	int count = 0;
	value = (unsigned char *)malloc(2*sizeof(unsigned char));
	result = strtok( input, "." );
	while( result != NULL ) {
		//printf( "result is \"%s\"\n", result );
		value[count] = atoi(result);
		result = strtok( NULL,  "." );
		count++;
	}
	//printf("value=0x%x%x\n",value[0],value[1]);
	return value;
}
#endif

static unsigned short calculateChecksum(char *buf, int len)
{
	int i, j;
	unsigned short sum=0, tmp;

	j = (len/2)*2;

	for (i=0; i<j; i+=2) {
		tmp = *((unsigned short *)(buf + i));
		sum += WORD_SWAP(tmp);
	}

	if ( len % 2 ) {
		tmp = buf[len-1];
		sum += WORD_SWAP(tmp);
	}
	return ~sum+1;
}
