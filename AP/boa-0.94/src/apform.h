/*
 *      Include file of form handler
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: apform.h,v 1.2 2005/05/13 07:52:54 tommy Exp $
 *
 */
#define BRIDGE0_IF	T("br0")
#define BRIDGE1_IF	T("br1")
#ifndef _INCLUDE_APFORM_H
#define _INCLUDE_APFORM_H
#ifdef _BOA_
#include "../webs.h"
#endif
#if HAVE_STDBOOL_H
# include <stdbool.h>
#else
typedef enum {false = 0, true = 1} bool;
#endif
#define noNO_ACTION
#ifdef HOME_GATEWAY
#ifdef _DEFAULT_WAN_IF_
	#define WAN_IF		T(_DEFAULT_WAN_IF_)
#else
	#define WAN_IF		T("eth2.2")
#endif
#ifndef __TARGET_BOARD__
#ifdef _DEFAULT_LAN_IF_
	#define BRIDGE_IF		T(_DEFAULT_LAN_IF_)
#else
	#define BRIDGE_IF		T("eth2.1")
#endif
#else
	#define BRIDGE_IF		T("br0")
	#define BRIDGE1_IF		T("br1")
#endif
#else
#define BRIDGE_IF			T("br0")
#define BRIDGE1_IF		T("br1")
#endif
#ifdef _DEFAULT_LAN_IF_
	#define ELAN_IF		T(_DEFAULT_LAN_IF_)
#else
	#define ELAN_IF		T("eth2.1")
#endif
#define PPPOE_IF			T("ppp0")
#ifndef __TARGET_BOARD__
  #define _CONFIG_SCRIPT_PATH	T(".")
  #define _LITTLE_ENDIAN_
#else
  #define _CONFIG_SCRIPT_PATH	T("/bin")
#endif
#ifdef _BOA_
#define FORM_FW_UPLOAD T("formUpload")
#define FORM_LANGUAGE_UPLOAD T("formLanguageUpgrade")
#define FORM_FW_CHECK T("formcheckfirmware")
#define FORM_CFG_UPLOAD T("formSetRestorePrev")
#define FORM_CFG_SAVE T("formSaveConfigSec")
#ifdef _Edimax_
#define FORM_SAVE_TEXT T("formSaveText")
#endif
#endif
#define _CONFIG_SCRIPT_PROG	T("init.sh")
#define _WLAN_SCRIPT_PROG	T("wlan.sh")
#define _PPPOE_SCRIPT_PROG	T("pppoe.sh")
#define _FIREWALL_SCRIPT_PROG	T("firewall.sh")
#define _PPPOE_DC_SCRIPT_PROG	T("disconnect.sh")
#define _IAPPAUTH_SCRIPT_PROG	T("iappauth.sh")
#define _RADIUS_SCRIPT_PROG	T("Radiusd.sh")

#if 1
#define DEBUGP strcpy
#else
#define DEBUGP(format, args...)
#endif

static bool _is_hex(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'F')) ||
            ((c >= 'a') && (c <= 'f')));
}
static bool _isdigit(char c)
{
    return ((c >= '0') && (c <= '9'));
}

static int __inline__ string_to_hex(char_t *string, unsigned char *key, int len)
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

static int __inline__ string_to_dec(char_t *string, int *val)
{
	int idx;
	int len = strlen(string);

	for (idx=0; idx<len; idx++) {
		if ( !_isdigit(string[idx]))
			return 0;
	}

	*val = strtol(string, (char**)NULL, 10);
	return 1;
}


#define REPLACE_MSG(url)\
{\
	websHeader(wp);\
	websWrite(wp, T("<body class=\"background\" onLoad=document.location.replace(\"%s\")>"), url);\
	websFooter(wp);\
	websDone(wp, 200);\
}

#define REPLACE1_MSG(url)\
{\
	websHeader(wp);\
	websWrite(wp, T("<body class=\"background\" onLoad=parent.lFrame.location.replace(\"%s\")>"), url);\
	websFooter(wp);\
	websDone(wp, 200);\
}

#define REPLACE2_MSG(url)\
{\
	websHeader(wp);\
	websWrite(wp, T("<body class=\"background\" onLoad=parent.leftFrame.location.replace(\"%s\")>"), url);\
	websFooter(wp);\
	websDone(wp, 200);\
}

#define REJECT_MSG()\
{\
	websHeader(wp);\
	websWrite(wp, T("<body class=\"background\" onLoad='window.close();'>\n</body>\n"));\
	websFooter(wp);\
	websDone(wp, 200);\
}

#define WAIT_MSG_START(msg)\
{\
	websHeader(wp);\
	websWrite(wp, T("<head>\n<link rel=\"stylesheet\" href=\"/file/set.css\">\n")); \
	websWrite(wp, T("<meta http-equiv='Content-Type' content='text/html; charset=%s'>\n"),charset); \
	websWrite(wp, T("<script language='javascript' src='/javascript.js'></script>\n")); \
	websWrite(wp, T("<script language='javascript' src='/file/allasp-n.var'></script>\n")); \
	websWrite(wp, T("<title>Wait!</title>\n</head>\n")); \
	websWrite(wp, T("<body class=\"background\"><blockquote><h4></h4>\n")); \
	websWrite(wp, T(" <span id=\"msgActId\" style=\"display:block\">\n")); \
			websWrite(wp, T("<script>if(window.waitmsg) bu1=showText(waitmsg); else bu1='%s';document.write(bu1);</script>\n</blockquote>\n</body>"),msg); \
			websWrite(wp, T("</span>\n")); \
}
#define OK_MSG_WIZDEFAULT(msg,msg1)\
{\
    websHeader(wp);\
        websWrite(wp, T("<!--OK_MSG_WIZDEFAULT--><head>\n"));\
        websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"));\
        websWrite(wp, T("<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"));\
        websWrite(wp, T("<meta http-equiv=\"Expires\" content=\"-1\">\n"));\
        websWrite(wp, T("<link rel=\"stylesheet\" href=\"/set.css\">\n"));\
        websWrite(wp, T("<meta http-equiv=\"Content-Type\" content=\"text/html\">\n"));\
        websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/javascript.js\"></script>\n"));\
        websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/multilanguage.var\"></script>\n"));\
        websWrite(wp, T("<title></title>\n"));\
        websWrite(wp, T("</head>\n"));\
        websWrite(wp, T("<body>\n"));\
        websWrite(wp, T("<blockquote><br><br><fieldset name=\"Wizard_fieldset\" id=\"Wizard_fieldset\" ><legend><script>dw(ResettoDefaults)</script></legend><br><br><form name=\"test\"><script>document.write('<span id=\"ok2\" style=\"display:block;\"><table width=\"600\" border=\"0\" cellspacing=\"1\" cellpadding=\"3\" align=\"center\">');\n"));\
				websWrite(wp, T("document.write('<tr><td class=\"itemText\" align=\"center\">'+%s+'');\n"),msg);\
				websWrite(wp, T("document.write('</td></tr></table></span><span id=\"ok1\" style=\"display:none;\"><table width=\"600\" border=\"0\" cellspacing=\"1\" cellpadding=\"3\" align=\"center\"><tr><td class=\"itemText\" style=\"line-height:25px;\" align=\"left\">'+%s+'</td></tr></table></span>');</script><br><br>\n"),msg1);\
	websWrite(wp, T("<script>document.write('<span id=\"ok\" style=\"display:block;\"><table width=\"600\" border=\"0\" cellspacing=\"1\" cellpadding=\"3\" align=\"center\"><tr><td class=\"itemText\" align=\"center\"><input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=\"\"></td></tr></table></span>');</script>\n"));\
        websWrite(wp, T("</form></fieldset>\n"));\
        websWrite(wp, T("</blockquote>\n"));\
        websWrite(wp, T("<script>\n"));\
        websWrite(wp, T("var secs = 90;\n"));\
        websWrite(wp, T("var wait = secs * 1000;\n"));\
        websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + secs + \")\";\n"));\
        websWrite(wp, T("document.test.okbutton.disabled = true;\n"));\
        websWrite(wp, T("for(i = 1; i <= secs; i++) setTimeout(\"update(\" + i + \")\", i * 1000);\n"));\
        websWrite(wp, T("setTimeout(\"timer()\", wait);\n"));\
        websWrite(wp, T("function update(num, value)\n"));\
        websWrite(wp, T("{\n"));\
        websWrite(wp, T("if (num == (wait/1000)) {document.getElementById(\"ok\").style.display = \"none\"; document.getElementById(\"ok2\").style.display = \"none\"; document.getElementById(\"ok1\").style.display = \"block\"; }\n"));\
        websWrite(wp, T("else\n"));\
        websWrite(wp, T("{\n"));\
        websWrite(wp, T("printnr = (wait / 1000)-num;\n"));\
        websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + printnr + \")\";\n"));\
        websWrite(wp, T("}\n"));\
        websWrite(wp, T("}\n"));\
        websWrite(wp, T("function timer()\n"));\
        websWrite(wp, T("{\n"));\
        websWrite(wp, T("document.test.okbutton.disabled = false;\n"));\
        websWrite(wp, T("document.test.okbutton.value = showText(OK);\n"));\
        websWrite(wp, T("}\n"));\
        websWrite(wp, T("</script>\n"));\
        websWrite(wp, T("</body>\n"));\
        websFooter(wp);\
        websDone(wp, 200);\
}
#define WAIT_MSG(msg, url)\
{\
	websHeader(wp);\
	websWrite(wp, T("<head>																			\n"));\
	websWrite(wp, T("	<link rel='stylesheet' href='/file/set.css'>														\n"));\
	websWrite(wp, T("	<meta http-equiv='Content-Type' content='text/html; charset=%s'>										\n"),charset);\
	websWrite(wp, T("	<script type='text/javascript' src ='/javascript.js'></script>											\n"));\
	websWrite(wp, T("	<script type='text/javascript' src ='/file/allasp-n.var'></script>										\n"));\
	websWrite(wp, T("</head><!--WAIT_MSG-->																	\n"));\
	websWrite(wp, T("<body bgcolor=\"white\" LINK=\"#ffffff\" VLINK=\"#ffffff\" ALINK=\"#FFfffff\" leftmargin=\"0\" topmargin=\"5\">					\n"));\
	websWrite(wp, T("<blockquote>																		\n"));\
	websWrite(wp, T("	<h4>%s</h4>																	\n"),msg);\
	websWrite(wp, T("	<form name=\"test\">																\n"));\
	websWrite(wp, T("	<script>																	\n"));\
	websWrite(wp, T("	if( '%s' != '' )																\n"),url);\
	websWrite(wp, T("		if ( %d == 1)																\n"),lanipChanged);\
	websWrite(wp, T("		document.write('<input type=button name=\"okbutton\" value=\" OK! \" style=\"width:100px\" OnClick=top.location.replace(\"%s\")>');	\n"),url);\
	websWrite(wp, T("		else																	\n"));\
	websWrite(wp, T("		document.write('<input type=button name=\"okbutton\" value=\" OK! \" style=\"width:100px\" OnClick=window.location.replace(\"%s\")>');\n"),url);\
	websWrite(wp, T("	else																		\n"));\
	websWrite(wp, T("		document.write('<input type=button name=\"okbutton\" value=\" OK! \" style=\"width:100px\" OnClick=window.close()>');			\n"));\
	websWrite(wp, T("	</script>																	\n"));\
	websWrite(wp, T("	</form>																		\n"));\
	websWrite(wp, T("</blockquote>																		\n"));\
	websWrite(wp, T("<script>																		\n"));\
	websWrite(wp, T("	var secs = 10;																	\n"));\
	websWrite(wp, T("	var wait = secs * 1000;																\n"));\
	websWrite(wp, T("	document.test.okbutton.value = \"OK!\" + \"(\" + secs + \")\";											\n"));\
	websWrite(wp, T("	document.test.okbutton.disabled = true;														\n"));\
	websWrite(wp, T("	for(i = 1; i <= secs; i++)															\n"));\
	websWrite(wp, T("	{																		\n"));\
	websWrite(wp, T("		setTimeout(\"update(\" + i + \")\", i * 1000);												\n"));\
	websWrite(wp, T("	}																		\n"));\
	websWrite(wp, T("	setTimeout(\"timer()\", wait);															\n"));\
	websWrite(wp, T("	function update(num, value)															\n"));\
	websWrite(wp, T("	{																		\n"));\
	websWrite(wp, T("		if (num == (wait/1000))															\n"));\
	websWrite(wp, T("		{																	\n"));\
	websWrite(wp, T("			document.test.okbutton.value = \"OK!\";												\n"));\
	websWrite(wp, T("		}																	\n"));\
	websWrite(wp, T("		else																	\n"));\
	websWrite(wp, T("		{																	\n"));\
	websWrite(wp, T("			printnr = (wait / 1000)-num;													\n"));\
	websWrite(wp, T("			document.test.okbutton.value = \"OK!\" + \"(\" + printnr + \")\";								\n"));\
	websWrite(wp, T("		}																	\n"));\
	websWrite(wp, T("	}																		\n"));\
	websWrite(wp, T("	function timer()																\n"));\
	websWrite(wp, T("	{																		\n"));\
	websWrite(wp, T("		document.test.okbutton.disabled = false;												\n"));\
	websWrite(wp, T("		document.test.okbutton.value = \"OK!\";													\n"));\
	websWrite(wp, T("	}																		\n"));\
	websWrite(wp, T("</script>																		\n"));\
	websWrite(wp, T("</body>																		\n"));\
	websFooter(wp); \
	websDone(wp, 200); \
}

#define WAIT_MSG_START_END(msg)\
{\
	websWrite(wp, T("<script>document.getElementById('msgActId').style.display=\"none\";</script>\n"));\
	websWrite(wp, T("%s"),msg);\
}

#define OK_MSG3_NOHEAD(msg, url)\
{\
	websWrite(wp, T("<h4>%s</h4>																	\n"),msg);\
	websWrite(wp, T("<form name=\"test\"><!--OK_MSG3_NOHEAD-->													\n"));\
	websWrite(wp, T("<script>																	\n"));\
	websWrite(wp, T("if( '%s' != '' )																\n"),url);\
	websWrite(wp, T("	if ( %d == 1 )																\n"),lanipChanged);\
	websWrite(wp, T("	document.write('<input type=button name=\"okbutton\" value=\"OK!\" style=\"width:100px\" OnClick=top.location.replace(\"%s\")>');	\n"),url);\
	websWrite(wp, T("	else																	\n"));\
	websWrite(wp, T("	document.write('<input type=button name=\"okbutton\" value=\"OK!\" style=\"width:100px\" OnClick=window.location.replace(\"%s\")>');	\n"),url);\
	websWrite(wp, T("else																		\n"));\
	websWrite(wp, T("	document.write('<input type=button name=\"okbutton\" value=\"OK!\" style=\"width:100px\" OnClick=window.close()>');			\n"));\
	websWrite(wp, T("</script>																	\n"));\
	websWrite(wp, T("</form>																	\n"));\
	websFooter(wp);\
	websDone(wp, 200);\
}

#define REDIRECT_NOHEAD(url)\
{\
	websWrite(wp, T("<script>document.location.replace(\"%s\")</script>"),url);\
	websFooter(wp);\
	websDone(wp, 200);\
}

#define UPGRADE_PROCESS_MSG(){\
	websHeader(wp);\
	websWrite(wp, T("<head>\n"));\
	websWrite(wp, T("	<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\n"));\
	websWrite(wp, T("	<meta http-equiv='Pragma' content='no-cache'>\n"));\
	websWrite(wp, T("	<meta http-equiv='Expires' content='-1'>\n"));\
	websWrite(wp, T("	<script type='text/javascript' src='/file/multilanguage.var'></script>\n"));\
	websWrite(wp, T("	<script type='text/javascript' src='/file/javascript.js'></script>\n"));\
	websWrite(wp, T("	<link rel='stylesheet' href='/set.css'>\n"));\
	websWrite(wp, T("	<script type='text/javascript' src='/file/jquery-1.7.1.min.js'></script>\n"));\
	websWrite(wp, T("	<title>Upgrading firmware</title>\n"));\
	websWrite(wp, T("	<style type=\"text/css\">\n"));\
	websWrite(wp, T("		#out{width:300px;height:20px;background:#CCCCCC;}\n"));\
	websWrite(wp, T("		#in{width:1px; height:20px;background:#ff7f50;color:#000000;text-align:center; FONT-SIZE: 11pt; FONT-FAMILY: Arial;}\n"));\
	websWrite(wp, T("		.background{	background-color:#EEEEEE;	color:#000000;	font-family:Arial, Helvetica; }\n"));\
	websWrite(wp, T("	</style>\n"));\
	websWrite(wp, T("	<script>\n"));\
	websWrite(wp, T("	var check_count = 1; //5000(5s) * 40(check_count)=200000(200s)\n"));\
	websWrite(wp, T("	function loading(){\n"));\
	websWrite(wp, T("	document.getElementById('progress').innerHTML=parseInt(($('#in').width()/300) * 100, 10)+'%%';\n"));\
	websWrite(wp, T("	}\n"));\
	websWrite(wp, T("	var setloading=setInterval('loading()', 1000);\n"));\
	websWrite(wp, T("	</script>\n"));\
	websWrite(wp, T("</head>\n"));\
	websWrite(wp, T("<body>\n"));\
	websWrite(wp, T("<form id=\"checkUpgrade\">\n"));\
	websWrite(wp, T("	<input type=\"hidden\" name=\"action\" value=\"checkFWupgrade\">\n"));\
	websWrite(wp, T("</form>\n"));\
	websWrite(wp, T("<span id='counting'>\n"));\
	websWrite(wp, T("	<blockquote>\n"));\
	websWrite(wp, T("		<br>\n"));\
	websWrite(wp, T("			<table width='700' border=0 align='left'>\n"));\
	websWrite(wp, T("				<tr>\n"));\
	websWrite(wp, T("					<td colspan='2'><span class='restartInfo'><script>dw(Firmwareupgrading);</script></span>\n"));\
	websWrite(wp, T("					</td>\n"));\
	websWrite(wp, T("				</tr>\n"));\
	websWrite(wp, T("		<br>\n"));\
	websWrite(wp, T("				<tr>\n"));\
	websWrite(wp, T("					<td style='width:305px;'>\n"));\
	websWrite(wp, T("						<div id='out'>\n"));\
	websWrite(wp, T("							<div id='in' style='width:1%%;'></div>\n"));\
	websWrite(wp, T("						</div>\n"));\
	websWrite(wp, T("					</td>\n"));\
	websWrite(wp, T("					<td id='progress' style='width:395px;'>1%%</td>\n"));\
	websWrite(wp, T("				</tr>\n"));\
	websWrite(wp, T("			</table>\n"));\
	websWrite(wp, T("	</blockquote>\n"));\
	websWrite(wp, T("</span>\n"));\
	websWrite(wp, T("<span id='ok' style='display:none'>\n"));\
	websWrite(wp, T("	<blockquote><br>\n"));\
	websWrite(wp, T("			<table width='700' border=0 align='left'><tr><td>\n"));\
	websWrite(wp, T("						<span class='restartInfo'><script>dw(upgrade_accomplished)</script></span>\n"));\
	websWrite(wp, T("					</td></tr></table>\n"));\
	websWrite(wp, T("			<br><br>\n"));\
	websWrite(wp, T("			<table width='700' border=0 align='left'><tr><td>\n"));\
	websWrite(wp, T("						<span class='restartitemText' style='line-height:22px;'><script>dw(upgrade_accomplished_content)</script></span>\n"));\
	websWrite(wp, T("					</td></tr></table>\n"));\
	websWrite(wp, T("	</blockquote>\n"));\
	websWrite(wp, T("</span>\n"));\
	websWrite(wp, T("	<script type=\"text/javascript\">\n"));\
	websWrite(wp, T("		$(document).ready(function() {\n"));\
	websWrite(wp, T("			$(\"#in\").animate({width: '99%%'}, 200000,function(){\n"));\
	websWrite(wp, T("				$.post(\"/goform/formJQueryData\", $(\"#checkUpgrade\").serialize(),function(data){\n"));\
	websWrite(wp, T("					window.clearInterval(check_upgrade_statStop);\n"));\
	websWrite(wp, T("					window.clearInterval(setloading);\n"));\
	websWrite(wp, T("					document.getElementById(\"progress\").innerHTML=\"100%%\";\n"));\
	websWrite(wp, T("					$(\"#in\").stop().animate({width: '100%%'},function(){\n"));\
	websWrite(wp, T("						document.getElementById('counting').style.display='none';\n"));\
	websWrite(wp, T("						document.getElementById('ok').style.display='block';\n"));\
	websWrite(wp, T("						parent.lFrame.location.reload();\n"));\
	websWrite(wp, T("					});\n"));\
	websWrite(wp, T("				});\n"));\
	websWrite(wp, T("			});\n"));\
	websWrite(wp, T("		});\n"));\
	websWrite(wp, T("		check_upgrade_statStop = setInterval(\"check_upgrade_stat()\", 5000);\n"));\
	websWrite(wp, T("		function check_upgrade_stat(){\n"));\
	websWrite(wp, T("			if(check_count>=40)\n"));\
	websWrite(wp, T("			{\n"));\
	websWrite(wp, T("				window.clearInterval(check_upgrade_statStop);\n"));\
	websWrite(wp, T("				$(\"#in\").stop().animate({width: '99%%'},function(){\n"));\
	websWrite(wp, T("					$(\"#in\").animate({width: '100%%'}, 2000,function(){\n"));\
	websWrite(wp, T("						window.clearInterval(setloading);\n"));\
	websWrite(wp, T("						document.getElementById(\"progress\").innerHTML=\"100%%\";\n"));\
	websWrite(wp, T("						$(\"#in\").stop().animate({width: '100%%'},function(){\n"));\
	websWrite(wp, T("							document.getElementById('counting').style.display='none';\n"));\
	websWrite(wp, T("							document.getElementById('ok').style.display='block';\n"));\
	websWrite(wp, T("							parent.lFrame.location.reload();\n"));\
	websWrite(wp, T("						});\n"));\
	websWrite(wp, T("					});\n"));\
	websWrite(wp, T("				});\n"));\
	websWrite(wp, T("			}\n"));\
	websWrite(wp, T("			else\n"));\
	websWrite(wp, T("			{\n"));\
	websWrite(wp, T("				check_count=check_count+1;\n"));\
	websWrite(wp, T("				$.post(\"/goform/formJQueryData\", $(\"#checkUpgrade\").serialize(),function(data){\n"));\
	websWrite(wp, T("				if(data==\"upgrade_ok\"){\n"));\
	websWrite(wp, T("					window.clearInterval(check_upgrade_statStop);\n"));\
	websWrite(wp, T("					$(\"#in\").stop().animate({width: '99%%'},function(){\n"));\
	websWrite(wp, T("						$(\"#in\").animate({width: '100%%'}, 2000,function(){\n"));\
	websWrite(wp, T("							window.clearInterval(setloading);\n"));\
	websWrite(wp, T("							document.getElementById(\"progress\").innerHTML=\"100%%\";\n"));\
	websWrite(wp, T("							$(\"#in\").stop().animate({width: '100%%'},function(){\n"));\
	websWrite(wp, T("								document.getElementById('counting').style.display='none';\n"));\
	websWrite(wp, T("								document.getElementById('ok').style.display='block';\n"));\
	websWrite(wp, T("								parent.lFrame.location.reload();\n"));\
	websWrite(wp, T("							});\n"));\
	websWrite(wp, T("						});\n"));\
	websWrite(wp, T("					});\n"));\
	websWrite(wp, T("				}\n"));\
	websWrite(wp, T("				});\n"));\
	websWrite(wp, T("			}\n"));\
	websWrite(wp, T("		}\n"));\
	websWrite(wp, T("	</script>\n"));\
	websWrite(wp, T("</body>\n"));\
	websFooter(wp);\
	websDone(wp, 200);\
}

#define LANGUAGE_PROCESS_MSG(selected){\
        websHeader(wp);\
        websWrite(wp, T("<head>\n"));\
        websWrite(wp, T("       <meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\n"));\
        websWrite(wp, T("       <title>Success!</title>\n"));\
        websWrite(wp, T("       <style type=\"text/css\">\n"));\
        websWrite(wp, T("               #out{width:300px;height:20px;background:#CCCCCC;}\n"));\
        websWrite(wp, T("               #in{width:0px; height:20px;background:#0080FF;color:#000000;text-align:center;}\n"));\
        websWrite(wp, T("               .background{    background-color:#EEEEEE;       color:#000000;  font-family:Arial, Helvetica; }\n"));\
        websWrite(wp, T("       </style>\n"));\
        websWrite(wp, T("       <script>\n"));\
        websWrite(wp, T("               function getCookie(c_name)\n"));\
        websWrite(wp, T("               {\n"));\
        websWrite(wp, T("                       if (document.cookie.length>0)\n"));\
        websWrite(wp, T("                       {\n"));\
        websWrite(wp, T("                               c_start=document.cookie.indexOf(c_name + \"=\")\n"));\
        websWrite(wp, T("                               if (c_start!=-1)\n"));\
        websWrite(wp, T("                               {\n"));\
        websWrite(wp, T("                                       c_start=c_start + c_name.length+1;\n"));\
        websWrite(wp, T("                                       c_end=document.cookie.indexOf(\";\",c_start);\n"));\
        websWrite(wp, T("                                       if (c_end==-1) c_end=document.cookie.length;\n"));\
	websWrite(wp, T("                                       return unescape(document.cookie.substring(c_start,c_end));\n"));\
        websWrite(wp, T("                               }\n"));\
        websWrite(wp, T("                       }\n"));\
        websWrite(wp, T("                       else\n"));\
        websWrite(wp, T("                       {\n"));\
        websWrite(wp, T("                               return 0;\n"));\
        websWrite(wp, T("                       }\n"));\
        websWrite(wp, T("               }\n"));\
        websWrite(wp, T("               var stype = getCookie('language');\n"));\
        websWrite(wp, T("               function showText(text)\n"));\
        websWrite(wp, T("               {\n"));\
        websWrite(wp, T("                       if(text[stype]==null || text[stype]==\"\") return text[0];\n"));\
        websWrite(wp, T("                       else                            return text[stype];\n"));\
        websWrite(wp, T("               }\n"));\
        websWrite(wp, T("               function dw(message,defaultMs)\n"));\
        websWrite(wp, T("               {\n"));\
        websWrite(wp, T("                       document.write(showText(message));\n"));\
        websWrite(wp, T("               }\n"));\
        websWrite(wp, T("               var firmware_upgrading=new Array\n"));\
        websWrite(wp, T("               (\n"));\
        websWrite(wp, T("               \"Rebooting, please wait.\"\n"));\
        websWrite(wp, T("               );\n"));\
        websWrite(wp, T("               var upgrade_accomplished=new Array\n"));\
	websWrite(wp, T("               (\n"));\
        websWrite(wp, T("               \"Upgrade Accomplished.\"\n"));\
        websWrite(wp, T("               );\n"));\
        websWrite(wp, T("               var upgrade_accomplished_content=new Array\n"));\
        websWrite(wp, T("               (\n"));\
        websWrite(wp, T("               \"You should be able to reconnect to the router by refreshing the web page now. If not, please restart the router by reconnecting the power line manually.\"\n"));\
        websWrite(wp, T("               );\n"));\
        websWrite(wp, T("       </script>\n"));\
        websWrite(wp, T("</head>\n"));\
        websWrite(wp, T("<body class=\"background\" onload=\"start();\">\n"));\
        websWrite(wp, T("       <div id='counting'>\n"));\
        websWrite(wp, T("               <blockquote><h4><script>dw(firmware_upgrading)</script></h4>\n"));\
        websWrite(wp, T("       <div id='out'>\n"));\
        websWrite(wp, T("               <div id=\"in\" style=\"width:0%%\">0%%</div>\n"));\
        websWrite(wp, T("       </div></blockquote>\n"));\
        websWrite(wp, T("       </div>\n"));\
        websWrite(wp, T("       <script type=\"text/javascript\">\n"));\
        websWrite(wp, T("               i=0;secs=50;\n"));\
        websWrite(wp, T("               function start()\n"));\
        websWrite(wp, T("               {\n"));\
        websWrite(wp, T("                       ba=setInterval(\"begin()\",secs*10);\n"));\
        websWrite(wp, T("               }\n"));\
        websWrite(wp, T("               function begin()\n"));\
        websWrite(wp, T("               {\n"));\
	websWrite(wp, T("                       i+=1;\n"));\
        websWrite(wp, T("                       if(i<=100)\n"));\
        websWrite(wp, T("                       {\n"));\
        websWrite(wp, T("                               document.getElementById(\"in\").style.width=i+\"%%\";\n"));\
        websWrite(wp, T("                               document.getElementById(\"in\").innerHTML=i+\"%%\";\n"));\
        websWrite(wp, T("                       }\n"));\
        websWrite(wp, T("                       else\n"));\
        websWrite(wp, T("                       {\n"));\
        websWrite(wp, T("                               clearInterval(ba);\n"));\
        websWrite(wp, T("                               document.getElementById(\"counting\").style.display=\"none\";\n"));\
        websWrite(wp, T("                               document.write(\"<head>\");\n"));\
        websWrite(wp, T("                               document.write(\"       <meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\");\n"));\
        websWrite(wp, T("                               document.write(\"       <title>Success!</title>\");\n"));\
        websWrite(wp, T("                               document.write(\"       <style type=\\\"text/css\\\">\");\n"));\
        websWrite(wp, T("                               document.write(\"               .background{ background-color:#EEEEEE; color:#000000; font-family:Arial, Helvetica;}\");\n"));\
        websWrite(wp, T("                               document.write(\"       </style>\");\n"));\
        websWrite(wp, T("                               document.write(\"</head>\");\n"));\
        websWrite(wp, T("                               document.write(\"<body class=\\\"background\\\">\");\n"));\
        websWrite(wp, T("                               document.write(\"       <blockquote>\");\n"));\
        websWrite(wp, T("                               document.write(\"               <h4>\"+showText(upgrade_accomplished)+\"</h4>\");\n"));\
	websWrite(wp, T("                               document.write(\"               <br>\");\n"));\
        websWrite(wp, T("                               document.write(\"               <font size=\\\"2\\\" class=\\\"textcolor\\\">\"+showText(upgrade_accomplished_content)+\"</font>\");\n"));\
        websWrite(wp, T("                               document.write(\"       </blockquote>\");\n"));\
        websWrite(wp, T("                               document.write(\"</body>\");\n"));\
        websWrite(wp, T("                               var vervaldatum = new Date()\n"));\
        websWrite(wp, T("                               vervaldatum.setDate(vervaldatum.getDate()+365);\n"));\
        websWrite(wp, T("                               parent.document.cookie=\"language=%d; expires=\"+vervaldatum.toGMTString()+\"; path=/\";\n"), selected);\
        websWrite(wp, T("                       }\n"));\
        websWrite(wp, T("               }\n"));\
        websWrite(wp, T("       </script>\n"));\
        websWrite(wp, T("</body>\n"));\
        websFooter(wp);\
        websDone(wp, 200);\
}



#define OK_MSG1(msg, url)\
{\
	websHeader(wp);\
	websWrite(wp, T("<head>              \n"));\
	websWrite(wp, T("	<meta http-equiv='Content-Type' content='text/html; charset=%s'>							\n"),charset);\
	websWrite(wp, T("	<script type='text/javascript' src='/file/javascript.js'></script>								\n"));\
	websWrite(wp, T("	<script type='text/javascript' src='/file/allasp-n.var'></script>							\n"));\
	websWrite(wp, T("<script type='text/javascript' src='/FUNCTION_SCRIPT'></script>\n"));\
	websWrite(wp, T("  <script>                                                                                                                                                                                             \n"));\
	websWrite(wp, T("	if( typeof _MODE_	== 'undefined') _MODE_ =	'General';												\n"));\
	websWrite(wp, T("	if( typeof _IS_GATEWAY_	== 'undefined') _IS_GATEWAY_ =	'y';												\n"));\
	websWrite(wp, T("  if( _MODE_ == 'Sitecom330' || _MODE_ == 'Sitecom331' ) document.write('<link href=\"file/set.css\" type=\"text/css\" rel=\"stylesheet\">');                                                              \n"));\
	websWrite(wp, T("  else                                                    document.write('<link href=\"/file/set.css\" type=\"text/css\" rel=\"stylesheet\">');                                                             \n"));\
	websWrite(wp, T("  </script>                                                                                                                                                                                            \n"));\
	websWrite(wp, T("<title>Success!</title>\n"));\
	websWrite(wp, T("</head><!--OK_MSG1-->														\n"));\
	websWrite(wp, T("<script>															\n"));\
	websWrite(wp, T("	if ( _MODE_ == 'Sitecom330' || _MODE_ == 'Sitecom331' )								\n"));\
	websWrite(wp, T("		document.write('<body style=\"background-image:none;background-color:transparent;\">');											\n"));\
	websWrite(wp, T("	else															\n"));\
	websWrite(wp, T("		document.write('<body class=\"mainbg\">');									\n"));\
	websWrite(wp, T("</script>															\n"));\
	websWrite(wp, T("<blockquote>															\n"));\
	websWrite(wp, T("<h4>%s</h4>															\n"),msg);\
	websWrite(wp, T("	<form>															\n"));\
	websWrite(wp, T("	<script>														\n"));\
	websWrite(wp, T("	if(window.ok1)	cont=showText(ok1);											\n"));\
	websWrite(wp, T("	else		cont='OK';												\n"));\
	websWrite(wp, T("	if( '%s' != '' )													\n"),url);\
	websWrite(wp, T("	{															\n"));\
	websWrite(wp, T("		buffer='<input type=button value=\"'+cont+'\" style=\"width:100px\" onClick=window.location.replace(\"%s\")>';	\n"),url);\
	websWrite(wp, T("	}															\n"));\
	websWrite(wp, T("	else															\n"));\
	websWrite(wp, T("	{															\n"));\
	websWrite(wp, T("		buffer='<input type=button value=\"'+cont+'\" style=\"width:100px\" onClick=window.close()>';			\n"));\
	websWrite(wp, T("	}															\n"));\
	websWrite(wp, T("	document.write(buffer);													\n"));\
	websWrite(wp, T("	</script>														\n"));\
	websWrite(wp, T("	</form>															\n"));\
	websWrite(wp, T("</blockquote>															\n"));\
	websWrite(wp, T("</body>															\n"));\
	websFooter(wp);\
	websDone(wp, 200);\
}


#define UPGRADE_MSG(msg, url) { \
	websHeader(wp); \
	websWrite(wp, T("<head><link rel='stylesheet' href='/file/set.css'>\n")); \
   	websWrite(wp, T("<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\n")); \
   	websWrite(wp, T("<script type='text/javascript' src='/javascript.js'></script>\n")); \
   	websWrite(wp, T("<script type='text/javascript' src='/file/allasp-n.var'></script>\n")); \
   	websWrite(wp, T("<title>Success!</title>\n</head>\n")); \
 	websWrite(wp, T("<body class=\"background\"><blockquote><h4>\n%s\n</h4>\n"),msg); \
	if (url) websWrite(wp, T("<form><input type=button name =\"okbutton\" value=\"OK!\" style=\"width:100px\" OnClick=window.location.replace(\"%s\")>\n</form></blockquote>\n</body>"), url); \
	else websWrite(wp, T("<form><input type=button name =\"okbutton\" value=\"OK!\" style=\"width:100px\" OnClick=window.close()>\n</form></blockquote>\n</body>")); \
	websFooter(wp); \
	websDone(wp, 200); \
}

#ifdef _WIFI_ROMAING_

#define OK_MSG3(msg, url)\
	{\
		websHeader(wp);\
		websWrite(wp, T("<!--OK_MSG3--><head>\n"));\
		websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"));\
		websWrite(wp, T("<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"));\
		websWrite(wp, T("<meta http-equiv=\"Expires\" content=\"-1\">\n"));\
		websWrite(wp, T("<link rel=\"stylesheet\" href=\"/set.css\">\n"));\
		websWrite(wp, T("<meta http-equiv=\"Content-Type\" content=\"text/html\">\n"));\
		websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/javascript.js\"></script>\n"));\
		websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/multilanguage.var\"></script>\n"));\
		websWrite(wp, T("<title></title>\n"));\
		websWrite(wp, T("</head>\n"));\
		websWrite(wp, T("<body>\n"));\
		websWrite(wp, T("<blockquote><br><br><div class=\"restartInfo\">%s</div><br><br>\n"),msg);\
		websWrite(wp, T("<form name=\"test\">\n"));\
		if(url)\
		{\
			if(lanipChanged==1)	websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=top.location.replace(\"%s\")>');</script>\n"),url);\
			else websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=window.location.replace(\"%s\")>');</script>\n"),url);\
		}\
		else websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=window.close()>');</script>\n"));\
		websWrite(wp, T("</form>\n"));\
		websWrite(wp, T("</blockquote>\n"));\
		websWrite(wp, T("<script>\n"));\
		websWrite(wp, T("var secs = 120;\n"));\
		websWrite(wp, T("var wait = secs * 1000;\n"));\
		websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + secs + \")\";\n"));\
		websWrite(wp, T("document.test.okbutton.disabled = true;\n"));\
		websWrite(wp, T("for(i = 1; i <= secs; i++)	setTimeout(\"update(\" + i + \")\", i * 1000);\n"));\
		websWrite(wp, T("setTimeout(\"timer()\", wait);\n"));\
		websWrite(wp, T("function update(num, value)\n"));\
		websWrite(wp, T("{\n"));\
		websWrite(wp, T("if (num == (wait/1000)) document.test.okbutton.value = showText(OK);\n"));\
		websWrite(wp, T("else\n"));\
		websWrite(wp, T("{\n"));\
		websWrite(wp, T("printnr = (wait / 1000)-num;\n"));\
		websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + printnr + \")\";\n"));\
		websWrite(wp, T("}\n"));\
		websWrite(wp, T("}\n"));\
		websWrite(wp, T("function timer()\n"));\
		websWrite(wp, T("{\n"));\
		websWrite(wp, T("document.test.okbutton.disabled = false;\n"));\
		websWrite(wp, T("document.test.okbutton.value = showText(OK);\n"));\
		websWrite(wp, T("}\n"));\
		websWrite(wp, T("</script>\n"));\
		websWrite(wp, T("</body>\n"));\
		websFooter(wp);\
		websDone(wp, 200);\
	}

#else

#define OK_MSG3(msg, url)\
	{\
		websHeader(wp);\
		websWrite(wp, T("<!--OK_MSG3--><head>\n"));\
		websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"));\
		websWrite(wp, T("<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"));\
		websWrite(wp, T("<meta http-equiv=\"Expires\" content=\"-1\">\n"));\
		websWrite(wp, T("<link rel=\"stylesheet\" href=\"/set.css\">\n"));\
		websWrite(wp, T("<meta http-equiv=\"Content-Type\" content=\"text/html\">\n"));\
		websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/javascript.js\"></script>\n"));\
		websWrite(wp, T("<script type=\"text/javascript\" src=\"/file/multilanguage.var\"></script>\n"));\
		websWrite(wp, T("<title></title>\n"));\
		websWrite(wp, T("</head>\n"));\
		websWrite(wp, T("<body>\n"));\
		websWrite(wp, T("<blockquote><br><br><div class=\"restartInfo\">%s</div><br><br>\n"),msg);\
		websWrite(wp, T("<form name=\"test\">\n"));\
		if(url)\
		{\
			if(lanipChanged==1)	websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=top.location.replace(\"%s\")>');</script>\n"),url);\
			else websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=window.location.replace(\"%s\")>');</script>\n"),url);\
		}\
		else websWrite(wp, T("<script>document.write('<input type=button name=\"okbutton\" value=\"'+showText(OK)+'\" class=\"ui-button\" OnClick=window.close()>');</script>\n"));\
		websWrite(wp, T("</form>\n"));\
		websWrite(wp, T("</blockquote>\n"));\
		websWrite(wp, T("<script>\n"));\
		websWrite(wp, T("var secs = 90;\n"));\
		websWrite(wp, T("var wait = secs * 1000;\n"));\
		websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + secs + \")\";\n"));\
		websWrite(wp, T("document.test.okbutton.disabled = true;\n"));\
		websWrite(wp, T("for(i = 1; i <= secs; i++)	setTimeout(\"update(\" + i + \")\", i * 1000);\n"));\
		websWrite(wp, T("setTimeout(\"timer()\", wait);\n"));\
		websWrite(wp, T("function update(num, value)\n"));\
		websWrite(wp, T("{\n"));\
		websWrite(wp, T("if (num == (wait/1000)) document.test.okbutton.value = showText(OK);\n"));\
		websWrite(wp, T("else\n"));\
		websWrite(wp, T("{\n"));\
		websWrite(wp, T("printnr = (wait / 1000)-num;\n"));\
		websWrite(wp, T("document.test.okbutton.value = showText(OK) + \"(\" + printnr + \")\";\n"));\
		websWrite(wp, T("}\n"));\
		websWrite(wp, T("}\n"));\
		websWrite(wp, T("function timer()\n"));\
		websWrite(wp, T("{\n"));\
		websWrite(wp, T("document.test.okbutton.disabled = false;\n"));\
		websWrite(wp, T("document.test.okbutton.value = showText(OK);\n"));\
		websWrite(wp, T("}\n"));\
		websWrite(wp, T("</script>\n"));\
		websWrite(wp, T("</body>\n"));\
		websFooter(wp);\
		websDone(wp, 200);\
	}

#endif
extern void formPasswordSetup(webs_t wp, char_t *path, char_t *query);
extern void formUploadIE(webs_t wp, char_t * path, char_t * query);
extern void formUpload(webs_t wp, char_t * path, char_t * query);
extern void formBootUpload(webs_t wp, char_t * path, char_t * query);
extern void formUploadBootCode(webs_t wp, char_t * path, char_t * query);
extern void formSaveConfig(webs_t wp, char_t *path, char_t *query);
extern void formSaveConfigSec(webs_t wp, char_t *path, char_t *query);
extern void formTestResult(webs_t wp, char_t *path, char_t *query);
extern void formResetDefault(webs_t wp, char_t *path, char_t *query);
extern void formSecLog(webs_t wp, char_t *path, char_t *query);
extern void formSysLog(webs_t wp, char_t *path, char_t *query);
extern void formDebug(webs_t wp, char_t *path, char_t *query);
extern void set_user_profile();
extern void reset_user_profile();
extern void formStats(webs_t wp, char_t *path, char_t *query);
extern void formWdsEncrypt(webs_t wp, char_t *path, char_t *query);
extern int wdsList(int eid, webs_t wp, int argc, char_t **argv);
extern void formJQueryData(webs_t wp, char_t *path, char_t *query);
#ifdef _IQv2_
extern void formwizResetDefault(webs_t wp, char_t *path, char_t *query);
#endif
extern char _WAN_IF_[10];
extern char _LAN_IF_[10];
extern int main_menu;
extern int sub_menu;
extern int third_menu;
extern int upgrade_status;
extern char charset[20];
extern char *submitUrlp;
extern int lanipChanged;
extern int sitesurveyed;
extern int sitesurveyed5G;
extern int stadrvenable;
extern void formWifiEnable(webs_t wp, char_t *path, char_t *query);
extern void formWpsEnable(webs_t wp, char_t *path, char_t *query);
extern void formWpsConfig(webs_t wp, char_t *path, char_t *query);
extern void formWpsRest(webs_t wp, char_t *path, char_t *query);
extern void formWpsStart(webs_t wp, char_t *path, char_t *query);
extern int wizardEnabled;
#ifdef _IQv2_
extern void wiz_5in1_redirect(webs_t wp, char_t * path, char_t * query);
extern void chkLink(webs_t wp, char_t * path, char_t * query);
extern void setWAN(webs_t wp, char_t * path, char_t * query);
extern void setWifi(webs_t wp, char_t * path, char_t * query);
extern void saveAndReboot(webs_t wp, char_t * path, char_t * query);
#endif
extern void formWlbasic(webs_t wp, char_t * path, char_t * query);
extern void formWlEnableSwitch(webs_t wp, char_t * path, char_t * query);
extern void formiNICbasic(webs_t wp, char_t * path, char_t * query);
extern void formiNICEnableSwitch(webs_t wp, char_t * path, char_t * query);
#ifdef _WIFI_ROMAING_
extern void formWlbasicREP(webs_t wp, char_t * path, char_t * query);
extern void formiNICbasicREP(webs_t wp, char_t * path, char_t * query);
#endif
#ifdef _DNS_PROXY_
#ifdef _EDIT_DNSPROXYURL_
extern void formDNSProxyrules(webs_t wp, char_t *path, char_t *query);
extern int DNSPROXYURLList(int eid, webs_t wp, int argc, char_t **argv);
extern int TemporaryDNSPROXYURLList(int eid, webs_t wp, int argc, char_t **argv);
#endif
#endif
#if defined(HOME_GATEWAY) || defined(_AP_WITH_TIMEZONE_)
extern void formTimeZoneSetup(webs_t wp, char_t *path, char_t *query);
#endif
#ifdef HOME_GATEWAY
extern void formWizSetup(webs_t wp, char_t *path, char_t *query);
extern void formReManagementSetup(webs_t wp, char_t *path, char_t *query);
extern void formOpMode(webs_t wp, char_t *path, char_t *query);
extern void formStcIpSetup(webs_t wp, char_t *path, char_t *query);
extern void formDynIpSetup(webs_t wp, char_t *path, char_t *query);
extern void formPPPoESetup(webs_t wp, char_t *path, char_t *query);
extern void formPPTPSetup(webs_t wp, char_t *path, char_t *query);
extern void formL2TPSetup(webs_t wp, char_t *path, char_t *query);
extern void formTELBPSetup(webs_t wp, char_t *path, char_t *query);
extern void formDDNSSetup(webs_t wp, char_t *path, char_t *query);
extern void formWlEnable(webs_t wp, char_t *path, char_t *query);
extern void formiNICEnable(webs_t wp, char_t *path, char_t *query);
extern void formiNICSetup(webs_t wp, char_t *path, char_t *query);
extern void formiNICAdvanceSetup(webs_t wp, char_t *path, char_t *query);
extern void formiNICEncrypt(webs_t wp, char_t *path, char_t *query);
extern void formiNICAc(webs_t wp, char_t *path, char_t *query);
extern int wliNICSiteSurveyTbl(int eid, webs_t wp, int argc, char_t **argv);
extern int wliNICAcList(int eid, webs_t wp, int argc, char_t **argv);
extern void formiNICSiteSurvey(webs_t wp, char_t *path, char_t *query);
extern void formiNICWirelessTbl(webs_t wp, char_t *path, char_t *query);
extern void formiNICWpsEnable(webs_t wp, char_t *path, char_t *query);
extern void formiNICWpsStart(webs_t wp, char_t *path, char_t *query);
extern void formiNICWdsEncrypt(webs_t wp, char_t *path, char_t *query);
extern void formMeshSetup(webs_t wp, char_t *path, char_t *query);
extern void formiNICMeshSetup(webs_t wp, char_t *path, char_t *query);
#ifdef _AUTO_FW_UPGRADE_
extern void formAutoFWupgradeIE(webs_t wp, char_t *path, char_t *query);
extern void formAutoFWupgrade(webs_t wp, char_t *path, char_t *query);
#endif
extern void formVirtualSv(webs_t wp, char_t *path, char_t *query);
extern int virtualSvList(int eid, webs_t wp, int argc, char_t **argv);
extern void formUPNPSetup(webs_t wp, char_t *path, char_t *query);
extern void formALGSetup(webs_t wp, char_t *path, char_t *query);
extern void formHWNATSetup(webs_t wp, char_t *path, char_t *query);
extern void formAPModeSwitch(webs_t wp, char_t *path, char_t *query);
extern void formNatEnable(webs_t wp, char_t *path, char_t *query);
#if defined(_IGMP_PROXY_)
extern void formIgmpEnable(webs_t wp, char_t *path, char_t *query);
#endif
extern void formFwEnable(webs_t wp, char_t *path, char_t *query);
extern int ACPCList(int eid, webs_t wp, int argc, char_t **argv);
extern void formPreventionSetup(webs_t wp, char_t *path, char_t *query);
extern int DMZList(int eid, webs_t wp, int argc, char_t **argv);
extern int StcRoutList(int eid, webs_t wp, int argc, char_t **argv);
extern int URLBList(int eid, webs_t wp, int argc, char_t **argv);
extern void formPSSetup(webs_t wp, char_t *path, char_t *query);
extern void formQoS(webs_t wp, char_t *path, char_t *query);
extern int Wan1QosList(int eid, webs_t wp, int argc, char_t **argv);
extern int QosShow(int eid, webs_t wp, int argc, char_t **argv);
extern void initQos();
extern int newpage;
extern int First, Changed;
extern void formConnect();
#endif
extern void formReboot();
extern void formApply();
extern void formAccept(webs_t wp, char_t *path, char_t *query);
extern void formLicence(webs_t wp, char_t *path, char_t *query);
extern int showSomeString(int eid, webs_t wp, int argc, char_t **argv);
extern int showWebsPasswd(int eid, webs_t wp, int argc, char_t **argv);
extern void OK_MSG(webs_t wp, char *url);
extern void ERR_MSG(webs_t wp, char *msg);
#ifdef _EZ_QOS_
extern void formEZQoS(webs_t wp, char_t *path, char_t *query);
extern void formEZQoSMode(webs_t wp, char_t *path, char_t *query);
extern void formBandwidth_Calc(webs_t wp, char_t *path, char_t *query);
#endif
extern int EZQosList(int eid, webs_t wp, int argc, char_t **argv);
#ifndef _BOA_
extern int getIndex(int eid, webs_t wp, int argc, char_t **argv);
#else
extern int getIndex(char *, char*);
extern int getIndexStr(int eid, webs_t wp, int argc, char_t **argv);
extern int getVar(int eid, webs_t wp, int argc, char_t **argv);
#endif
extern int getInfo(int eid, webs_t wp, int argc, char_t **argv);
extern int getMyName(int eid, webs_t wp, int argc, char_t **argv);
#ifndef _BOA_
extern int getiNICIndex(int eid, webs_t wp, int argc, char_t **argv);
#else
extern int getiNICIndex(char *, char*);
extern int getiNICIndexStr(int eid, webs_t wp, int argc, char_t **argv);
extern int getiNICVar(int eid, webs_t wp, int argc, char_t **argv);
#endif
extern int getiNICInfo(int eid, webs_t wp, int argc, char_t **argv);
#ifdef HOME_GATEWAY
extern int isConnectPPP();
#endif
extern void formWlanSetup(webs_t wp, char_t *path, char_t *query);
extern int wlAcList(int eid, webs_t wp, int argc, char_t **argv);
#ifdef _WIFI_ROMAING_
extern int SlaveList(int eid, webs_t wp, int argc, char_t **argv);
extern void formWiFiRomaingFWupgrade(webs_t wp, char_t *path, char_t *query);
#endif
extern void formWlAc(webs_t wp, char_t *path, char_t *query);
extern void formAdvanceSetup(webs_t wp, char_t *path, char_t *query);
extern int wirelessClientList(int eid, webs_t wp, int argc, char_t **argv);
extern int wirelessiNICClientList(int eid, webs_t wp, int argc, char_t **argv);
extern void formWirelessTbl(webs_t wp, char_t *path, char_t *query);
extern void formWlSiteSurvey(webs_t wp, char_t *path, char_t *query);
extern int wlSiteSurveyTbl(int eid, webs_t wp, int argc, char_t **argv);
extern int wlSurveyOnlyTbl(int eid, webs_t wp, int argc, char_t **argv);
extern int wliNICSiteSurveyonlyTbl(int eid, webs_t wp, int argc, char_t **argv);
#ifdef _ADV_CONTROL_
extern void formAdvManagement(webs_t wp, char_t *path, char_t *query);
#endif
extern int SDHCPList(int eid, webs_t wp, int argc, char_t **argv);
extern void formSDHCP(webs_t wp, char_t *path, char_t *query);
#ifdef WLAN_WPA
extern void formWlEncrypt(webs_t wp, char_t *path, char_t *query);
#endif
#ifndef HOME_GATEWAY
extern void formCerload(webs_t wp, char_t * path, char_t * query);
extern void formStaEncrypt(webs_t wp, char_t *path, char_t *query);
extern void formStaSec(webs_t wp, char_t *path, char_t *query);
extern void formRadius(webs_t wp, char_t *path, char_t *query);
extern int RadiusCltList(int eid, webs_t wp, int argc, char_t **argv);
extern int RadiusUsrList(int eid, webs_t wp, int argc, char_t **argv);
#endif
extern void formTcpipSetup(webs_t wp, char_t *path, char_t *query);
extern int isDhcpClientExist(char *name);
#ifdef HOME_GATEWAY
extern void formWanTcpipSetup(webs_t wp, char_t *path, char_t *query);
extern void formReflashClientTbl(webs_t wp, char_t *path, char_t *query);
extern int dhcpClientList(int eid, webs_t wp, int argc, char_t **argv);
extern void formPortFw(webs_t wp, char_t *path, char_t *query);
extern void formFilter(webs_t wp, char_t *path, char_t *query);
extern void formUrlb(webs_t wp, char_t *path, char_t *query);
extern int portFwList(int eid, webs_t wp, int argc, char_t **argv);
extern int macFilterList(int eid, webs_t wp, int argc, char_t **argv);
extern void formTriggerPort(webs_t wp, char_t *path, char_t *query);
extern int triggerPortList(int eid, webs_t wp, int argc, char_t **argv);
#endif
#ifdef _MSSID_
extern void formMultipleSSID(webs_t wp, char_t *path, char_t *query);
#endif
#ifdef _INIC_MSSID_
extern void formiNICMultipleSSID(webs_t wp, char_t *path, char_t *query);
#endif
#ifdef _WISP_
void formStaDrvSetup(webs_t wp, char_t *path, char_t *query);
#endif
void formrefresh(webs_t wp, char_t *path, char_t *query);
extern int ConnectionList(int eid, webs_t wp, int argc, char_t **argv);
#ifdef _EXPORT_SYS_LOG_
void formSystemLog(webs_t wp, char_t *path, char_t *query);
#endif
#ifdef _WIFI_ROMAING_
void formGALog(webs_t wp, char_t *path, char_t *query);
void formSlaveResult(webs_t wp, char_t *path, char_t *query);
void formSlaveFW(webs_t wp, char_t *path, char_t *query);
#endif
#if defined(_WIRELESS_SCHEDULE_V2_)
extern void formWIRESch(webs_t wp, char_t *path, char_t *query);
extern int WIRESchList(int eid, webs_t wp, int argc, char_t **argv);
#endif
#if defined(_LOCAL_SEARCH_)
extern void life_local_agent();
#endif
#ifdef _TEXTFILE_CONFIG_
extern void formSaveText(webs_t wp, char_t *path, char_t *query);
#endif
#ifdef _OPENVPN_
extern void formVPNenabled(webs_t wp, char_t *path, char_t *query);
extern void formVPNsetup(webs_t wp, char_t *path, char_t *query);
extern void formVPNuser(webs_t wp, char_t *path, char_t *query);
extern void formVPNCaCrt(webs_t wp, char_t *path, char_t *query);
extern void formVPNDHPem(webs_t wp, char_t *path, char_t *query);
extern void formVPNServerCrt(webs_t wp, char_t *path, char_t *query);
extern void formVPNServerKey(webs_t wp, char_t *path, char_t *query);
extern void formVPNClientCrt(webs_t wp, char_t *path, char_t *query);
extern void formVPNClientKey(webs_t wp, char_t *path, char_t *query);
extern void formVPNStaticKey(webs_t wp, char_t *path, char_t *query);
extern void formVPNExportClientConf(webs_t wp, char_t *path, char_t *query);
extern int OpenVpnAccountList(int eid, webs_t wp, int argc, char_t **argv);
#endif
#ifdef _Edimax_
extern int wispSiteSurveyTbl5G(int eid, webs_t wp, int argc, char_t **argv);
extern int wispSiteSurveyTbl(int eid, webs_t wp, int argc, char_t **argv);
#endif
#ifdef _DISCONMSG_
extern void formToken(webs_t wp, char_t *path, char_t *query);
#endif
#endif // _INCLUDE_APFORM_H
