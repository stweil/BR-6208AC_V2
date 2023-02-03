//#include "globals.h"

#ifndef ASP_PAGE_H
#define ASP_PAGE_H



#ifdef SUPPORT_ASP
#define ERROR -1
#define FAILED -1
#define SUCCESS 0
#define TRUE 1
#define FALSE 0

#define MAX_QUERY_TEMP_VAL_SIZE 2048
#define	T(s)	s



typedef int (*asp_funcptr)(int eid, request * req,  int argc, char **argv);
typedef void (*form_funcptr)(request * req, char* path, char* query);

typedef struct asp_name_s
{
	char *pagename;
	asp_funcptr function;
	struct asp_name_s *next;
} asp_name_t;

typedef struct form_name_s
{
	char *pagename;
	form_funcptr function;
	struct form_name_s *next;
} form_name_t;

typedef struct temp_mem_s
{
	char *str;
	struct temp_mem_s *next;
} temp_mem_t;


int websWriteBlock(request *req, char *buf, int nChars);
int websWrite(request *req, const char *format, ...);

int  websRedirect(request *req,char *url);
char *websGetVar(request *req, char *var, char *defaultGetValue);

void websFooter(request * wp);
void websHeader(request *wp);

int parseMultiPart(webs_t wp);

//#define websDone(arg1,arg2) send_r_request_ok(arg1)		// For WebsDone(200) only
//#define websDone(arg1,arg2) websWrite(arg1, "%s", "") // For WebsDone(200) only
#define websError(arg1,arg2,arg3) send_r_bad_request(arg1)	// For WebsError(200, "Bad request") only
#define websWriteDataNonBlock websWriteBlock


#define ejArgs formArgs 
#define ejSetResult(arg1,arg2) strcpy(indexValue, arg2)

#define gstrcmp         strcmp


#endif
#endif // define _H
