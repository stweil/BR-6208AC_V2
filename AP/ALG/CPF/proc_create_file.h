#ifndef _LANCE_PROC_CREATE_FILE_H_
#define _LANCE_PROC_CREATE_FILE_H_

typedef struct _proc_file_table{
	char*                     name;
	mode_t                    mode;
	struct file_operations*   proc_fops;
	struct inode_operations*  proc_iops;
	int 					  (*read_proc)(char*,char**,off_t,int,int*,void*);
}PROC_FILE_TABLE,*PPROC_FILE_TABLE;

// port trigger
#define MAX_BUF_SIZE 1024
static char file_buf[MAX_BUF_SIZE];
static int current_len=0;

#define TRIGGER_PORT_FILENAME	"trigger_port_list"

static ssize_t trigger_port_readfile(struct file *, char *, size_t, loff_t *);
static int     trigger_port_writefile(struct file *, const char *, size_t, loff_t *);

// URL blocking
#define URL_FILENAME	"url_blocking_list"
#define MAX_URL_LIST	32
#define MAX_URL_LEN		34
static char url_list[MAX_URL_LIST][MAX_URL_LEN];
static int  url_count=0;

//static int trigger_port_read(char* , char** , off_t , int , int , void* );

#endif
