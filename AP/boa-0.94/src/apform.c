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
#include "../webs.h"
#include "apmib.h"
#include "utility.h"
#include "apform.h"

int formsDefine(void)
{
websFormDefine(T("formWlanSetup"), formWlanSetup);  
#ifdef _ADV_CONTROL_
websFormDefine(T("formAdvManagement"), formAdvManagement);
#endif
websFormDefine(T("formJQueryData"), formJQueryData);
websFormDefine(T("formWdsEncrypt"), formWdsEncrypt);
websFormDefine(T("formTcpipSetup"), formTcpipSetup);
websAspDefine(T("showSomeString"), showSomeString);
websAspDefine(T("showWebsPasswd"), showWebsPasswd);
websAspDefine(T("getInfo"), getInfo);
websAspDefine(T("getIndex"), getIndex);
websAspDefine(T("getMyName"), getMyName);
websAspDefine(T("wlAcList"), wlAcList);
websFormDefine(T("formPasswordSetup"), formPasswordSetup);
#ifdef _WIFI_ROMAING_
websAspDefine(T("SlaveList"), SlaveList);
#endif
websFormDefine(T("formUploadIE"), formUploadIE);
websFormDefine(T("formUpload"), formUpload);
websFormDefine(T("formBootUpload"), formBootUpload);
websFormDefine(T("formWlAc"), formWlAc);
websFormDefine(T("formAdvanceSetup"), formAdvanceSetup);
websFormDefine(T("formSecLog"), formSecLog);
#ifdef _IQv2_
websFormDefine(T("formwizResetDefault"), formwizResetDefault);
#endif
#ifdef _AUTO_FW_UPGRADE_
websFormDefine(T("formAutoFWupgradeIE"), formAutoFWupgradeIE);
websFormDefine(T("formAutoFWupgrade"), formAutoFWupgrade);
#endif
websFormDefine(T("formSysLog"), formSysLog);
websFormDefine(T("formDebug"), formDebug);
websFormDefine(T("formSaveConfig"), formSaveConfig);
websFormDefine(T("formSaveConfigSec"), formSaveConfigSec);
websFormDefine(T("formTestResult"), formTestResult);
websFormDefine(T("formResetDefault"), formResetDefault);
websFormDefine(T("formWlSiteSurvey"), formWlSiteSurvey);
websAspDefine(T("wlSiteSurveyTbl"), wlSiteSurveyTbl);
websAspDefine(T("wlSurveyOnlyTbl"), wlSurveyOnlyTbl);
websAspDefine(T("wliNICSiteSurveyonlyTbl"), wliNICSiteSurveyonlyTbl);
websAspDefine(T("wdsList"), wdsList);
websAspDefine(T("SDHCPList"), SDHCPList);
websFormDefine(T("formSDHCP"), formSDHCP);
#ifdef HOME_GATEWAY
websAspDefine(T("Wan1QosList"), Wan1QosList);
#ifdef _EZ_QOS_
websFormDefine(T("formEZQoS"), formEZQoS);
websFormDefine(T("formEZQoSMode"), formEZQoSMode);
websAspDefine(T("EZQosList"), EZQosList);
websFormDefine(T("formBandwidth_Calc"), formBandwidth_Calc);
#endif
websAspDefine(T("QosShow"), QosShow);
websFormDefine(T("formQoS"), formQoS);
#endif
#ifdef WLAN_WPA
websFormDefine(T("formWlEncrypt"), formWlEncrypt);
#endif

#ifndef HOME_GATEWAY
websFormDefine(T("formStaEncrypt"), formStaEncrypt);
websFormDefine(T("formRadius"), formRadius);
websAspDefine(T("RadiusCltList"), RadiusCltList);
websAspDefine(T("RadiusUsrList"), RadiusUsrList);
#endif
websFormDefine(T("formWpsEnable"),formWpsEnable);
websFormDefine(T("formWpsStart"),formWpsStart);
websFormDefine(T("formWifiEnable"), formWifiEnable);
#if defined(HOME_GATEWAY) || defined(_AP_WITH_TIMEZONE_)
websFormDefine(T("formTimeZoneSetup"), formTimeZoneSetup);
#endif
#ifdef HOME_GATEWAY
websFormDefine(T("formPSSetup"), formPSSetup);
websFormDefine(T("formOpMode"), formOpMode);
websAspDefine(T("dhcpClientList"), dhcpClientList);
websFormDefine(T("formReflashClientTbl"), formReflashClientTbl);
websFormDefine(T("formWizSetup"), formWizSetup);
websFormDefine(T("formReManagementSetup"), formReManagementSetup);
websFormDefine(T("formWanTcpipSetup"), formWanTcpipSetup);
websFormDefine(T("formDynIpSetup"), formDynIpSetup);
websFormDefine(T("formStcIpSetup"), formStcIpSetup);
websFormDefine(T("formPPPoESetup"), formPPPoESetup);
websFormDefine(T("formPPTPSetup"), formPPTPSetup);
websFormDefine(T("formL2TPSetup"), formL2TPSetup);
websFormDefine(T("formTELBPSetup"), formTELBPSetup);
websFormDefine(T("formDDNSSetup"), formDDNSSetup);
websFormDefine(T("formWlEnable"), formWlEnable);
websAspDefine(T("getiNICInfo"), getiNICInfo);
websAspDefine(T("getiNICIndex"), getiNICIndex);
websFormDefine(T("formiNICEnable"), formiNICEnable);
websFormDefine(T("formiNICSetup"), formiNICSetup);
websFormDefine(T("formiNICAdvanceSetup"), formiNICAdvanceSetup);
websFormDefine(T("formiNICEncrypt"), formiNICEncrypt);
websFormDefine(T("formiNICAc"), formiNICAc);
websFormDefine(T("formiNICSiteSurvey"), formiNICSiteSurvey);
websAspDefine(T("wliNICSiteSurveyTbl"), wliNICSiteSurveyTbl);
websAspDefine(T("wliNICAcList"), wliNICAcList);
websFormDefine(T("formiNICWpsEnable"),formiNICWpsEnable);
websFormDefine(T("formiNICWpsStart"),formiNICWpsStart);
websFormDefine(T("formiNICWdsEncrypt"), formiNICWdsEncrypt);
websFormDefine(T("formPortFw"), formPortFw);
websFormDefine(T("formNatEnable"), formNatEnable);
websFormDefine(T("formAPModeSwitch"), formAPModeSwitch);
#if defined(_IGMP_PROXY_)
websFormDefine(T("formIgmpEnable"), formIgmpEnable);
#endif
websFormDefine(T("formVirtualSv"), formVirtualSv);
websFormDefine(T("formTriggerPort"), formTriggerPort);
websFormDefine(T("formUPNPSetup"), formUPNPSetup);
websFormDefine(T("formALGSetup"), formALGSetup);
websFormDefine(T("formHWNATSetup"), formHWNATSetup);
websAspDefine(T("portFwList"), portFwList);
websAspDefine(T("virtualSvList"), virtualSvList);
websAspDefine(T("triggerPortList"), triggerPortList);
websFormDefine(T("formFwEnable"), formFwEnable);
websFormDefine(T("formFilter"), formFilter);
websFormDefine(T("formPreventionSetup"), formPreventionSetup);
websAspDefine(T("macFilterList"), macFilterList);
websAspDefine(T("ACPCList"), ACPCList);
websAspDefine(T("URLBList"), URLBList); 
websAspDefine(T("DMZList"), DMZList);
websAspDefine(T("StcRoutList"), StcRoutList);
websFormDefine(T("formUrlb"), formUrlb);
websFormDefine(T("formConnect"), formConnect);
#endif
websFormDefine(T("formApply"), formApply);
websFormDefine(T("formReboot"), formReboot);
websFormDefine(T("formAccept"), formAccept);
websFormDefine(T("formLicence"), formLicence);
websAspDefine(T("wirelessClientList"), wirelessClientList);
websAspDefine(T("wirelessiNICClientList"), wirelessiNICClientList);
websFormDefine(T("formWirelessTbl"), formWirelessTbl);
websFormDefine(T("formStats"), formStats);
#ifdef _MSSID_
websFormDefine(T("formMultipleSSID"), formMultipleSSID);
#endif
#ifdef _INIC_MSSID_
websFormDefine(T("formiNICMultipleSSID"), formiNICMultipleSSID);
#endif
#ifdef _WISP_
websFormDefine(T("formStaDrvSetup"),formStaDrvSetup);
#endif
#ifdef _IQv2_
websFormDefine(T("wiz_5in1_redirect"),wiz_5in1_redirect);
websFormDefine(T("chkLink"),chkLink);
websFormDefine(T("setWAN"),setWAN);
websFormDefine(T("setWifi"),setWifi);
websFormDefine(T("saveAndReboot"),saveAndReboot);
#endif
websFormDefine(T("formiNICbasic"),formiNICbasic);
websFormDefine(T("formWlbasic"),formWlbasic);
websFormDefine(T("formWlEnableSwitch"),formWlEnableSwitch);
websFormDefine(T("formiNICEnableSwitch"),formiNICEnableSwitch);
#ifdef _WIFI_ROMAING_
websFormDefine(T("formWlbasicREP"),formWlbasicREP);
websFormDefine(T("formiNICbasicREP"),formiNICbasicREP);
websFormDefine(T("formWiFiRomaingFWupgrade"), formWiFiRomaingFWupgrade);
#endif
#ifdef _DNS_PROXY_
#ifdef _EDIT_DNSPROXYURL_ //patrick add
websFormDefine(T("formDNSProxyrules"), formDNSProxyrules);
websAspDefine(T("DNSPROXYURLList"), DNSPROXYURLList);
websAspDefine(T("TemporaryDNSPROXYURLList"), TemporaryDNSPROXYURLList);
#endif
#endif
websAspDefine(T("ConnectionList"), ConnectionList);
websFormDefine(T("formrefresh"),formrefresh);
#ifdef _EXPORT_SYS_LOG_
websFormDefine(T("formSystemLog"), formSystemLog);
#endif
#ifdef _WIFI_ROMAING_
websFormDefine(T("formGALog"), formGALog);
websFormDefine(T("formSlaveResult"), formSlaveResult);
websFormDefine(T("formSlaveFW"), formSlaveFW);
#endif
#if defined(_WIRELESS_SCHEDULE_V2_)
websFormDefine(T("formWIRESch"), formWIRESch);
websAspDefine(T("WIRESchList"), WIRESchList); 
#endif
#ifdef _OPENVPN_
websFormDefine(T("formVPNenabled"), formVPNenabled);
websFormDefine(T("formVPNsetup"), formVPNsetup);
websFormDefine(T("formVPNuser"), formVPNuser);
websFormDefine(T("formVPNCaCrt"), formVPNCaCrt);
websFormDefine(T("formVPNDHPem"), formVPNDHPem);
websFormDefine(T("formVPNServerCrt"), formVPNServerCrt);
websFormDefine(T("formVPNServerKey"), formVPNServerKey);
websFormDefine(T("formVPNClientCrt"), formVPNClientCrt);
websFormDefine(T("formVPNClientKey"), formVPNClientKey);
websFormDefine(T("formVPNStaticKey"), formVPNStaticKey);
websFormDefine(T("formVPNExportClientConf"), formVPNExportClientConf);
websAspDefine(T("OpenVpnAccountList"), OpenVpnAccountList);
#endif
#ifdef _TEXTFILE_CONFIG_
websFormDefine(T("formSaveText"), formSaveText);
#endif
#ifdef _Edimax_
websAspDefine(T("wispSiteSurveyTbl5G"), wispSiteSurveyTbl5G);
websAspDefine(T("wispSiteSurveyTbl"), wispSiteSurveyTbl);
#endif
#ifdef _DISCONMSG_
websFormDefine(T("formToken"), formToken);
#endif
oem_get_define_form ();
oem_set_define_form ();
return 0;

}
              
