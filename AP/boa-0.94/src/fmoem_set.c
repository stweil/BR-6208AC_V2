/*-- System inlcude files --*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <linux/wireless.h>
#include <dirent.h>
#include <fcntl.h>
/*-- Local inlcude files --*/
#include "../webs.h"
#include "apmib.h"
#include "utility.h"
#include "apform.h"


void oem_set_define_form ()
{
	return;
}

void OK_MSG(webs_t wp, char *url)
{
	websHeader(wp);
	websWrite(wp, T("<!--OK_MSG--><head>\n"));
	websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Expires\" content=\"-1\">\n"));
	websWrite(wp, T("<link rel=\"stylesheet\" href=\"/file/set.css\">\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/javascript.js\"></script>\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/multilanguage.var\"></script>\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/allasp-n.var\"></script>\n"));
	websWrite(wp, T("<title>Success!</title>\n"));
	websWrite(wp, T("</head>\n"));
	websWrite(wp, T("<body class=\"mainbg\">\n"));
	websWrite(wp, T("<blockquote><br><a class=\"titlecolor\"><script>dw(SaveSettingsSuccessfully)</script></a><br><br>\n"));
	websWrite(wp, T("<a class=\"textcolor\"><script>dw(ApplyToReset)</script></a>\n"));
	websWrite(wp, T("<br><br>\n"));

	if (url){
		websWrite(wp, T("<form method=POST action=\"/goform/formApply\" name=okform>\n"));
		websWrite(wp, T("<script>\n"));
		websWrite(wp, T("document.write('<input type=\"button\" value=\"'+showText(CONTINUE)+'\" class=\"button\" OnClick=window.location.replace(\"%s\")>&nbsp;&nbsp;');\n"),url);
		websWrite(wp, T("document.write('<input type=\"submit\" value=\"'+showText(APPLY)+'\" class=\"button\">');\n"));
		websWrite(wp, T("document.write('<input type=hidden name=\"submit-url\" value=\"%s\">');\n"),url);
		websWrite(wp, T("document.write('<input type=hidden name=\"KeyID\" value=\"%s\">');\n"),url);
		websWrite(wp, T("</script>\n"));
	}else{
		websWrite(wp, T("<form>\n"));
		websWrite(wp, T("<script>document.write('<input type=button value=\"'+showText(OK)+'\" class=\"button\" onClick=window.close();>');</script>\n"));
	}
	
	websWrite(wp, T("</form>\n"));
	websWrite(wp, T("</blockquote>\n"));
	websWrite(wp, T("</body>\n"));
	websFooter(wp);
	websDone(wp, 200);
}

void ERR_MSG(webs_t wp, char *msg)
{
	websHeader(wp);
	websWrite(wp, T("<!--ERR_MSG--><head>\n"));
	websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Expires\" content=\"-1\">\n"));
	websWrite(wp, T("<link rel=\"stylesheet\" href=\"/set.css\">\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/javascript.js\"></script>\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/multilanguage.var\"></script>\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/allasp-n.var\"></script>\n"));
	websWrite(wp, T("<title>Error!</title>\n"));
	websWrite(wp, T("</head>\n"));
	websWrite(wp, T("<body class=\"mainbg\">\n"));
	websWrite(wp, T("<blockquote>\n"));
	websWrite(wp, T("<br><a class=\"restartInfo\">%s</a><br><br>\n"),msg);
	websWrite(wp, T("<script>\n"));
	websWrite(wp, T("document.write('<input type=button value=\"'+showText(OK)+'\" class=\"ui-button-nolimit\" onClick=\"history.go (-1);\">');\n"));
	websWrite(wp, T("</script>\n"));
	websWrite(wp, T("</blockquote>\n"));
	websFooter(wp);
	websDone(wp, 200);
}


void MY_ERR_MSG(webs_t wp, char *msg)
{
	websHeader(wp);
	websWrite(wp, T("<!--MY_ERR_MSG--><head>\n"));
	websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Expires\" content=\"-1\">\n"));
	websWrite(wp, T("<link rel=\"stylesheet\" href=\"/file/set.css\">\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/javascript.js\"></script>\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/multilanguage.var\"></script>\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/allasp-n.var\"></script>\n"));
	websWrite(wp, T("<title>Error!</title>\n"));
	websWrite(wp, T("</head>\n"));
	websWrite(wp, T("<body class=\"mainbg\">\n"));
	websWrite(wp, T("<blockquote>\n"));
	websWrite(wp, T("<br><a class=\"titlecolor\"><script>document.write(showText(%s))</script></a><br><br>\n"),msg);
	websWrite(wp, T("<script>\n"));
	websWrite(wp, T("document.write('<input type=button value=\"'+showText(OK)+'\" class=\"button\" onClick=\"history.go (-1);\">');\n"));
	websWrite(wp, T("</script>\n"));
	websWrite(wp, T("</blockquote>\n"));
	websFooter(wp);
	websDone(wp, 200);
}

void REDIRECT_PAGE(webs_t wp, char *url)
{
	websHeader(wp);
	websWrite(wp, T("<body class=\"background\" onLoad=document.location.replace(\"%s\")>"), url);
	websFooter(wp);
	websDone(wp, 200);
}

#ifdef _WIFI_ROMAING_
void REBOOT_MSG(webs_t wp, char *msg, char *url)
{
	websHeader(wp);
	websWrite(wp, T("<!--REBOOT_MSG--><head>\n"));
	websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Expires\" content=\"-1\">\n"));
	websWrite(wp, T("<link rel=\"stylesheet\" href=\"/set.css\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Content-Type\" content=\"text/html\">\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/javascript.js\"></script>\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/multilanguage.var\"></script>\n"));
	websWrite(wp, T("<title></title>\n"));
	websWrite(wp, T("</head>\n"));
	websWrite(wp, T("<body>\n"));
	websWrite(wp, T("<blockquote><br><br><div class=\"restartInfo\">%s</div><br><br>\n"),msg);
	websWrite(wp, T("<form name=\"test\">\n"));
	if(url)
	{
		if(lanipChanged==1)	websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=top.location.replace(\"%s\")>');</script>\n"),url);
		else websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=window.location.replace(\"%s\")>');</script>\n"),url);
	}
	else websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=window.close()>');</script>\n"));
	websWrite(wp, T("</form>\n"));
	websWrite(wp, T("</blockquote>\n"));
	websWrite(wp, T("<script>\n"));
	websWrite(wp, T("var secs = 120;\n"));
	websWrite(wp, T("var wait = secs * 1000;\n"));
	websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + secs + \")\";\n"));
	websWrite(wp, T("document.test.okbutton.disabled = true;\n"));
	websWrite(wp, T("for(i = 1; i <= secs; i++)	setTimeout(\"update(\" + i + \")\", i * 1000);\n"));
	websWrite(wp, T("setTimeout(\"timer()\", wait);\n"));
	websWrite(wp, T("function update(num, value)\n"));
	websWrite(wp, T("{\n"));
	websWrite(wp, T("if (num == (wait/1000)) document.test.okbutton.value = showText(OK);\n"));
	websWrite(wp, T("else\n"));
	websWrite(wp, T("{\n"));
	websWrite(wp, T("printnr = (wait / 1000)-num;\n"));
	websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + printnr + \")\";\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("function timer()\n"));
	websWrite(wp, T("{\n"));
	websWrite(wp, T("document.test.okbutton.disabled = false;\n"));
	websWrite(wp, T("document.test.okbutton.value = showText(OK);\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("</script>\n"));
	websWrite(wp, T("</body>\n"));
	websFooter(wp);
	websDone(wp, 200);
}
#else
void REBOOT_MSG(webs_t wp, char *msg, char *url)
{
	websHeader(wp);
	websWrite(wp, T("<!--REBOOT_MSG--><head>\n"));
	websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Expires\" content=\"-1\">\n"));
	websWrite(wp, T("<link rel=\"stylesheet\" href=\"/set.css\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Content-Type\" content=\"text/html\">\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/javascript.js\"></script>\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/multilanguage.var\"></script>\n"));
	websWrite(wp, T("<title></title>\n"));
	websWrite(wp, T("</head>\n"));
	websWrite(wp, T("<body>\n"));
	websWrite(wp, T("<blockquote><br><br><div class=\"restartInfo\">%s</div><br><br>\n"),msg);
	websWrite(wp, T("<form name=\"test\">\n"));
	if(url)
	{
		if(lanipChanged==1)	websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=top.location.replace(\"%s\")>');</script>\n"),url);
		else websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=window.location.replace(\"%s\")>');</script>\n"),url);
	}
	else websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=window.close()>');</script>\n"));
	websWrite(wp, T("</form>\n"));
	websWrite(wp, T("</blockquote>\n"));
	websWrite(wp, T("<script>\n"));
	websWrite(wp, T("var secs = 90;\n"));
	websWrite(wp, T("var wait = secs * 1000;\n"));
	websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + secs + \")\";\n"));
	websWrite(wp, T("document.test.okbutton.disabled = true;\n"));
	websWrite(wp, T("for(i = 1; i <= secs; i++)	setTimeout(\"update(\" + i + \")\", i * 1000);\n"));
	websWrite(wp, T("setTimeout(\"timer()\", wait);\n"));
	websWrite(wp, T("function update(num, value)\n"));
	websWrite(wp, T("{\n"));
	websWrite(wp, T("if (num == (wait/1000)) document.test.okbutton.value = showText(OK);\n"));
	websWrite(wp, T("else\n"));
	websWrite(wp, T("{\n"));
	websWrite(wp, T("printnr = (wait / 1000)-num;\n"));
	websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + printnr + \")\";\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("function timer()\n"));
	websWrite(wp, T("{\n"));
	websWrite(wp, T("document.test.okbutton.disabled = false;\n"));
	websWrite(wp, T("document.test.okbutton.value = showText(OK);\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("</script>\n"));
	websWrite(wp, T("</body>\n"));
	websFooter(wp);
	websDone(wp, 200);
}
#endif




void closelater_MSG(webs_t wp, char *msg)
{
	websHeader(wp);
	websWrite(wp, T("<!--closelater_MSG--><head>\n"));
	websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Expires\" content=\"-1\">\n"));
	websWrite(wp, T("<link rel=\"stylesheet\" href=\"/set.css\">\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/javascript.js\"></script>\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/multilanguage.var\"></script>\n"));
	websWrite(wp, T("<title> </title>\n"));
	websWrite(wp, T("</head>\n"));
	websWrite(wp, T("<body>\n"));
	websWrite(wp, T("<blockquote>\n"));
	websWrite(wp, T("<br><a class=\"restartInfo\">%s</a><br><br>\n"),msg);
	websWrite(wp, T("<script>\n"));
	websWrite(wp, T("	document.write('<p align=\"center\"><input type=button value=\"'+showText(OK)+'\" class=\"ui-button\"  onClick=\"self.opener.location.reload();window.close();\">');\n"));
	websWrite(wp, T("</script>\n"));
	websWrite(wp, T("</blockquote>\n"));
	websFooter(wp);
	websDone(wp, 200);
}

#ifdef _WIFI_ROMAING_
void DEFAULTandGOTOurl(webs_t wp, char *msg, char *url)
{
	websHeader(wp);
	websWrite(wp, T("<!--DEFAULTandGOTOurl--><head>\n"));
	websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Expires\" content=\"-1\">\n"));
	websWrite(wp, T("<link rel=\"stylesheet\" href=\"/set.css\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Content-Type\" content=\"text/html\">\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/javascript.js\"></script>\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/multilanguage.var\"></script>\n"));
	websWrite(wp, T("<title></title>\n"));
	websWrite(wp, T("</head>\n"));
	websWrite(wp, T("<body>\n"));
	websWrite(wp, T("<blockquote><br><br><div class=\"restartInfo\">%s</div><br><br>\n"),msg);
	websWrite(wp, T("<form name=\"test\">\n"));
	if(url)
	{
		if(lanipChanged==1)	websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=top.location.replace(\"%s\")>');</script>\n"),url);
		else websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=parent.location.replace(\"%s\")>');</script>\n"),url);
	}
	else websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=window.close()>');</script>\n"));
	websWrite(wp, T("</form>\n"));
	websWrite(wp, T("</blockquote>\n"));
	websWrite(wp, T("<script>\n"));
	websWrite(wp, T("var secs = 120;\n"));
	websWrite(wp, T("var wait = secs * 1000;\n"));
	websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + secs + \")\";\n"));
	websWrite(wp, T("document.test.okbutton.disabled = true;\n"));
	websWrite(wp, T("for(i = 1; i <= secs; i++)	setTimeout(\"update(\" + i + \")\", i * 1000);\n"));
	websWrite(wp, T("setTimeout(\"timer()\", wait);\n"));
	websWrite(wp, T("function update(num, value)\n"));
	websWrite(wp, T("{\n"));
	websWrite(wp, T("if (num == (wait/1000)) document.test.okbutton.value = showText(OK);\n"));
	websWrite(wp, T("else\n"));
	websWrite(wp, T("{\n"));
	websWrite(wp, T("printnr = (wait / 1000)-num;\n"));
	websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + printnr + \")\";\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("function timer()\n"));
	websWrite(wp, T("{\n"));
	websWrite(wp, T("document.test.okbutton.disabled = false;\n"));
	websWrite(wp, T("document.test.okbutton.value = showText(OK);\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("</script>\n"));
	websWrite(wp, T("</body>\n"));
	websFooter(wp);
	websDone(wp, 200);
}
#else
void DEFAULTandGOTOurl(webs_t wp, char *msg, char *url)
{
	websHeader(wp);
	websWrite(wp, T("<!--DEFAULTandGOTOurl--><head>\n"));
	websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Expires\" content=\"-1\">\n"));
	websWrite(wp, T("<link rel=\"stylesheet\" href=\"/set.css\">\n"));
	websWrite(wp, T("<meta http-equiv=\"Content-Type\" content=\"text/html\">\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/javascript.js\"></script>\n"));
	websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/multilanguage.var\"></script>\n"));
	websWrite(wp, T("<title></title>\n"));
	websWrite(wp, T("</head>\n"));
	websWrite(wp, T("<body>\n"));
	websWrite(wp, T("<blockquote><br><br><div class=\"restartInfo\">%s</div><br><br>\n"),msg);
	websWrite(wp, T("<form name=\"test\">\n"));
	if(url)
	{
		if(lanipChanged==1)	websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=top.location.replace(\"%s\")>');</script>\n"),url);
		else websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=parent.location.replace(\"%s\")>');</script>\n"),url);
	}
	else websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=window.close()>');</script>\n"));
	websWrite(wp, T("</form>\n"));
	websWrite(wp, T("</blockquote>\n"));
	websWrite(wp, T("<script>\n"));
	websWrite(wp, T("var secs = 90;\n"));
	websWrite(wp, T("var wait = secs * 1000;\n"));
	websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + secs + \")\";\n"));
	websWrite(wp, T("document.test.okbutton.disabled = true;\n"));
	websWrite(wp, T("for(i = 1; i <= secs; i++)	setTimeout(\"update(\" + i + \")\", i * 1000);\n"));
	websWrite(wp, T("setTimeout(\"timer()\", wait);\n"));
	websWrite(wp, T("function update(num, value)\n"));
	websWrite(wp, T("{\n"));
	websWrite(wp, T("if (num == (wait/1000)) document.test.okbutton.value = showText(OK);\n"));
	websWrite(wp, T("else\n"));
	websWrite(wp, T("{\n"));
	websWrite(wp, T("printnr = (wait / 1000)-num;\n"));
	websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + printnr + \")\";\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("function timer()\n"));
	websWrite(wp, T("{\n"));
	websWrite(wp, T("document.test.okbutton.disabled = false;\n"));
	websWrite(wp, T("document.test.okbutton.value = showText(OK);\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("</script>\n"));
	websWrite(wp, T("</body>\n"));
	websFooter(wp);
	websDone(wp, 200);
}
#endif


