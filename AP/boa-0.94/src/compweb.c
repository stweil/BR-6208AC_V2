#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXFNAME	60
#define WEB_PAGE_OFFSET 0x10000

#ifndef _LITTLE_ENDIAN_
#define DWORD_SWAP(v) ( (((v&0xff)<<24)&0xff000000) | ((((v>>8)&0xff)<<16)&0xff0000) | \
				                ((((v>>16)&0xff)<<8)&0xff00) | (((v>>24)&0xff)&0xff) )
#define WORD_SWAP(v) ((unsigned short)(((v>>8)&0xff) | ((v<<8)&0xff00)))

#else
#define DWORD_SWAP(v) (v)
#define WORD_SWAP(v) (v)
#endif

#define __PACK__	__attribute__ ((packed))

typedef struct _file_entry {
	char name[MAXFNAME];
	unsigned long size;
} file_entry_T;

typedef struct _header {
	unsigned char signature[4];
	unsigned long addr;
    unsigned long burnAddr;
	unsigned long len;
} HEADER_T, *HEADER_Tp;

//char *tag="WEBP";
//char *tagGW="WBWB";
//char *tag6104WG="WBGW";
//char *tag6104K="WB4K";

/////////////////////////////////////////////////////////////////////////////
static int compress(char *inFile, char *outFile)
{
	char tmpBuf[100];

	sprintf(tmpBuf, "gzip -c %s > %s", inFile, outFile);
	system(tmpBuf);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
static unsigned char CHECKSUM(unsigned char *data, int len)
{
	int i;
	unsigned char sum=0;

	for (i=0; i<len; i++)
		sum += data[i];

	sum = ~sum + 1;
	return sum;
}


/////////////////////////////////////////////////////////////////////////////
static int lookfor_homepage_dir(FILE *lp, char *dirpath, int is_for_web)
{
	char file[MAXFNAME];
	char *p;
	struct stat sbuf;

	fseek(lp, 0L, SEEK_SET);
	dirpath[0] = '\0';

	while (fgets(file, sizeof(file), lp) != NULL) {
		if ((p = strchr(file, '\n')) || (p = strchr(file, '\r'))) {
			*p = '\0';
		}
		if (*file == '\0') {
			continue;
		}
		if (stat(file, &sbuf) == 0 && sbuf.st_mode & S_IFDIR) {
			continue;
		}
		if (is_for_web)
// 			p=strstr(file, "home.asp"); // Lance 2003.06.15
			p=strstr(file, "index.asp");

		else
			p=strrchr(file, '/');
		if (p) {

			*p = '\0';
			strcpy(dirpath, file);
// for debug
//printf("Found dir=%s\n", dirpath);
			return 0;
		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
static void strip_dirpath(char *file, char *dirpath)
{
	char *p, tmpBuf[MAXFNAME];

	if ((p=strstr(file, dirpath))) {
		strcpy(tmpBuf, &p[strlen(dirpath)]);
		strcpy(file, tmpBuf);
	}
// for debug
//printf("adding file %s\n", file);
}

typedef struct{
	char	name[20];
	char	SigTag[5];
} Model_List;

Model_List modelList[] = 
{
	{"7206APG",  "APKG"},
	{"7205ASG",  "APSG"},
	{"6214WSG",  "WRSG"},
	{"6204WKG",	 "WBWB"},
	{"6216MG",	 "WRRM"},
	{"", ""}
};

/////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	char *outFile, *fileList;
	int fh;
	struct stat sbuf;
	FILE *lp;
	char file[MAXFNAME];
	char tmpFile[100], dirpath[100];
	char buf[512];
	file_entry_T entry;
	unsigned char	*p;
	int i, len, fd, nFile, is_web=1;
	HEADER_T head;
	int model = 0;

	// Lance 2003.08.06
	while( strcmp(modelList[model].name, ""))
	{
		if( !strcmp( argv[1], modelList[model].name))
			break;
		model ++;
	}

	printf("\nModel = %d, argv[1] = %s\n", model, argv[1]);

	if( !strcmp(modelList[model].name, ""))
	{
		printf("Usage: %s [7206APG|7205ASG|6214WSG|6204WKG|6216MG] in_dir out_file\n",argv[0]);
		exit(1);
	}
	
	fileList = argv[2];
	outFile = argv[3];
	if ( argc > 4)
		is_web = 0;

	fh = open(outFile, O_RDWR|O_CREAT|O_TRUNC);
	if (fh == -1) {
		printf("Create output file error %s!\n", outFile );
		exit(1);
	}
	lseek(fh, 0L, SEEK_SET);

	if ((lp = fopen(fileList, "r")) == NULL) {
		printf("Can't open file list %s\n!", fileList);
		exit(1);
	}
	if (lookfor_homepage_dir(lp, dirpath, is_web)<0) {
//		printf("Can't find home.asp page\n"); // Lance 2003.06.15
		printf("Can't find index.asp page\n");
		fclose(lp);
		exit(1);
	}

	fseek(lp, 0L, SEEK_SET);
	nFile = 0;
	while (fgets(file, sizeof(file), lp) != NULL) {
		if ((p = strchr(file, '\n')) || (p = strchr(file, '\r'))) {
			*p = '\0';
		}
		if (*file == '\0') {
			continue;
		}
		if (stat(file, &sbuf) == 0 && sbuf.st_mode & S_IFDIR) {
			continue;
		}

		if ((fd = open(file, O_RDONLY)) < 0) {
			printf("Can't open file %s\n", file);
			exit(1);
		}
		lseek(fd, 0L, SEEK_SET);

		strip_dirpath(file, dirpath);

		strcpy(entry.name, file);

//#ifndef _LITTLE_ENDIAN_   
		entry.size = DWORD_SWAP(sbuf.st_size);
//#endif

		if ( write(fh, (const void *)&entry, sizeof(entry))!=sizeof(entry) ) {
			printf("Write file failed!\n");
			exit(1);
		}

		i = 0;
		while ((len = read(fd, buf, sizeof(buf))) > 0) {
			if ( write(fh, (const void *)buf, len)!=len ) {
				printf("Write file failed!\n");
				exit(1);
			}
			i += len;
		}
		close(fd);
		if ( i != sbuf.st_size ) {
			printf("Size mismatch in file %s!\n", file );
		}

		nFile++;
	}

	fclose(lp);
	close(fh);
	sync();

// for debug -------------
#if 0
sprintf(tmpFile, "cp %s web.lst -f", outFile);
system(tmpFile);
#endif
//-------------------------

	sprintf(tmpFile, "%sXXXXXX",  outFile);
	mkstemp(tmpFile);
	if ( compress(outFile, tmpFile) < 0) {
		printf("compress file error!\n");
		exit(1);
	}

//printf("\ntmpFile=\"%s\" outFile=\"%s\"\n",tmpFile,outFile);
	// append header
	if (stat(tmpFile, &sbuf) != 0) {
		printf("Create file error!\n");
		exit(1);
	}

	p = malloc(sbuf.st_size + 1);
	if ( p == NULL ) {
		printf("allocate buffer failed!\n");
		exit(1);
	}

	// Lance 2003.08.06
	 
	memcpy(head.signature, modelList[model].SigTag, 4);
	
	head.len = sbuf.st_size + 1;

//	head.len = DWORD_SWAP(head.len);
	head.len = DWORD_SWAP(0x56565);
	head.addr = DWORD_SWAP(WEB_PAGE_OFFSET);
//	head.burnAddr = DWORD_SWAP(WEB_PAGE_OFFSET);
	head.burnAddr = DWORD_SWAP(0x10111213);
			
	if ((fd = open(tmpFile, O_RDONLY)) < 0) {
		printf("Can't open file %s\n", tmpFile);
		exit(1);
	}
	lseek(fd, 0L, SEEK_SET);
	if ( read(fd, p, sbuf.st_size) != sbuf.st_size ) {
		printf("read file error!\n");
		exit(1);
	}
	close(fd);

	p[sbuf.st_size] = CHECKSUM(p, sbuf.st_size);
	fh = open(outFile, O_RDWR|O_CREAT|O_TRUNC);
	if (fh == -1) {
		printf("Create output file error %s!\n", outFile );
		exit(1);
	}

	if ( write(fh, &head, sizeof(head)) != sizeof(head)) {
		printf("write header failed!\n");
		exit(1);
	}

	if ( write(fh, p, sbuf.st_size+1 ) != sbuf.st_size+1) {
		printf("write data failed!\n");
		exit(1);
	}

	close(fh);
//printf("\ntmpFile=\"%s\" outFile=\"%s\"\n",tmpFile,outFile);
	chmod(outFile, S_IREAD);

	sync();

	free(p);
	unlink(tmpFile);

	return 0;
}
