/*
 *      utility to merge all binary into one image
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: mgbin.c,v 1.1.1.1 2005/03/09 04:52:51 rex Exp $
 *
 */
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#ifdef WIN32
	#include <io.h>
#endif

/////////////////////////////////////////////////////////////////////////////
/* Input file type */
//typedef enum { BOOT_CODE=0, CONFIG, WEB_PAGES, SYS } TYPE_T;

typedef enum { BOOT_CODE=0, SYS, CONFIG, WEB_PAGES} TYPE_T;

typedef struct _sector {
	TYPE_T type;
	char filename[80];
//	unsigned long offset;
	unsigned long boundary;		// Rex 
	unsigned long size;
} SECTOR_T;

/////////////////////////////////////////////////////////////////////////////
static char *copyright="Copyright (c) Realtek Semiconductor Corp., 2003. All Rights Reserved.";
static char *version="1.0";
static SECTOR_T sector[]={
    {BOOT_CODE, "\x0", 0,       0x20000},
    {SYS,       "\x0", 0,		0x3A0000},    
    {CONFIG,    "\x0", 0,       0x40000}     
};

void dump_mem(void *ptr, int size)
{   
    int i;
			    
    printf("\n");
    for(i=0; i < size; i++)
    {   
        printf("%4.2x", (unsigned )(((unsigned char*)ptr)[i]));
        if((i%16 == 0) && (i!=0))
            printf("\n");
    }
    printf("\n");
}

/////////////////////////////////////////////////////////////////////////////
static TYPE_T checkInputFile(char *filename)
{	
	int fh;
	char signature[4];

	fh = open(filename, O_RDONLY);
	if ( fh == -1 )
		return -1;

	lseek(fh, 0, SEEK_SET);
	
	if ( read(fh, signature, sizeof(signature)) != sizeof(signature)) {
		close(fh);
		return -1;
	}
	close(fh);


	if ( !memcmp(signature, "\x16\x00\x00\xEA", 4))
		return BOOT_CODE;


	if ( !memcmp(signature, "DS", 2) )
		return CONFIG;
	
	// Lance 2003.08.06
/*	if ( (!memcmp(signature, "WEBP", 4)) || (!memcmp(signature, "WBWB", 4)) 	//Erwin
		 								 || (!memcmp(signature, "WB4K", 4))
		 								 || (!memcmp(signature, "WBGW", 4))
		 								 || (!memcmp(signature, "WB7P", 4))
		 								 || (!memcmp(signature, "WBGA", 4))
		 								 || (!memcmp(signature, "WBIP", 4))
		 								 || (!memcmp(signature, "WBWB", 4))
		 								 || (!memcmp(signature, "WBPS", 4))
		 								 || (!memcmp(signature, "WBKP", 4))
		 								 || (!memcmp(signature, "EWAS", 4))
		 								 || (!memcmp(signature, "WGWP", 4))
		 								 || (!memcmp(signature, "EWCG", 4))
										 									)
		return WEB_PAGES;
*/
	// End Lance 2003.08.06

	if ( !memcmp(signature, "CSYS", 4) )
		return SYS;
	
	return -1;
}

////////////////////////////////////////////////////////////////////////////////
static void showHelp(void)
{
	printf("\nUtility to merge binary.\n");
	printf("%s Ver %s.\n\n", copyright, version);
//	printf("usage: mgbin -o outputfile -u upgradefile bootcode config webpages linux\n\n");
	printf("usage: mgbin -o outputfile -u upgradefile bootcode config linux\n\n");
}

/////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	int argNum=1;
	char outFile[80]={0};
	char upFile[80]={0};
	TYPE_T type;
	int mask=0, sect_num, cont_sect_num, fh_out, fh_in, len, i, total=0, offset=0,total_1=0;
	char *buf;
	struct stat sbuf;

	while (argNum < argc) {
        if ( !strcmp(argv[argNum], "-u") ) {
		    if (++argNum >= argc)
			    break;
			sscanf(argv[argNum], "%s", upFile);
        }
		else
		{
			if ( !strcmp(argv[argNum], "-o") ) {
				if (++argNum >= argc)
					break;
				sscanf(argv[argNum], "%s", outFile);
			}
			else {
				if ( (type=checkInputFile(argv[argNum])) > 4) {
					printf("Invalid input file %s!\n", argv[argNum]);
					showHelp();
					exit(1);
				}
				// printf("argNum = %d, type= %d\n", argNum, type);
				strcpy(sector[type].filename, argv[argNum]);
				mask |= (1 << type);			
			}
		}
		argNum++;
	}
	
	if (!outFile[0]) {
		printf("No output file specified!\n");
		showHelp();
		exit(1);
	}

    if (!upFile[0]) {
	    printf("No upgrade file specified!\n");
	    showHelp();
	    exit(1);
	}
	
	if (mask == 0) {
		printf("No valid input image found!\n");	
		exit(1);
	}
	
	// search image start and end section



	for (sect_num=0, cont_sect_num=0, i=BOOT_CODE; i<=CONFIG; i++) {
		if ( (mask >> i) & 1 ) {
			sect_num++;

#if defined (_RDC3210_)
			if ( i != BOOT_CODE) {
#else			
			if ( i != SYS) {
			
#endif				
				if ( (mask >> (i+1) ) & 1 )  // continous section
					cont_sect_num++;
			}
		}
	}
	
	if (sect_num != (cont_sect_num+1)) {
		printf("Invalid input file sequence!\n");
		exit(1);
	}

	// Create output file
#ifdef WIN32
	_chmod(outFile, S_IREAD|S_IWRITE);
	fh_out = open(outFile, O_RDWR|O_CREAT|O_TRUNC|O_BINARY);
#else
	chmod(outFile, S_IREAD|S_IWRITE);
	fh_out = open(outFile, O_RDWR|O_CREAT|O_TRUNC);
#endif
	if (fh_out == -1) {
		printf("Create output file %s error!\n", outFile);
		exit(1);
	}

	printf("\nMerge ");


	for (i=BOOT_CODE; i<=CONFIG; i++) {
		if (sector[i].filename[0]) {
			if ( stat(sector[i].filename, &sbuf) != 0 ) {
				printf("Stat file %s error!\n", sector[i].filename);
				exit(1);
			}
			
		#if defined (_RDC3210_)	
		if (sector[i].type==CONFIG) {
		    
			buf = malloc(sbuf.st_size+UNUSED_SIZE);
			
		}	
		else 	
		#endif 	
		
			buf = malloc(sbuf.st_size);
	        	
			if (buf == NULL) {
				printf("allocate buffer failed %d!\n", sbuf.st_size);
				exit(1);
			}
#ifdef WIN32
			fh_in = open(sector[i].filename, O_RDONLY|O_BINARY);
#else
			fh_in = open(sector[i].filename, O_RDONLY);
#endif
			if (fh_in < 0) {
				printf("Open file %s error!\n", sector[i].filename);
				close(fh_out);
				exit(1);
			}

			switch(sector[i].type) {
			case BOOT_CODE:
				printf("\nBOOT-CODE    0x%x", offset);
				break;
			case CONFIG:
				printf("\nCONFIG-DATA  0x%x", offset);
				break;
/*			case WEB_PAGES:		//Erwin
				printf("\nWEB-PAGES    0x%x", offset);
				break;
*/
			case SYS:
				printf("\nLINUX        0x%x", offset);
				break;
			}
			
			#if defined (_RDC3210_)	
		
			if (sector[i].type==CONFIG)    	buf+=UNUSED_SIZE;
		        			 
			#endif
			
			if (read(fh_in, buf, sbuf.st_size) != sbuf.st_size) {
				printf("Read file %s error!\n", sector[i].filename);
				close(fh_in);
				close(fh_out);
				free(buf);
				exit(1);
			}
			
				
			#if defined (_RDC3210_)	
		
			if (sector[i].type==CONFIG) {
			buf-=UNUSED_SIZE;
			sbuf.st_size+=UNUSED_SIZE;
			}
			
			# endif
			
			if ( write(fh_out, buf, sbuf.st_size) != sbuf.st_size) {
				printf("Write output file %s error!\n", outFile);
				close(fh_in);
				close(fh_out);
				free(buf);
				exit(1);
			}
				
			free(buf);
			close(fh_in);

			offset += sbuf.st_size;
			printf("%s(file size:%ld) sector[i].size:%d\n", sector[i].filename,sbuf.st_size,sector[i].size);	
			if(sbuf.st_size != ((int)sector[i].size))
			{
			
				if (sbuf.st_size < sector[i].size)  {
				
					len = sector[i].size - sbuf.st_size;
					 printf("pending %d",len);
						
		               }			     			
				else 
				if ((sbuf.st_size > (int)sector[i].size) && (sector[i].boundary != 0)) {
					len = ((offset / sector[i].boundary) + 1) * sector[i].boundary - offset;
				
				}	
				else 
				{
					printf("\n Error !!!, %s out of size\n");
					exit(1);
				}
					
		
				buf = calloc(len, 1);
				
				if (buf == NULL) {
					printf("allocate buffer failed %d!\n", len);
					exit(1);
				}
			   	
				write(fh_out, buf, len);	// pad 0				
				free(buf);
				offset += len;
				total_1+=len;	
			}
		
			// } Rex add for if webPage > default valus (64k) will append to 64k boundary
			total_1+=sbuf.st_size;
			total += sbuf.st_size;
		}
	}

	close(fh_out);

#ifdef WIN32
	_chmod(outFile, S_IREAD);
#else
	chmod(outFile, S_IREAD);
#endif

	printf("=> %s ok, size=%d (pending to %d)\n", outFile, total,total_1);

    // Create upFile file
#ifdef WIN32
    _chmod(upFile, S_IREAD|S_IWRITE);
    fh_out = open(upFile, O_RDWR|O_CREAT|O_TRUNC|O_BINARY);
#else
    chmod(upFile, S_IREAD|S_IWRITE);
    fh_out = open(upFile, O_RDWR|O_CREAT|O_TRUNC);
#endif
    if (fh_out == -1) {
	    printf("Create upgrade file %s error!\n", upFile);
        exit(1);
    }
                                         

  //-------------------------- joseph add ------------------------------------
      exit (0);
//-------------------------- joseph add ------------------------------------                                    
    printf("\nMerge upgrade ");
	
//  Lance - make UI + FW
    offset = 0x10000;
total_1=0;
total=0;
//    for (i=WEB_PAGES; i<=SYS; i++) {
    for (i=SYS; i<=SYS; i++) {	//Erwin
        if (sector[i].filename[0]) {
            if ( stat(sector[i].filename, &sbuf) != 0 ) {
	            printf("Stat file %s error!\n", sector[i].filename);
	            exit(1);
	        }
            buf = malloc(sbuf.st_size);
            if (buf == NULL) {
	            printf("allocate buffer failed %d!\n", sbuf.st_size);
	            exit(1);
	        }
#ifdef WIN32
            fh_in = open(sector[i].filename, O_RDONLY|O_BINARY);
#else
            fh_in = open(sector[i].filename, O_RDONLY);
#endif
            if (fh_in < 0) {
	            printf("Open file %s error!\n", sector[i].filename);
                close(fh_out);
                exit(1);
            }
            switch(sector[i].type) {
	        case BOOT_CODE:
	            printf("BOOT-CODE ");
	            break;
            case CONFIG:
	            printf("CONFIG-DATA ");
                break;
/*            case WEB_PAGES:	//Erwin
                printf("WEB-PAGES ");
                break;
*/
            case SYS:
                printf("LINUX ");
                break;
            }
            if (read(fh_in, buf, sbuf.st_size) != sbuf.st_size) {
                printf("Read file %s error!\n", sector[i].filename);
                close(fh_in);
                close(fh_out);
                free(buf);
                exit(1);
            }
            if ( write(fh_out, buf, sbuf.st_size) != sbuf.st_size) {
	            printf("Write output file %s error!\n", outFile);
                close(fh_in);
                close(fh_out);
                free(buf);
                exit(1);
            }
            free(buf);
            close(fh_in);
		
	offset += sbuf.st_size;
			
	if(!((sbuf.st_size == ((int)sector[i].size)) || (sector[i].type == SYS)))
	{
		if (sbuf.st_size < ((int)sector[i].size))  
			len = sector[i].size - sbuf.st_size;
		else 
		if ((sbuf.st_size > (int)sector[i].size) && (sector[i].boundary != 0)) 
			len = ((offset / sector[i].boundary) + 1) * sector[i].boundary - offset;
		else 
		{
			printf("\n Error !!!, %s out of size\n");
			exit(1);
		}
					
		
		buf = calloc(len, 1);
				
		if (buf == NULL) {
			printf("allocate buffer failed %d!\n", len);
			exit(1);
		}
		write(fh_out, buf, len);	// pad 0				
		free(buf);
		offset += len;
		total_1+=len;	
	}
	// } Rex add for if webPage > default valus (64k) will append to 64k boundary
        total_1+=sbuf.st_size;
	total += sbuf.st_size;
        }
    }
    close(fh_out);

#ifdef WIN32
    _chmod(upFile, S_IREAD);
#else
    chmod(upFile, S_IREAD);
#endif
    	
		printf("=>Upgrade file %s ok, size=%d (need not pending)\n", upFile, total);
	
    	printf("\n");
				
	exit(0);
}

