/*
 * Licensed under GNU GPL version 2 Copyright Lance Wu
 * Version: 0.0.1
 *
 * 2003.06.19
 *  - trigger port
 *
 * 2003.07.02
 *  - URL blocking
 *
 */

#include <linux/module.h>
#include <asm/uaccess.h>

#include <linux/proc_fs.h>

#include "proc_create_file.h"

MODULE_AUTHOR("Lance Wu ");
MODULE_DESCRIPTION("Create file in /proc");
MODULE_LICENSE("GPL");

#if 0
//#define DEBUGP(format, args...) printk(__FILE__ ":" format, ## args)
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

// URL blocking

static ssize_t url_blocking_readfile(struct file* file, char* buf, size_t count, loff_t* ppos)
{
	int i;
    static size_t n=0;
    int err = 0;
//	char tmpbuf[MAX_URL_LEN]={0};

//    DEBUGP("into url_blocking_readfile...\n");

    if( n )
    {
//        DEBUGP("end...\n");
		n = 0;
        return 0;
    }
	
	for( i=0; i<MAX_URL_LIST; i++ )
	{
		if( url_list[i][0] == 0 )
			break;
		if( count < strlen(url_list[i]) )
		{
			DEBUGP("buffer to small..!!\n");
			return -EFAULT;
		}
	    err = copy_to_user(buf+n, url_list[i], strlen(url_list[i]));
    	if (err)
	    {
    	    DEBUGP("copy to user fail!!\n");
        	return -EFAULT;
	    }
		
		n += strlen(url_list[i]);
 		*ppos += strlen(url_list[i]);
		count -= strlen(url_list[i]);
	}

    return n;
}

static int     url_blocking_writefile(struct file* file, const char* buf, size_t count, loff_t* ppos)
{
    char *tmp;

//    DEBUGP("into url_blocking_writefile...\n");

//    DEBUGP("count= %d\n",count);

	if( count > MAX_URL_LEN )
    {
		DEBUGP("size too long...!!\n");
        return -EINVAL;
    }

	if( url_count == MAX_URL_LIST )
	{
		DEBUGP("list is full!!\n");
		return -EINVAL;
	}

    if( copy_from_user( &url_list[url_count], buf, count))	
    {
        DEBUGP("copy from user fail!!\n");
        return -EFAULT;
    }

    url_list[url_count][count] = '\0';
	
//    DEBUGP("write to list %d : %s\n",url_count,url_list[url_count]);
	
	url_count++;
	
    return count;
}

static int     url_blocking_read(char* page, char** start, off_t off, int count, int * eof, void *data)
{
	int i;
    int len=0;
	        
//	DEBUGP("into url_blocking_read ...\n");

//	if( len )
//	{
//		len = 0;
//		return 0;
//	}

	for( i=0; i<MAX_URL_LIST; i++ )
	{
		if( url_list[i][0] == 0 )
			break;
		
	    memcpy( page+len, url_list[i], strlen(url_list[i]) );

//		DEBUGP("print:(%d) %s", i, url_list[i]);
		
		len += strlen(url_list[i]);
    	if( len <= off+count )
	    	*eof = 1;
	    *start = page + off;
	}
	
    return len;
}

struct file_operations url_blocking_file_operations = {
    read: url_blocking_readfile,
    write: url_blocking_writefile,
};

// port trigger
static ssize_t trigger_port_readfile(struct file* file, char* buf, size_t count, loff_t* ppos)
{
    size_t n;
    int err = 0;

//    DEBUGP("into trigger_port_readfile...\n");

    if( *ppos >= current_len )
    {
//        DEBUGP("*ppos(%d) >= current_len(%d)\n",*ppos,current_len);
        return 0;
    }
	
    if( count >= current_len )
	    n = current_len;
	else
	    n = count;
		
    if( n == 0 )
    {
//        DEBUGP("n=%d\n",n);
        return -EFAULT;
    }
	
    err = copy_to_user(buf, file_buf + *ppos, n);
    if (err)
    {
        DEBUGP("copy to user fail!!\n");
        return -EFAULT;
    }
																															    *ppos += n;

    return n;
}

static int     trigger_port_writefile(struct file* file, const char* buf, size_t count, loff_t* ppos)
{
    char *tmp;

//    DEBUGP("into trigger_port_writefile...\n");

//    DEBUGP("count= %d\n",count);

    if( count > (MAX_BUF_SIZE-current_len) )
    {
        DEBUGP("size too long...!!\n");
        return -EINVAL;
    }

    if( copy_from_user( &file_buf[current_len], buf, count))
    {
        DEBUGP("copy from user fail!!\n");
        return -EFAULT;
    }

    current_len += count;
    file_buf[current_len] = '\0';

//    DEBUGP("current_len= %d\n",current_len);

    return count;
}
            
static int trigger_port_read(char* page, char** start, off_t off, int count, int * eof, void *data)
{
	int len = current_len;
	
//	DEBUGP("into trigger_port_read ...\n");

	memcpy( page, file_buf, len );
	
    if( len <= off+count ) 
		*eof = 1;
    *start = page + off;
    len -= off;
	if( len > count ) 
		len = count;
    if( len < 0 )
		len = 0;

    return len;
}

struct file_operations trigger_port_file_operations = {
    read: trigger_port_readfile,
    write: trigger_port_writefile,
};

PROC_FILE_TABLE proc_table[] = {
		{ TRIGGER_PORT_FILENAME, S_IRUGO | S_IWUGO, &trigger_port_file_operations, NULL, trigger_port_read},
		{ URL_FILENAME         , S_IRUGO | S_IWUGO, &url_blocking_file_operations, NULL, url_blocking_read},
		{NULL}
};

int init_module(void)
{
	int i;
	struct proc_dir_entry *de;

//	DEBUGP("init Create_Proc_File module...\n");
//	printk("init Create_Proc_File module...\n");

	
	for( i=0; proc_table[i].name; i++ )
	{
		de = create_proc_entry( proc_table[i].name, proc_table[i].mode, &proc_root);
		if( de )
		{
			de->proc_fops = proc_table[i].proc_fops;
			de->proc_iops = proc_table[i].proc_iops;
			de->read_proc = proc_table[i].read_proc;

			DEBUGP("create procfile [%s] success!!\n",proc_table[i].name);
		}
		else
            DEBUGP("create procfile [%s] fail!!\n",proc_table[i].name);
	}
	
	return 0;
}

void cleanup_module(void)
{
	int i;

//    DEBUGP("cleanup Create_Proc_File module...\n");
	
	for( i=0; proc_table[i].name; i++ )
	{
		remove_proc_entry( proc_table[i].name, &proc_root);
		
        DEBUGP("remove procfile [%s]... !!\n",proc_table[i].name);
	}
}
