/*
 *      AP MIB binary<->text convertion tool
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: cvcfg.c,v 1.1.1.1 2005/03/09 04:52:51 rex Exp $
 *
 */

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#ifndef WIN32
	#include <unistd.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#else
	#include <Winsock2.h>
	#include <io.h>
#endif

#include "apmib.h"
#include "mibtbl.h"

// File type mask
#define TYPE_MASK		0x0f
#define CS_TYPE			0x1	// bit mask for cs
#define DS_TYPE			0X2	// bit mask for ds
#define HS_TYPE			0x4	// bit mask for hs

#define RAW_TYPE		0x80	// bit nask for raw data with pad

// mode select
#define MODE_MASK		0xf0
#define TXT_MODE		0x10
#define TARGET_MODE		0X20
#define PC_MODE			0X40

#define SPACE	(' ')
#define EOL		('\n')
#define TAB		('\x9')

// MIB value, id mapping table
typedef struct _mib_table_ {
	int id;
	char *name;
} mib_table_T;

#ifdef WIN32
typedef unsigned long in_addr_t;
extern int inet_aton(const char *cp, struct in_addr *addr);
#endif

//////////////////////////////////////////////////////////////////////////////
static int _is_hex(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'F')) ||
            ((c >= 'a') && (c <= 'f')));
}

#if 0
// Validate digit
static int _isdigit(char c)
{
    return ((c >= '0') && (c <= '9'));
}
#endif
void dump_mem(void *ptr, int size)
{
	int i;

	printf("\n");
	for(i=0; i < size; i++)
	{
		printf("%4.2x", (unsigned )(((unsigned char*)ptr)[i]));
		if(i%16 == 0)
			printf("\n");
	}
	printf("\n");
}

//////////////////////////////////////////////////////////////////////////////
static int string_to_hex(char *string, unsigned char *key, int len)
{
	char tmpBuf[4];
	int idx, ii=0;
	for (idx=0; idx<len; idx+=2) {
		tmpBuf[0] = string[idx];
		tmpBuf[1] = string[idx+1];
		tmpBuf[2] = 0;
		if ( !_is_hex(tmpBuf[0]) || !_is_hex(tmpBuf[1]))
			return 0;

		key[ii++] = (unsigned char) strtol(tmpBuf, (char**)NULL, 16);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////////
static char *getVal(char *value, char **p)
{
	int len=0;

	while (*value == ' ' ) value++;

	*p = value;

	while (*value && *value!=',') {
		value++;
		len++;
	}

	if ( !len ) {
		*p = NULL;
		return NULL;
	}

	if ( *value == 0)
		return NULL;

	*value = 0;
	value++;

	return value;
}
#define WRITE_LINE(format, prefix, name, value) { \
	sprintf(line, format, prefix, name, value); \
	fwrite(line, 1, strlen(line), fp); \
}
static int acNum;
#ifdef HOME_GATEWAY
static int macFilterNum, portFilterNum, ipFilterNum, portFwNum, triggerPortNum, vserNum, DMZNum, ACPCNum, URLBNum;
#endif

struct all_config {
	int hwmib_exist, dsmib_exist, csmib_exist;
	int hwmib_ver, dsmib_ver, csmib_ver;
	int hwmib_len, dsmib_len, csmib_len;
	HW_SETTING_T hwmib;
	unsigned char tmp1[100];
	APMIB_T dsmib;
	unsigned char tmp2[100];
	APMIB_T csmib;
	unsigned char tmp3[100];
};

static struct all_config config;

/* Local function decalaration */
static void showHelp(void);
static int checkFileType(char *filename);
static int parseBinConfig(int type, char *filename, struct all_config *pConfig);
static int parseTxtConfig(char *filename, struct all_config *pConfig);
static int getToken(char *line, char *value, int *def, int *hwtbl);
static int generateBinFile(int type, char *filename, int flag);
static int generateTxtFile(char *filename);
static int set_mib(struct all_config *pConfig, int id, void *value, int def, int hw);
static void swap_mib_word_value(APMIB_Tp pMib);
static void getVal2(char *value, char **p1, char **p2);
#ifdef HOME_GATEWAY
static void getVal3(char *value, char **p1, char **p2, char **p3);
static void getVal4(char *value, char **p1, char **p2, char **p3, char **p4);
static void getVal5(char *value, char **p1, char **p2, char **p3, char **p4, char **p5);
static void getVal7(char *value, char **p1, char **p2, char **p3, char **p4, char **p5, char **p6, char **p7);
static void getVal15(char *value, char **p1, char **p2, char **p3, char **p4, char **p5, char **p6, char **p7, char **p8, char **p9, char **p10, char **p11, char **p12, char **p13, char **p14, char **p15);
#endif

/////////////////////////////////////////////////////////////////////////////
static char *copyright="Copyright (c) Realtek Semiconductor Corp., 2003. All Rights Reserved.";
static char *version="1.0";

/////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	int argNum=1, outFileType=0, inFileType, flag, raw_data=0;
	char inFile[80]={0}, outFile[80]={0}, imsg[100]={0},omsg[100]={0};

	printf("\nsizeof APMIB_T = %d\n", sizeof(APMIB_T));

	while (argNum < argc) {
		if ( !strcmp(argv[argNum], "-in") ) {
			if (++argNum >= argc)
				break;
			sscanf(argv[argNum], "%s", inFile);
		}
		else if ( !strcmp(argv[argNum], "-ot") ) {
			if (++argNum >= argc)
				break;
			outFileType = TXT_MODE;
			sscanf(argv[argNum], "%s", outFile);
		}
		else if ( !strcmp(argv[argNum], "-ogu") ) {
			if (++argNum >= argc)
				break;
			outFileType = TARGET_MODE;
			sscanf(argv[argNum], "%s", outFile);
			flag = 1;	// update
		}
		else if ( !strcmp(argv[argNum], "-ogg") ) {
			if (++argNum >= argc)
				break;
			outFileType = TARGET_MODE;
			sscanf(argv[argNum], "%s", outFile);
			flag = 0;	// upgrade
		}
		else if ( !strcmp(argv[argNum], "-ogf") ) {
			if (++argNum >= argc)
				break;
			outFileType = TARGET_MODE;
			sscanf(argv[argNum], "%s", outFile);
			flag = 2;	// force
		}
		else if ( !strcmp(argv[argNum], "-op") ) {
			if (++argNum >= argc)
				break;
			outFileType = PC_MODE;
			sscanf(argv[argNum], "%s", outFile);
		}
		else if ( !strcmp(argv[argNum], "-h") ||
				!strcmp(argv[argNum], "/h") ) {
			showHelp();
			exit(1);
		}
		else if ( !strcmp(argv[argNum], "-r") ) {
			raw_data = 1;
		}
		else {
			if ( !inFile[0] ) {
				sscanf(argv[argNum], "%s", inFile);
			}
			else if (!outFile[0]) {
				outFileType = TARGET_MODE;
				sscanf(argv[argNum], "%s", outFile);
				flag = 1;	// update
			}
		}
		argNum++;
	}



	if ( !inFile[0] || !outFile[0] ) {
		showHelp();
		exit(1);
	}
	if (raw_data)
		flag |= RAW_TYPE;

	inFileType=checkFileType(inFile);
	if (inFileType == 0) {
		printf("\nInvalid input file!\n");
		exit(1);
	}

	if ( (inFileType & MODE_MASK) == TXT_MODE )
		strcat(imsg, "TXT");
	else {
		if ((inFileType & MODE_MASK) == PC_MODE )
			strcat(imsg, "PC");
		else
			strcat(imsg, "TARGET");
		if ((inFileType& TYPE_MASK) & HS_TYPE)
			strcat(imsg, "-HS");
		if ((inFileType& TYPE_MASK) & DS_TYPE)
			strcat(imsg, "-DS");
		if ((inFileType& TYPE_MASK) & CS_TYPE)
			strcat(imsg, "-CS");
	}


	if ( (outFileType & MODE_MASK) == TXT_MODE )
		strcat(omsg, "TXT");
	else {
		if ((outFileType & MODE_MASK) == TARGET_MODE)
			strcat(omsg, "TARGET");
		else
			strcat(omsg, "PC");
	}

	printf("\nConverting %s to %s", imsg, omsg);

	memset(&config, '\0', sizeof(config) );

	switch (inFileType & MODE_MASK) {
		case PC_MODE:
		case TARGET_MODE:
			if ( parseBinConfig(inFileType, inFile, &config) < 0) {
				printf("Parse binary file error!\n");
				exit(1);
			}
			break;

		case TXT_MODE:
			if ( parseTxtConfig(inFile, &config) < 0) {
				printf("Parse text file error!\n");
				exit(1);
			}
			break;
	}

	switch (outFileType & MODE_MASK) {
		case PC_MODE:
		case TARGET_MODE:
			if ( generateBinFile(outFileType, outFile, flag) < 0) {
				printf("Generate binary output file error!\n");
				exit(1);
			}
#ifdef WIN32
			_chmod(outFile, S_IREAD);
#else
			chmod(outFile, S_IREAD);
#endif
			break;

		case TXT_MODE:
			if ( generateTxtFile(outFile) < 0) {
				printf("Generate text output file error!\n");
				exit(1);
			}
			chmod(outFile, S_IREAD | S_IWRITE);
			break;
	}

	printf(" => %s ok.\n", outFile);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
static void showHelp(void)
{
	printf("\nRTL8181 config file converting tool [%s].\n",
#ifdef HOME_GATEWAY
		"gw"
#else
		"ap"
#endif
		);

	printf("%s Ver %s.\n\n", copyright, version);
	printf("Usage: arguments...\n");
	printf("  Where arguments are:\n");
	printf("    -in input_filename, specify the input filename.\n");
	printf("    -ot out_filename, generate a output filename in text mode.\n");
	printf("    -ogu out_filename, generate a output filename for target mode [update].\n");
	printf("    -ogg out_filename, generate a output filename for target mode [upgrade].\n");
	printf("    -ogf out_filename, generate a output filename for target mode [force].\n");
	printf("    -op out_filename, generate a output filename PC mode.\n");
	printf("    -r generate raw configutation with pad.\n\n");
}

////////////////////////////////////////////////////////////////////////////////
static int checkFileType(char *filename)
{
//printf("\nSSS\n");
	int fh=0;
	struct stat status;
	char *buf=NULL, *ptr;
	int flag=0, len;
	PARAM_HEADER_Tp pHeader;

	if ( stat(filename, &status) < 0 )
		goto back;
//printf("\n st_size=%d\n", status.st_size);
	buf = malloc(status.st_size);

	if ( buf == NULL)
		goto back;
#ifdef WIN32
	fh = open(filename, O_RDONLY|O_BINARY);
#else
	fh = open(filename, O_RDONLY);
#endif
	if ( fh == -1 )
		goto back;

	lseek(fh, 0L, SEEK_SET);
	if ( read(fh, buf, status.st_size) != status.st_size)
		goto back;

	// Assume PC config file
	flag = PC_MODE;
	len = HW_SETTING_OFFSET;
	pHeader = (PARAM_HEADER_Tp)&buf[len];
	if ( (int)(pHeader->len+sizeof(PARAM_HEADER_T)) > status.st_size )
		goto check_target;

	ptr = &buf[len+sizeof(PARAM_HEADER_T)];
	if ( !CHECKSUM_OK(ptr, pHeader->len) )
		goto check_target;
	if (memcmp(pHeader->signature, HW_SETTING_HEADER_TAG, TAG_LEN))
		goto check_target;
	flag |= HS_TYPE;

	len = DEFAULT_SETTING_OFFSET;
	pHeader = (PARAM_HEADER_Tp)&buf[len];
	ptr = &buf[len+sizeof(PARAM_HEADER_T)];
	if ( !CHECKSUM_OK(ptr, pHeader->len) )
		goto check_target;

	if (memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_TAG, TAG_LEN))
		goto check_target;
	flag |= DS_TYPE;

/*	len = CURRENT_SETTING_OFFSET;		//removed by Erwin
	pHeader = (PARAM_HEADER_Tp)&buf[len];
	if ( !CHECKSUM_OK(ptr, pHeader->len) )
		goto check_target;
	if (memcmp(pHeader->signature, CURRENT_SETTING_HEADER_TAG, TAG_LEN))
		goto check_target;
	flag |= CS_TYPE;
	goto back;
*/
check_target:
	// Assume target mode
	flag = TARGET_MODE;
	pHeader = (PARAM_HEADER_Tp)buf;
	pHeader->len = WORD_SWAP(pHeader->len);
	len = 0;
	while (len < status.st_size) {
		if ( (int)(pHeader->len+sizeof(PARAM_HEADER_T)) <= status.st_size ) {
			ptr = &buf[len+sizeof(PARAM_HEADER_T)];
			DECODE_DATA(ptr, pHeader->len);
			if ( CHECKSUM_OK(ptr, pHeader->len) ) {
				if (!memcmp(pHeader->signature, HW_SETTING_HEADER_TAG, TAG_LEN) ||
					!memcmp(pHeader->signature, HW_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) ||
					!memcmp(pHeader->signature, HW_SETTING_HEADER_FORCE_TAG, TAG_LEN) )
					flag |= HS_TYPE;
				else if (!memcmp(pHeader->signature, CURRENT_SETTING_HEADER_TAG, TAG_LEN) ||
						!memcmp(pHeader->signature, CURRENT_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) ||
						!memcmp(pHeader->signature, CURRENT_SETTING_HEADER_FORCE_TAG, TAG_LEN) )
					flag |= CS_TYPE;
				else if (!memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_TAG, TAG_LEN) ||
						!memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) ||
						!memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_FORCE_TAG, TAG_LEN) )
					flag |= DS_TYPE;
				len = len + sizeof(PARAM_HEADER_T) + pHeader->len;
				pHeader = (PARAM_HEADER_Tp)&buf[len];
				pHeader->len = WORD_SWAP(pHeader->len);
			}
			else
				break;
		}
		else
			break;
	}
	if ( !(flag & TYPE_MASK) )
		flag = TXT_MODE;	// if no config found, set as txt mode

back:
	if (fh >0)
		close(fh);
	if (buf)
		free(buf);

	return (flag);

}

////////////////////////////////////////////////////////////////////////////////
static int parseBinConfig(int type, char *filename, struct all_config *pMib)
{
	int fh, ver;
	char *data;
	PARAM_HEADER_T header;
#ifdef WIN32
	fh = open(filename, O_RDONLY|O_BINARY);
#else
	fh = open(filename, O_RDONLY);
#endif
	if ( fh == -1 )
		return -1;

	lseek(fh, 0, SEEK_SET);

	// Read hw setting
	if ( (type & TYPE_MASK) & HS_TYPE ) {
		if ((type & MODE_MASK) == PC_MODE)
			lseek(fh, HW_SETTING_OFFSET, SEEK_SET);

		if ( read(fh, &header, sizeof(PARAM_HEADER_T)) != sizeof(PARAM_HEADER_T)) {
			printf("Read hs header failed!\n");
			close(fh);
			return -1;
		}
		if ((type & MODE_MASK) == TARGET_MODE)
			header.len = WORD_SWAP(header.len);

		if (header.len > sizeof(HW_SETTING_T) + 100) {
			printf("hs length too large!");
			close(fh);
			return -1;
		}
		if ( read(fh, (char *)&pMib->hwmib, header.len) != header.len) {
			printf("Read hw mib failed!\n");
			close(fh);
			return -1;
		}
		data = (char *)&pMib->hwmib;

		if ((type & MODE_MASK) == TARGET_MODE) {
			DECODE_DATA(data, header.len);
			swap_mib_word_value((APMIB_Tp)data);
		}

		if ( CHECKSUM_OK(data, header.len) == 0) {
			printf("hs checksum error!\n");
			close(fh);
			return -1;
		}
		pMib->hwmib_exist = 1;
		sscanf(&header.signature[TAG_LEN], "%02d", &ver);
		pMib->hwmib_ver = ver;
		pMib->hwmib_len = header.len;

	}

	// Read default setting
	if ( (type & TYPE_MASK) & DS_TYPE ) {
		if ((type & MODE_MASK) == PC_MODE)
			lseek(fh, DEFAULT_SETTING_OFFSET, SEEK_SET);

		if ( read(fh, &header, sizeof(PARAM_HEADER_T)) != sizeof(PARAM_HEADER_T)) {
			printf("Read ds header failed!\n");
			close(fh);
			return -1;
		}
		if ((type & MODE_MASK) == TARGET_MODE)
			header.len = WORD_SWAP(header.len);

		if (header.len > sizeof(APMIB_T) + 100) {
			printf("ds length too large!");
			close(fh);
			return -1;
		}
		if ( read(fh, (char *)&pMib->dsmib, header.len) != header.len) {
			printf("Read ds mib failed!\n");
			close(fh);
			return -1;
		}
		data = (char *)&pMib->dsmib;

		if ((type & MODE_MASK) == TARGET_MODE) {
			DECODE_DATA(data, header.len);
			swap_mib_word_value((APMIB_Tp)data);
		}

		if ( CHECKSUM_OK(data, header.len) == 0) {
			printf("ds checksum error!\n");
			close(fh);
			return -1;
		}
		pMib->dsmib_exist = 1;
		sscanf(&header.signature[TAG_LEN], "%02d", &ver);
		pMib->dsmib_ver = ver;
		pMib->dsmib_len = header.len;
	}

	// Read current setting
/*	if ( (type & TYPE_MASK) & CS_TYPE ) {	//removed by Erwin
		if ((type & MODE_MASK) == PC_MODE)
			lseek(fh, CURRENT_SETTING_OFFSET, SEEK_SET);

		if ( read(fh, &header, sizeof(PARAM_HEADER_T)) != sizeof(PARAM_HEADER_T)) {
			printf("Read cs header failed!\n");
			close(fh);
			return -1;
		}
		if ((type & MODE_MASK) == TARGET_MODE)
			header.len = WORD_SWAP(header.len);

		if (header.len > sizeof(APMIB_T) + 100) {
			printf("cs length too large!");
			close(fh);
			return -1;
		}
		if ( read(fh, (char *)&pMib->csmib, header.len) != header.len) {
			printf("Read cs mib failed!\n");
			close(fh);
			return -1;
		}
		data = (char *)&pMib->csmib;

		if ((type & MODE_MASK) == TARGET_MODE) {
			DECODE_DATA(data, header.len);
			swap_mib_word_value((APMIB_Tp)data);
		}

		if ( CHECKSUM_OK(data, header.len) == 0) {
			printf("cs checksum error!\n");
			close(fh);
			return -1;
		}
		pMib->csmib_exist = 1;
		sscanf(&header.signature[TAG_LEN], "%02d", &ver);
		pMib->csmib_ver = ver;
		pMib->csmib_len = header.len;
	}
*/
	close(fh);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
static int parseTxtConfig(char *filename, struct all_config *pMib)
{
	char line[700], value[700];
	FILE *fp;
	int id, def_flag, hw_tbl;

	fp = fopen(filename, "r");
	if ( fp == NULL )
		return -1;

	acNum = 0;
#ifdef HOME_GATEWAY
	portFilterNum = ipFilterNum = macFilterNum = portFwNum = triggerPortNum = vserNum = DMZNum = URLBNum = ACPCNum = 0;
#endif

	while ( fgets(line, 700, fp) ) {
		id = getToken(line, value, &def_flag, &hw_tbl);
		if ( id == 0 )
			continue;
		if ( set_mib(&config, id, value, def_flag, hw_tbl) < 0) {
			printf("Parse MIB [%d] error!\n", id );
			fclose(fp);
			return -1;
		}
	}

	fclose(fp);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
static int getToken(char *line, char *value, int *def_flag, int *hw_tbl)
{
	char *ptr=line, *p1;
	char token[200]={0};
	int len=0, idx;
	mib_table_entry_T *pTbl=mib_table;

	*def_flag = 0;
	*hw_tbl = 0;

	if ( *ptr == ';' )	// comments
		return 0;

	// get token
	while (*ptr && *ptr!=EOL && *ptr!=';') {
		if ( *ptr == '=' ) {
			memcpy(token, line, len);

			// delete ending space
			for (idx=len-1; idx>=0; idx--) {
				if (token[idx]!= SPACE && token[idx]!= TAB)
					break;
			}
			token[idx+1] = '\0';
			ptr++;
			break;
		}
		ptr++;
		len++;
	}
	if ( !token[0] )
		return 0;

	// get value
	len=0;
	while (*ptr == SPACE ) ptr++; // delete space

	p1 = ptr;
	while ( *ptr && *ptr!=EOL && *ptr!=';' && *ptr!='\r') {
		ptr++;
		len++;
	}
	memcpy(value, p1, len );
	value[len] = '\0';

	// delete ending space
	for (idx=len-1; idx>=0; idx--) {
		if (value[idx]!= SPACE && value[idx]!= TAB)
			break;
	}
	value[idx+1] = '\0';

	p1 = token;
	if ( !memcmp(token, "DEF_", 4)) {
		p1 = &token[4];
		*def_flag = 1;
	}
	else if ( !memcmp(token, "HW_", 3)) {
		pTbl = hwmib_table;
		*hw_tbl = 1;
	}

	idx = 0;
	while (pTbl[idx].id) {
		if (!strcmp(pTbl[idx].name, p1))
			return pTbl[idx].id;
		idx++;
	}
	return 0;
}


////////////////////////////////////////////////////////////////////////////////
static int set_mib(struct all_config *pConfig, int id, void *value, int def_mib, int hw_tbl)
{

	unsigned char key[180];
	char *p1, *p2;
#ifdef HOME_GATEWAY
	char *p3, *p4, *p5, *p6, *p7, *p8, *p9, *p10, *p11, *p12, *p13, *p14, *p15;
#endif
	struct in_addr inAddr;
	int i, j, k, int_val, max_chan;
	MACFILTER_Tp pWlAc;
#ifdef HOME_GATEWAY
	PORTFW_Tp pPortFw;
	PORTFILTER_Tp pPortFilter;
	IPFILTER_Tp pIpFilter;
	MACFILTER_Tp pMacFilter;
	TRIGGERPORT_Tp pTriggerPort;
	DMZ_Tp pDMZ;
	URLB_Tp pURLB;
	ACPC_Tp pACPC;
	PORTFW_Tp pVSER;
#endif
	mib_table_entry_T *pTbl;
	char *pMib;

	if (def_mib) {
		pMib = (char *)&pConfig->dsmib;
		pTbl = mib_table;
		pConfig->dsmib_exist = 1;
	}
	else if (hw_tbl) {
		pMib = (char *)&pConfig->hwmib;
		pTbl = hwmib_table;
		pConfig->hwmib_exist = 1;
	}
	else {
		pMib = (char *)&pConfig->csmib;
		pTbl = mib_table;
		pConfig->csmib_exist = 1;
	}


	for (i=0; pTbl[i].id; i++) {
		if ( pTbl[i].id == id )
			break;
	}
	if ( pTbl[i].id == 0 )
		return -1;

	switch (pTbl[i].type) {
	case BYTE_T:
		*((unsigned char *)(((long)pMib) + pTbl[i].offset)) = (unsigned char)atoi(value);
		break;

	case WORD_T:
		*((unsigned short *)(((long)pMib) + pTbl[i].offset)) = (unsigned short)atoi(value);
		break;

	case STRING_T:
		if ( (int)(strlen(value)+1) > pTbl[i].size )
			return 0;
		strcpy((char *)(((long)pMib) + pTbl[i].offset), (char *)value);
		break;

	case BYTE5_T:
		if (strlen(value)!=10 || !string_to_hex(value, key, 10))
			return -1;
		memcpy((unsigned char *)(((long)pMib) + pTbl[i].offset), key, 5);
		break;

	case BYTE6_T:
		if (strlen(value)!=12 || !string_to_hex(value, key, 12))
			return -1;
		memcpy((unsigned char *)(((long)pMib) + pTbl[i].offset), key, 6);
		break;

	case BYTE13_T:
		if (strlen(value)!=26 || !string_to_hex(value, key, 26))
			return -1;
		memcpy((unsigned char *)(((long)pMib) + pTbl[i].offset), key, 13);
		break;

	case BYTE16_T:
		if (strlen(value)!=32 || !string_to_hex(value, key, 32))
			return -1;
		memcpy((unsigned char *)(((long)pMib) + pTbl[i].offset), key, 16);
		break;

	case DWORD_T:
		*((unsigned int *)(((long)pMib) + pTbl[i].offset)) = strtoul(value, NULL, 0);
		break;

	case INDEX_T :
		*((unsigned long *)(((long)pMib) + pTbl[i].offset)) = strtoul(value, NULL, 0);
		break;
	case INDEXDOS_T:

		*((unsigned long *)(((long)pMib) + pTbl[i].offset)) = strtoul(value, NULL, 0);
		break;

#ifdef WLAN_WPA
#if 0
	case BYTE32_T:
		if (strlen(value)!=64 || !string_to_hex(value, key, 64))
			return -1;
		memcpy((unsigned char *)(((long)pMib) + pTbl[i].offset), key, 32);
		break;
#endif
#endif

	case IA_T:
		if ( !inet_aton(value, &inAddr) )
			return -1;
		memcpy((unsigned char *)(((long)pMib) + pTbl[i].offset), (unsigned char *)&inAddr,  4);
		break;


	case BYTE_ARRAY_T:
		max_chan = (pTbl[i].id == MIB_HW_TX_POWER_CCK)? MAX_CCK_CHAN_NUM:MAX_OFDM_CHAN_NUM;
		if ( pTbl[i].id == MIB_HW_TX_POWER_CCK || pTbl[i].id == MIB_HW_TX_POWER_OFDM ) {
			p1 = (char *)value;
			for (k=0, j=0; k<max_chan ; k++) {
				if ( !sscanf(&p1[j], "%d", &int_val) ) {
					printf("invalid value!\n");
					return -1;
				}
				key[k] = (unsigned char)int_val;
				while ( p1[++j] ) {
					if ( p1[j]==' ') {
						j++;
						break;
					}
				}
			}
			memcpy((unsigned char *)(((long)pMib) + pTbl[i].offset), (unsigned char *)key, pTbl[i].size );
		}
		break;



/*
	case BYTE_ARRAY_T:
		if ( pTbl[i].id == MIB_HW_TX_POWER_CCK || pTbl[i].id == MIB_HW_TX_POWER_OFDM ) {
			p1 = (char *)value;
			for (k=0, j=0; k<MAX_CHAN_NUM; k++) {
				if ( !sscanf(&p1[j], "%d", &int_val) ) {
					printf("invalid value!\n");
					return -1;
				}
				key[k] = (unsigned char)int_val;
				while ( p1[++j] ) {
					if ( p1[j]==' ') {
						j++;
						break;
					}
				}
			}
			memcpy((unsigned char *)(((long)pMib) + pTbl[i].offset), (unsigned char *)key, pTbl[i].size );
		}
		break;
*/
	case WLAC_ARRAY_T:
#if defined(_MULTIPLE_WLAN_ACCESS_CONTROL_ )
	case WLAC_ARRAY2_T:
	case WLAC_ARRAY3_T:
	case WLAC_ARRAY4_T:
#endif
		getVal2((char *)value, &p1, &p2);
		if (p1 == NULL) {
			printf("Invalid WLAC in argument!\n");
			break;
		}
		if (strlen(p1)!=12 || !string_to_hex(p1, key, 12))
			return -1;

		pWlAc = (MACFILTER_Tp)(((long)pMib)+pTbl[i].offset+acNum*sizeof(MACFILTER_T));
		memcpy(pWlAc->macAddr, key, 6);
		if (p2 != NULL )
			strcpy(pWlAc->comment, p2);
		acNum++;
		break;
#ifdef HOME_GATEWAY
////////////////////////////////////////////////////////////////////////////////////////////////
	case VSER_ARRAY_T:
		getVal5((char *)value, &p1, &p2, &p3, &p4, &p5);
		if (p1 == NULL || p2 == NULL || p3 == NULL || p4 == NULL ) {
			printf("Invalid VSER arguments!\n");
			break;
		}
		if ( !inet_aton(p1, &inAddr) )
			return -1;

		pVSER = (PORTFW_Tp)(((long)pMib)+mib_table[i].offset+vserNum*sizeof(PORTFW_T));
		memcpy(pVSER->ipAddr, (unsigned char *)&inAddr, 4);
		pVSER->fromPort = (unsigned short)atoi(p2);
		pVSER->toPort = (unsigned short)atoi(p3);
		pVSER->protoType = (unsigned char)atoi(p4);
		if ( p5 )
			strcpy( pVSER->comment, p5 );
		vserNum++;
		break;


	case DMZ_ARRAY_T:
		getVal2((char *)value, &p1, &p2);
		if (p1 == NULL || p2 == NULL) {
			printf("Invalid DMZ arguments!\n");
			break;
		}
		pDMZ = (DMZ_Tp)(((long)pMib)+mib_table[i].offset+DMZNum*sizeof(DMZ_T));
		if ( !inet_aton(p1, &inAddr) )
			return -1;
		memcpy(pDMZ->pipAddr, (unsigned char *)&inAddr, 4);
		if ( !inet_aton(p2, &inAddr) )
			return -1;
		memcpy(pDMZ->cipAddr, (unsigned char *)&inAddr, 4);
		DMZNum++;
		break;

	case ACPC_ARRAY_T:
		getVal15((char *)value, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15);
		if (p1 == NULL || p2 == NULL) {
			printf("Invalid ACPC arguments!\n");
			break;
		}
		if ( !inet_aton(p1, &inAddr) )
			return -1;
		pACPC = (ACPC_Tp)(((long)pMib)+mib_table[i].offset+ACPCNum*sizeof(ACPC_T));
		memcpy(pACPC->sipAddr, (unsigned char *)&inAddr, 4);
		if ( !inet_aton(p2, &inAddr) )
			return -1;
		memcpy(pACPC->eipAddr, (unsigned char *)&inAddr, 4);


	pACPC->Port[0][0] = (unsigned long)atoi(p3);	pACPC->Port[0][1] = (unsigned long)atoi(p4);
	pACPC->Port[1][0] = (unsigned long)atoi(p5);	pACPC->Port[1][1] = (unsigned long)atoi(p6);
	pACPC->Port[2][0] = (unsigned long)atoi(p7);	pACPC->Port[2][1] = (unsigned long)atoi(p8);
	pACPC->Port[3][0] = (unsigned long)atoi(p9);	pACPC->Port[3][1] = (unsigned long)atoi(p10);
	pACPC->Port[4][0] = (unsigned long)atoi(p11);	pACPC->Port[4][1] = (unsigned long)atoi(p12);

		pACPC->serindex = (unsigned char)atol(p13);
		pACPC->protoType = (unsigned char)atoi(p14);
		if ( p15 )
			strcpy( pACPC->comment, p15 );
		ACPCNum++;
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
	case MACFILTER_ARRAY_T:
		getVal2((char *)value, &p1, &p2);
		if (p1 == NULL) {
			printf("Invalid MACFILTER in argument!\n");
			break;
		}
		if (strlen(p1)!=12 || !string_to_hex(p1, key, 12))
			return -1;

		pMacFilter = (MACFILTER_Tp)(((long)pMib)+pTbl[i].offset+macFilterNum*sizeof(MACFILTER_T));
		memcpy(pMacFilter->macAddr, key, 6);
		if (p2 != NULL )
			strcpy(pMacFilter->comment, p2);
		macFilterNum++;
		break;

	case PORTFW_ARRAY_T:
		getVal5((char *)value, &p1, &p2, &p3, &p4, &p5);
		if (p1 == NULL || p2 == NULL || p3 == NULL || p4 == NULL ) {
			printf("Invalid PORTFW arguments!\n");
			break;
		}
		if ( !inet_aton(p1, &inAddr) )
			return -1;

		pPortFw = (PORTFW_Tp)(((long)pMib)+pTbl[i].offset+portFwNum*sizeof(PORTFW_T));
		memcpy(pPortFw->ipAddr, (unsigned char *)&inAddr, 4);
		pPortFw->fromPort = (unsigned short)atoi(p2);
		pPortFw->toPort = (unsigned short)atoi(p3);
		pPortFw->protoType = (unsigned char)atoi(p4);
		if ( p5 )
			strcpy( pPortFw->comment, p5 );
		portFwNum++;
		break;

	case IPFILTER_ARRAY_T:
		getVal3((char *)value, &p1, &p2, &p3);
		if (p1 == NULL || p2 == NULL) {
			printf("Invalid IPFILTER arguments!\n");
			break;
		}
		if ( !inet_aton(p1, &inAddr) )
			return -1;
		pIpFilter = (IPFILTER_Tp)(((long)pMib)+pTbl[i].offset+ipFilterNum*sizeof(IPFILTER_T));
		memcpy(pIpFilter->ipAddr, (unsigned char *)&inAddr, 4);
		pIpFilter->protoType = (unsigned char)atoi(p2);
		if ( p3 )
			strcpy( pIpFilter->comment, p3 );
		ipFilterNum++;
		break;

	case PORTFILTER_ARRAY_T:
		getVal4((char *)value, &p1, &p2, &p3, &p4);
		if (p1 == NULL || p2 == NULL || p3 == NULL) {
			printf("Invalid PORTFILTER arguments!\n");
			break;
		}
		pPortFilter = (PORTFILTER_Tp)(((long)pMib)+pTbl[i].offset+portFilterNum*sizeof(PORTFILTER_T));
		pPortFilter->fromPort = (unsigned short)atoi(p1);
		pPortFilter->toPort = (unsigned short)atoi(p2);
		pPortFilter->protoType = (unsigned char)atoi(p3);
		if ( p4 )
			strcpy( pPortFilter->comment, p4 );
		portFilterNum++;
		break;

	case TRIGGERPORT_ARRAY_T:
	/*
		getVal15((char *)value, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15);
		if (p1 == NULL || p2 == NULL || p3 == NULL || p4 == NULL || p5 == NULL || p6 == NULL) {
			printf("Invalid TRIGGERPORT arguments!\n");
			break;
		}
		pTriggerPort = (TRIGGERPORT_Tp)(((long)pMib)+pTbl[i].offset+triggerPortNum*sizeof(TRIGGERPORT_T));
		pTriggerPort->tri_fromPort = (unsigned short)atoi(p1);
		pTriggerPort->tri_toPort = (unsigned short)atoi(p2);
		pTriggerPort->tri_protoType = (unsigned char)atoi(p3);
		pTriggerPort->inc_fromPort[0][0] = (unsigned char)atoi(p4);
		pTriggerPort->inc_fromPort[0][1] = (unsigned char)atoi(p5);
		pTriggerPort->inc_fromPort[1][0] = (unsigned char)atoi(p6);
		pTriggerPort->inc_fromPort[1][1] = (unsigned char)atoi(p7);
		pTriggerPort->inc_fromPort[2][0] = (unsigned char)atoi(p8);
		pTriggerPort->inc_fromPort[2][1] = (unsigned char)atoi(p9);
		pTriggerPort->inc_fromPort[3][0] = (unsigned char)atoi(p10);
		pTriggerPort->inc_fromPort[3][1] = (unsigned char)atoi(p11);
		pTriggerPort->inc_fromPort[4][0] = (unsigned char)atoi(p12);
		pTriggerPort->inc_fromPort[4][1] = (unsigned char)atoi(p13);

		pTriggerPort->inc_protoType = (unsigned char)atoi(p14);

		if ( p15 )
			strcpy( pTriggerPort->comment, p15);
		triggerPortNum++;
		*/
		break;
//-------------------------------------------------------------------
#endif

	default:
		return -1;
	}
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////
static int generateBinFile(int type, char *filename, int flag)
{	int fh;
	char *ptr, *pHsTag, *pDsTag, *pCsTag;
	PARAM_HEADER_T header;
	unsigned char checksum;
	int mib_len, section_len;

	if (type == PC_MODE) {
		char *buf;
		buf=calloc(1, DF_SETTING_LEN);
		if ( buf == NULL ) {
			printf("Allocate buffer failed!\n");
			return -1;
		}
		fh = open(filename, O_RDWR|O_CREAT|O_TRUNC);
		write(fh, buf, DF_SETTING_LEN);
		free(buf);

		pHsTag = HW_SETTING_HEADER_TAG;

		pDsTag = DEFAULT_SETTING_HEADER_TAG;
		pCsTag = CURRENT_SETTING_HEADER_TAG;
	}
	else {
#ifdef WIN32
		_chmod(filename, S_IREAD|S_IWRITE);
		fh = open(filename, O_RDWR|O_CREAT|O_TRUNC|O_BINARY);
#else
		chmod(filename, S_IREAD|S_IWRITE);
		fh = open(filename, O_RDWR|O_CREAT|O_TRUNC);
#endif
		if ((flag & 0x7f) == 0) {  // upgrade
			pHsTag = HW_SETTING_HEADER_UPGRADE_TAG;
			pDsTag = DEFAULT_SETTING_HEADER_UPGRADE_TAG;
			pCsTag = CURRENT_SETTING_HEADER_UPGRADE_TAG;
		}
		else if ((flag & 0x7f) == 1) { // update
			pHsTag = HW_SETTING_HEADER_TAG;
			pDsTag = DEFAULT_SETTING_HEADER_TAG;
			pCsTag = CURRENT_SETTING_HEADER_TAG;
		}
		else {
			pHsTag = HW_SETTING_HEADER_FORCE_TAG;
			pDsTag = DEFAULT_SETTING_HEADER_FORCE_TAG;
			pCsTag = CURRENT_SETTING_HEADER_FORCE_TAG;
		}
	}

	if ( fh == -1 )
		return -1;

	lseek(fh, HW_SETTING_OFFSET, SEEK_SET);

	if (config.hwmib_exist) {
		if (config.hwmib_ver == 0) {
			sprintf(header.signature, "%s%02d", pHsTag, HW_SETTING_VER);
			header.len = sizeof(HW_SETTING_T) + sizeof(checksum);
		}
		else {
			sprintf(header.signature, "%s%02d", pHsTag, config.hwmib_ver);
			header.len = config.hwmib_len;
		}
		mib_len = header.len;
		ptr = (char *)&config.hwmib;
//		checksum = CHECKSUM(ptr, header.len-1);
//		ptr[header.len-1] = checksum;
		if ( type == PC_MODE )
			lseek(fh, HW_SETTING_OFFSET, SEEK_SET);
		else {
			if ( !(flag & RAW_TYPE) ) {
   				ENCODE_DATA(ptr, header.len);
			}
			header.len = WORD_SWAP(header.len);
		}

		checksum = CHECKSUM(ptr, mib_len-1);
		ptr[mib_len-1] = checksum;

		write(fh, &header, sizeof(header));
		write(fh, ptr, mib_len);
		if ( flag & RAW_TYPE ) { // raw type, pad 0
			section_len = HW_SETTING_LEN - sizeof(header) - mib_len;
			while (section_len-- >0) {
				write(fh, "\x0", 1);
			}
		}
	}

	lseek(fh, DEFAULT_SETTING_OFFSET, SEEK_SET);
	if (config.dsmib_exist) {
		if (config.dsmib_ver == 0) {
			sprintf(header.signature, "%s%02d", pDsTag, DEFAULT_SETTING_VER);
			header.len = sizeof(APMIB_T) + sizeof(checksum);
		}
		else {
			sprintf(header.signature, "%s%02d", pDsTag, config.dsmib_ver);
			header.len = config.dsmib_len;
		}
		mib_len = header.len;
		ptr = (void *)&config.dsmib;

		checksum = CHECKSUM(ptr, header.len-1);
		ptr[header.len-1] = checksum;
//		dump_mem(ptr, 0x1ffa);

		if ( type == PC_MODE )
			lseek(fh, DEFAULT_SETTING_OFFSET, SEEK_SET);
		else {
			swap_mib_word_value(&config.dsmib);
			if ( !(flag&RAW_TYPE) ) {
				ENCODE_DATA(ptr, header.len);
			}
			header.len = WORD_SWAP(header.len);
		}

		write(fh, &header, sizeof(header));
		write(fh, ptr, mib_len);

		if ( flag & RAW_TYPE ) { // raw type, pad 0
			section_len = DF_SETTING_LEN - sizeof(header) - mib_len;
			while (section_len-- >0) {
				write(fh, "\x0", 1);
			}
		}
	}

/*	if (config.csmib_exist) {	//removed by Erwin
		if (config.csmib_ver == 0) {
			sprintf(header.signature, "%s%02d", pCsTag, CURRENT_SETTING_VER);
			header.len = sizeof(APMIB_T) + sizeof(checksum);
		}
		else {
			sprintf(header.signature, "%s%02d", pCsTag, config.csmib_ver);
			header.len = config.csmib_len;
		}
		mib_len = header.len;
		ptr = (char *)&config.csmib;
		checksum = CHECKSUM(ptr, header.len-1);
		ptr[header.len-1] = checksum;
		if ( type == PC_MODE )
			lseek(fh, CURRENT_SETTING_OFFSET, SEEK_SET);
		else {
			swap_mib_word_value(&config.csmib);
			if (!(flag&RAW_TYPE)) {
				ENCODE_DATA(ptr, header.len);
			}
			header.len = WORD_SWAP(header.len);
		}
		write(fh, &header, sizeof(header));
		write(fh, ptr, mib_len);

		if ( flag & RAW_TYPE ) { // raw type, pad 0
			section_len = DF_SETTING_LEN - sizeof(header) - mib_len;
			while (section_len-- >0) {
				write(fh, "\x0", 1);
			}
		}
	}
*/
	close(fh);

#ifdef WIN32
	_chmod(filename, S_IREAD);
#else
	chmod(filename, S_IREAD);
#endif

	return 0;
}


////////////////////////////////////////////////////////////////////////////////
static int generateTxtFile(char *filename)
{


	struct in_addr ia_value;
	unsigned char *p;
	FILE *fp;
	char buf[500], line[300], prefix[20],subbuf[20];
	char  tmp1[400];
	int i, j;
	MACFILTER_Tp pWlAc;
    int max_chan_num;
#ifdef HOME_GATEWAY
	PORTFW_Tp pPortFw;
	PORTFILTER_Tp pPortFilter;
	IPFILTER_Tp pIpFilter;
	MACFILTER_Tp pMacFilter;
	TRIGGERPORT_Tp pTriggerPort;
	URLB_Tp pURLB;
	ACPC_Tp pACPC;
	DMZ_Tp pDMZ;
	PORTFW_Tp pVSER;
#endif
	int idx=0;
	mib_table_entry_T *pTbl;
	char *pMib;
	APMIB_Tp pApMib;

	fp = fopen(filename, "w");
	if ( fp == NULL)
		return -1;

next_config:
	if ( ++idx>3) {
		fclose(fp);
		return 0;
	}
	if (idx == 1) {
		if ( !config.hwmib_exist )
			goto next_config;
		pTbl = hwmib_table;
		pMib = (char *)&config.hwmib;
		prefix[0] = '\0';
	}
	else if (idx == 2) {
		if ( !config.dsmib_exist )
			goto next_config;
		pTbl = mib_table;
		pMib = (char *)&config.dsmib;
		pApMib = &config.dsmib;
		strcpy(prefix, "DEF_");
	}

	else if (idx == 3) {
		if ( !config.csmib_exist )
			goto next_config;
		pTbl = mib_table;
		pMib = (char *)&config.csmib;
		pApMib = &config.csmib;
		prefix[0] = '\0';
	}

	for (i=0; pTbl[i].id; i++) {
		switch (pTbl[i].type) {
		case BYTE_T:
			WRITE_LINE("%s%s=%d\n", prefix, pTbl[i].name,
				*((unsigned char *)(((long)pMib) + pTbl[i].offset)));
			break;

		case WORD_T:
			WRITE_LINE("%s%s=%d\n", prefix, pTbl[i].name,
				*((unsigned short *)(((long)pMib) + pTbl[i].offset)));
			break;

		case STRING_T:
			WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name,
				(char *)(((long)pMib) + pTbl[i].offset));
			break;

		case BYTE5_T:
			p = (unsigned char *)(((long)pMib) + pTbl[i].offset);
			sprintf(buf, "%02x%02x%02x%02x%02x", p[0], p[1], p[2], p[3], p[4]);
			WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			break;

		case BYTE6_T:
			p = (unsigned char *)(((long)pMib) + pTbl[i].offset);
			sprintf(buf, "%02x%02x%02x%02x%02x%02x", p[0], p[1], p[2], p[3], p[4], p[5]);
			WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			break;

		case BYTE13_T:
			p = (unsigned char *)(((long)pMib) + pTbl[i].offset);
			sprintf(buf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
				p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12]);
			WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			break;

		case BYTE16_T:
			p = (unsigned char *)(((long)pMib) + pTbl[i].offset);
			sprintf(buf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
				p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
			WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			break;

		case DWORD_T:
			WRITE_LINE("%s%s=%d\n", prefix, pTbl[i].name,
				*((unsigned long *)(((long)pMib) + pTbl[i].offset)));
			break;

		case INDEX_T:
			WRITE_LINE("%s%s=%d\n", prefix, pTbl[i].name,
				*((unsigned long *)(((long)pMib) + pTbl[i].offset)));
			break;
		case INDEXDOS_T:
			WRITE_LINE("%s%s=%d\n", prefix, pTbl[i].name,
				*((unsigned long *)(((long)pMib) + pTbl[i].offset)));
			break;

#ifdef WLAN_WPA
#if 0
		case BYTE32_T:
			p = (unsigned char *)(((long)pMib) + pTbl[i].offset);
			buf[0] = '\0';
			for (i=0; i<32; i++) {
				sprintf(line, "%02x", p[i]);
				strcat(buf, line);
			}
			WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			break;
#endif
#endif

		case IA_T:
		 memcpy( (unsigned char *) &ia_value,(unsigned char *)(((long)pMib) + pTbl[i].offset), 4);
			WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name,inet_ntoa(ia_value));
			break;

/*		case BYTE_ARRAY_T:
			if ( pTbl[i].id == MIB_HW_TX_POWER_CCK || pTbl[i].id == MIB_HW_TX_POWER_OFDM ) {
				buf[0] = '\0';
				p = (unsigned char *)(((long)pMib) + pTbl[i].offset);
				for (j=0; j<MAX_CHAN_NUM; j++) {
					sprintf(line, "%d", *(p+ j) );
					if ( j+1 != MAX_CHAN_NUM)
						strcat(line, " ");
					strcat(buf, line);
				}
				WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			}
			break;
*/
		case BYTE_ARRAY_T:
			if ( pTbl[i].id == MIB_HW_TX_POWER_CCK || pTbl[i].id == MIB_HW_TX_POWER_OFDM ) {
				max_chan_num = (pTbl[i].id == MIB_HW_TX_POWER_CCK)? MAX_CCK_CHAN_NUM: MAX_OFDM_CHAN_NUM;
				buf[0] = '\0';
				p = (unsigned char *)(((long)pMib) + pTbl[i].offset);
				for (j=0; j<max_chan_num; j++) {
					sprintf(line, "%d", *(p+ j) );
					if ( j+1 != max_chan_num)
						strcat(line, " ");
					strcat(buf, line);
				}
				WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			}
			break;

		case WLAC_ARRAY_T:
			for (j=0; j<pApMib->acNum; j++) {
				pWlAc = (MACFILTER_Tp)(((long)pMib) + pTbl[i].offset + j*sizeof(MACFILTER_T));
				sprintf(buf, "%02x%02x%02x%02x%02x%02x", pWlAc->macAddr[0], pWlAc->macAddr[1],
					pWlAc->macAddr[2], pWlAc->macAddr[3], pWlAc->macAddr[4], pWlAc->macAddr[5]);
				if (strlen(pWlAc->comment)) {
					strcat(buf, ", ");
					strcat(buf, pWlAc->comment);
				}
				WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			}
			break;
#ifdef HOME_GATEWAY
//////////////////////////////////////////////////////////////////////////////////////////////
		case VSER_ARRAY_T:
			for (j=0; j<pApMib->vserNum; j++) {
				pVSER = (PORTFW_Tp)(((long)pMib) + pTbl[i].offset + j*sizeof(PORTFW_T));
				memcpy( (unsigned char *)&ia_value,pVSER->ipAddr, 4);
				sprintf(buf, "%s, %d, %d, %d", inet_ntoa(ia_value),
					pVSER->fromPort, pVSER->toPort, pVSER->protoType);
				if (strlen(pVSER->comment)) {
					strcat(buf, ", ");
					strcat(buf, pVSER->comment);
				}
				WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			}
			break;

		case DMZ_ARRAY_T:
			for (j=0; j<pApMib->DMZNum; j++) {
				pDMZ = (DMZ_Tp)(((long)pMib) + pTbl[i].offset + j*sizeof(DMZ_T));
				memcpy( (unsigned char *)&ia_value,pDMZ->pipAddr, 4);
				sprintf(buf, "%s",inet_ntoa(ia_value));///////////////
				memcpy( (unsigned char *)&ia_value,pDMZ->cipAddr, 4);
				sprintf(subbuf, ", %s",inet_ntoa(ia_value));////////////
				strcat(buf, subbuf);////////////////////
				if (strlen(pPortFw->comment)) {
					strcat(buf, ", ");
					strcat(buf, pPortFw->comment);
				}
				WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			}
			break;

		case ACPC_ARRAY_T:
			for (j=0; j<pApMib->ACPCNum; j++) {
				pACPC = (ACPC_Tp)(((long)pMib) + pTbl[i].offset + j*sizeof(ACPC_T));
				memcpy( (unsigned char *)&ia_value,pACPC->eipAddr, 4);
				sprintf(subbuf, ", %s", inet_ntoa(ia_value));
				memcpy( (unsigned char *)&ia_value,pACPC->sipAddr, 4);
				sprintf(buf, "%s, %s, %s, %lu, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
				pIpFilter->comment, inet_ntoa(ia_value), subbuf,
				pACPC->serindex, pACPC->protoType, pACPC->Port[0][0], pACPC->Port[0][1],
				pACPC->Port[1][0], pACPC->Port[1][1],pACPC->Port[2][0], pACPC->Port[2][1],
				pACPC->Port[3][0], pACPC->Port[3][1],pACPC->Port[4][0], pACPC->Port[4][1]);
				WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			}
			break;

		case URLB_ARRAY_T:
			for (j=0; j<pApMib->URLBNum; j++) {
				pURLB = (URLB_Tp)(((long)pMib) + pTbl[i].offset + j*sizeof(URLB_T));
				sprintf(buf, "%s", pURLB->URLBAddr);////////////////////////////////////
				WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			}
			break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case PORTFW_ARRAY_T:
			for (j=0; j<pApMib->portFwNum; j++) {
				pPortFw = (PORTFW_Tp)(((long)pMib) + pTbl[i].offset + j*sizeof(PORTFW_T));
				memcpy( (unsigned char *)&ia_value,pPortFw->ipAddr, 4);
				sprintf(buf, "%s, %d, %d, %d", inet_ntoa(ia_value),
					pPortFw->fromPort, pPortFw->toPort, pPortFw->protoType);
				if (strlen(pPortFw->comment)) {
					strcat(buf, ", ");
					strcat(buf, pPortFw->comment);
				}
				WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			}
			break;

		case PORTFILTER_ARRAY_T:
			for (j=0; j<pApMib->portFilterNum; j++) {
				pPortFilter = (PORTFILTER_Tp)(((long)pMib) + pTbl[i].offset + j*sizeof(PORTFILTER_T));
				sprintf(buf, "%d, %d, %d", pPortFilter->fromPort,
					pPortFilter->toPort, pPortFilter->protoType);
				if (strlen(pPortFilter->comment)) {
					strcat(buf, ", ");
					strcat(buf, pPortFilter->comment);
				}
				WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			}
			break;

		case IPFILTER_ARRAY_T:
			for (j=0; j<pApMib->ipFilterNum; j++) {
				pIpFilter = (IPFILTER_Tp)(((long)pMib) + pTbl[i].offset + j*sizeof(IPFILTER_T));
					memcpy( (unsigned char *)&ia_value,pIpFilter->ipAddr, 4);
				sprintf(buf, "%s, %d", inet_ntoa(ia_value), pIpFilter->protoType);
				if (strlen(pIpFilter->comment)) {
					strcat(buf, ", ");
					strcat(buf, pIpFilter->comment);
				}
				WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			}
			break;

		case MACFILTER_ARRAY_T:
			for (j=0; j<pApMib->macFilterNum; j++) {
				pMacFilter = (MACFILTER_Tp)(((long)pMib) + pTbl[i].offset + j*sizeof(MACFILTER_T));
				sprintf(buf, "%02x%02x%02x%02x%02x%02x", pMacFilter->macAddr[0], pMacFilter->macAddr[1],
					pMacFilter->macAddr[2], pMacFilter->macAddr[3], pMacFilter->macAddr[4], pMacFilter->macAddr[5]);
				if (strlen(pMacFilter->comment)) {
					strcat(buf, ", ");
					strcat(buf, pMacFilter->comment);
				}
				WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			}
			break;

		case TRIGGERPORT_ARRAY_T:
			for (j=0; j<pApMib->triggerPortNum; j++) {
				pTriggerPort = (TRIGGERPORT_Tp)(((long)pMib) + pTbl[i].offset + j*sizeof(TRIGGERPORT_T));
			memset(tmp1,0,sizeof(tmp1));
			memcpy( (unsigned char *)&ia_value,pTriggerPort->ip, 4);
		sprintf(buf, "%s:", inet_ntoa(ia_value));

		memset(tmp1,0,sizeof(tmp1));
			for (j=0 ; j<5 ; j++)
		{
			if ( pTriggerPort->tcp_port[j][0] != 0)
			{
				if ( pTriggerPort->tcp_port[j][0] == pTriggerPort->tcp_port[j][1] )
					sprintf(tmp1,  "%d", pTriggerPort->tcp_port[j][0]);
				else
					sprintf(tmp1,  "%d-%d", pTriggerPort->tcp_port[j][0], pTriggerPort->tcp_port[j][1]);
				if(j != 0)
					strcat(buf, ",");
				strcat(buf, tmp1);
			}
		}
		strcat(buf, ":TCP:");

		for (j=0 ; j<5 ; j++)
		{
			if ( pTriggerPort->udp_port[j][0] != 0)
			{
				if ( pTriggerPort->udp_port[j][0] == pTriggerPort->udp_port[j][1] )
					sprintf(tmp1,  "%d", pTriggerPort->udp_port[j][0]);
				else
					sprintf(tmp1,  "%d-%d", pTriggerPort->udp_port[j][0], pTriggerPort->udp_port[j][1]);
				if(j != 0)
					strcat(buf, ",");
				strcat(buf, tmp1);
			}
		}
		strcat(buf, ":UDP");
				/*
				sprintf(buf, "%d, %d, %d, %d, %d, %d",
					pTriggerPort->tri_fromPort,pTriggerPort->tri_toPort, pTriggerPort->tri_protoType,
					pTriggerPort->inc_fromPort,pTriggerPort->inc_toPort, pTriggerPort->inc_protoType);
				if (strlen(pTriggerPort->comment)) {
					strcat(buf, ", ");
					strcat(buf, p"%d:%d"Port->comment);
				}
				*/


				WRITE_LINE("%s%s=%s\n", prefix, pTbl[i].name, buf);
			}
			break;

#endif

		default:
			printf("set mib with invalid type!\n");
			break;
		}
	}

	goto next_config;
}


////////////////////////////////////////////////////////////////////////////////
static void swap_mib_word_value(APMIB_Tp pMib)
{

	pMib->fragThreshold = WORD_SWAP(pMib->fragThreshold);
	pMib->rtsThreshold = WORD_SWAP(pMib->rtsThreshold);
	pMib->supportedRates = WORD_SWAP(pMib->supportedRates);
	pMib->basicRates = WORD_SWAP(pMib->basicRates);
	pMib->beaconInterval = WORD_SWAP(pMib->beaconInterval);
	pMib->inactivityTime = DWORD_SWAP(pMib->inactivityTime);
	pMib->leaseTime = DWORD_SWAP(pMib->leaseTime);				//Lan

#ifdef WLAN_WPA
	pMib->wpaGroupRekeyTime = DWORD_SWAP(pMib->wpaGroupRekeyTime);
	pMib->rsPort = WORD_SWAP(pMib->rsPort);
	pMib->accountRsPort = WORD_SWAP(pMib->accountRsPort);
	pMib->accountRsUpdateDelay = WORD_SWAP(pMib->accountRsUpdateDelay);
	pMib->rsIntervalTime = WORD_SWAP(pMib->rsIntervalTime);
	pMib->accountRsIntervalTime = WORD_SWAP(pMib->accountRsIntervalTime);
#endif

#ifdef HOME_GATEWAY
{
	int i,j;
	pMib->pppMTU = WORD_SWAP(pMib->pppMTU);						//PPPoE
	pMib->pptpMTU = WORD_SWAP(pMib->pptpMTU);					//PPTP
	pMib->L2TPMTU = WORD_SWAP(pMib->L2TPMTU);					//L2TP
	pMib->pppIdleTime = WORD_SWAP(pMib->pppIdleTime);
	pMib->pptpIdleTime = WORD_SWAP(pMib->pptpIdleTime);			//PPTP
	pMib->reManPort = WORD_SWAP(pMib->reManPort);				//Remote Management
	pMib->L2TPIdleTime = WORD_SWAP(pMib->L2TPIdleTime);			//L2TP
	pMib->appLayerGateway = DWORD_SWAP(pMib->appLayerGateway);	//ALG
	pMib->TimeZoneSel = WORD_SWAP(pMib->TimeZoneSel);			//Time Zone
	pMib->startMonth = WORD_SWAP(pMib->startMonth);
	pMib->startDay = WORD_SWAP(pMib->startDay);
	pMib->endMonth = WORD_SWAP(pMib->endMonth);
	pMib->endDay = WORD_SWAP(pMib->endDay);
	pMib->dhisHostID = DWORD_SWAP(pMib->dhisHostID);
	pMib->podPack = WORD_SWAP(pMib->podPack);
	pMib->podBur = WORD_SWAP(pMib->podBur);
	pMib->synPack = WORD_SWAP(pMib->synPack);
	pMib->synBur = WORD_SWAP(pMib->synBur);
	pMib->scanNum = DWORD_SWAP(pMib->scanNum);
	for (i=0; i<pMib->portFwNum; i++) {
		pMib->portFwArray[i].fromPort = WORD_SWAP(pMib->portFwArray[i].fromPort);
		pMib->portFwArray[i].toPort = WORD_SWAP(pMib->portFwArray[i].toPort);
	}
	for (i=0; i<pMib->portFilterNum; i++) {
		pMib->portFilterArray[i].fromPort = WORD_SWAP(pMib->portFilterArray[i].fromPort);
		pMib->portFilterArray[i].toPort = WORD_SWAP(pMib->portFilterArray[i].toPort);
	}
	for (i=0; i<pMib->triggerPortNum; i++) {
		//kyle
		for (j=0; j<5;j++) {
			pMib->triggerPortArray[i].tcp_port[j][0] = WORD_SWAP(pMib->triggerPortArray[i].tcp_port[j][0]);
			pMib->triggerPortArray[i].tcp_port[j][1] = WORD_SWAP(pMib->triggerPortArray[i].tcp_port[j][1]);
			pMib->triggerPortArray[i].udp_port[j][0] = WORD_SWAP(pMib->triggerPortArray[i].udp_port[j][0]);
			pMib->triggerPortArray[i].udp_port[j][1] = WORD_SWAP(pMib->triggerPortArray[i].udp_port[j][1]);
		}
	}
	for (i=0; i<pMib->vserNum; i++) {
		pMib->vserArray[i].fromPort = WORD_SWAP(pMib->vserArray[i].fromPort);
		pMib->vserArray[i].toPort = WORD_SWAP(pMib->vserArray[i].toPort);
	}
	for (i=0; i<pMib->ACPCNum; i++) {
		pMib->ACPCArray[i].serindex = DWORD_SWAP(pMib->ACPCArray[i].serindex);
		for (j=0; j<5;j++) {
			pMib->ACPCArray[i].Port[j][0] = WORD_SWAP(pMib->ACPCArray[i].Port[j][0]);
			pMib->ACPCArray[i].Port[j][1] = WORD_SWAP(pMib->ACPCArray[i].Port[j][1]);
		}
	}
	for (i=0; i<pMib->sroutNum; i++) {
		pMib->sroutArray[i].hopCount = WORD_SWAP(pMib->sroutArray[i].hopCount);
	}
}
#endif
}

////////////////////////////////////////////////////////////////////////////////
static void getVal2(char *value, char **p1, char **p2)
{
	value = getVal(value, p1);
	if ( value )
		getVal(value, p2);
	else
		*p2 = NULL;
}

#ifdef HOME_GATEWAY
////////////////////////////////////////////////////////////////////////////////
static void getVal3(char *value, char **p1, char **p2, char **p3)
{
	*p1 = *p2 = *p3 = NULL;

	value = getVal(value, p1);
	if ( !value )
		return;
	value = getVal(value, p2);
	if ( !value )
		return;
	getVal(value, p3);
}

////////////////////////////////////////////////////////////////////////////////
static void getVal4(char *value, char **p1, char **p2, char **p3, char **p4)
{
	*p1 = *p2 = *p3 = *p4 = NULL;

	value = getVal(value, p1);
	if ( !value )
		return;
	value = getVal(value, p2);
	if ( !value )
		return;
	value = getVal(value, p3);
	if ( !value )
		return;
	getVal(value, p4);
}

////////////////////////////////////////////////////////////////////////////////
static void getVal5(char *value, char **p1, char **p2, char **p3, char **p4, char **p5)
{
	*p1 = *p2 = *p3 = *p4 = *p5 = NULL;

	value = getVal(value, p1);
	if ( !value )
		return;
	value = getVal(value, p2);
	if ( !value )
		return;
	value = getVal(value, p3);
	if ( !value )
		return;
	value = getVal(value, p4);
	if ( !value )
		return;
	getVal(value, p5);
}

////////////////////////////////////////////////////////////////////////////////
static void getVal7(char *value, char **p1, char **p2, char **p3, char **p4, \
	char **p5, char **p6, char **p7)
{
	*p1 = *p2 = *p3 = *p4 = *p5 = *p6 = *p7 = NULL;

	value = getVal(value, p1);
	if ( !value )
		return;
	value = getVal(value, p2);
	if ( !value )
		return;
	value = getVal(value, p3);
	if ( !value )
		return;
	value = getVal(value, p4);
	if ( !value )
		return;
	value = getVal(value, p5);
	if ( !value )
		return;
	value = getVal(value, p6);
	if ( !value )
		return;
	value = getVal(value, p7);
}
static void getVal15(char *value, char **p1, char **p2, char **p3, char **p4, char **p5, char **p6, char **p7, char **p8, char **p9, char **p10, char **p11, char **p12, char **p13, char **p14, char **p15)
{
	*p1 = *p2 = *p3 = *p4 = *p5 = *p6 = *p7 = *p8 = *p9 = *p10= *p11 = *p12 = *p13 = *p14 = *p15 = NULL;

	value = getVal(value, p1);
	if ( !value )
		return;
	value = getVal(value, p2);
	if ( !value )
		return;
	value = getVal(value, p3);
	if ( !value )
		return;
	value = getVal(value, p4);
	if ( !value )
		return;
	value = getVal(value, p5);
	if ( !value )
		return;
	value = getVal(value, p6);
	if ( !value )
		return;
	value = getVal(value, p7);
	if ( !value )
		return;
	value = getVal(value, p8);
	if ( !value )
		return;
	value = getVal(value, p9);
	if ( !value )
		return;
	value = getVal(value, p10);
	if ( !value )
		return;
	value = getVal(value, p11);
	if ( !value )
		return;
	value = getVal(value, p12);
	if ( !value )
		return;
	value = getVal(value, p13);
	if ( !value )
		return;
	value = getVal(value, p14);
	if ( !value )
		return;
	getVal(value, p15);
}
#endif

#ifdef WIN32
/*
 * Check whether "cp" is a valid ascii representation
 * of an Internet address and convert to a binary address.
 * Returns 1 if the address is valid, 0 if not.
 * This replaces inet_addr, the return value from which
 * cannot distinguish between failure and a local broadcast address.
 */
int inet_aton(cp, addr)
	register const char *cp;
	struct in_addr *addr;
{
	register in_addr_t val;
	register int base, n;
	register char c;
	unsigned int parts[4];
	register unsigned int *pp = parts;

	c = *cp;
	for (;;) {
		/*
		 * Collect number up to ``.''.
		 * Values are specified as for C:
		 * 0x=hex, 0=octal, isdigit=decimal.
		 */
		if (!isdigit(c))
			return (0);
		val = 0; base = 10;
		if (c == '0') {
			c = *++cp;
			if (c == 'x' || c == 'X')
				base = 16, c = *++cp;
			else
				base = 8;
		}
		for (;;) {
			if (isascii(c) && isdigit(c)) {
				val = (val * base) + (c - '0');
				c = *++cp;
			} else if (base == 16 && isascii(c) && isxdigit(c)) {
				val = (val << 4) |
					(c + 10 - (islower(c) ? 'a' : 'A'));
				c = *++cp;
			} else
				break;
		}
		if (c == '.') {
			/*
			 * Internet format:
			 *	a.b.c.d
			 *	a.b.c	(with c treated as 16 bits)
			 *	a.b	(with b treated as 24 bits)
			 */
			if (pp >= parts + 3)
				return (0);
			*pp++ = val;
			c = *++cp;
		} else
			break;
	}
	/*
	 * Check for trailing characters.
	 */
	if (c != '\0' && (!isascii(c) || !isspace(c)))
		return (0);
	/*
	 * Concoct the address according to
	 * the number of parts specified.
	 */
	n = pp - parts + 1;
	switch (n) {

	case 0:
		return (0);		/* initial nondigit */

	case 1:				/* a -- 32 bits */

// david ----------
		val <<=24;
//-----------------

		break;

	case 2:				/* a.b -- 8.24 bits */

// david ----------
		val <<=16;
//-----------------
		if ((val > 0xffffff) || (parts[0] > 0xff))
			return (0);
		val |= parts[0] << 24;
		break;

	case 3:				/* a.b.c -- 8.8.16 bits */
// david ----------
		val <<=8;
//-----------------

		if ((val > 0xffff) || (parts[0] > 0xff) || (parts[1] > 0xff))
			return (0);
		val |= (parts[0] << 24) | (parts[1] << 16);
		break;

	case 4:				/* a.b.c.d -- 8.8.8.8 bits */
		if ((val > 0xff) || (parts[0] > 0xff) || (parts[1] > 0xff) || (parts[2] > 0xff))
			return (0);
		val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
		break;
	}
	if (addr)
		addr->s_addr = htonl(val);
	return (1);
}
#endif
