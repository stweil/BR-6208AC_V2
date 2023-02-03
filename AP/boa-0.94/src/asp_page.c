
//#include <linux/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h> //davidhsu
#include "boa.h"
//#include "fmasp.h"

#ifdef SUPPORT_ASP

#include "asp_page.h"
#include "apmib.h"
#include "apform.h"
//#include "uemf.h"
#if defined(SUPPORT_HNAP)
void send_r_request_ok_hnap(request * req);
extern int isSetGetToggle;
#endif
extern void create_chklist_file(int type);
extern int isPackExist;
asp_name_t *root_asp=NULL;
form_name_t *root_form=NULL;
temp_mem_t root_temp;
char *query_temp_var=NULL;

//char *WAN_IF;
//char *BRIDGE_IF;
//char *ELAN_IF;
//char *PPPOE_IF;
//char WLAN_IF[20];
int wlan_num;
//int last_wantype=-1;
void handleForm(request *req); // davidhsu

int websAspDefine(char* name, int (*fn)(request * req,  int argc, char **argv))
{
	asp_name_t *asp;
	asp=(asp_name_t*)malloc(sizeof(asp_name_t));

	if(asp==NULL) 
	{
//		printf("memory alloc fail\n");
		return ERROR;
	}
	asp->pagename=(char *)malloc(strlen(name)+1);
	strcpy(asp->pagename,name);
	asp->next=NULL;
	asp->function=fn;

	
	if(root_asp==NULL) 
		root_asp=asp;
	else
	{
		//modify the previous's next pointer
		asp_name_t *now_asp;
		now_asp=root_asp;
		while(now_asp->next!=NULL) now_asp=now_asp->next;
		now_asp->next=asp;
	}		
//	printf("ASP define success\n");
	
	return SUCCESS;	
}

int websFormDefine(char* name, void (*fn)(request * req,  char* path, char* query))
{

	form_name_t *form;
	form=(form_name_t *)malloc(sizeof(form_name_t));
	if(form==NULL) return ERROR;
	form->pagename=(char *)malloc(strlen(name)+1);
	strcpy(form->pagename,name);
	form->next=NULL;
	form->function=fn;
	
	if(root_form==NULL) 
		root_form=form;
	else
	{
		//modify the previous's next pointer
		form_name_t *now_form;
		now_form=root_form;
		while(now_form->next!=NULL) now_form=now_form->next;
		now_form->next=form;
	}		
	return SUCCESS;	

}

/******************************************************************************/
/*
 *	Write a webs header. This is a convenience routine to write a common
 *	header for a form back to the browser.
 */

void websHeader(request *wp)
{
	//a_assert(websValid(wp));

	websWrite(wp, T("%s"), T("HTTP/1.0 200 OK\n"));

/*
 *	By license terms the following line of code must not be modified
 */
	websWrite(wp, T("Server: %s\r\n"), SERVER_VERSION);

	websWrite(wp, T("%s"), T("Pragma: no-cache\n"));
	websWrite(wp, T("%s"), T("Cache-control: no-cache\n"));
	websWrite(wp, T("%s"), T("Content-Type: text/html\n"));
	websWrite(wp, T("%s"), T("\n"));
	websWrite(wp, T("%s"), T("<html>\n"));
}

/******************************************************************************/
/*
 *	Write a webs footer
 */

void websFooter(request * wp)
{
	//a_assert(websValid(wp));

	websWrite(wp, T("%s"), T("</html>\n"));
}

/******************************************************************************/
int addTempStr(char *str)
{
	temp_mem_t *temp,*newtemp;
	temp=&root_temp;
	while(temp->next)
	{
		temp=temp->next;
	}
	newtemp=(temp_mem_t *)malloc(sizeof(temp_mem_t));
	if(newtemp==NULL) return FAILED;
	newtemp->str=str;
	newtemp->next=NULL;
	temp->next=newtemp;	
	return SUCCESS;
}

void freeAllTempStr(void)
{
	temp_mem_t *temp,*ntemp;
	temp=root_temp.next;
	root_temp.next=NULL;
	while(1)
	{
		if(temp==NULL) break;
		ntemp=temp->next;
		free(temp->str);
		free(temp);
		temp=ntemp;
	}
}	

int getcgiparam(char *dst,char *query_string,char *param,int maxlen)
{
	int len,plen;
	int y;

	plen=strlen(param);
	while (*query_string)
	{
		len=strlen(query_string);

		if ((len=strlen(query_string))>plen)
			if (!strncmp(query_string,param,plen))
				if (query_string[plen]=='=')
				{//copy parameter
					query_string+=plen+1;
					y=0;
					while ((*query_string)&&(*query_string!='&'))
					{	
						if ((*query_string=='%')&&(strlen(query_string)>2))
							if ((isxdigit(query_string[1]))&&(isxdigit(query_string[2])))
							{
								if (y<maxlen)
									dst[y++]=((toupper(query_string[1])>='A'?toupper(query_string[1])-'A'+0xa:toupper(query_string[1])-'0') << 4)
									+ (toupper(query_string[2])>='A'?toupper(query_string[2])-'A'+0xa:toupper(query_string[2])-'0');
								query_string+=3;
								continue;
							}
						if (*query_string=='+')
						{
							if (y<maxlen)
								dst[y++]=' ';
							query_string++;
							continue;
						}
						if (y<maxlen)
							dst[y++]=*query_string;
						query_string++;
					}
					if (y<maxlen) dst[y]=0;
					return y;
				}
		while ((*query_string)&&(*query_string!='&')) query_string++;
		query_string++;
	}
	if (maxlen) dst[0]=0;
	return -1;
}


char *websGetVar(request *req, char *var, char *defaultGetValue)
{
	char *buf;
#ifndef NEW_POST	
	struct stat statbuf;
#endif
	int ret=-1;

	if (req->method == M_POST)
	{
		int i;
// davidhsu --------------------------------		
#ifndef NEW_POST		
		fstat(req->post_data_fd, &statbuf);
		buf=(char *)malloc(statbuf.st_size+1);
		if(buf==NULL) return (char *)defaultGetValue;
		lseek(req->post_data_fd, SEEK_SET, 0);
		read(req->post_data_fd,buf,statbuf.st_size);
		buf[statbuf.st_size]=0;
		i=statbuf.st_size - 1;
#else
		buf=(char *)malloc(req->post_data_len+1);
		if(buf==NULL) return (char *)defaultGetValue;
		req->post_data_idx = 0;
		memcpy(buf, req->post_data, req->post_data_len);
		buf[req->post_data_len]=0;
		i=req->post_data_len -1;
#endif		
//-------------------------------------------
		while(i > 0)
		{
			if((buf[i]==0x0a)||(buf[i]==0x0d)) buf[i]=0;
			else break;
			i--;
		}
	}
	else
	{	
		buf=req->query_string;
	}

	if(buf!=NULL)
	{
		ret=getcgiparam(query_temp_var,buf,var,MAX_QUERY_TEMP_VAL_SIZE);	
	}
	
	if(req->method == M_POST) free(buf);
	
	if(ret<0) return (char *)defaultGetValue;
	buf=(char *)malloc(ret+1);
	memcpy(buf,query_temp_var,ret);
	buf[ret]=0;
	addTempStr(buf);
	return (char *)buf;	//this buffer will be free by freeAllTempStr().

}


unsigned int __flash_base;
unsigned int __crmr_addr;
unsigned int __wdtcnr_addr;

int board_cfgmgr_init(unsigned char syncAll);

#define STREAM_SAME(s1, s2) \
	((strlen(s1) == strlen(s2)) && (strcmp(s1, s2) == 0))

void websAspInit(int argc,char **argv)
{
//	extern char boot_ver[];
	char reset_head[16]={0};
	int fh;
	int numWrite=0;
	int mount_ret=-1;
	root_temp.next=NULL;
	root_temp.str=NULL;	


	formsDefine(); // apform.c

        websAspDefine(T("getVar"), getVar);
        websAspDefine(T("getiNICVar"), getiNICVar);
        websAspDefine(T("getIndex"), getVar);
        websAspDefine(T("getiNICIndex"), getiNICVar);

	if ( apmib_init() == 0 ) {
		printf("Initialize AP MIB failed!\n");
		return;
	}

	query_temp_var=(char *)malloc(MAX_QUERY_TEMP_VAL_SIZE);
	if(query_temp_var==NULL) exit(0);

	return;
	
	exit(0);
}

int  websRedirect(request *req,char *url)
{
	req->buffer_end=0;
// davidhsu	
	send_redirect_perm(req,url);
	return SUCCESS;
}

int allocNewBuffer(request *req)
{
	char *newBuffer;
	newBuffer=(char *)malloc(req->max_buffer_size*2+1);
	if(newBuffer==NULL) return FAILED;
	memcpy(newBuffer,req->buffer,req->max_buffer_size);
	req->max_buffer_size<<=1;	
	free(req->buffer);	
	req->buffer=newBuffer;
	return SUCCESS;
}


extern int middle_segment; //Brad add for update content length
int websWriteBlock(request *req, char *buf, int nChars)
{

	int bob=nChars;	
#ifndef SUPPORT_ASP
	if((bob+req->buffer_end)>BUFFER_SIZE) bob=BUFFER_SIZE- req->buffer_end;
#else
	while((bob+req->buffer_end+10)>req->max_buffer_size)  //Brad modify
	{
		int ret;
		ret=allocNewBuffer(req);
		if(ret==FAILED) {bob=BUFFER_SIZE- req->buffer_end; break;}
	}
#endif

	
	middle_segment = middle_segment+bob; //brad add for update exact length of asp file
	if(bob>0)
	{
		memcpy(req->buffer + req->buffer_end,	buf, bob);	
		req->buffer_end+=bob;
		return 0;
	}
	return -1;
}

int websWrite(request *req, const char *format, ...) 
{
    char temp[1024];
    va_list ap;
    va_start(ap, format);
    vsnprintf(temp, sizeof(temp), format, ap);
    va_end(ap);
	 
    websWriteBlock(req, temp, strlen(temp));
}

int websDone(request *req, const char *format, ...) 
{
    req_flush(req);
}


void handleForm(request *req)
{
	char *ptr;
#if defined(SUPPORT_HNAP)
	char *content_length_orig1=NULL;
	char *content_length_orig2=NULL;
	int orig_char_length=0;
	int exact_char_length=0;
	int byte_shift=0;
	int total_length_shift=0;
	int head_offset=0;
	int first_offset=0;
#endif
	#define SCRIPT_ALIAS "/goform/"
	extern int check_auth_flag;
	ptr=strstr(req->request_uri,SCRIPT_ALIAS);

//	apmib_get(MIB_WAN_DHCP, (void *)&last_wantype);
#if defined(SUPPORT_HNAP)
	if ((strncasecmp(req->request_uri, "/HNAP", 5) == 0)) {
		form_name_t *now_form;
		now_form=root_form;
		while(1)
		{
			if ((memcmp("formSetHNAP11",now_form->pagename,strlen(now_form->pagename))==0)){
			
				send_r_request_ok_hnap(req);
				first_offset=req->buffer_end;
				now_form->function(req,NULL,NULL);
				exact_char_length = strlen(simple_itoa(req->filesize));
				content_length_orig1 = strstr(req->buffer, "Content-Length:");
				if(content_length_orig1!=NULL){
				content_length_orig2 = strstr(content_length_orig1, "\r\n");
				content_length_orig1 = content_length_orig1 + strlen("Content-Length: ");
				orig_char_length = content_length_orig2 - content_length_orig1;
				//fprintf(stderr, "the orig_char_length=%d\n", orig_char_length);
				exact_char_length = strlen(simple_itoa(req->filesize));
					//fprintf(stderr, "the exact_char_length=%d\n", exact_char_length);
					if(orig_char_length == exact_char_length) {
						//fprintf(stderr, "Update the content length with the same char length!\n");
						memcpy(content_length_orig1, simple_itoa(req->filesize),exact_char_length); 
					}else if(orig_char_length < exact_char_length) {
						//fprintf(stderr, " Update the content length with shift to later bytes!\n");
						byte_shift = exact_char_length - orig_char_length;
						head_offset = first_offset- (content_length_orig2 - req->buffer);
						total_length_shift = head_offset+req->filesize;
						memmove((content_length_orig2+byte_shift), content_length_orig2, total_length_shift);
						memcpy(content_length_orig1, simple_itoa(req->filesize),exact_char_length); 
						req->buffer_end = req->buffer_end+byte_shift; 
					}else {
						//fprintf(stderr, "Update the content length with shift to preceding bytes!\n");
						byte_shift = orig_char_length - exact_char_length;
						head_offset = first_offset- (content_length_orig2 - req->buffer);
						total_length_shift = head_offset+req->filesize;
						memmove((content_length_orig2-byte_shift), content_length_orig2, total_length_shift);
						memcpy(content_length_orig1, simple_itoa(req->filesize),exact_char_length); 
						req->buffer_end = req->buffer_end-byte_shift;  
					}
				}
						freeAllTempStr();
						return;
			}
			if(now_form->next==NULL) break;
				now_form=now_form->next;
			
		}

	}else {
#endif
	if(ptr==NULL)
	{
		send_r_not_found(req);
		return;
	}
	else
	{
		form_name_t *now_form;
		ptr+=strlen(SCRIPT_ALIAS);

		now_form=root_form;
		while(1)
		{
			if (	(strlen(ptr) == strlen(now_form->pagename)) &&
				(memcmp(ptr,now_form->pagename,strlen(now_form->pagename))==0))
			{
//				send_r_request_ok(req);		/* All's well */	
//brad modify for tr_11n				
//#ifdef USE_AUTH
#if 0
				if (check_auth_flag == 1 && strcmp(req->request_uri,"/goform/formLogin")) { // user
					//brad add for wizard
					if(!strcmp(req->request_uri,"/goform/formSetEnableWizard"))
					{				
						//websRedirect(req, "/wizard_back.asp");
						websRedirect(req, WEB_PAGE_USER);	
					}
					else
					{
						ptr= (char *)websGetVar(req, T("webpage"), T(""));
						if(ptr ==NULL)
							sprintf(last_url, "/%s", directory_index);
						else
							strcpy(last_url, ptr);				
						if(!strcmp(req->request_uri,"/goform/formSetRestorePrev") || !strcmp(req->request_uri,"/goform/formSetFactory")){
							sprintf(last_url, "%s","/system_config.asp");
							if (req->upload_data)
								free(req->upload_data);	
						}
						if(!strcmp(req->request_uri,"/goform/formFirmwareUpgrade")){
							sprintf(last_url, "%s","/system_firmware.asp");	
							if (req->upload_data)
								free(req->upload_data);	
						}	
#if 0						
						if(!strcmp(req->request_uri,"/goform/formWPS")){
							wps_action= (char *)websGetVar(req, T("wpsAction"), T(""));
							if(!strcmp(wps_action, "pbc") || !strcmp(wps_action, "pin")){
								now_form->function(req,NULL,NULL);
								freeAllTempStr();
								return;
							}
						}	
#endif						
						websRedirect(req, WEB_PAGE_USER);	
					}
				}
				else
#endif	
				//now_form->function(0, req,NULL,NULL);
				now_form->function(req,NULL,NULL);
				freeAllTempStr();
				return;
			}
			
			if(now_form->next==NULL) break;
			now_form=now_form->next;
		}
	}
		send_r_not_found(req);
		return;
#if defined(SUPPORT_HNAP)		
	}
#endif	

}

void handleScript(request *req,char *left1,char *right1)	
{
	char *left=left1,*right=right1;
	asp_name_t *now_asp;
	unsigned int funcNameLength;
	int i;
	left+=2;
	right-=1;
	while(1)
	{
		while(*left==' ') {if(left>=right) break;++left;}
		while(*left==';') {if(left>=right) break;++left;}
		while(*left=='(') {if(left>=right) break;++left;}
		while(*left==')') {if(left>=right) break;++left;}
		while(*left==',') {if(left>=right) break;++left;}
		if(left>=right) break;

		/* count the function name length */
		{
			char *ptr = left;

			funcNameLength = 0;
			while (*ptr!='(' && *ptr!=' '){
				ptr++;
				funcNameLength++;
				if ((unsigned int )ptr >= (unsigned int)right) {
					break;
				}
			}
		}

		now_asp=root_asp;
		while(1)
		{
			if(	(strlen(now_asp->pagename) == funcNameLength) &&
				(memcmp(left,now_asp->pagename,strlen(now_asp->pagename))==0))
			{
				char *leftc,*rightc;
				int argc=0;
				char *argv[8]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
				left+=strlen(now_asp->pagename);
				
				while(1) 
				{
					int size,exit=0;
					while(1)
					{
						if(*left==')') {exit=1; break;}
						if(*left=='\"') break;
						if((unsigned int)left>(unsigned int)right) {exit=1; break;}
						left++;	
					}
					
					if(exit==1) break;					
					leftc=left;
					leftc++;
					rightc=strstr(leftc,"\"");
					if(rightc==NULL) break;
					size=(unsigned int)rightc-(unsigned int)leftc+1;
					argv[argc]=(char *)malloc(size);
					if(argv[argc]==NULL) break;
					memcpy(argv[argc],leftc,size-1);
					argv[argc][size-1]='\0';
					argc++;
					left=rightc+1;
				}
				now_asp->function(0, req,argc,argv);

				for(i=0;i<argc;i++) free(argv[i]);
				break;
			}

			if(now_asp->next==NULL) break;
			now_asp=now_asp->next;
		}
		++left;
	}
}


#if defined(SUPPORT_HNAP)

void handle_hnap_GethandleScript(request *req)
{
	char *content_length_orig1=NULL;
	char *content_length_orig2=NULL;
	int orig_char_length=0;
	int exact_char_length=0;
	int byte_shift=0;
	int total_length_shift=0;
	int head_offset=0;
	int first_offset=0;	
	
	
	if ((strncasecmp(req->request_uri, "/HNAP", 5) == 0)) {
		asp_name_t *now_asp;
		now_asp=root_asp;
		while(1)
		{
			if ((memcmp("formGetHNAP11",now_asp->pagename,strlen(now_asp->pagename))==0)){
				req->buffer_end=0;
				send_r_request_ok_hnap(req);
				
				first_offset=req->buffer_end;
				now_asp->function(req,0,NULL);
				exact_char_length = strlen(simple_itoa(req->filesize));
				content_length_orig1 = strstr(req->buffer, "Content-Length:");
						if(content_length_orig1!=NULL){
				content_length_orig2 = strstr(content_length_orig1, "\r\n");
				content_length_orig1 = content_length_orig1 + strlen("Content-Length: ");
				orig_char_length = content_length_orig2 - content_length_orig1;
				//fprintf(stderr, "the orig_char_length=%d\n", orig_char_length);
				exact_char_length = strlen(simple_itoa(req->filesize));
					//fprintf(stderr, "the exact_char_length=%d\n", exact_char_length);
					if(orig_char_length == exact_char_length) {
						//fprintf(stderr, "Update the content length with the same char length!\n");
						memcpy(content_length_orig1, simple_itoa(req->filesize),exact_char_length); 
					}else if(orig_char_length < exact_char_length) {
						//fprintf(stderr, " Update the content length with shift to later bytes!\n");
						byte_shift = exact_char_length - orig_char_length;
						head_offset = first_offset- (content_length_orig2 - req->buffer);
						total_length_shift = head_offset+req->filesize;
						memmove((content_length_orig2+byte_shift), content_length_orig2, total_length_shift);
						memcpy(content_length_orig1, simple_itoa(req->filesize),exact_char_length); 
						req->buffer_end = req->buffer_end+byte_shift; 
					}else {
						//fprintf(stderr, "Update the content length with shift to preceding bytes!\n");
						byte_shift = orig_char_length - exact_char_length;
						head_offset = first_offset- (content_length_orig2 - req->buffer);
						total_length_shift = head_offset+req->filesize;
						memmove((content_length_orig2-byte_shift), content_length_orig2, total_length_shift);
						memcpy(content_length_orig1, simple_itoa(req->filesize),exact_char_length); 
						req->buffer_end = req->buffer_end-byte_shift;  
					}
						}
					return;
			}

			if(now_asp->next==NULL) break;
			now_asp=now_asp->next;
		}
	}
}
#endif
#endif

int formArgs(int argc, char **argv, char *fmt, ...)
{
        va_list args;
        char    *c_p, **s_p;
        int     *int_p, i;

        if (argv == NULL) 
		return 0;

        va_start(args, fmt);
        for (i=0, c_p = fmt; c_p && *c_p && argv[i]; ) 
	{
                if (*c_p++ != '%') 
                        continue;
                switch (*c_p) 
		{
                case 'd':
                        int_p = va_arg(args, int*);
                        *int_p = atoi(argv[i]);
                        break;
                case 's':
                        s_p = va_arg(args, char_t**);
                        *s_p = argv[i];
                        break;
                }
                i++;
        }

        va_end(args);
        return i;
}
#if defined(_LAN_WAN_ACCESS_) && defined(__TARGET_BOARD__)
void setLanWanAccess(char remote_ip_addr)
{
FILE *fp;
int i, j;
char buf[100]={0}, cmd[100]={0}, macaddr[20]={0};

system("flash get AP_ROUTER_SWITCH | cut -d '=' -f 2 > /tmp/AP_ROUTER_SWITCH");
if ((fp = fopen("/tmp/AP_ROUTER_SWITCH","r")) != NULL) {
        if (fgets(buf, 100, fp) > 0)
                i = strtol(buf, (char **)NULL, 10);
        fclose(fp);
        system("rm -f /tmp/AP_ROUTER_SWITCH");
}

sprintf(cmd, "cat /proc/net/arp | grep %s | grep eth1 | wc -l > /tmp/proc_net_arp", remote_ip_addr);
system(cmd);
if ((fp = fopen("/tmp/proc_net_arp","r")) != NULL) {
        if (fgets(buf,100, fp) > 0)
                j = strtol(buf, (char **)NULL, 10);
        fclose(fp);
        system("rm -f /tmp/proc_net_arp");
}

if (i == 0 && j == 0) {
        sprintf(cmd, "cat /proc/net/arp | grep %s | tr -s ' ' | cut -d ' ' -f 4 > /tmp/macaddr", remote_ip_addr);
        system(cmd);
        if ((fp = fopen("/tmp/macaddr","r")) != NULL) {
                if (fgets(buf,100, fp) > 0)
                        snprintf(macaddr, 18, "%s", buf);
                fclose(fp);
                system("rm -f /tmp/macaddr");
        }

        sprintf(cmd, "brctl showmacs br0 | grep %s | cut -b 3 > /tmp/portno", macaddr);
        system(cmd);
        if ((fp = fopen("/tmp/portno","r")) != NULL) {
                if (fgets(buf,100, fp) > 0)
                        i=strtol( buf, (char **)NULL, 10);
                fclose(fp);
                system("rm -f /tmp/portno");
        }

        if (i > 2)
                return 1;
	}
}
#endif  //end of LAN_WAN_ACCESS


/*  return 0: SUCC
 * 	   1: PASS Fail 
 *	   2: USER Fail
 */
int auth_check_userpass2(request * req, char *login_userid,char *login_password)
{

char *url=req->request_uri;
char last_url[100];

        if(!strcmp(req->request_uri, "/"))
	{
		sprintf(last_url, "/%s", directory_index);
		send_redirect_perm(req, last_url);

		SQUASH_KA(req);
		return 0;
	}
#if defined(_LAN_WAN_ACCESS_) && defined(__TARGET_BOARD__)
	setLanWanAccess(req->remote_ip_addr);
#endif

        char userName[MAX_NAME_LEN]="admin";
        char userPass[MAX_NAME_LEN]="1234";
#ifdef __TARGET_BOARD__
        apmib_get(MIB_USER_NAME, userName);
        apmib_get(MIB_USER_PASSWORD, userPass);
#endif
	if (strcmp(login_userid, userName))
       		return 2;

	if (strcmp(login_password, userPass))
       		return 1;

        return 0;
}


int getVar(int eid, webs_t wp, int argc, char_t **argv)
{
	char *arg1, *arg2, *arg3;
	char tmpBuf[200];

	formArgs(argc, argv, "%s %s %s", &arg1, &arg2, &arg3);

	getIndex(arg1, tmpBuf);

	if(!strcmp(arg2, ""))
		websWrite(wp, "%s", tmpBuf);
	if(!strcmp(arg2, tmpBuf))
		websWriteBlock(wp, arg3, strlen(arg3));
}

int getiNICVar(int eid, webs_t wp, int argc, char_t **argv)
{
	char *arg1, *arg2, *arg3;
	char tmpBuf[200];

	formArgs(argc, argv, "%s %s %s", &arg1, &arg2, &arg3);

	getiNICIndex(arg1, tmpBuf);

	if(!strcmp(arg2, ""))
		websWrite(wp, "%s", tmpBuf);
	if(!strcmp(arg2, tmpBuf))
		websWriteBlock(wp, arg3, strlen(arg3));
}

int getIndexStr(int eid, webs_t wp, int argc, char_t **argv)
{
	char *arg1, *arg2, *arg3;
	char tmpBuf[200];

	formArgs(argc, argv, "%s", &arg1);

	getIndex(arg1, tmpBuf);
	websWrite(wp, "%s", tmpBuf);
}


int getiNICIndexStr(int eid, webs_t wp, int argc, char_t **argv)
{
	char *arg1, *arg2, *arg3;
	char tmpBuf[200];

	formArgs(argc, argv, "%s", &arg1);

	getiNICIndex(arg1, tmpBuf);
	websWrite(wp, "%s", tmpBuf);
}
#define WINIE6_STR T("/octet-stream\x0d\x0a\0x0d\0x0a")
#define WINIE8_STR T("/plain\x0d\x0a\0x0d\0x0a")
#define MACIE5_FWSTR T("/macbinary\x0d\x0a\0x0d\0x0a")
#define OPERA_FWSTR T("/x-macbinary\x0d\x0a\0x0d\0x0a")
#define LINE_FWSTR T("\x0d\x0a\0x0d\0x0a")
int parseMultiPart(webs_t wp)
{
        int head_offset=0 ;
        unsigned char *pStart=NULL;
        int iestr_offset=0;
        int isunKnow=0;
        char *dquote;
        char *dquote1;
	char *upload_data;

	upload_data = wp->upload_data;
	pStart = strstr(upload_data, WINIE6_STR);
	if (pStart == NULL) {
		pStart = strstr(upload_data, MACIE5_FWSTR);
		if (pStart == NULL) {
			pStart = strstr(upload_data, OPERA_FWSTR);
			if (pStart == NULL) {
				pStart = strstr(upload_data, WINIE8_STR);
				if (pStart == NULL) {
					pStart = strstr(upload_data, "filename=");
					if (pStart == NULL) {
						printf("Firmware Update Failure: Invalid file format!");         
						goto ret_upload;
					}
					else
					{
						dquote =  strstr(pStart, "\"");
						if(dquote !=NULL)
						{
							dquote1 = strstr(dquote, LINE_FWSTR);
							if(dquote1!=NULL){
								iestr_offset = 4;
								pStart = dquote1;
							}
							else
							{
								printf("Firmware Update Failure: Invalid file format!");
								goto ret_upload;
							}
						}
						else
						{
        						printf("Firmware Update Failure: Invalid file format!"); 
        						goto ret_upload;
						}
					}
				}
				else
				{
					iestr_offset = 10;
				}
			}
			else
			{
				iestr_offset = 16;
			}
		}
		else
		{
			iestr_offset = 14;
		}
	}
	else
	{
		iestr_offset = 17;
	}

    head_offset = (int)(((unsigned long)pStart)-((unsigned long)upload_data)) + iestr_offset;

    return head_offset;
ret_upload:
    return 0;
}





