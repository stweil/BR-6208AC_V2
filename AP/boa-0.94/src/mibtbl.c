/*
 *      MIB table declaration
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: mibtbl.c,v 1.2 2005/05/13 07:54:13 tommy Exp $
 *
 */

/* Include files */
#include "apmib.h"
#include "mibtbl.h"

/* Global variable definitions */
mib_table_entry_T mib_table[]={
{MIB_DHCP_CLIENT_START,			"DHCP_CLIENT_START",		IA_T,			_OFFSET(dhcpClientStart),	_SIZE(dhcpClientStart)},
{MIB_DHCP_CLIENT_END,			"DHCP_CLIENT_END",		IA_T,			_OFFSET(dhcpClientEnd),		_SIZE(dhcpClientEnd)},
{MIB_LICENCE,				"LICENCE",			BYTE_T,			_OFFSET(licence),		_SIZE(licence)},
#ifdef HOME_GATEWAY
{MIB_WAN_DHCP,				"WAN_DHCP",			BYTE_T,			_OFFSET(wanDhcp),		_SIZE(wanDhcp)},
{MIB_WAN_DNS_MODE,			"DNS_MODE",			BYTE_T,			_OFFSET(dnsMode),		_SIZE(dnsMode)},
{MIB_WAN_DNS1,				"DNS1",				IA_T,			_OFFSET(dns1),			_SIZE(dns1)},
{MIB_WAN_DNS2,				"DNS2",				IA_T,			_OFFSET(dns2),			_SIZE(dns2)},
{MIB_WAN_DNS3,				"DNS3",				IA_T,			_OFFSET(dns3),			_SIZE(dns3)},
{MIB_WAN_MODE,				"WAN_MODE",			BYTE_T,			_OFFSET(wanMode),		_SIZE(wanMode)},
//************************************** Dynamic Ip **************************************
{MIB_DYNIP_HOSTNAME,			"DYNIP_HOSTNAME",		STRING_T,		_OFFSET(dynIPHostName),		_SIZE(dynIPHostName)},
{MIB_WAN_MAC_ADDR,			"WAN_MAC_ADDR",			BYTE6_T,		_OFFSET(wanMacAddr),		_SIZE(wanMacAddr)},
//************************************** Static Ip **************************************
{MIB_WAN_IP_ADDR,			"WAN_IP_ADDR",			IA_T,			_OFFSET(wanIpAddr),		_SIZE(wanIpAddr)},
{MIB_WAN_SUBNET_MASK,			"WAN_SUBNET_MASK",		IA_T,			_OFFSET(wanSubnetMask),		_SIZE(wanSubnetMask)},
{MIB_WAN_DEFAULT_GATEWAY,		"WAN_DEFAULT_GATEWAY",		IA_T,			_OFFSET(wanDefaultGateway),	_SIZE(wanDefaultGateway)},
//************************************** PPPoE Setting **************************************
{MIB_PPP_USER,				"PPP_USER_NAME",		STRING_T,		_OFFSET(pppUserName),		_SIZE(pppUserName)},
{MIB_PPP_PASSWORD,			"PPP_PASSWORD",			STRING_T,		_OFFSET(pppPassword),		_SIZE(pppPassword)},
{MIB_PPP_IDLE_TIME,			"PPP_IDLE_TIME",		WORD_T,			_OFFSET(pppIdleTime),		_SIZE(pppIdleTime)},
{MIB_PPP_CONNECT_TYPE,			"PPP_CONNECT_TYPE",		BYTE_T,			_OFFSET(pppConnectType),	_SIZE(pppConnectType)},
{MIB_PPP_SERVNAME,			"PPP_SERVNAME",			STRING_T,		_OFFSET(pppServName),		_SIZE(pppServName)},
{MIB_PPP_MTU,				"PPP_MTU",			WORD_T,			_OFFSET(pppMTU),		_SIZE(pppMTU)},
{MIB_PPP_TTL_ENABLED,			"PPP_TTL_ENABLED",		BYTE_T,			_OFFSET(pppEnTtl),		_SIZE(pppEnTtl)},
{MIB_PPTP_IPMODE,			"PPTP_IPMODE",			BYTE_T,			_OFFSET(pptpIpMode),		_SIZE(pptpIpMode)},
{MIB_PPTPIP_ADDR,			"PPTP_IP_ADDR",			IA_T,			_OFFSET(pptpIPAddr),		_SIZE(pptpIPAddr)},
{MIB_PPTPIPMAKE_ADDR,			"PPTP_IP_MASK_ADDR",		IA_T,			_OFFSET(pptpIPMaskAddr),	_SIZE(pptpIPMaskAddr)},
{MIB_PPTPDFGATEWAY,			"PPTP_DEF_GATEWAY",		IA_T,			_OFFSET(pptpDfGateway),		_SIZE(pptpDfGateway)},
#ifdef _PPTP_FQDN_
{MIB_PPTPGATEWAY,			"PPTP_GATEWAY",			STRING_T,		_OFFSET(pptpGateway),		_SIZE(pptpGateway)},
#else
{MIB_PPTPGATEWAY,			"PPTP_GATEWAY",			IA_T,			_OFFSET(pptpGateway),		_SIZE(pptpGateway)},
#endif
{MIB_PPTP_USER,				"PPTP_USER_NAME",		STRING_T,		_OFFSET(pptpUserName),		_SIZE(pptpUserName)},
{MIB_PPTP_PASSWORD,			"PPTP_PASSWORD",		STRING_T,		_OFFSET(pptpPassword),		_SIZE(pptpPassword)},
{MIB_PPTP_IDLE_TIME,			"PPTP_IDLE_TIME",		WORD_T,			_OFFSET(pptpIdleTime),		_SIZE(pptpIdleTime)},
{MIB_PPTP_CONNECT_TYPE,			"PPTP_CONNECT_TYPE",		BYTE_T,			_OFFSET(pptpConnectType),	_SIZE(pptpConnectType)},
{MIB_PPTP_CONNT_ID,			"PPTP_CONNT_ID",		STRING_T,		_OFFSET(pptpConntID),		_SIZE(pptpConntID)},
{MIB_PPTP_BEZEQ_ENABLE,			"PPTP_BEZEQ_ENABLED",		BYTE_T,			_OFFSET(pptpBEZEQEnable),	_SIZE(pptpBEZEQEnable)},
{MIB_PPTP_MTU,				"PPTP_MTU",			WORD_T,			_OFFSET(pptpMTU),		_SIZE(pptpMTU)},
//************************************** L2TP Setting **************************************
{MIB_L2TP_IPMODE,			"L2TP_IPMODE",			BYTE_T,			_OFFSET(L2TPIpMode),		_SIZE(L2TPIpMode)},
{MIB_L2TP_IP_ADDR,			"L2TP_IP_ADDR",			IA_T,			_OFFSET(L2TPIPAddr),		_SIZE(L2TPIPAddr)},
{MIB_L2TP_MAKEADDR,			"L2TP_MASK_ADDR",		IA_T,			_OFFSET(L2TPMaskAddr),		_SIZE(L2TPMaskAddr)},
{MIB_L2TP_DEFGATEWAY,			"L2TP_DEFGATEWAY",		IA_T,			_OFFSET(L2TPDefGateway),	_SIZE(L2TPDefGateway)},
{MIB_L2TP_GATEWAY,			"L2TP_GATEWAY",			STRING_T,		_OFFSET(L2TPGateway),		_SIZE(L2TPGateway)},
{MIB_L2TP_USER,				"L2TP_USER_NAME",		STRING_T,		_OFFSET(L2TPUserName),		_SIZE(L2TPUserName)},
{MIB_L2TP_PASSWORD,			"L2TP_PASSWORD",		STRING_T,		_OFFSET(L2TPPassword),		_SIZE(L2TPPassword)},
{MIB_L2TP_IDLE_TIME,			"L2TP_IDLE_TIME",		WORD_T,			_OFFSET(L2TPIdleTime),		_SIZE(L2TPIdleTime)},
{MIB_L2TP_CONNECT_TYPE,			"L2TP_CONNECT_TYPE",		BYTE_T,			_OFFSET(L2TPConnectType),	_SIZE(L2TPConnectType)},
{MIB_L2TP_MTU,				"L2TP_MTU",			WORD_T,			_OFFSET(L2TPMTU),		_SIZE(L2TPMTU)},
//************************************** TelStra Big Pond **************************************
{MIB_TELBPIP_ADDR,			"TELBP_IP_ADDR",		IA_T,			_OFFSET(telBPIPAddr),		_SIZE(telBPIPAddr)},
{MIB_TELBP_USER,			"TELBP_USER_NAME",		STRING_T,		_OFFSET(telBPUserName),		_SIZE(telBPUserName)},
{MIB_TELBP_PASSWORD,			"TELBP_PASSWORD",		STRING_T,		_OFFSET(telBPPassword),		_SIZE(telBPPassword)},
{MIB_TELBP_ENABLED,			"TELBP_ENABLED",		BYTE_T,			_OFFSET(telBPEnabled),		_SIZE(telBPEnabled)},
//************************************** Port Forwarding **************************************
{MIB_PORTFW_ENABLED,			"PORTFW_ENABLED",		BYTE_T,			_OFFSET(portFwEnabled),		_SIZE(portFwEnabled)},
{MIB_PORTFW_NUM,			"PORTFW_TBL_NUM",		BYTE_T,			_OFFSET(portFwNum),		_SIZE(portFwNum)},
{MIB_PORTFW,				"PORTFW_TBL",			PORTFW_ARRAY_T,		_OFFSET(portFwArray),		_SIZE(portFwArray)},
//-------------------------------------------------------------------------------------------
{MIB_VSER,				"VSER_TBL",			VSER_ARRAY_T,		_OFFSET(vserArray),		_SIZE(vserArray)},

{MIB_IPFILTER_ENABLED,			"IPFILTER_ENABLED",		BYTE_T,			_OFFSET(ipFilterEnabled),	_SIZE(ipFilterEnabled)},
{MIB_IPFILTER_NUM,			"IPFILTER_TBL_NUM",		BYTE_T,			_OFFSET(ipFilterNum),		_SIZE(ipFilterNum)},
{MIB_IPFILTER,				"IPFILTER_TBL",			IPFILTER_ARRAY_T,	_OFFSET(ipFilterArray),		_SIZE(ipFilterArray)},

{MIB_DMZ_NUM,				"DMZ_TBL_NUM",			BYTE_T,			_OFFSET(DMZNum),		_SIZE(DMZNum)},
{MIB_DMZ,				"DMZ_TBL",			DMZ_ARRAY_T,		_OFFSET(DMZArray),		_SIZE(DMZArray)},
//************************************** URL Blocking **************************************
{MIB_URLB_ENABLED,			"URLB_ENABLED",			BYTE_T,			_OFFSET(URLBEnabled),		_SIZE(URLBEnabled)},
{MIB_URLB_NUM,				"URLB_TBL_NUM",			BYTE_T,			_OFFSET(URLBNum),		_SIZE(URLBNum)},
{MIB_URLB,				"URLB_TBL",			URLB_ARRAY_T,		_OFFSET(URLBArray),		_SIZE(URLBArray)},
//************************************** Access Control **************************************
{MIB_ACPC_ENABLED,			"ACPC_ENABLED",			BYTE_T,			_OFFSET(ACPCEnabled),		_SIZE(ACPCEnabled)},
{MIB_IPDENY_ENABLED,			"IPDENY_ENABLED",		BYTE_T,			_OFFSET(ipDenyEnabled),		_SIZE(ipDenyEnabled)},
{MIB_ACPC_NUM,				"ACPC_TBL_NUM",			BYTE_T,			_OFFSET(ACPCNum),		_SIZE(ACPCNum)},
{MIB_ACPC,				"ACPC_TBL",			ACPC_ARRAY_T,		_OFFSET(ACPCArray),		_SIZE(ACPCArray)},

{MIB_MACDENY_ENABLED,			"MACDENY_ENABLED",		BYTE_T,			_OFFSET(macDenyEnabled),	_SIZE(macDenyEnabled)},
{MIB_MACFILTER_ENABLED,			"MACFILTER_ENABLED",		BYTE_T,			_OFFSET(macFilterEnabled),	_SIZE(macFilterEnabled)},
{MIB_MACFILTER_NUM,			"MACFILTER_TBL_NUM",		BYTE_T,			_OFFSET(macFilterNum),		_SIZE(macFilterNum)},
{MIB_MACFILTER,				"MACFILTER_TBL",		MACFILTER_ARRAY_T,	_OFFSET(macFilterArray),	_SIZE(macFilterArray)},

{MIB_PORTFILTER_ENABLED,		"PORTFILTER_ENABLED",		BYTE_T,			_OFFSET(portFilterEnabled),	_SIZE(portFilterEnabled)},
{MIB_PORTFILTER_NUM,			"PORTFILTER_TBL_NUM",		BYTE_T,			_OFFSET(portFilterNum),		_SIZE(portFilterNum)},
{MIB_PORTFILTER,			"PORTFILTER_TBL",		PORTFILTER_ARRAY_T,	_OFFSET(portFilterArray),	_SIZE(portFilterArray)},
//************************************** Special Application **************************************
{MIB_TRIGGERPORT_ENABLED,		"TRIGGERPORT_ENABLED",		BYTE_T,			_OFFSET(triggerPortEnabled),	_SIZE(triggerPortEnabled)},
{MIB_TRIGGERPORT_NUM,			"TRIGGERPORT_TBL_NUM",		BYTE_T,			_OFFSET(triggerPortNum),	_SIZE(triggerPortNum)},
{MIB_TRIGGERPORT,			"TRIGGERPORT_TBL",		TRIGGERPORT_ARRAY_T,	_OFFSET(triggerPortArray),	_SIZE(triggerPortArray)},

{MIB_DMZ_ENABLED,			"DMZ_ENABLED", 			BYTE_T,			_OFFSET(dmzEnabled),		_SIZE(dmzEnabled)},
//************************************** Static Routing **************************************
{MIB_SROUT_ENABLED,			"SROUT_ENABLED",		BYTE_T,			_OFFSET(sroutEnabled),		_SIZE(sroutEnabled)},
{MIB_SROUT_NUM,				"SROUT_TBL_NUM",		BYTE_T,			_OFFSET(sroutNum),		_SIZE(sroutNum)},
{MIB_SROUT,				"SROUT_TBL",			SROUT_ARRAY_T,		_OFFSET(sroutArray),		_SIZE(sroutArray)},
{MIB_WAN1_QOS_ENABLED,			"WAN1_QOS_ENABLED",		BYTE_T,			_OFFSET(QosEnabled[0]),		_SIZE(QosEnabled[0])},
{MIB_WAN1_QOS_NUM,			"WAN1_QOS_TBL_NUM",		BYTE_T,			_OFFSET(QosNum[0]),		_SIZE(QosNum[0])},
{MIB_WAN1_QOS,				"WAN1_QOS_TBL",			WAN1_QOS_ARRAY_T,	_OFFSET(QosArray[0]),		_SIZE(QosArray[0])},
#endif

// Static DHCP Lease -------------------------------------------------------------------------------------
{MIB_STATIC_DHCP_ENABLED,		"SDHCP_ENABLED",		BYTE_T,			_OFFSET(SDHCPEnabled),		_SIZE(SDHCPEnabled)},
{MIB_STATIC_DHCP_NUM,			"SDHCP_TBL_NUM",		BYTE_T,			_OFFSET(SDHCPNum),		_SIZE(SDHCPNum)},
{MIB_STATIC_DHCP,			"SDHCP_TBL",			SDHCP_ARRAY_T,		_OFFSET(SDHCPArray),		_SIZE(SDHCPArray)},
//------------------------------------------------------------------------------------------------------------

#ifndef HOME_GATEWAY
//************************************** RADIUS Server **************************************
{MIB_RSER_ENABLED,			"RSER_ENABLED",			BYTE_T,			_OFFSET(rserEnabled),		_SIZE(rserEnabled)},
{MIB_RSER_CLT_NUM,			"RSER_CLT_TBL_NUM",		BYTE_T,			_OFFSET(rserCltNum),		_SIZE(rserCltNum)},
{MIB_RSER_CLT,				"RSER_CLT_TBL",			RSER_CLT_ARRAY_T,	_OFFSET(rserCltArray),		_SIZE(rserCltArray)},

{MIB_RSER_USR_NUM,			"RSER_USR_TBL_NUM",		BYTE_T,			_OFFSET(rserUsrNum),		_SIZE(rserUsrNum)},
{MIB_RSER_USR,				"RSER_USR_TBL",			RSER_USR_ARRAY_T,	_OFFSET(rserUsrArray),		_SIZE(rserUsrArray)},
#endif

//#ifdef HOME_GATEWAY
// RexHua
{MIB_WLAN_TX_POWER,			"WLAN_TX_POWER",		BYTE_T,			_OFFSET(wlanTxPower),		_SIZE(wlanTxPower)},
{MIB_WLAN_WMM,				"WLAN_WMM",			BYTE_T,			_OFFSET(wlanWmm),		_SIZE(wlanWmm)},
{MIB_WLAN_TURBO,			"WLAN_TURBO",			BYTE_T,			_OFFSET(wlanTurbo),		_SIZE(wlanTurbo)},
{MIB_WLAN_N_FIX_RATE,			"WLAN_N_FIXRTAE",		BYTE_T,			_OFFSET(wlanNFixRate),		_SIZE(wlanNFixRate)},
{MIB_WLAN_N_CHANNEL_WIDTH,		"WLAN_N_CHAN_WIDTH",		BYTE_T,			_OFFSET(wlanNChanWidth),	_SIZE(wlanNChanWidth)},
//#endif

{MIB_WLAN_TRAN_RATE,                    "WLAN_TRAN_RATE",               STRING_T,               _OFFSET(wlanTranRate),          _SIZE(wlanTranRate)},
{MIB_WLAN_RATE_MODE,                    "WLAN_RATE_MODE",               BYTE_T,                 _OFFSET(wlanRateMode),          _SIZE(wlanRateMode)},
{MIB_WLAN_CTS,                          "WLAN_CTS",                     BYTE_T,                 _OFFSET(wlanCts),               _SIZE(wlanCts)},
{MIB_WLAN_BURST,                        "WLAN_BURST",                   BYTE_T,                 _OFFSET(wlanBurst),             _SIZE(wlanBurst)},

//*************************************** wireless setting ****************************************
{MIB_1X_MODE,				"DOT1X_MODE",			BYTE_T,			_OFFSET(sel1xMode),		_SIZE(sel1xMode)},
{MIB_ELAN_MAC_ADDR,			"ELAN_MAC_ADDR",		BYTE6_T,		_OFFSET(elanMacAddr),		_SIZE(elanMacAddr)},
{MIB_WLAN_MAC_ADDR,			"WLAN_MAC_ADDR",		BYTE6_T,		_OFFSET(wlanMacAddr),		_SIZE(wlanMacAddr)},
{MIB_WLAN_SSID,				"SSID",				STRING_T,		_OFFSET(ssid),			_SIZE(ssid)},
{MIB_WLAN_CHAN_NUM,			"CHANNEL",			BYTE_T,			_OFFSET(channel),		_SIZE(channel)},
{MIB_WLAN_WEP,				"WEP",				BYTE_T,			_OFFSET(wep),			_SIZE(wep)},
{MIB_WLAN_WEP64_KEY1,			"WEP64_KEY1",			BYTE5_T,		_OFFSET(wep64Key1),		_SIZE(wep64Key1)},
{MIB_WLAN_WEP64_KEY2,			"WEP64_KEY2",			BYTE5_T,		_OFFSET(wep64Key2),		_SIZE(wep64Key2)},
{MIB_WLAN_WEP64_KEY3,			"WEP64_KEY3",			BYTE5_T,		_OFFSET(wep64Key3),		_SIZE(wep64Key3)},
{MIB_WLAN_WEP64_KEY4,			"WEP64_KEY4",			BYTE5_T,		_OFFSET(wep64Key4),		_SIZE(wep64Key4)},
{MIB_WLAN_WEP128_KEY1,			"WEP128_KEY1",			BYTE13_T,		_OFFSET(wep128Key1),		_SIZE(wep128Key1)},
{MIB_WLAN_WEP128_KEY2,			"WEP128_KEY2",			BYTE13_T,		_OFFSET(wep128Key2),		_SIZE(wep128Key2)},
{MIB_WLAN_WEP128_KEY3,			"WEP128_KEY3",			BYTE13_T,		_OFFSET(wep128Key3),		_SIZE(wep128Key3)},
{MIB_WLAN_WEP128_KEY4,			"WEP128_KEY4",			BYTE13_T,		_OFFSET(wep128Key4),		_SIZE(wep128Key4)},
{MIB_WLAN_WEP_DEFAULT_KEY,		"WEP_DEFAULT_KEY",		BYTE_T,			_OFFSET(wepDefaultKey),		_SIZE(wepDefaultKey)},
{MIB_WLAN_WEP_KEY_TYPE,			"WEP_KEY_TYPE",			BYTE_T,			_OFFSET(wepKeyType),		_SIZE(wepKeyType)},
{MIB_WLAN_FRAG_THRESHOLD,		"FRAG_THRESHOLD",		WORD_T,			_OFFSET(fragThreshold),		_SIZE(fragThreshold)},
{MIB_WLAN_SUPPORTED_RATE,		"SUPPORTED_RATES",		WORD_T,			_OFFSET(supportedRates),	_SIZE(supportedRates)},
{MIB_WLAN_BEACON_INTERVAL,		"BEACON_INTERVAL",		WORD_T,			_OFFSET(beaconInterval),	_SIZE(beaconInterval)},
{MIB_WLAN_PREAMBLE_TYPE,		"PREAMBLE_TYPE",		BYTE_T,			_OFFSET(preambleType),		_SIZE(preambleType)},
{MIB_WLAN_BASIC_RATE,			"BASIC_RATES",			WORD_T,			_OFFSET(basicRates),		_SIZE(basicRates)},
{MIB_WLAN_RTS_THRESHOLD,		"RTS_THRESHOLD",		WORD_T,			_OFFSET(rtsThreshold),		_SIZE(rtsThreshold)},
{MIB_WLAN_AUTH_TYPE,			"AUTH_TYPE",			BYTE_T,			_OFFSET(authType),		_SIZE(authType)},
{MIB_WLAN_HIDDEN_SSID,			"HIDDEN_SSID",			BYTE_T,			_OFFSET(hiddenSSID),		_SIZE(hiddenSSID)},
{MIB_WLAN_DISABLED,			"WLAN_DISABLED",		BYTE_T,			_OFFSET(wlanDisabled),		_SIZE(wlanDisabled)},
{MIB_WLAN_INACTIVITY_TIME,		"INACTIVITY_TIME",		DWORD_T,		_OFFSET(inactivityTime),	_SIZE(inactivityTime)},
{MIB_WLAN_RATE_ADAPTIVE_ENABLED,	"RATE_ADAPTIVE_ENABLED",	BYTE_T,			_OFFSET(rateAdaptiveEnabled),	_SIZE(rateAdaptiveEnabled)},
{MIB_WLAN_DTIM_PERIOD,			"DTIM_PERIOD",			BYTE_T,			_OFFSET(dtimPeriod),		_SIZE(dtimPeriod)},
{MIB_WLAN_NETWORK_TYPE,			"NETWORK_TYPE",			BYTE_T,			_OFFSET(networkType),		_SIZE(networkType)},
{MIB_WLAN_IAPP_DISABLED,		"IAPP_DISABLED",		BYTE_T,			_OFFSET(iappDisabled),		_SIZE(iappDisabled)},
{MIB_WLAN_PROTECTION_DISABLED,		"PROTECTION_DISABLED",		BYTE_T,			_OFFSET(protectionDisabled),	_SIZE(protectionDisabled)},
{MIB_WLAN_NAT25_MAC_CLONE,		"MACCLONE_ENABLED",		BYTE_T,			_OFFSET(maccloneEnabled),	_SIZE(maccloneEnabled)},
{MIB_WLAN_BAND,				"BAND",				BYTE_T,			_OFFSET(wlanBand),		_SIZE(wlanBand)},
{MIB_WLAN_FIX_RATE,			"FIX_RATE",			WORD_T,			_OFFSET(fixedTxRate),		_SIZE(fixedTxRate)},

{MIB_WLAN_WPA2_PRE_AUTH,		"WPA2_PRE_AUTH",		BYTE_T,			_OFFSET(wpa2PreAuth),		_SIZE(wpa2PreAuth)},
{MIB_WLAN_WPA2_CIPHER_SUITE,		"WPA2_CIPHER_SUITE",		BYTE_T,			_OFFSET(wpa2Cipher),		_SIZE(wpa2Cipher)},


{MIB_WLAN_SET_TX,			"WLAN_SET_TX",			BYTE_T,			_OFFSET(wlanSetTx),		_SIZE(wlanSetTx)},
{MIB_AP_MODE,				"AP_MODE",			BYTE_T,			_OFFSET(apMode),		_SIZE(apMode)},
{MIB_SECURITY_MODE,			"SECURITY_MODE",		BYTE_T,			_OFFSET(secMode),		_SIZE(secMode)},
{MIB_CLIENT_IP_DISABLED,		"CLIENT_IP_DISABLED",		BYTE_T,			_OFFSET(clIpDisabled),		_SIZE(clIpDisabled)},
{MIB_WLAN_BLOCK_RELAY,			"WLAN_BLOCK_RELAY",		BYTE_T,			_OFFSET(blockRelay),		_SIZE(blockRelay)},
{MIB_AUTO_MAC_CLONE,			"AUTO_MAC_CLONE",		BYTE_T,			_OFFSET(autoMacClone),		_SIZE(autoMacClone)},
{MIB_OP_MODE,				"OP_MODE",			BYTE_T,			_OFFSET(opMode),		_SIZE(opMode)},
{MIB_WISP_WAN_ID,			"WISP_WAN_ID",			BYTE_T,			_OFFSET(wispWanId),		_SIZE(wispWanId)},

//************************************* STATION of SECURITY *************************************
#ifndef HOME_GATEWAY
{MIB_STA_SSID,				"STA_SSID",			STRING_T,		_OFFSET(staSsid),		_SIZE(staSsid)},
{MIB_STA_SEC_MODE,			"STA_SEC_MODE",			BYTE_T,			_OFFSET(staSecMode),		_SIZE(staSecMode)},
{MIB_ADHOC_SEC_MODE,			"ADHOC_SEC_MODE",		BYTE_T,			_OFFSET(adhocSecMode),		_SIZE(adhocSecMode)},

{MIB_STA_ENCRYPT,			"STA_ENCRYPT",			BYTE_T,			_OFFSET(staEncrypt),		_SIZE(staEncrypt)},
{MIB_STA_WPA_AUTH,			"STA_WPA_AUTH",			BYTE_T,			_OFFSET(staWpaAuth),		_SIZE(staWpaAuth)},
{MIB_STA_AUTH_TYPE,			"STA_AUTH_TYPE",		BYTE_T,			_OFFSET(staAuthType),		_SIZE(staAuthType)},
{MIB_STA_WPA_CIPHER_SUITE,		"STA_WPA_CIPHER_SUITE",		BYTE_T,			_OFFSET(staWpaCipher),		_SIZE(staWpaCipher)},
{MIB_STA_WEP,				"STA_WEP",			BYTE_T,			_OFFSET(staWep),		_SIZE(staWep)},
{MIB_STA_WEP64_KEY1,			"STA_WEP64_KEY1",		BYTE5_T,		_OFFSET(sta64Key1),		_SIZE(sta64Key1)},
{MIB_STA_WEP64_KEY2,			"STA_WEP64_KEY2",		BYTE5_T,		_OFFSET(sta64Key2),		_SIZE(sta64Key2)},
{MIB_STA_WEP64_KEY3,			"STA_WEP64_KEY3",		BYTE5_T,		_OFFSET(sta64Key3),		_SIZE(sta64Key3)},
{MIB_STA_WEP64_KEY4,			"STA_WEP64_KEY4",		BYTE5_T,		_OFFSET(sta64Key4),		_SIZE(sta64Key4)},
{MIB_STA_WEP128_KEY1,			"STA_WEP128_KEY1",		BYTE13_T,		_OFFSET(sta128Key1),		_SIZE(sta128Key1)},
{MIB_STA_WEP128_KEY2,			"STA_WEP128_KEY2",		BYTE13_T,		_OFFSET(sta128Key2),		_SIZE(sta128Key2)},
{MIB_STA_WEP128_KEY3,			"STA_WEP128_KEY3",		BYTE13_T,		_OFFSET(sta128Key3),		_SIZE(sta128Key3)},
{MIB_STA_WEP128_KEY4,			"STA_WEP128_KEY4",		BYTE13_T,		_OFFSET(sta128Key4),		_SIZE(sta128Key4)},
{MIB_STA_WEP_DEFKEY,			"STA_WEP_DEFKEY",		BYTE_T,			_OFFSET(staWepDefKey),		_SIZE(staWepDefKey)},
{MIB_STA_WEP_KEY_TYPE,			"STA_WEP_KEY_TYPE",		BYTE_T,			_OFFSET(staKeyType),		_SIZE(staKeyType)},
{MIB_STA_WPA_PSK_FORMAT,		"STA_PSK_FORMAT",		BYTE_T,			_OFFSET(staPSKFormat),		_SIZE(staPSKFormat)},
{MIB_STA_WPA_PSK,			"STA_WPA_PSK",			STRING_T,		_OFFSET(staPSK),		_SIZE(staPSK)},
{MIB_WLAN_BSSID,			"WLAN_BSSID",			BYTE6_T,		_OFFSET(wlanBssid),		_SIZE(wlanBssid)},

//{MIB_STA_METHOD,			"STA_METHOD",			BYTE_T,			_OFFSET(staMethod),		_SIZE(staMethod)},
{MIB_STA_ENRADIUS,			"STA_ENRADIUS",			BYTE_T,			_OFFSET(staEnRadius),		_SIZE(staEnRadius)},
{MIB_STA_EAP_TYPE,			"STA_EAP_TYPE",			BYTE_T,			_OFFSET(staEapTp),		_SIZE(staEapTp)},
{MIB_STA_ENCLT_CER,			"STA_ENCLT_CER",		BYTE_T,			_OFFSET(staEnCltCer),		_SIZE(staEnCltCer)},
{MIB_STA_ENSER_CER,			"STA_ENSER_CER",		BYTE_T,			_OFFSET(staEnSerCer),		_SIZE(staEnSerCer)},
{MIB_CLT_CER_FILE,			"CLT_CER_FILE",			BYTE_T,			_OFFSET(cltCerFile),		_SIZE(cltCerFile)},
{MIB_SER_CER_FILE,			"SER_CER_FILE",			BYTE_T,			_OFFSET(serCerFile),		_SIZE(serCerFile)},

{MIB_STA_ID,				"STA_ID",			STRING_T,		_OFFSET(staId),			_SIZE(staId)},
{MIB_STA_PASS,				"STA_PASS",			STRING_T,		_OFFSET(staPass),		_SIZE(staPass)},
{MIB_STA_PROTOCOL,			"STA_PROTOCOL",			STRING_T,		_OFFSET(staPoto),		_SIZE(staPoto)},
{MIB_STA_EAP_ID,			"STA_EAP_ID",			STRING_T,		_OFFSET(staEapId),		_SIZE(staEapId)},
{MIB_STA_EAP_PASS,			"STA_EAP_PASS",			STRING_T,		_OFFSET(staEapPass),		_SIZE(staEapPass)},
{MIB_STA_CLIENT_PASS,			"STA_CLIENT_PASS",		STRING_T,		_OFFSET(staCltPass),		_SIZE(staCltPass)},
{MIB_STA_SERVER_PASS,			"STA_SERVER_PASS",		STRING_T,		_OFFSET(staSerPass),		_SIZE(staSerPass)},

{MIB_STA_RTS,				"STA_RTS",			WORD_T,			_OFFSET(staRts),		_SIZE(staRts)},
{MIB_STA_FRAGMENT,			"STA_FRAGMENT",			WORD_T,			_OFFSET(staFrag),		_SIZE(staFrag)},
{MIB_STA_PREAMBLE,			"STA_PREAMBLE",			BYTE_T,			_OFFSET(staPream),		_SIZE(staPream)},
//	#endif
#endif
//**************************************** WEP of WDS****************************************
{MIB_WL_LINKMAC1, 			"WL_LINKMAC1",			BYTE6_T,		_OFFSET(wlLinkMac1),		_SIZE(wlLinkMac1)},
{MIB_WL_LINKMAC2, 			"WL_LINKMAC2",			BYTE6_T,		_OFFSET(wlLinkMac2),		_SIZE(wlLinkMac2)},
{MIB_WL_LINKMAC3, 			"WL_LINKMAC3",			BYTE6_T,		_OFFSET(wlLinkMac3),		_SIZE(wlLinkMac3)},
{MIB_WL_LINKMAC4, 			"WL_LINKMAC4",			BYTE6_T,		_OFFSET(wlLinkMac4),		_SIZE(wlLinkMac4)},
{MIB_WL_LINKMAC5, 			"WL_LINKMAC5",			BYTE6_T,		_OFFSET(wlLinkMac5),		_SIZE(wlLinkMac5)},
{MIB_WL_LINKMAC6, 			"WL_LINKMAC6",			BYTE6_T,		_OFFSET(wlLinkMac6),		_SIZE(wlLinkMac6)},

{MIB_WLAN_WDS_ENABLED,			"WDS_ENABLED",			BYTE_T,			_OFFSET(wdsEnabled),		_SIZE(wdsEnabled)},

{MIB_WLAN_WDS_ENCRYPT,			"WLAN_WDS_ENCRYPT",		BYTE_T,			_OFFSET(wdsEncrypt),		_SIZE(encrypt)},
{MIB_WLAN_WDS_WPA_AUTH,			"WLAN_WDS_WPA_AUTH",		BYTE_T,			_OFFSET(wdsWpaAuth),		_SIZE(wpaAuth)},
{MIB_WLAN_WDS_WPA_CIPHER_SUITE,		"WLAN_WDS_WPA_CIPHER_SUITE",	BYTE_T,			_OFFSET(wdsWpaCipher),		_SIZE(wpaCipher)},
{MIB_WLAN_WDS_WPA2_CIPHER_SUITE,	"WLAN_WDS_WPA2_CIPHER_SUITE",	BYTE_T,			_OFFSET(wdsWpa2Cipher),		_SIZE(wpa2Cipher)},
{MIB_WLAN_WDS_WPA_PSK,			"WLAN_WDS_WPA_PSK",		STRING_T,		_OFFSET(wdsWpaPSK),		_SIZE(wpaPSK)},
{MIB_WLAN_WDS_WPA_PSK_FORMAT,		"WLAN_WDS_PSK_FORMAT",		BYTE_T,			_OFFSET(wdsWpaPSKFormat),	_SIZE(wpaPSKFormat)},
//--------------------------------------------------------------------------------------------

#ifdef WLAN_WPA
{MIB_WLAN_ENCRYPT,			"WLAN_ENCRYPT",			BYTE_T,			_OFFSET(encrypt),		_SIZE(encrypt)},
{MIB_WLAN_ENABLE_SUPP_NONWPA,		"WLAN_ENABLE_SUPP_NONWPA",	BYTE_T,			_OFFSET(enableSuppNonWpa),	_SIZE(enableSuppNonWpa)},
{MIB_WLAN_SUPP_NONWPA,			"WLAN_SUPP_NONWPA",		BYTE_T,			_OFFSET(suppNonWpa),		_SIZE(suppNonWpa)},
{MIB_WLAN_WPA_AUTH,			"WLAN_WPA_AUTH",		BYTE_T,			_OFFSET(wpaAuth),		_SIZE(wpaAuth)},
{MIB_WLAN_WPA_CIPHER_SUITE,		"WLAN_WPA_CIPHER_SUITE",	BYTE_T,			_OFFSET(wpaCipher),		_SIZE(wpaCipher)},
{MIB_WLAN_WPA_PSK,			"WLAN_WPA_PSK",			STRING_T,		_OFFSET(wpaPSK),		_SIZE(wpaPSK)},
{MIB_WLAN_WPA_GROUP_REKEY_TIME,		"WLAN_WPA_GROUP_REKEY_TIME",	DWORD_T,		_OFFSET(wpaGroupRekeyTime),	_SIZE(wpaGroupRekeyTime)},
{MIB_WLAN_ENABLE_MAC_AUTH,		"MAC_AUTH_ENABLED",		BYTE_T,			_OFFSET(macAuthEnabled),	_SIZE(macAuthEnabled)},
{MIB_WLAN_RS_IP,			"RS_IP",			IA_T,			_OFFSET(rsIpAddr),		_SIZE(rsIpAddr)},
{MIB_WLAN_RS_PORT,			"RS_PORT",			WORD_T,			_OFFSET(rsPort),		_SIZE(rsPort)},
{MIB_WLAN_RS_PASSWORD,			"RS_PASSWORD",			STRING_T,		_OFFSET(rsPassword),		_SIZE(rsPassword)},
{MIB_WLAN_RS_RETRY,			"RS_MAXRETRY",			BYTE_T,			_OFFSET(rsMaxRetry),		_SIZE(rsMaxRetry)},
{MIB_WLAN_RS_INTERVAL_TIME,		"RS_INTERVAL_TIME",		WORD_T,			_OFFSET(rsIntervalTime),	_SIZE(rsIntervalTime)},
{MIB_WLAN_ACCOUNT_RS_ENABLED,		"ACCOUNT_RS_ENABLED",		BYTE_T,			_OFFSET(accountRsEnabled),	_SIZE(accountRsEnabled)},
{MIB_WLAN_ACCOUNT_RS_IP,		"ACCOUNT_RS_IP",		IA_T,			_OFFSET(accountRsIpAddr),	_SIZE(accountRsIpAddr)},
{MIB_WLAN_ACCOUNT_RS_PORT,		"ACCOUNT_RS_PORT",		WORD_T,			_OFFSET(accountRsPort),		_SIZE(accountRsPort)},
{MIB_WLAN_ACCOUNT_RS_PASSWORD,		"ACCOUNT_RS_PASSWORD",		STRING_T,		_OFFSET(accountRsPassword),	_SIZE(accountRsPassword)},
{MIB_WLAN_ACCOUNT_UPDATE_ENABLED,	"ACCOUNT_RS_UPDATE_ENABLED",	BYTE_T,			_OFFSET(accountRsUpdateEnabled),_SIZE(accountRsUpdateEnabled)},
{MIB_WLAN_ACCOUNT_UPDATE_DELAY,		"ACCOUNT_RS_UPDATE_DELAY",	WORD_T,			_OFFSET(accountRsUpdateDelay),	_SIZE(accountRsUpdateDelay)},
{MIB_WLAN_ACCOUNT_RS_RETRY,		"ACCOUNT_RS_MAXRETRY",		BYTE_T,			_OFFSET(accountRsMaxRetry),	_SIZE(accountRsMaxRetry)},
{MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME,	"ACCOUNT_RS_INTERVAL_TIME",	WORD_T,			_OFFSET(accountRsIntervalTime),	_SIZE(accountRsIntervalTime)},
//{MIB_WLAN_SUPP_NONWPA,		"WLAN_SUPP_NONWPA",		BYTE_T,			_OFFSET(suppNonWpa),		_SIZE(suppNonWpa)},
{MIB_WLAN_ENABLE_1X,			"WLAN_ENABLE_1X",		BYTE_T,			_OFFSET(enable1X),		_SIZE(enable1X)},
{MIB_WLAN_WPA_PSK_FORMAT,		"WLAN_PSK_FORMAT",		BYTE_T,			_OFFSET(wpaPSKFormat),		_SIZE(wpaPSKFormat)},
//{MIB_WLAN_RS_INTERVAL_TIME,		"MAC_AUTH_ENABLED",		BYTE_T,			_OFFSET(macAuthEnabled),	_SIZE(macAuthEnabled)},
#endif

//{MIB_ALIAS_NAME,			"ALIAS_NAME",			STRING_T,		_OFFSET(aliasName),		_SIZE(aliasName)},
{MIB_IP_ADDR,				"IP_ADDR",			IA_T,			_OFFSET(ipAddr),		_SIZE(ipAddr)},
//--------------------------------------------------------------------------------------------

{MIB_DHCPGATEWAYIP_ADDR,		"DHCPGATEWAYIP_ADDR",		IA_T,			_OFFSET(DhcpGatewayIPAddr),	_SIZE(DhcpGatewayIPAddr)},
{MIB_DHCPNAMESERVER_ADDR,		"DHCPNAMESERVER_ADDR",		IA_T,			_OFFSET(DhcpNameServerIPAddr),	_SIZE(DhcpNameServerIPAddr)},
{MIB_DOMAIN_NAME,			"DOMAIN_NAME",			STRING_T,		_OFFSET(DomainNames),		_SIZE(DomainNames)},
{MIB_LAN_LEASETIME,			"LAN_LEASE_TIME",		DWORD_T,		_OFFSET(leaseTime),		_SIZE(leaseTime)},
//--------------------------------------------------------------------------------------------
{MIB_SUBNET_MASK,			"SUBNET_MASK",			IA_T,			_OFFSET(subnetMask),		_SIZE(subnetMask)},
{MIB_DEFAULT_GATEWAY,			"DEFAULT_GATEWAY",		IA_T,			_OFFSET(defaultGateway),	_SIZE(defaultGateway)},
{MIB_DHCP,				"DHCP",				BYTE_T,			_OFFSET(dhcp),			_SIZE(dhcp)},
{MIB_STP_ENABLED,			"STP_ENABLED",			BYTE_T,			_OFFSET(stpEnabled),		_SIZE(stpEnabled)},
{MIB_WLAN_AC_NUM,			"WLAN_MACAC_NUM",		BYTE_T,			_OFFSET(acNum),			_SIZE(acNum)},
{MIB_WLAN_AC_ENABLED,			"WLAN_MACAC_ENABLED",		BYTE_T,			_OFFSET(acEnabled),		_SIZE(acEnabled)},
{MIB_WLAN_AC_ADDR,			"WLAN_MACAC_ADDR",		WLAC_ARRAY_T,		_OFFSET(acAddrArray),		_SIZE(acAddrArray)},
{MIB_SUPER_NAME,			"SUPER_NAME",			STRING_T,		_OFFSET(superName),		_SIZE(superName)},
{MIB_SUPER_PASSWORD,			"SUPER_PASSWORD",		STRING_T,		_OFFSET(superPassword),		_SIZE(superPassword)},
{MIB_USER_NAME,				"USER_NAME",			STRING_T,		_OFFSET(userName),		_SIZE(userName)},
{MIB_USER_PASSWORD,			"USER_PASSWORD",		STRING_T,		_OFFSET(userPassword),		_SIZE(userPassword)},
//----------------------------------------TimeZone------------------------------------------------
#if defined(HOME_GATEWAY) || defined(_AP_WITH_TIMEZONE_)
{MIB_TIME_ZONE_SEL,			"TIME_ZONE_SEL",		WORD_T,			_OFFSET(TimeZoneSel),		_SIZE(TimeZoneSel)},
#ifdef _ADD_WLSCH_HOUR_
{MIB_START_HOUR,                        "START_HOUR",                   WORD_T,                 _OFFSET(startHour),             _SIZE(startHour)},
{MIB_END_HOUR,                          "END_HOUR",                    	WORD_T,                 _OFFSET(endHour),               _SIZE(endHour)},
#endif
{MIB_START_MONTH,			"START_MONTH",			WORD_T,			_OFFSET(startMonth),		_SIZE(startMonth)},
{MIB_START_DAY,				"START_DAY",			WORD_T,			_OFFSET(startDay),		_SIZE(startDay)},
{MIB_END_MONTH,				"END_MONTH",			WORD_T,			_OFFSET(endMonth),		_SIZE(endMonth)},
{MIB_END_DAY,				"END_DAY",			WORD_T,			_OFFSET(endDay),		_SIZE(endDay)},
{MIB_TIMESERVER_ADDR,			"TIMESERVER_ADDR",		IA_T,			_OFFSET(TimeServerAddr),	_SIZE(TimeServerAddr)},
{MIB_DAYLIGHT_ENABLE,			"DAYLIGHT_ENABLE",		BYTE_T,			_OFFSET(DayLightEnable),	_SIZE(DayLightEnable)},
#ifdef _AUTO_DAYLIGHT_ENABLE_
{MIB_AUTO_DAYLIGHT_ENABLE,		"AUTO_DAYLIGHT_ENABLE",		BYTE_T,			_OFFSET(AutoDayLightEnable),	_SIZE(AutoDayLightEnable)},
#endif
#endif


#ifdef HOME_GATEWAY
//------------------------------------Remote Management Host Address-----------------------------
{MIB_REMANHOST_ADDR,			"REMANHOST_ADDR",		IA_T,			_OFFSET(reManHostAddr),		_SIZE(reManHostAddr)},
{MIB_REMAN_PORT,			"REMAN_PORT",			WORD_T,			_OFFSET(reManPort),		_SIZE(reManPort)},
{MIB_REMANG_ENABLE,			"REMANG_ENABLE",		BYTE_T,			_OFFSET(reMangEnable),		_SIZE(reMangEnable)},

{MIB_UPNP_ENABLE,			"UPNP_ENABLE",			BYTE_T,			_OFFSET(upnpEnable),		_SIZE(upnpEnable)},
{MIB_NAT_ENABLE,			"NAT_ENABLE",			BYTE_T,			_OFFSET(natEnable),		_SIZE(natEnable)},
{MIB_FAST_NAT_ENABLE,			"FAST_NAT_ENABLE",		BYTE_T,			_OFFSET(fastNatEnable),		_SIZE(fastNatEnable)},
{MIB_FIREWALL_ENABLE,			"FIREWALL_ENABLE",		BYTE_T,			_OFFSET(FirewallEnable),	_SIZE(FirewallEnable)},

{MIB_VSER_ENABLED,			"VSER_ENABLED",			BYTE_T,			_OFFSET(vserEnabled),		_SIZE(vserEnabled)},
{MIB_VSER_NUM,				"VSER_TBL_NUM",			BYTE_T,			_OFFSET(vserNum),		_SIZE(vserNum)},

{MIB_POD_ENABLED,			"POD_ENABLED",			BYTE_T,			_OFFSET(podEnable),		_SIZE(podEnable)},
{MIB_PING_ENABLED,			"PING_ENABLED",			BYTE_T,			_OFFSET(pingEnable),		_SIZE(pingEnable)},
{MIB_SCAN_ENABLED,			"SCAN_ENABLED",			BYTE_T,			_OFFSET(scanEnable),		_SIZE(scanEnable)},
{MIB_SYN_ENABLED,			"SYN_ENABLED",			BYTE_T,			_OFFSET(synEnable),		_SIZE(synEnable)},
{MIB_POD_PACK,				"POD_PACK",			WORD_T,			_OFFSET(podPack),		_SIZE(podPack)},
{MIB_POD_TIME,				"POD_TIME",			BYTE_T,			_OFFSET(podTime),		_SIZE(podTime)},
{MIB_POD_BUR,				"POD_BUR",			WORD_T,			_OFFSET(podBur),		_SIZE(podBur)},
{MIB_SYN_PACK,				"SYN_PACK",			WORD_T,			_OFFSET(synPack),		_SIZE(synPack)},
{MIB_SYN_TIME,				"SYN_TIME",			BYTE_T,			_OFFSET(synTime),		_SIZE(synTime)},
{MIB_SYN_BUR,				"SYN_BUR",			WORD_T,			_OFFSET(synBur),		_SIZE(synBur)},
{MIB_SCAN_NUM,				"SCAN_NUM",			INDEXDOS_T,		_OFFSET(scanNum),		_SIZE(scanNum)},
{MIB_DDNS_NAME,				"DDNS_NAME",			STRING_T,		_OFFSET(ddnsName),		_SIZE(ddnsName)},
{MIB_DDNS_ACCOUNT,			"DDNS_ACCOUNT",			STRING_T,		_OFFSET(ddnsAccount),		_SIZE(ddnsAccount)},
{MIB_DDNS_PASS,				"DDNS_PASS",			STRING_T,		_OFFSET(ddnsPass),		_SIZE(ddnsPass)},
{MIB_DDNS_ENABLE,			"DDNS_ENABLED",			BYTE_T,			_OFFSET(ddnsEnable),		_SIZE(ddnsEnable)},
{MIB_DDNS_PVIDSEL,			"DDNS_PVID_SEL",		STRING_T,		_OFFSET(ddnspvidSel),		_SIZE(ddnspvidSel)},
{MIB_APP_LAYER_GATEWAY,			"APP_LAYER_GATEWAY",		INDEX_T,		_OFFSET(appLayerGateway),	_SIZE(appLayerGateway)},
{MIB_DHIS_HOSTID,			"DHIS_HOSTID",			DWORD_T,		_OFFSET(dhisHostID),		_SIZE(dhisHostID)},
{MIB_DHIS_ISADDR,			"DHIS_ISADDR",			STRING_T,		_OFFSET(dhisISAddr),		_SIZE(dhisISAddr)},
{MIB_DHIS_PASSWORD,			"DHIS_PASSWORD",		STRING_T,		_OFFSET(dhispass),		_SIZE(dhispass)},
{MIB_DHIS_AUTH_P1,			"DHIS_AUTH_P1",			STRING_T,		_OFFSET(dhisAuthP1),		_SIZE(dhisAuthP1)},
{MIB_DHIS_AUTH_P2,			"DHIS_AUTH_P2",			STRING_T,		_OFFSET(dhisAuthP2),		_SIZE(dhisAuthP2)},
{MIB_DHIS_AUTH_Q1,			"DHIS_AUTH_Q1",			STRING_T,		_OFFSET(dhisAuthQ1),		_SIZE(dhisAuthQ1)},
{MIB_DHIS_AUTH_Q2,			"DHIS_AUTH_Q2",			STRING_T,		_OFFSET(dhisAuthQ2),		_SIZE(dhisAuthQ2)},
{MIB_DHIS_SELECT,			"DHIS_SELECT",			BYTE_T,			_OFFSET(dhisSelect),		_SIZE(dhisSelect)},
#endif

#ifdef HOME_GATEWAY  // For Guarantee QoS
{MIB_MAX_DOWNLOAD_BANDWIDTH,		"MAX_DOWNLOAD_BANDWIDTH",	DWORD_T,		_OFFSET(MaxDownBandwidth),	_SIZE(MaxDownBandwidth)},
{MIB_MAX_UPLOAD_BANDWIDTH,		"MAX_UPLOAD_BANDWIDTH",		DWORD_T,		_OFFSET(MaxUpBandwidth),	_SIZE(MaxUpBandwidth)},
#endif

#ifdef UNIVERSAL_REPEATER
{MIB_REPEATER_ENABLED,			"REPEATER_ENABLED",		BYTE_T,			_OFFSET(repeaterEnabled),	_SIZE(repeaterEnabled)},
{MIB_REPEATER_SSID,			"REPEATER_SSID",		STRING_T,		_OFFSET(repeaterSSID),		_SIZE(repeaterSSID)},
#endif

//------------------------------------------------------------------------------------------------
// Lance 2004.2.16
{MIB_PS_ENABLE,				"PS_ENABLE",			BYTE_T,			_OFFSET(psEnable),		_SIZE(psEnable)},
{MIB_PS_IPPENABLE,			"PS_IPPENABLE",			BYTE_T,			_OFFSET(psIPPEnable),		_SIZE(psIPPEnable)},
{MIB_PS_LPRENABLE,			"PS_LPRENABLE",			BYTE_T,			_OFFSET(psLPREnable),		_SIZE(psLPREnable)},
{MIB_PS_NAME,				"PS_NAME",			STRING_T,		_OFFSET(psName),		_SIZE(psName)},
{MIB_PS_PORT1NAME,			"PS_PORT1NAME",			STRING_T,		_OFFSET(psPort1Name),		_SIZE(psPort1Name)},
{MIB_PS_PORT2NAME,			"PS_PORT2NAME",			STRING_T,		_OFFSET(psPort2Name),		_SIZE(psPort2Name)},
#ifdef HOME_GATEWAY
{MIB_AP_ROUTER_SWITCH,			"AP_ROUTER_SWITCH",		BYTE_T,			_OFFSET(apRouterSwitch),	_SIZE(apRouterSwitch)},
#endif
// kyle 2007.03.12 for WPS---------------------------------------------------------------------------
{MIB_WPS_ENABLE,			"WPS_ENABLE",			BYTE_T,			_OFFSET(wpsEnable),		_SIZE(wpsEnable)},
{MIB_WPS_CONFIG_MODE,			"WPS_CONFIG_MODE",		BYTE_T,			_OFFSET(wpsConfigMode),		_SIZE(wpsConfigMode)},
{MIB_WPS_PROXY_ENABLE,			"WPS_PROXY_ENABLE",		BYTE_T,			_OFFSET(wpsProxyEnable),	_SIZE(wpsProxyEnable)},
{MIB_WPS_INTERNAL_REG,			"WPS_INTERNAL_REG",		BYTE_T,			_OFFSET(wpsInternalReg),	_SIZE(wpsInternalReg)},
{MIB_WPS_CONFIG_TYPE,			"WPS_CONFIG_TYPE",		BYTE_T,			_OFFSET(wpsConfigType),		_SIZE(wpsConfigType)},
{MIB_WPS_CONFIG_STATUS,			"WPS_CONFIG_STATUS",		BYTE_T,			_OFFSET(wpsConfigStatus),	_SIZE(wpsConfigStatus)},
{MIB_WPS_PING_CODE,			"WPS_PING_CODE",		STRING_T,		_OFFSET(wpsPingCode),		_SIZE(wpsPingCode)},
{MIB_WPS_DISPLAY_KEY,			"WPS_DISPLAY_KEY",		BYTE_T,			_OFFSET(wpsDisplayKey),		_SIZE(wpsDisplayKey)},
//kyle add for noForwarding
#if defined(HOME_GATEWAY) || defined(_AP_WLAN_NOFORWARD_)
{MIB_WLAN_NOFORWARD,			"WLAN_NOFORWARD",		BYTE_T,			_OFFSET(wlanForward),		_SIZE(wlanForward)},
#endif
{MIB_DDNS_STATUS,			"DDNS_STATUS",			BYTE_T,			_OFFSET(ddnsStatus),		_SIZE(ddnsStatus)},
{MIB_AUTO_BRIDGE,			"AUTO_BRIDGE",			BYTE_T,			_OFFSET(autoBridge),		_SIZE(autoBridge)},
{MIB_WIFI_TEST,				"WIFI_TEST",			BYTE_T,			_OFFSET(wifiTest),		_SIZE(wifiTest)},
{MIB_IS_RESET_DEFAULT,			"IS_RESET_DEFAULT",		BYTE_T,			_OFFSET(isResetDefault),	_SIZE(isResetDefault)},
{MIB_IGMP_PROXY_ENABLED,		"IGMP_PROXY_ENABLED",		BYTE_T,			_OFFSET(igmpproxyEnabled ),	_SIZE(igmpproxyEnabled )},
#ifdef _Customer_
{MIB_TIMEZONE_SELECTED,			"TIMEZONE_SELECTED",		BYTE_T,			_OFFSET(timezoneselected),	_SIZE(timezoneselected)},
#endif

//Wise
#ifdef _MSSID_
{MIB_WLAN_SSID_1,			"SSID_1",			STRING_T,		_OFFSET(ssid1),			_SIZE(ssid1)},
{MIB_WLAN_SSID_2,			"SSID_2",			STRING_T,		_OFFSET(ssid2),			_SIZE(ssid2)},
{MIB_WLAN_SSID_3,			"SSID_3",			STRING_T,		_OFFSET(ssid3),			_SIZE(ssid3)},
{MIB_WLAN_SSID_4,			"SSID_4",			STRING_T,		_OFFSET(ssid4),			_SIZE(ssid4)},
{MIB_WLAN_SSID_5,			"SSID_5",			STRING_T,		_OFFSET(ssid5),			_SIZE(ssid5)},
{MIB_WLAN_SSID_6,			"SSID_6",			STRING_T,		_OFFSET(ssid6),			_SIZE(ssid6)},
{MIB_WLAN_SSID_7,			"SSID_7",			STRING_T,		_OFFSET(ssid7),			_SIZE(ssid7)},
{MIB_WLAN_WEP64_KEY1_1,			"WEP64_KEY1_1",			BYTE5_T,		_OFFSET(wep64Key11),		_SIZE(wep64Key11)},
{MIB_WLAN_WEP64_KEY2_1,			"WEP64_KEY2_1",			BYTE5_T,		_OFFSET(wep64Key21),		_SIZE(wep64Key21)},
{MIB_WLAN_WEP64_KEY3_1,			"WEP64_KEY3_1",			BYTE5_T,		_OFFSET(wep64Key31),		_SIZE(wep64Key31)},
{MIB_WLAN_WEP64_KEY4_1,			"WEP64_KEY4_1",			BYTE5_T,		_OFFSET(wep64Key41),		_SIZE(wep64Key41)},
{MIB_WLAN_WEP64_KEY1_2,			"WEP64_KEY1_2",			BYTE5_T,		_OFFSET(wep64Key12),		_SIZE(wep64Key12)},
{MIB_WLAN_WEP64_KEY2_2,			"WEP64_KEY2_2",			BYTE5_T,		_OFFSET(wep64Key22),		_SIZE(wep64Key22)},
{MIB_WLAN_WEP64_KEY3_2,			"WEP64_KEY3_2",			BYTE5_T,		_OFFSET(wep64Key32),		_SIZE(wep64Key32)},
{MIB_WLAN_WEP64_KEY4_2,			"WEP64_KEY4_2",			BYTE5_T,		_OFFSET(wep64Key42),		_SIZE(wep64Key42)},
{MIB_WLAN_WEP64_KEY1_3,			"WEP64_KEY1_3",			BYTE5_T,		_OFFSET(wep64Key13),		_SIZE(wep64Key13)},
{MIB_WLAN_WEP64_KEY2_3,			"WEP64_KEY2_3",			BYTE5_T,		_OFFSET(wep64Key23),		_SIZE(wep64Key23)},
{MIB_WLAN_WEP64_KEY3_3,			"WEP64_KEY3_3",			BYTE5_T,		_OFFSET(wep64Key33),		_SIZE(wep64Key33)},
{MIB_WLAN_WEP64_KEY4_3,			"WEP64_KEY4_3",			BYTE5_T,		_OFFSET(wep64Key43),		_SIZE(wep64Key43)},
{MIB_WLAN_WEP64_KEY1_4,			"WEP64_KEY1_4",			BYTE5_T,		_OFFSET(wep64Key14),		_SIZE(wep64Key14)},
{MIB_WLAN_WEP64_KEY2_4,			"WEP64_KEY2_4",			BYTE5_T,		_OFFSET(wep64Key24),		_SIZE(wep64Key24)},
{MIB_WLAN_WEP64_KEY3_4,			"WEP64_KEY3_4",			BYTE5_T,		_OFFSET(wep64Key34),		_SIZE(wep64Key34)},
{MIB_WLAN_WEP64_KEY4_4,			"WEP64_KEY4_4",			BYTE5_T,		_OFFSET(wep64Key44),		_SIZE(wep64Key44)},
{MIB_WLAN_WEP64_KEY1_5,			"WEP64_KEY1_5",			BYTE5_T,		_OFFSET(wep64Key15),		_SIZE(wep64Key15)},
{MIB_WLAN_WEP64_KEY2_5,			"WEP64_KEY2_5",			BYTE5_T,		_OFFSET(wep64Key25),		_SIZE(wep64Key25)},
{MIB_WLAN_WEP64_KEY3_5,			"WEP64_KEY3_5",			BYTE5_T,		_OFFSET(wep64Key35),		_SIZE(wep64Key35)},
{MIB_WLAN_WEP64_KEY4_5,			"WEP64_KEY4_5",			BYTE5_T,		_OFFSET(wep64Key45),		_SIZE(wep64Key45)},
{MIB_WLAN_WEP64_KEY1_6,			"WEP64_KEY1_6",			BYTE5_T,		_OFFSET(wep64Key16),		_SIZE(wep64Key16)},
{MIB_WLAN_WEP64_KEY2_6,			"WEP64_KEY2_6",			BYTE5_T,		_OFFSET(wep64Key26),		_SIZE(wep64Key26)},
{MIB_WLAN_WEP64_KEY3_6,			"WEP64_KEY3_6",			BYTE5_T,		_OFFSET(wep64Key36),		_SIZE(wep64Key36)},
{MIB_WLAN_WEP64_KEY4_6,			"WEP64_KEY4_6",			BYTE5_T,		_OFFSET(wep64Key46),		_SIZE(wep64Key46)},
{MIB_WLAN_WEP64_KEY1_7,			"WEP64_KEY1_7",			BYTE5_T,		_OFFSET(wep64Key17),		_SIZE(wep64Key17)},
{MIB_WLAN_WEP64_KEY2_7,			"WEP64_KEY2_7",			BYTE5_T,		_OFFSET(wep64Key27),		_SIZE(wep64Key27)},
{MIB_WLAN_WEP64_KEY3_7,			"WEP64_KEY3_7",			BYTE5_T,		_OFFSET(wep64Key37),		_SIZE(wep64Key37)},
{MIB_WLAN_WEP64_KEY4_7,			"WEP64_KEY4_7",			BYTE5_T,		_OFFSET(wep64Key47),		_SIZE(wep64Key47)},
{MIB_WLAN_WEP128_KEY1_1,		"WEP128_KEY1_1",		BYTE13_T,		_OFFSET(wep128Key11),		_SIZE(wep128Key11)},
{MIB_WLAN_WEP128_KEY2_1,		"WEP128_KEY2_1",		BYTE13_T,		_OFFSET(wep128Key21),		_SIZE(wep128Key21)},
{MIB_WLAN_WEP128_KEY3_1,		"WEP128_KEY3_1",		BYTE13_T,		_OFFSET(wep128Key31),		_SIZE(wep128Key31)},
{MIB_WLAN_WEP128_KEY4_1,		"WEP128_KEY4_1",		BYTE13_T,		_OFFSET(wep128Key41),		_SIZE(wep128Key41)},
{MIB_WLAN_WEP128_KEY1_2,		"WEP128_KEY1_2",		BYTE13_T,		_OFFSET(wep128Key12),		_SIZE(wep128Key12)},
{MIB_WLAN_WEP128_KEY2_2,		"WEP128_KEY2_2",		BYTE13_T,		_OFFSET(wep128Key22),		_SIZE(wep128Key22)},
{MIB_WLAN_WEP128_KEY3_2,		"WEP128_KEY3_2",		BYTE13_T,		_OFFSET(wep128Key32),		_SIZE(wep128Key32)},
{MIB_WLAN_WEP128_KEY4_2,		"WEP128_KEY4_2",		BYTE13_T,		_OFFSET(wep128Key42),		_SIZE(wep128Key42)},
{MIB_WLAN_WEP128_KEY1_3,		"WEP128_KEY1_3",		BYTE13_T,		_OFFSET(wep128Key13),		_SIZE(wep128Key13)},
{MIB_WLAN_WEP128_KEY2_3,		"WEP128_KEY2_3",		BYTE13_T,		_OFFSET(wep128Key23),		_SIZE(wep128Key23)},
{MIB_WLAN_WEP128_KEY3_3,		"WEP128_KEY3_3",		BYTE13_T,		_OFFSET(wep128Key33),		_SIZE(wep128Key33)},
{MIB_WLAN_WEP128_KEY4_3,		"WEP128_KEY4_3",		BYTE13_T,		_OFFSET(wep128Key43),		_SIZE(wep128Key43)},
{MIB_WLAN_WEP128_KEY1_4,		"WEP128_KEY1_4",		BYTE13_T,		_OFFSET(wep128Key14),		_SIZE(wep128Key14)},
{MIB_WLAN_WEP128_KEY2_4,		"WEP128_KEY2_4",		BYTE13_T,		_OFFSET(wep128Key24),		_SIZE(wep128Key24)},
{MIB_WLAN_WEP128_KEY3_4,		"WEP128_KEY3_4",		BYTE13_T,		_OFFSET(wep128Key34),		_SIZE(wep128Key34)},
{MIB_WLAN_WEP128_KEY4_4,		"WEP128_KEY4_4",		BYTE13_T,		_OFFSET(wep128Key44),		_SIZE(wep128Key44)},
{MIB_WLAN_WEP128_KEY1_5,		"WEP128_KEY1_5",		BYTE13_T,		_OFFSET(wep128Key15),		_SIZE(wep128Key15)},
{MIB_WLAN_WEP128_KEY2_5,		"WEP128_KEY2_5",		BYTE13_T,		_OFFSET(wep128Key25),		_SIZE(wep128Key25)},
{MIB_WLAN_WEP128_KEY3_5,		"WEP128_KEY3_5",		BYTE13_T,		_OFFSET(wep128Key35),		_SIZE(wep128Key35)},
{MIB_WLAN_WEP128_KEY4_5,		"WEP128_KEY4_5",		BYTE13_T,		_OFFSET(wep128Key45),		_SIZE(wep128Key45)},
{MIB_WLAN_WEP128_KEY1_6,		"WEP128_KEY1_6",		BYTE13_T,		_OFFSET(wep128Key16),		_SIZE(wep128Key16)},
{MIB_WLAN_WEP128_KEY2_6,		"WEP128_KEY2_6",		BYTE13_T,		_OFFSET(wep128Key26),		_SIZE(wep128Key26)},
{MIB_WLAN_WEP128_KEY3_6,		"WEP128_KEY3_6",		BYTE13_T,		_OFFSET(wep128Key36),		_SIZE(wep128Key36)},
{MIB_WLAN_WEP128_KEY4_6,		"WEP128_KEY4_6",		BYTE13_T,		_OFFSET(wep128Key46),		_SIZE(wep128Key46)},
{MIB_WLAN_WEP128_KEY1_7,		"WEP128_KEY1_7",		BYTE13_T,		_OFFSET(wep128Key17),		_SIZE(wep128Key17)},
{MIB_WLAN_WEP128_KEY2_7,		"WEP128_KEY2_7",		BYTE13_T,		_OFFSET(wep128Key27),		_SIZE(wep128Key27)},
{MIB_WLAN_WEP128_KEY3_7,		"WEP128_KEY3_7",		BYTE13_T,		_OFFSET(wep128Key37),		_SIZE(wep128Key37)},
{MIB_WLAN_WEP128_KEY4_7,		"WEP128_KEY4_7",		BYTE13_T,		_OFFSET(wep128Key47),		_SIZE(wep128Key47)},
{MIB_WLAN_WPA_PSK_1,			"WLAN_WPA_PSK_1",		STRING_T,		_OFFSET(wpaPSK1),		_SIZE(wpaPSK1)},
{MIB_WLAN_WPA_PSK_2,			"WLAN_WPA_PSK_2",		STRING_T,		_OFFSET(wpaPSK2),		_SIZE(wpaPSK2)},
{MIB_WLAN_WPA_PSK_3,			"WLAN_WPA_PSK_3",		STRING_T,		_OFFSET(wpaPSK3),		_SIZE(wpaPSK3)},
{MIB_WLAN_WPA_PSK_4,			"WLAN_WPA_PSK_4",		STRING_T,		_OFFSET(wpaPSK4),		_SIZE(wpaPSK4)},
{MIB_WLAN_WPA_PSK_5,			"WLAN_WPA_PSK_5",		STRING_T,		_OFFSET(wpaPSK5),		_SIZE(wpaPSK5)},
{MIB_WLAN_WPA_PSK_6,			"WLAN_WPA_PSK_6",		STRING_T,		_OFFSET(wpaPSK6),		_SIZE(wpaPSK6)},
{MIB_WLAN_WPA_PSK_7,			"WLAN_WPA_PSK_7",		STRING_T,		_OFFSET(wpaPSK7),		_SIZE(wpaPSK7)},
{MIB_SECURITY_MODE_1,			"SECURITY_MODE_1",		BYTE_T,			_OFFSET(secMode1),		_SIZE(secMode1)},
{MIB_SECURITY_MODE_2,			"SECURITY_MODE_2",		BYTE_T,			_OFFSET(secMode2),		_SIZE(secMode2)},
{MIB_SECURITY_MODE_3,			"SECURITY_MODE_3",		BYTE_T,			_OFFSET(secMode3),		_SIZE(secMode3)},
{MIB_SECURITY_MODE_4,			"SECURITY_MODE_4",		BYTE_T,			_OFFSET(secMode4),		_SIZE(secMode4)},
{MIB_SECURITY_MODE_5,			"SECURITY_MODE_5",		BYTE_T,			_OFFSET(secMode5),		_SIZE(secMode5)},
{MIB_SECURITY_MODE_6,			"SECURITY_MODE_6",		BYTE_T,			_OFFSET(secMode6),		_SIZE(secMode6)},
{MIB_SECURITY_MODE_7,			"SECURITY_MODE_7",		BYTE_T,			_OFFSET(secMode7),		_SIZE(secMode7)},
{MIB_WLAN_ENABLE_1X_1,			"WLAN_ENABLE_1X_1",		BYTE_T,			_OFFSET(enable1X1),		_SIZE(enable1X1)},
{MIB_WLAN_ENABLE_1X_2,			"WLAN_ENABLE_1X_2",		BYTE_T,			_OFFSET(enable1X2),		_SIZE(enable1X2)},
{MIB_WLAN_ENABLE_1X_3,			"WLAN_ENABLE_1X_3",		BYTE_T,			_OFFSET(enable1X3),		_SIZE(enable1X3)},
{MIB_WLAN_ENABLE_1X_4,			"WLAN_ENABLE_1X_4",		BYTE_T,			_OFFSET(enable1X4),		_SIZE(enable1X4)},
{MIB_WLAN_ENABLE_1X_5,			"WLAN_ENABLE_1X_5",		BYTE_T,			_OFFSET(enable1X5),		_SIZE(enable1X5)},
{MIB_WLAN_ENABLE_1X_6,			"WLAN_ENABLE_1X_6",		BYTE_T,			_OFFSET(enable1X6),		_SIZE(enable1X6)},
{MIB_WLAN_ENABLE_1X_7,			"WLAN_ENABLE_1X_7",		BYTE_T,			_OFFSET(enable1X7),		_SIZE(enable1X7)},
{MIB_WLAN_WEP_1,			"WEP_1",			BYTE_T,			_OFFSET(wep1),			_SIZE(wep1)},
{MIB_WLAN_WEP_2,			"WEP_2",			BYTE_T,			_OFFSET(wep2),			_SIZE(wep2)},
{MIB_WLAN_WEP_3,			"WEP_3",			BYTE_T,			_OFFSET(wep3),			_SIZE(wep3)},
{MIB_WLAN_WEP_4,			"WEP_4",			BYTE_T,			_OFFSET(wep4),			_SIZE(wep4)},
{MIB_WLAN_WEP_5,			"WEP_5",			BYTE_T,			_OFFSET(wep5),			_SIZE(wep5)},
{MIB_WLAN_WEP_6,			"WEP_6",			BYTE_T,			_OFFSET(wep6),			_SIZE(wep6)},
{MIB_WLAN_WEP_7,			"WEP_7",			BYTE_T,			_OFFSET(wep7),			_SIZE(wep7)},
{MIB_WLAN_WEP_KEY_TYPE_1,		"WEP_KEY_TYPE_1",		BYTE_T,			_OFFSET(wepKeyType1),		_SIZE(wepKeyType1)},
{MIB_WLAN_WEP_KEY_TYPE_2,		"WEP_KEY_TYPE_2",		BYTE_T,			_OFFSET(wepKeyType2),		_SIZE(wepKeyType2)},
{MIB_WLAN_WEP_KEY_TYPE_3,		"WEP_KEY_TYPE_3",		BYTE_T,			_OFFSET(wepKeyType3),		_SIZE(wepKeyType3)},
{MIB_WLAN_WEP_KEY_TYPE_4,		"WEP_KEY_TYPE_4",		BYTE_T,			_OFFSET(wepKeyType4),		_SIZE(wepKeyType4)},
{MIB_WLAN_WEP_KEY_TYPE_5,		"WEP_KEY_TYPE_5",		BYTE_T,			_OFFSET(wepKeyType5),		_SIZE(wepKeyType5)},
{MIB_WLAN_WEP_KEY_TYPE_6,		"WEP_KEY_TYPE_6",		BYTE_T,			_OFFSET(wepKeyType6),		_SIZE(wepKeyType6)},
{MIB_WLAN_WEP_KEY_TYPE_7,		"WEP_KEY_TYPE_7",		BYTE_T,			_OFFSET(wepKeyType7),		_SIZE(wepKeyType7)},
{MIB_WLAN_WPA_CIPHER_SUITE_1,		"WLAN_WPA_CIPHER_SUITE_1",	BYTE_T,			_OFFSET(wpaCipher1),		_SIZE(wpaCipher1)},
{MIB_WLAN_WPA_CIPHER_SUITE_2,		"WLAN_WPA_CIPHER_SUITE_2",	BYTE_T,			_OFFSET(wpaCipher2),		_SIZE(wpaCipher2)},
{MIB_WLAN_WPA_CIPHER_SUITE_3,		"WLAN_WPA_CIPHER_SUITE_3",	BYTE_T,			_OFFSET(wpaCipher3),		_SIZE(wpaCipher3)},
{MIB_WLAN_WPA_CIPHER_SUITE_4,		"WLAN_WPA_CIPHER_SUITE_4",	BYTE_T,			_OFFSET(wpaCipher4),		_SIZE(wpaCipher4)},
{MIB_WLAN_WPA_CIPHER_SUITE_5,		"WLAN_WPA_CIPHER_SUITE_5",	BYTE_T,			_OFFSET(wpaCipher5),		_SIZE(wpaCipher5)},
{MIB_WLAN_WPA_CIPHER_SUITE_6,		"WLAN_WPA_CIPHER_SUITE_6",	BYTE_T,			_OFFSET(wpaCipher6),		_SIZE(wpaCipher6)},
{MIB_WLAN_WPA_CIPHER_SUITE_7,		"WLAN_WPA_CIPHER_SUITE_7",	BYTE_T,			_OFFSET(wpaCipher7),		_SIZE(wpaCipher7)},
{MIB_WLAN_WPA2_CIPHER_SUITE_1,		"WPA2_CIPHER_SUITE_1",		BYTE_T,			_OFFSET(wpa2Cipher1),		_SIZE(wpa2Cipher1)},
{MIB_WLAN_WPA2_CIPHER_SUITE_2,		"WPA2_CIPHER_SUITE_2",		BYTE_T,			_OFFSET(wpa2Cipher2),		_SIZE(wpa2Cipher2)},
{MIB_WLAN_WPA2_CIPHER_SUITE_3,		"WPA2_CIPHER_SUITE_3",		BYTE_T,			_OFFSET(wpa2Cipher3),		_SIZE(wpa2Cipher3)},
{MIB_WLAN_WPA2_CIPHER_SUITE_4,		"WPA2_CIPHER_SUITE_4",		BYTE_T,			_OFFSET(wpa2Cipher4),		_SIZE(wpa2Cipher4)},
{MIB_WLAN_WPA2_CIPHER_SUITE_5,		"WPA2_CIPHER_SUITE_5",		BYTE_T,			_OFFSET(wpa2Cipher5),		_SIZE(wpa2Cipher5)},
{MIB_WLAN_WPA2_CIPHER_SUITE_6,		"WPA2_CIPHER_SUITE_6",		BYTE_T,			_OFFSET(wpa2Cipher6),		_SIZE(wpa2Cipher6)},
{MIB_WLAN_WPA2_CIPHER_SUITE_7,		"WPA2_CIPHER_SUITE_7",		BYTE_T,			_OFFSET(wpa2Cipher7),		_SIZE(wpa2Cipher7)},
{MIB_WLAN_WPA_PSK_FORMAT_1,		"WLAN_PSK_FORMAT_1",		BYTE_T,			_OFFSET(wpaPSKFormat1),		_SIZE(wpaPSKFormat1)},
{MIB_WLAN_WPA_PSK_FORMAT_2,		"WLAN_PSK_FORMAT_2",		BYTE_T,			_OFFSET(wpaPSKFormat2),		_SIZE(wpaPSKFormat2)},
{MIB_WLAN_WPA_PSK_FORMAT_3,		"WLAN_PSK_FORMAT_3",		BYTE_T,			_OFFSET(wpaPSKFormat3),		_SIZE(wpaPSKFormat3)},
{MIB_WLAN_WPA_PSK_FORMAT_4,		"WLAN_PSK_FORMAT_4",		BYTE_T,			_OFFSET(wpaPSKFormat4),		_SIZE(wpaPSKFormat4)},
{MIB_WLAN_WPA_PSK_FORMAT_5,		"WLAN_PSK_FORMAT_5",		BYTE_T,			_OFFSET(wpaPSKFormat5),		_SIZE(wpaPSKFormat5)},
{MIB_WLAN_WPA_PSK_FORMAT_6,		"WLAN_PSK_FORMAT_6",		BYTE_T,			_OFFSET(wpaPSKFormat6),		_SIZE(wpaPSKFormat6)},
{MIB_WLAN_WPA_PSK_FORMAT_7,		"WLAN_PSK_FORMAT_7",		BYTE_T,			_OFFSET(wpaPSKFormat7),		_SIZE(wpaPSKFormat7)},
{MIB_WLAN_WEP_DEFAULT_KEY_1,		"WEP_DEFAULT_KEY_1",		BYTE_T,			_OFFSET(wepDefaultKey1),	_SIZE(wepDefaultKey1)},
{MIB_WLAN_WEP_DEFAULT_KEY_2,		"WEP_DEFAULT_KEY_2",		BYTE_T,			_OFFSET(wepDefaultKey2),	_SIZE(wepDefaultKey2)},
{MIB_WLAN_WEP_DEFAULT_KEY_3,		"WEP_DEFAULT_KEY_3",		BYTE_T,			_OFFSET(wepDefaultKey3),	_SIZE(wepDefaultKey3)},
{MIB_WLAN_WEP_DEFAULT_KEY_4,		"WEP_DEFAULT_KEY_4",		BYTE_T,			_OFFSET(wepDefaultKey4),	_SIZE(wepDefaultKey4)},
{MIB_WLAN_WEP_DEFAULT_KEY_5,		"WEP_DEFAULT_KEY_5",		BYTE_T,			_OFFSET(wepDefaultKey5),	_SIZE(wepDefaultKey5)},
{MIB_WLAN_WEP_DEFAULT_KEY_6,		"WEP_DEFAULT_KEY_6",		BYTE_T,			_OFFSET(wepDefaultKey6),	_SIZE(wepDefaultKey6)},
{MIB_WLAN_WEP_DEFAULT_KEY_7,		"WEP_DEFAULT_KEY_7",		BYTE_T,			_OFFSET(wepDefaultKey7),	_SIZE(wepDefaultKey7)},
{MIB_WLAN_SSID_MIRROR_1,		"SSID_MIRROR_1",		BYTE_T,			_OFFSET(ssidMirror1),		_SIZE(ssidMirror1)},
{MIB_WLAN_SSID_MIRROR_2,		"SSID_MIRROR_2",		BYTE_T,			_OFFSET(ssidMirror2),		_SIZE(ssidMirror2)},
{MIB_WLAN_SSID_MIRROR_3,		"SSID_MIRROR_3",		BYTE_T,			_OFFSET(ssidMirror3),		_SIZE(ssidMirror3)},
{MIB_WLAN_SSID_MIRROR_4,		"SSID_MIRROR_4",		BYTE_T,			_OFFSET(ssidMirror4),		_SIZE(ssidMirror4)},
{MIB_WLAN_SSID_MIRROR_5,		"SSID_MIRROR_5",		BYTE_T,			_OFFSET(ssidMirror5),		_SIZE(ssidMirror5)},
{MIB_WLAN_SSID_MIRROR_6,		"SSID_MIRROR_6",		BYTE_T,			_OFFSET(ssidMirror6),		_SIZE(ssidMirror6)},
{MIB_WLAN_SSID_MIRROR_7,		"SSID_MIRROR_7",		BYTE_T,			_OFFSET(ssidMirror7),		_SIZE(ssidMirror7)},
{MIB_WLAN_FIX_RATE_1,			"FIX_RATE_1",			WORD_T,			_OFFSET(fixedTxRate1),		_SIZE(fixedTxRate1)},
{MIB_WLAN_FIX_RATE_2,			"FIX_RATE_2",			WORD_T,			_OFFSET(fixedTxRate2),		_SIZE(fixedTxRate2)},
{MIB_WLAN_FIX_RATE_3,			"FIX_RATE_3",			WORD_T,			_OFFSET(fixedTxRate3),		_SIZE(fixedTxRate3)},
{MIB_WLAN_FIX_RATE_4,			"FIX_RATE_4",			WORD_T,			_OFFSET(fixedTxRate4),		_SIZE(fixedTxRate4)},
{MIB_WLAN_FIX_RATE_5,			"FIX_RATE_5",			WORD_T,			_OFFSET(fixedTxRate5),		_SIZE(fixedTxRate5)},
{MIB_WLAN_FIX_RATE_6,			"FIX_RATE_6",			WORD_T,			_OFFSET(fixedTxRate6),		_SIZE(fixedTxRate6)},
{MIB_WLAN_FIX_RATE_7,			"FIX_RATE_7",			WORD_T,			_OFFSET(fixedTxRate7),		_SIZE(fixedTxRate7)},
{MIB_WLAN_HIDDEN_SSID_1,		"HIDDEN_SSID_1",		BYTE_T,			_OFFSET(hiddenSSID1),		_SIZE(hiddenSSID1)},
{MIB_WLAN_HIDDEN_SSID_2,		"HIDDEN_SSID_2",		BYTE_T,			_OFFSET(hiddenSSID2),		_SIZE(hiddenSSID2)},
{MIB_WLAN_HIDDEN_SSID_3,		"HIDDEN_SSID_3",		BYTE_T,			_OFFSET(hiddenSSID3),		_SIZE(hiddenSSID3)},
{MIB_WLAN_HIDDEN_SSID_4,		"HIDDEN_SSID_4",		BYTE_T,			_OFFSET(hiddenSSID4),		_SIZE(hiddenSSID4)},
{MIB_WLAN_HIDDEN_SSID_5,		"HIDDEN_SSID_5",		BYTE_T,			_OFFSET(hiddenSSID5),		_SIZE(hiddenSSID5)},
{MIB_WLAN_HIDDEN_SSID_6,		"HIDDEN_SSID_6",		BYTE_T,			_OFFSET(hiddenSSID6),		_SIZE(hiddenSSID6)},
{MIB_WLAN_HIDDEN_SSID_7,		"HIDDEN_SSID_7",		BYTE_T,			_OFFSET(hiddenSSID7),		_SIZE(hiddenSSID7)},
{MIB_WLAN_WMM_1,			"WLAN_WMM_1",			BYTE_T,			_OFFSET(wlanWmm1),		_SIZE(wlanWmm1)},
{MIB_WLAN_WMM_2,			"WLAN_WMM_2",			BYTE_T,			_OFFSET(wlanWmm2),		_SIZE(wlanWmm2)},
{MIB_WLAN_WMM_3,			"WLAN_WMM_3",			BYTE_T,			_OFFSET(wlanWmm3),		_SIZE(wlanWmm3)},
{MIB_WLAN_WMM_4,			"WLAN_WMM_4",			BYTE_T,			_OFFSET(wlanWmm4),		_SIZE(wlanWmm4)},
{MIB_WLAN_WMM_5,			"WLAN_WMM_5",			BYTE_T,			_OFFSET(wlanWmm5),		_SIZE(wlanWmm5)},
{MIB_WLAN_WMM_6,			"WLAN_WMM_6",			BYTE_T,			_OFFSET(wlanWmm6),		_SIZE(wlanWmm6)},
{MIB_WLAN_WMM_7,			"WLAN_WMM_7",			BYTE_T,			_OFFSET(wlanWmm7),		_SIZE(wlanWmm7)},
{MIB_WLAN_ENCRYPT_1,			"WLAN_ENCRYPT_1",		BYTE_T,			_OFFSET(encrypt1),		_SIZE(encrypt1)},
{MIB_WLAN_ENCRYPT_2,			"WLAN_ENCRYPT_2",		BYTE_T,			_OFFSET(encrypt2),		_SIZE(encrypt2)},
{MIB_WLAN_ENCRYPT_3,			"WLAN_ENCRYPT_3",		BYTE_T,			_OFFSET(encrypt3),		_SIZE(encrypt3)},
{MIB_WLAN_ENCRYPT_4,			"WLAN_ENCRYPT_4",		BYTE_T,			_OFFSET(encrypt4),		_SIZE(encrypt4)},
{MIB_WLAN_ENCRYPT_5,			"WLAN_ENCRYPT_5",		BYTE_T,			_OFFSET(encrypt5),		_SIZE(encrypt5)},
{MIB_WLAN_ENCRYPT_6,			"WLAN_ENCRYPT_6",		BYTE_T,			_OFFSET(encrypt6),		_SIZE(encrypt6)},
{MIB_WLAN_ENCRYPT_7,			"WLAN_ENCRYPT_7",		BYTE_T,			_OFFSET(encrypt7),		_SIZE(encrypt7)},

#if defined(_ENVLAN_)
{MIB_WLAN_VLANID_1,			"VLANID_1",			WORD_T,			_OFFSET(vlanid1),		_SIZE(vlanid1)},
{MIB_WLAN_VLANID_2,			"VLANID_2",			WORD_T,			_OFFSET(vlanid2),		_SIZE(vlanid2)},
{MIB_WLAN_VLANID_3,			"VLANID_3",			WORD_T,			_OFFSET(vlanid3),		_SIZE(vlanid3)},
{MIB_WLAN_VLANID_4,			"VLANID_4",			WORD_T,			_OFFSET(vlanid4),		_SIZE(vlanid4)},
{MIB_WLAN_VLANID_5,			"VLANID_5",			WORD_T,			_OFFSET(vlanid5),		_SIZE(vlanid5)},
{MIB_WLAN_VLANID_6,			"VLANID_6",			WORD_T,			_OFFSET(vlanid6),		_SIZE(vlanid6)},
{MIB_WLAN_VLANID_7,			"VLANID_7",			WORD_T,			_OFFSET(vlanid7),		_SIZE(vlanid7)},
#endif
#endif

#ifdef _DALGFTP_
{MIB_ALG_FTP_PORTS,			"ALG_FTP_PORTS",		WORD_T,			_OFFSET(ftpPorts),		_SIZE(ftpPorts)},
#endif

#ifdef _DBAND_
{MIB_DUAL_BAND_SWITCH,			"DUAL_BAND_SWITCH",		BYTE_T,			_OFFSET(dualBandSwitch),	_SIZE(dualBandSwitch)},
{MIB_ABAND_MODE,			"ABAND_MODE",			BYTE_T,			_OFFSET(aBandMode),		_SIZE(aBandMode)},
{MIB_ABAND_CHANNEL,			"ABAND_CHANNEL",		BYTE_T,			_OFFSET(aBandChannel),		_SIZE(aBandChannel)},
#endif

#ifdef _LAN_WAN_ACCESS_
{MIB_LAN_WAN_ACCESS,			"LAN_WAN_ACCESS",		DWORD_T,		_OFFSET(enableLanWanAccess),	_SIZE(enableLanWanAccess)},
#endif

#ifdef _SREBOOT_BLINK_POWER_
{MID_IS_REBOOT,				"IS_REBOOT",			BYTE_T,			_OFFSET(isReboot),		_SIZE(isReboot)},
#endif

#ifdef _WATCH_DOG_
{MIB_WATCH_DOG_ENABLE,				"WATCH_DOG_ENABLE",			BYTE_T,			_OFFSET(watchdog_enable),		_SIZE(watchdog_enable)},
{MIB_WATCH_DOG_INTERVAL,				"WATCH_DOG_INTERVAL",			WORD_T,			_OFFSET(watchdog_interval),		_SIZE(watchdog_interval)},
{MIB_WATCH_DOG_HOST,			"WATCH_DOG_HOST",			IA_T,			_OFFSET(watchdog_host),		_SIZE(watchdog_host)},
#endif

#if defined(_MULTIPLE_WLAN_ACCESS_CONTROL_ )
{MIB_WLAN_AC2_NUM,			"WLAN_MACAC2_NUM",		BYTE_T,			_OFFSET(acNum2),			_SIZE(acNum2)},
{MIB_WLAN_AC2_ENABLED,			"WLAN_MACAC2_ENABLED",		BYTE_T,			_OFFSET(acEnabled2),		_SIZE(acEnabled2)},
{MIB_WLAN_AC2_ADDR,			"WLAN_MACAC2_ADDR",		WLAC_ARRAY2_T,		_OFFSET(acAddrArray2),		_SIZE(acAddrArray2)},

{MIB_WLAN_AC3_NUM,			"WLAN_MACAC3_NUM",		BYTE_T,			_OFFSET(acNum3),			_SIZE(acNum3)},
{MIB_WLAN_AC3_ENABLED,			"WLAN_MACAC3_ENABLED",			BYTE_T,			_OFFSET(acEnabled3),		_SIZE(acEnabled3)},
{MIB_WLAN_AC3_ADDR,			"WLAN_MACAC3_ADDR",		WLAC_ARRAY3_T,		_OFFSET(acAddrArray3),		_SIZE(acAddrArray3)},

{MIB_WLAN_AC4_NUM,			"WLAN_MACAC4_NUM",		BYTE_T,			_OFFSET(acNum4),			_SIZE(acNum4)},
{MIB_WLAN_AC4_ENABLED,			"WLAN_MACAC4_ENABLED",		BYTE_T,			_OFFSET(acEnabled4),		_SIZE(acEnabled4)},
{MIB_WLAN_AC4_ADDR,			"WLAN_MACAC4_ADDR",		WLAC_ARRAY4_T,		_OFFSET(acAddrArray4),		_SIZE(acAddrArray4)},
#endif

#if defined(_WLAN_AC_SWITCH_)
{MIB_WLAN_AC_SWITCH,			"WLAN_AC_SWITCH",			BYTE_T,			_OFFSET(wlanacswitch),			_SIZE(wlanacswitch)},
{MIB_WLAN_AC1_SWITCH,		"WLAN_AC1_SWITCH",		BYTE_T,			_OFFSET(wlanac1switch),			_SIZE(wlanac1switch)},
{MIB_WLAN_AC2_SWITCH,		"WLAN_AC2_SWITCH",		BYTE_T,			_OFFSET(wlanac2switch),			_SIZE(wlanac2switch)},
{MIB_WLAN_AC3_SWITCH,		"WLAN_AC3_SWITCH",		BYTE_T,			_OFFSET(wlanac3switch),			_SIZE(wlanac3switch)},
#endif
#ifdef _WISP_
{MIB_STADRV_SSID,					"STADRV_SSID",				STRING_T,		_OFFSET(stadrv_ssid),			_SIZE(stadrv_ssid)},
{MIB_STADRV_CHAN_NUM,			"STADRV_CHANNEL",			BYTE_T,			_OFFSET(stadrv_channel),		_SIZE(stadrv_channel)},
{MIB_STADRV_BAND,					"STADRV_BAND",				BYTE_T,			_OFFSET(stadrv_band),			_SIZE(stadrv_band)},
{MIB_STADRV_ENCRYPT_TYPE,		"STADRV_ENCRYPT_TYPE",	BYTE_T,			_OFFSET(stadrv_encrypttype),	_SIZE(stadrv_encrypttype)},
{MIB_STADRV_WEP_LENGTH,			"STADRV_WEP_LENGTH",		BYTE_T,			_OFFSET(stadrv_weplength),		_SIZE(stadrv_weplength)},
{MIB_STADRV_WEP_FORMAT1,		"STADRV_WEP_FORMAT1",	BYTE_T,			_OFFSET(stadrv_wepformat1),	_SIZE(stadrv_wepformat1)},
{MIB_STADRV_WEP_FORMAT2,		"STADRV_WEP_FORMAT2",	BYTE_T,			_OFFSET(stadrv_wepformat2),	_SIZE(stadrv_wepformat2)},
{MIB_STADRV_WEP_FORMAT3,		"STADRV_WEP_FORMAT3",	BYTE_T,			_OFFSET(stadrv_wepformat3),	_SIZE(stadrv_wepformat3)},
{MIB_STADRV_WEP_FORMAT4,		"STADRV_WEP_FORMAT4",	BYTE_T,			_OFFSET(stadrv_wepformat4),	_SIZE(stadrv_wepformat4)},
{MIB_STADRV_WEP_DEFAULT_KEY,	"STADRV_WEP_DEFAULT_KEY",BYTE_T,			_OFFSET(stadrv_defaultkey),	_SIZE(stadrv_defaultkey)},
{MIB_STADRV_WPA_CIPHER,			"STADRV_WPA_CIPHER",		BYTE_T,			_OFFSET(stadrv_wpacipher),		_SIZE(stadrv_wpacipher)},
{MIB_STADRV_PSK_FORMAT,			"STADRV_PSK_FORMAT",		BYTE_T,			_OFFSET(stadrv_pskformat),		_SIZE(stadrv_pskformat)},
{MIB_STADRV_WEPKEY1,				"STADRV_WEPKEY1",			STRING_T,		_OFFSET(stadrv_wepkey1),		_SIZE(stadrv_wepkey1)},
{MIB_STADRV_WEPKEY2,				"STADRV_WEPKEY2",			STRING_T,		_OFFSET(stadrv_wepkey2),		_SIZE(stadrv_wepkey2)},
{MIB_STADRV_WEPKEY3,				"STADRV_WEPKEY3",			STRING_T,		_OFFSET(stadrv_wepkey3),		_SIZE(stadrv_wepkey3)},
{MIB_STADRV_WEPKEY4,				"STADRV_WEPKEY4",			STRING_T,		_OFFSET(stadrv_wepkey4),		_SIZE(stadrv_wepkey4)},
{MIB_STADRV_PSKKEY,				"STADRV_PSKKEY",			STRING_T,		_OFFSET(stadrv_pskkey),			_SIZE(stadrv_pskkey)},
{MIB_STADRV_CLONEMAC,			"STADRV_CLONEMAC",		BYTE6_T,			_OFFSET(stadrv_clonemac),		_SIZE(stadrv_clonemac)},
#endif
#ifdef _Unicorn_
{MIB_QOS_MODE,                       "QOS_MODE",             BYTE_T,        _OFFSET(qosmode),             _SIZE(qosmode)},
#endif
#ifdef _SDMZ_
{MIB_DMZ_MODE,                       "DMZ_MODE",             BYTE_T,        _OFFSET(dmzmode),           _SIZE(dmzmode)},
{MIB_DMZ_MAC_ADDR,		         "DMZ_MAC_ADDR",			     BYTE6_T,		 _OFFSET(dmzMacAddr),		   _SIZE(dmzMacAddr)},
{MIB_SUPERDMZ_REFRESHTIME,        "REFRESHTIME",       WORD_T,        _OFFSET(refreshtime),           _SIZE(refreshtime)},
#endif
#ifdef _LEDSWITCH_
{MIB_LED_CONTROL,                    "LED_CONTROL",                    BYTE_T,                 _OFFSET(ledContorl),              _SIZE(ledContorl)},
#endif
#ifdef _SNTP_A_M_CTL_ //vance 2009.02.20
{MIB_TIMESERVER_DOMAIN,			"TIMESERVER_DOMAIN",		STRING_T,			_OFFSET(TimeServerDomain),	_SIZE(TimeServerDomain)},
{MIB_TIMESERVER_ENABLE,			"TIMESERVER_ENABLE",		BYTE_T,		_OFFSET(TimeServerEnable),	_SIZE(TimeServerEnable)},
{MIB_STATIC_SYSTEM_TIME,		"STATIC_SYSTEM_TIME",	STRING_T,			_OFFSET(StaticSystemTime),	_SIZE(StaticSystemTime)},
#endif
#ifdef _DHCP_SERVER_WITH_GW_DNS_
{MIB_DHCP_SERVER_GW,				"DHCP_SERVER_GW",			IA_T,				_OFFSET(dhcp_server_gw),		_SIZE(dhcp_server_gw)},
{MIB_DHCP_SERVER_DNS1,				"DHCP_SERVER_DNS1",			IA_T,				_OFFSET(dhcp_server_dns1),		_SIZE(dhcp_server_dns1)},
{MIB_DHCP_SERVER_DNS2,				"DHCP_SERVER_DNS2",			IA_T,				_OFFSET(dhcp_server_dns2),		_SIZE(dhcp_server_dns2)},
#endif
#ifdef _CONVERT_IP_ADDR_
{MIB_CONVERT_IP_ADDR,                           "CONVERT_IP_ADDR",                      IA_T,                   _OFFSET(convertipAddr),                _SIZE(convertipAddr)},
#endif
#ifdef _DUALL_WAN_ACCESS_
{MIB_DUALL_ACCESS_ENABLE,  "DUALL_ACCESS_ENABLE",       BYTE_T,         _OFFSET(dual_access_enable),        _SIZE(dual_access_enable)},
{MIB_DUALL_ACCESS_MODE,    "DUALL_ACCESS_MODE",         BYTE_T,         _OFFSET(dual_access_mode),          _SIZE(dual_access_mode)},
#endif
#ifdef _TIMESERVER_ //def _TIME_ZONE_TYPE_
{MIB_TIME_TYPE,				"TIME_TYPE",			BYTE_T,				_OFFSET(time_type),		_SIZE(time_type)},
{MIB_NTPS_ADDR,				"NTPS_ADDR",			STRING_T,				_OFFSET(ntps_addr),		_SIZE(ntps_addr)},
{MIB_SERVER_SEL,			"SERVER_SEL",			WORD_T,			_OFFSET(ServerSel),	_SIZE(ServerSel)},
{MIB_SERVER_AUTO,			"SERVER_AUTO",			BYTE_T,			_OFFSET(ServerAuto),	_SIZE(ServerAuto)},
{MIB_NTPS_CONFIG,  "NTPS_CONFIG",	BYTE_T,	_OFFSET(ntpsconfig),		_SIZE(ntpsconfig)},
#endif
#ifdef _IPV6_DSLITE_SUPPORT_
{MIB_IPV6_CGN_IP,		"IPV6_CGN_IP",			IPV6_T,		_OFFSET(IPv6CGNIP),		_SIZE(IPv6CGNIP)},
#endif
#ifdef _IPV6_6RD_SUPPORT_
{MIB_IPV6_6RD_SERVER,		"IPV6_6RD_SERVER",		STRING_T,	_OFFSET(IPv66rdServer),		_SIZE(IPv66rdServer)},
{MIB_IPV6_6RD_PREFIX,		"IPV6_6RD_PREFIX",		STRING_T,	_OFFSET(IPv66rdPrefix),		_SIZE(IPv66rdPrefix)},
#endif

#ifdef _IP_SWITCH_
{MIB_IP_ADDR_AP,			"IP_ADDR_AP",		IA_T,			_OFFSET(ipAddrap),		_SIZE(ipAddrap)},
{MIB_IP_ADDR_ROUTER,		"IP_ADDR_ROUTER",	IA_T,			_OFFSET(ipAddrrouter),	_SIZE(ipAddrrouter)},
{MIB_DHCP_CLIENT_AP_START,			"DHCP_CLIENT_AP_START",			IA_T,			_OFFSET(dhcpClientapStart),	_SIZE(dhcpClientapStart)},
{MIB_DHCP_CLIENT_AP_END,			"DHCP_CLIENT_AP_END",			IA_T,			_OFFSET(dhcpClientapEnd),		_SIZE(dhcpClientapEnd)},
{MIB_DHCP_CLIENT_ROUTER_START,	"DHCP_CLIENT_ROUTER_START",	IA_T,			_OFFSET(dhcpClientrouterStart),	_SIZE(dhcpClientrouterStart)},
{MIB_DHCP_CLIENT_ROUTER_END,		"DHCP_CLIENT_ROUTER_END",		IA_T,			_OFFSET(dhcpClientrouterEnd),		_SIZE(dhcpClientrouterEnd)},
#endif

#if defined(_EZ_QOS_)
{MIB_EZ_QOS_ENABLED,			"EZ_QOS_ENABLED",		BYTE_T,			_OFFSET(EZQosEnabled),		_SIZE(EZQosEnabled)},
{MIB_EZ_QOS_NUM,			"EZ_QOS_TBL_NUM",		BYTE_T,			_OFFSET(EZQosNum),		_SIZE(EZQosNum)},
{MIB_EZ_QOS,				"EZ_QOS_TBL",			EZ_QOS_ARRAY_T,	_OFFSET(EZQosArray),		_SIZE(EZQosArray)},
{MIB_EZ_QOS_MODE,    "EZ_QOS_MODE",    BYTE_T,        _OFFSET(EZQosMode),     _SIZE(EZQosMode)},
//{MIB_EZ_QOS_TYPE,			"EZ_QOS_TYPE",		BYTE_T,			_OFFSET(EZQosType),		_SIZE(EZQosType)},
//{MIB_EZ_QOS_CONNECTIONS, "EZ_QOS_CONNECTIONS",   WORD_T,  _OFFSET(EZQosConnections),  _SIZE(EZQosConnections)},
#endif

#ifdef _AP_DHCP_SPACE_
{MIB_AP_DHCP,				"AP_DHCP",				BYTE_T,			_OFFSET(ap_dhcp),			_SIZE(ap_dhcp)},
#endif

#ifdef _IGMP_SNOOPING_ENABLED_
{MIB_IGMP_SNOOP_ENABLED,                "IGMP_SNOOP_ENABLED",           BYTE_T,                 _OFFSET(igmpsnoopEnable),                       _SIZE(igmpsnoopEnable)},
#endif
#ifdef _MCAST_RATE_SUPPORT_
{MIB_MCAST_RATE_ENABLED,		"MCAST_RATE_ENABLED",		BYTE_T,			_OFFSET(McastRateEnable),	_SIZE(McastRateEnable)},
{MIB_MCAST_RATE,			"MCAST_RATE",			BYTE_T,			_OFFSET(McastRate),		_SIZE(McastRate)},
#endif

#ifdef _DISABLE_WIRELESS_AP_
{MIB_DISABLE_WIRELESS_AP, "DISABLE_WIRELESS_AP", BYTE_T, _OFFSET(disable_wireless_ap), _SIZE(disable_wireless_ap)},
#endif

{MIB_INIC_TX_POWER,			"INIC_TX_POWER",		BYTE_T,			_OFFSET(iNIC_wlanTxPower),	_SIZE(iNIC_wlanTxPower)},
{MIB_INIC_WMM,				"INIC_WMM",			BYTE_T,			_OFFSET(iNIC_wlanWmm),		_SIZE(iNIC_wlanWmm)},
{MIB_INIC_TURBO,			"INIC_TURBO",			BYTE_T,			_OFFSET(iNIC_wlanTurbo),	_SIZE(iNIC_wlanTurbo)},
{MIB_INIC_N_FIX_RATE,			"INIC_N_FIXRTAE",		BYTE_T,			_OFFSET(iNIC_wlanNFixRate),	_SIZE(iNIC_wlanNFixRate)},
{MIB_INIC_N_CHANNEL_WIDTH,		"INIC_N_CHAN_WIDTH",		BYTE_T,			_OFFSET(iNIC_wlanNChanWidth),	_SIZE(iNIC_wlanNChanWidth)},

{MIB_INIC_TRAN_RATE,			"INIC_TRAN_RATE",               STRING_T,               _OFFSET(iNIC_wlanTranRate),     _SIZE(iNIC_wlanTranRate)},
{MIB_INIC_RATE_MODE,			"INIC_RATE_MODE",               BYTE_T,                 _OFFSET(iNIC_wlanRateMode),     _SIZE(iNIC_wlanRateMode)},
{MIB_INIC_CTS,				"INIC_CTS",                     BYTE_T,                 _OFFSET(iNIC_wlanCts),          _SIZE(iNIC_wlanCts)},
{MIB_INIC_BURST,			"INIC_BURST",                   BYTE_T,                 _OFFSET(iNIC_wlanBurst),        _SIZE(iNIC_wlanBurst)},
{MIB_INIC_1X_MODE,			"INIC_DOT1X_MODE",		BYTE_T,			_OFFSET(iNIC_sel1xMode),	_SIZE(iNIC_sel1xMode)},
{MIB_INIC_MAC_ADDR,			"INIC_MAC_ADDR",		BYTE6_T,		_OFFSET(iNIC_wlanMacAddr),		_SIZE(iNIC_wlanMacAddr)},
{MIB_INIC_SSID,				"INIC_SSID",			STRING_T,		_OFFSET(iNIC_ssid),		_SIZE(iNIC_ssid)},
{MIB_INIC_CHAN_NUM,			"INIC_CHANNEL",			BYTE_T,			_OFFSET(iNIC_channel),		_SIZE(iNIC_channel)},
{MIB_INIC_WEP,				"INIC_WEP",			BYTE_T,			_OFFSET(iNIC_wep),		_SIZE(iNIC_wep)},
{MIB_INIC_WEP64_KEY1,			"INIC_WEP64_KEY1",		BYTE5_T,		_OFFSET(iNIC_wep64Key1),	_SIZE(iNIC_wep64Key1)},
{MIB_INIC_WEP64_KEY2,			"INIC_WEP64_KEY2",		BYTE5_T,		_OFFSET(iNIC_wep64Key2),	_SIZE(iNIC_wep64Key2)},
{MIB_INIC_WEP64_KEY3,			"INIC_WEP64_KEY3",		BYTE5_T,		_OFFSET(iNIC_wep64Key3),	_SIZE(iNIC_wep64Key3)},
{MIB_INIC_WEP64_KEY4,			"INIC_WEP64_KEY4",		BYTE5_T,		_OFFSET(iNIC_wep64Key4),	_SIZE(iNIC_wep64Key4)},
{MIB_INIC_WEP128_KEY1,			"INIC_WEP128_KEY1",		BYTE13_T,		_OFFSET(iNIC_wep128Key1),	_SIZE(iNIC_wep128Key1)},
{MIB_INIC_WEP128_KEY2,			"INIC_WEP128_KEY2",		BYTE13_T,		_OFFSET(iNIC_wep128Key2),	_SIZE(iNIC_wep128Key2)},
{MIB_INIC_WEP128_KEY3,			"INIC_WEP128_KEY3",		BYTE13_T,		_OFFSET(iNIC_wep128Key3),	_SIZE(iNIC_wep128Key3)},
{MIB_INIC_WEP128_KEY4,			"INIC_WEP128_KEY4",		BYTE13_T,		_OFFSET(iNIC_wep128Key4),	_SIZE(iNIC_wep128Key4)},
{MIB_INIC_WEP_DEFAULT_KEY,		"INIC_WEP_DEFAULT_KEY",		BYTE_T,			_OFFSET(iNIC_wepDefaultKey),	_SIZE(iNIC_wepDefaultKey)},
{MIB_INIC_WEP_KEY_TYPE,			"INIC_WEP_KEY_TYPE",		BYTE_T,			_OFFSET(iNIC_wepKeyType),	_SIZE(iNIC_wepKeyType)},
{MIB_INIC_FRAG_THRESHOLD,		"INIC_FRAG_THRESHOLD",		WORD_T,			_OFFSET(iNIC_fragThreshold),		_SIZE(iNIC_fragThreshold)},
{MIB_INIC_SUPPORTED_RATE,		"INIC_SUPPORTED_RATES",		WORD_T,			_OFFSET(iNIC_supportedRates),	_SIZE(iNIC_supportedRates)},
{MIB_INIC_BEACON_INTERVAL,		"INIC_BEACON_INTERVAL",		WORD_T,			_OFFSET(iNIC_beaconInterval),	_SIZE(iNIC_beaconInterval)},
{MIB_INIC_PREAMBLE_TYPE,		"INIC_PREAMBLE_TYPE",		BYTE_T,			_OFFSET(iNIC_preambleType),		_SIZE(iNIC_preambleType)},
{MIB_INIC_BASIC_RATE,			"INIC_BASIC_RATES",		WORD_T,			_OFFSET(iNIC_basicRates),		_SIZE(iNIC_basicRates)},
{MIB_INIC_RTS_THRESHOLD,		"INIC_RTS_THRESHOLD",		WORD_T,			_OFFSET(iNIC_rtsThreshold),		_SIZE(iNIC_rtsThreshold)},
{MIB_INIC_AUTH_TYPE,			"INIC_AUTH_TYPE",		BYTE_T,			_OFFSET(iNIC_authType),		_SIZE(iNIC_authType)},
{MIB_INIC_HIDDEN_SSID,			"INIC_HIDDEN_SSID",		BYTE_T,			_OFFSET(iNIC_hiddenSSID),		_SIZE(iNIC_hiddenSSID)},
{MIB_INIC_DISABLED,			"INIC_DISABLED",		BYTE_T,			_OFFSET(iNIC_Disabled),		_SIZE(iNIC_Disabled)},
{MIB_INIC_INACTIVITY_TIME,		"INIC_INACTIVITY_TIME",	DWORD_T,		_OFFSET(iNIC_inactivityTime),	_SIZE(iNIC_inactivityTime)},
{MIB_INIC_RATE_ADAPTIVE_ENABLED,	"INIC_RATE_ADAPTIVE_ENABLED",	BYTE_T,			_OFFSET(iNIC_rateAdaptiveEnabled),	_SIZE(iNIC_rateAdaptiveEnabled)},
{MIB_INIC_DTIM_PERIOD,			"INIC_DTIM_PERIOD",		BYTE_T,			_OFFSET(iNIC_dtimPeriod),		_SIZE(iNIC_dtimPeriod)},
{MIB_INIC_NETWORK_TYPE,		"INIC_NETWORK_TYPE",		BYTE_T,			_OFFSET(iNIC_networkType),		_SIZE(iNIC_networkType)},
{MIB_INIC_IAPP_DISABLED,		"INIC_IAPP_DISABLED",		BYTE_T,			_OFFSET(iNIC_iappDisabled),		_SIZE(iNIC_iappDisabled)},
{MIB_INIC_PROTECTION_DISABLED,		"INIC_PROTECTION_DISABLED",	BYTE_T,			_OFFSET(iNIC_protectionDisabled),	_SIZE(iNIC_protectionDisabled)},
{MIB_INIC_NAT25_MAC_CLONE,		"INIC_MACCLONE_ENABLED",	BYTE_T,			_OFFSET(iNIC_maccloneEnabled),	_SIZE(iNIC_maccloneEnabled)},
{MIB_INIC_BAND,			"INIC_BAND",			BYTE_T,			_OFFSET(iNIC_wlanBand),		_SIZE(iNIC_wlanBand)},
{MIB_INIC_FIX_RATE,			"INIC_FIX_RATE",		WORD_T,			_OFFSET(iNIC_fixedTxRate),		_SIZE(iNIC_fixedTxRate)},
{MIB_INIC_WPA2_PRE_AUTH,		"INIC_WPA2_PRE_AUTH",		BYTE_T,			_OFFSET(iNIC_wpa2PreAuth),		_SIZE(iNIC_wpa2PreAuth)},
{MIB_INIC_WPA2_CIPHER_SUITE,		"INIC_WPA2_CIPHER_SUITE",	BYTE_T,			_OFFSET(iNIC_wpa2Cipher),		_SIZE(iNIC_wpa2Cipher)},

{MIB_INIC_SET_TX,			"INIC_SET_TX",			BYTE_T,			_OFFSET(iNIC_wlanSetTx),		_SIZE(iNIC_wlanSetTx)},
{MIB_INIC_AP_MODE,			"INIC_AP_MODE",			BYTE_T,			_OFFSET(iNIC_apMode),		_SIZE(iNIC_apMode)},
{MIB_INIC_SECURITY_MODE,		"INIC_SECURITY_MODE",		BYTE_T,			_OFFSET(iNIC_secMode),		_SIZE(iNIC_secMode)},
{MIB_INIC_CLIENT_IP_DISABLED,		"INIC_CLIENT_IP_DISABLED",		BYTE_T,			_OFFSET(iNIC_clIpDisabled),		_SIZE(iNIC_clIpDisabled)},
{MIB_INIC_BLOCK_RELAY,			"INIC_BLOCK_RELAY",		BYTE_T,			_OFFSET(iNIC_blockRelay),		_SIZE(iNIC_blockRelay)},
{MIB_INIC_AUTO_MAC_CLONE,		"INIC_AUTO_MAC_CLONE",		BYTE_T,			_OFFSET(iNIC_autoMacClone),		_SIZE(iNIC_autoMacClone)},
{MIB_INIC_WISP_WAN_ID,			"INIC_WISP_WAN_ID",			BYTE_T,			_OFFSET(iNIC_wispWanId),		_SIZE(iNIC_wispWanId)},

//**************************************** WEP of WDS****************************************
{MIB_INIC_WL_LINKMAC1, 		"WL_INIC_LINKMAC1",		BYTE6_T,		_OFFSET(iNIC_wlLinkMac1),		_SIZE(iNIC_wlLinkMac1)},
{MIB_INIC_WL_LINKMAC2, 		"WL_INIC_LINKMAC2",		BYTE6_T,		_OFFSET(iNIC_wlLinkMac2),		_SIZE(iNIC_wlLinkMac2)},
{MIB_INIC_WL_LINKMAC3, 		"WL_INIC_LINKMAC3",		BYTE6_T,		_OFFSET(iNIC_wlLinkMac3),		_SIZE(iNIC_wlLinkMac3)},
{MIB_INIC_WL_LINKMAC4, 		"WL_INIC_LINKMAC4",		BYTE6_T,		_OFFSET(iNIC_wlLinkMac4),		_SIZE(iNIC_wlLinkMac4)},
{MIB_INIC_WL_LINKMAC5, 		"WL_INIC_LINKMAC5",		BYTE6_T,		_OFFSET(iNIC_wlLinkMac5),		_SIZE(iNIC_wlLinkMac5)},
{MIB_INIC_WL_LINKMAC6, 		"WL_INIC_LINKMAC6",		BYTE6_T,		_OFFSET(iNIC_wlLinkMac6),		_SIZE(iNIC_wlLinkMac6)},

{MIB_INIC_WDS_ENABLED,			"INIC_WDS_ENABLED",		BYTE_T,			_OFFSET(iNIC_wdsEnabled),		_SIZE(iNIC_wdsEnabled)},

{MIB_INIC_WDS_ENCRYPT,			"INIC_WDS_ENCRYPT",		BYTE_T,			_OFFSET(iNIC_wdsEncrypt),		_SIZE(iNIC_encrypt)},
{MIB_INIC_WDS_WPA_AUTH,		"INIC_WDS_WPA_AUTH",		BYTE_T,			_OFFSET(iNIC_wdsWpaAuth),		_SIZE(iNIC_wpaAuth)},
{MIB_INIC_WDS_WPA_CIPHER_SUITE,	"INIC_WDS_WPA_CIPHER_SUITE",	BYTE_T,			_OFFSET(iNIC_wdsWpaCipher),		_SIZE(iNIC_wpaCipher)},
{MIB_INIC_WDS_WPA2_CIPHER_SUITE,	"INIC_WDS_WPA2_CIPHER_SUITE",	BYTE_T,			_OFFSET(iNIC_wdsWpa2Cipher),		_SIZE(iNIC_wpa2Cipher)},
{MIB_INIC_WDS_WPA_PSK,			"INIC_WDS_WPA_PSK",		STRING_T,		_OFFSET(iNIC_wdsWpaPSK),		_SIZE(iNIC_wpaPSK)},
{MIB_INIC_WDS_WPA_PSK_FORMAT,		"INIC_WDS_PSK_FORMAT",		BYTE_T,			_OFFSET(iNIC_wdsWpaPSKFormat),	_SIZE(iNIC_wpaPSKFormat)},
//--------------------------------------------------------------------------------------------
#ifdef WLAN_WPA
{MIB_INIC_ENCRYPT,			"INIC_ENCRYPT",		BYTE_T,			_OFFSET(iNIC_encrypt),			_SIZE(iNIC_encrypt)},
{MIB_INIC_ENABLE_SUPP_NONWPA,		"INIC_ENABLE_SUPP_NONWPA",	BYTE_T,			_OFFSET(iNIC_enableSuppNonWpa),	_SIZE(iNIC_enableSuppNonWpa)},
{MIB_INIC_SUPP_NONWPA,			"INIC_SUPP_NONWPA",		BYTE_T,			_OFFSET(iNIC_suppNonWpa),		_SIZE(iNIC_suppNonWpa)},
{MIB_INIC_WPA_AUTH,			"INIC_WPA_AUTH",		BYTE_T,			_OFFSET(iNIC_wpaAuth),			_SIZE(iNIC_wpaAuth)},
{MIB_INIC_WPA_CIPHER_SUITE,		"INIC_WPA_CIPHER_SUITE",	BYTE_T,			_OFFSET(iNIC_wpaCipher),		_SIZE(iNIC_wpaCipher)},
{MIB_INIC_WPA_PSK,			"INIC_WPA_PSK",		STRING_T,		_OFFSET(iNIC_wpaPSK),			_SIZE(iNIC_wpaPSK)},
{MIB_INIC_WPA_GROUP_REKEY_TIME,	"INIC_WPA_GROUP_REKEY_TIME",	DWORD_T,		_OFFSET(iNIC_wpaGroupRekeyTime),	_SIZE(iNIC_wpaGroupRekeyTime)},
{MIB_INIC_ENABLE_MAC_AUTH,		"INIC_MAC_AUTH_ENABLED",	BYTE_T,			_OFFSET(iNIC_macAuthEnabled),		_SIZE(iNIC_macAuthEnabled)},
{MIB_INIC_RS_IP,			"INIC_RS_IP",			IA_T,			_OFFSET(iNIC_rsIpAddr),		_SIZE(iNIC_rsIpAddr)},
{MIB_INIC_RS_PORT,			"INIC_RS_PORT",		WORD_T,			_OFFSET(iNIC_rsPort),			_SIZE(iNIC_rsPort)},
{MIB_INIC_RS_PASSWORD,			"INIC_RS_PASSWORD",		STRING_T,		_OFFSET(iNIC_rsPassword),		_SIZE(iNIC_rsPassword)},
{MIB_INIC_RS_RETRY,			"INIC_RS_MAXRETRY",		BYTE_T,			_OFFSET(iNIC_rsMaxRetry),		_SIZE(iNIC_rsMaxRetry)},
{MIB_INIC_RS_INTERVAL_TIME,		"INIC_RS_INTERVAL_TIME",	WORD_T,			_OFFSET(iNIC_rsIntervalTime),		_SIZE(iNIC_rsIntervalTime)},
{MIB_INIC_ACCOUNT_RS_ENABLED,		"INIC_ACCOUNT_RS_ENABLED",	BYTE_T,			_OFFSET(iNIC_accountRsEnabled),	_SIZE(iNIC_accountRsEnabled)},
{MIB_INIC_ACCOUNT_RS_IP,		"INIC_ACCOUNT_RS_IP",		IA_T,			_OFFSET(iNIC_accountRsIpAddr),		_SIZE(iNIC_accountRsIpAddr)},
{MIB_INIC_ACCOUNT_RS_PORT,		"INIC_ACCOUNT_RS_PORT",		WORD_T,		_OFFSET(iNIC_accountRsPort),		_SIZE(iNIC_accountRsPort)},
{MIB_INIC_ACCOUNT_RS_PASSWORD,		"INIC_ACCOUNT_RS_PASSWORD",		STRING_T,	_OFFSET(iNIC_accountRsPassword),	_SIZE(iNIC_accountRsPassword)},
{MIB_INIC_ACCOUNT_UPDATE_ENABLED,	"INIC_ACCOUNT_RS_UPDATE_ENABLED",	BYTE_T,		_OFFSET(iNIC_accountRsUpdateEnabled),	_SIZE(iNIC_accountRsUpdateEnabled)},
{MIB_INIC_ACCOUNT_UPDATE_DELAY,	"INIC_ACCOUNT_RS_UPDATE_DELAY",	WORD_T,		_OFFSET(iNIC_accountRsUpdateDelay),	_SIZE(iNIC_accountRsUpdateDelay)},
{MIB_INIC_ACCOUNT_RS_RETRY,		"INIC_ACCOUNT_RS_MAXRETRY",		BYTE_T,		_OFFSET(iNIC_accountRsMaxRetry),	_SIZE(iNIC_accountRsMaxRetry)},
{MIB_INIC_ACCOUNT_RS_INTERVAL_TIME,	"INIC_ACCOUNT_RS_INTERVAL_TIME",	WORD_T,		_OFFSET(iNIC_accountRsIntervalTime),	_SIZE(iNIC_accountRsIntervalTime)},
{MIB_INIC_ENABLE_1X,			"INIC_ENABLE_1X",		BYTE_T,			_OFFSET(iNIC_enable1X),		_SIZE(iNIC_enable1X)},
{MIB_INIC_WPA_PSK_FORMAT,		"INIC_PSK_FORMAT",		BYTE_T,			_OFFSET(iNIC_wpaPSKFormat),		_SIZE(iNIC_wpaPSKFormat)},
#endif

{MIB_INIC_WPS_ENABLE,			"INIC_WPS_ENABLE",			BYTE_T,			_OFFSET(iNIC_wpsEnable),		_SIZE(iNIC_wpsEnable)},
{MIB_INIC_WPS_CONFIG_MODE,		"INIC_WPS_CONFIG_MODE",		BYTE_T,			_OFFSET(iNIC_wpsConfigMode),		_SIZE(iNIC_wpsConfigMode)},
{MIB_INIC_WPS_PROXY_ENABLE,		"INIC_WPS_PROXY_ENABLE",		BYTE_T,			_OFFSET(iNIC_wpsProxyEnable),	_SIZE(iNIC_wpsProxyEnable)},
{MIB_INIC_WPS_INTERNAL_REG,		"INIC_WPS_INTERNAL_REG",		BYTE_T,			_OFFSET(iNIC_wpsInternalReg),	_SIZE(iNIC_wpsInternalReg)},
{MIB_INIC_WPS_CONFIG_TYPE,		"INIC_WPS_CONFIG_TYPE",		BYTE_T,			_OFFSET(iNIC_wpsConfigType),		_SIZE(iNIC_wpsConfigType)},
{MIB_INIC_WPS_CONFIG_STATUS,		"INIC_WPS_CONFIG_STATUS",		BYTE_T,			_OFFSET(iNIC_wpsConfigStatus),	_SIZE(iNIC_wpsConfigStatus)},
{MIB_INIC_WPS_PING_CODE,		"INIC_WPS_PING_CODE",		STRING_T,		_OFFSET(iNIC_wpsPingCode),		_SIZE(iNIC_wpsPingCode)},
{MIB_INIC_WPS_DISPLAY_KEY,		"INIC_WPS_DISPLAY_KEY",		BYTE_T,			_OFFSET(iNIC_wpsDisplayKey),		_SIZE(iNIC_wpsDisplayKey)},
#if defined(HOME_GATEWAY) || defined(_AP_WLAN_NOFORWARD_)
{MIB_INIC_NOFORWARD,			"INIC_NOFORWARD",		BYTE_T,			_OFFSET(iNIC_wlanForward),		_SIZE(iNIC_wlanForward)},
#endif

#ifdef _DBAND_
{MIB_INIC_ABAND_MODE,			"INIC_ABAND_MODE",		BYTE_T,			_OFFSET(iNIC_aBandMode),		_SIZE(iNIC_aBandMode)},
{MIB_INIC_DUAL_BAND_SWITCH,		"INIC_DUAL_BAND_SWITCH",	BYTE_T,			_OFFSET(iNIC_dualBandSwitch),		_SIZE(iNIC_dualBandSwitch)},
{MIB_INIC_ABAND_CHANNEL,		"INIC_ABAND_CHANNEL",		BYTE_T,			_OFFSET(iNIC_aBandChannel),		_SIZE(iNIC_aBandChannel)},
#endif

#ifdef UNIVERSAL_REPEATER
{MIB_INIC_REPEATER_ENABLED,		"INIC_REPEATER_ENABLED",	BYTE_T,			_OFFSET(iNIC_repeaterEnabled),		_SIZE(iNIC_repeaterEnabled)},
{MIB_INIC_REPEATER_SSID,		"INIC_REPEATER_SSID",		STRING_T,		_OFFSET(iNIC_repeaterSSID),		_SIZE(iNIC_repeaterSSID)},
#endif

{MIB_INIC_AC_NUM,			"INIC_MACAC_NUM",		BYTE_T,			_OFFSET(iNIC_acNum),			_SIZE(iNIC_acNum)},
{MIB_INIC_AC_ENABLED,			"INIC_MACAC_ENABLED",		BYTE_T,			_OFFSET(iNIC_acEnabled),		_SIZE(iNIC_acEnabled)},
{MIB_INIC_AC_ADDR,			"INIC_MACAC_ADDR",		INIC_AC_ARRAY_T,	_OFFSET(iNIC_acAddrArray),		_SIZE(iNIC_acAddrArray)},

#if defined(_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ )
{MIB_INIC_AC2_NUM,			"INIC_MACAC2_NUM",		BYTE_T,			_OFFSET(iNIC_acNum2),			_SIZE(iNIC_acNum2)},
{MIB_INIC_AC2_ENABLED,			"INIC_MACAC2_ENABLED",		BYTE_T,			_OFFSET(iNIC_acEnabled2),		_SIZE(iNIC_acEnabled2)},
{MIB_INIC_AC2_ADDR,			"INIC_MACAC2_ADDR",		INIC_AC_ARRAY2_T,	_OFFSET(iNIC_acAddrArray2),		_SIZE(iNIC_acAddrArray2)},

{MIB_INIC_AC3_NUM,			"INIC_MACAC3_NUM",		BYTE_T,			_OFFSET(iNIC_acNum3),			_SIZE(iNIC_acNum3)},
{MIB_INIC_AC3_ENABLED,			"INIC_MACAC3_ENABLED",		BYTE_T,			_OFFSET(iNIC_acEnabled3),		_SIZE(iNIC_acEnabled3)},
{MIB_INIC_AC3_ADDR,			"INIC_MACAC3_ADDR",		INIC_AC_ARRAY3_T,	_OFFSET(iNIC_acAddrArray3),		_SIZE(iNIC_acAddrArray3)},

{MIB_INIC_AC4_NUM,			"INIC_MACAC4_NUM",		BYTE_T,			_OFFSET(iNIC_acNum4),			_SIZE(iNIC_acNum4)},
{MIB_INIC_AC4_ENABLED,			"INIC_MACAC4_ENABLED",		BYTE_T,			_OFFSET(iNIC_acEnabled4),		_SIZE(iNIC_acEnabled4)},
{MIB_INIC_AC4_ADDR,			"INIC_MACAC4_ADDR",		INIC_AC_ARRAY4_T,	_OFFSET(iNIC_acAddrArray4),		_SIZE(iNIC_acAddrArray4)},
#endif //_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_

#ifdef _MCAST_RATE_SUPPORT_
{MIB_INIC_MCAST_RATE,			"INIC_MCAST_RATE",		BYTE_T,			_OFFSET(iNIC_McastRate),		_SIZE(iNIC_McastRate)},
#endif

{MIB_INIC_DFS,				"INIC_DFS",			BYTE_T,			_OFFSET(iNIC_DFSEnable),		_SIZE(iNIC_DFSEnable)},
{MIB_INIC_BF,				"INIC_BF",			BYTE_T,			_OFFSET(iNIC_BFEnable),			_SIZE(iNIC_BFEnable)},

//Wise
#ifdef _INIC_MSSID_
{MIB_INIC_SSID_1,			"INIC_SSID_1",			STRING_T,		_OFFSET(iNIC_ssid1),			_SIZE(iNIC_ssid1)},
{MIB_INIC_SSID_2,			"INIC_SSID_2",			STRING_T,		_OFFSET(iNIC_ssid2),			_SIZE(iNIC_ssid2)},
{MIB_INIC_SSID_3,			"INIC_SSID_3",			STRING_T,		_OFFSET(iNIC_ssid3),			_SIZE(iNIC_ssid3)},
{MIB_INIC_SSID_4,			"INIC_SSID_4",			STRING_T,		_OFFSET(iNIC_ssid4),			_SIZE(iNIC_ssid4)},
{MIB_INIC_SSID_5,			"INIC_SSID_5",			STRING_T,		_OFFSET(iNIC_ssid5),			_SIZE(iNIC_ssid5)},
{MIB_INIC_SSID_6,			"INIC_SSID_6",			STRING_T,		_OFFSET(iNIC_ssid6),			_SIZE(iNIC_ssid6)},
{MIB_INIC_SSID_7,			"INIC_SSID_7",			STRING_T,		_OFFSET(iNIC_ssid7),			_SIZE(iNIC_ssid7)},
{MIB_INIC_WEP64_KEY1_1,			"INIC_WEP64_KEY1_1",		BYTE5_T,		_OFFSET(iNIC_wep64Key11),		_SIZE(iNIC_wep64Key11)},
{MIB_INIC_WEP64_KEY2_1,			"INIC_WEP64_KEY2_1",		BYTE5_T,		_OFFSET(iNIC_wep64Key21),		_SIZE(iNIC_wep64Key21)},
{MIB_INIC_WEP64_KEY3_1,			"INIC_WEP64_KEY3_1",		BYTE5_T,		_OFFSET(iNIC_wep64Key31),		_SIZE(iNIC_wep64Key31)},
{MIB_INIC_WEP64_KEY4_1,			"INIC_WEP64_KEY4_1",		BYTE5_T,		_OFFSET(iNIC_wep64Key41),		_SIZE(iNIC_wep64Key41)},
{MIB_INIC_WEP64_KEY1_2,			"INIC_WEP64_KEY1_2",		BYTE5_T,		_OFFSET(iNIC_wep64Key12),		_SIZE(iNIC_wep64Key12)},
{MIB_INIC_WEP64_KEY2_2,			"INIC_WEP64_KEY2_2",		BYTE5_T,		_OFFSET(iNIC_wep64Key22),		_SIZE(iNIC_wep64Key22)},
{MIB_INIC_WEP64_KEY3_2,			"INIC_WEP64_KEY3_2",		BYTE5_T,		_OFFSET(iNIC_wep64Key32),		_SIZE(iNIC_wep64Key32)},
{MIB_INIC_WEP64_KEY4_2,			"INIC_WEP64_KEY4_2",		BYTE5_T,		_OFFSET(iNIC_wep64Key42),		_SIZE(iNIC_wep64Key42)},
{MIB_INIC_WEP64_KEY1_3,			"INIC_WEP64_KEY1_3",		BYTE5_T,		_OFFSET(iNIC_wep64Key13),		_SIZE(iNIC_wep64Key13)},
{MIB_INIC_WEP64_KEY2_3,			"INIC_WEP64_KEY2_3",		BYTE5_T,		_OFFSET(iNIC_wep64Key23),		_SIZE(iNIC_wep64Key23)},
{MIB_INIC_WEP64_KEY3_3,			"INIC_WEP64_KEY3_3",		BYTE5_T,		_OFFSET(iNIC_wep64Key33),		_SIZE(iNIC_wep64Key33)},
{MIB_INIC_WEP64_KEY4_3,			"INIC_WEP64_KEY4_3",		BYTE5_T,		_OFFSET(iNIC_wep64Key43),		_SIZE(iNIC_wep64Key43)},
{MIB_INIC_WEP64_KEY1_4,			"INIC_WEP64_KEY1_4",		BYTE5_T,		_OFFSET(iNIC_wep64Key14),		_SIZE(iNIC_wep64Key14)},
{MIB_INIC_WEP64_KEY2_4,			"INIC_WEP64_KEY2_4",		BYTE5_T,		_OFFSET(iNIC_wep64Key24),		_SIZE(iNIC_wep64Key24)},
{MIB_INIC_WEP64_KEY3_4,			"INIC_WEP64_KEY3_4",		BYTE5_T,		_OFFSET(iNIC_wep64Key34),		_SIZE(iNIC_wep64Key34)},
{MIB_INIC_WEP64_KEY4_4,			"INIC_WEP64_KEY4_4",		BYTE5_T,		_OFFSET(iNIC_wep64Key44),		_SIZE(iNIC_wep64Key44)},
{MIB_INIC_WEP64_KEY1_5,			"INIC_WEP64_KEY1_5",		BYTE5_T,		_OFFSET(iNIC_wep64Key15),		_SIZE(iNIC_wep64Key15)},
{MIB_INIC_WEP64_KEY2_5,			"INIC_WEP64_KEY2_5",		BYTE5_T,		_OFFSET(iNIC_wep64Key25),		_SIZE(iNIC_wep64Key25)},
{MIB_INIC_WEP64_KEY3_5,			"INIC_WEP64_KEY3_5",		BYTE5_T,		_OFFSET(iNIC_wep64Key35),		_SIZE(iNIC_wep64Key35)},
{MIB_INIC_WEP64_KEY4_5,			"INIC_WEP64_KEY4_5",		BYTE5_T,		_OFFSET(iNIC_wep64Key45),		_SIZE(iNIC_wep64Key45)},
{MIB_INIC_WEP64_KEY1_6,			"INIC_WEP64_KEY1_6",		BYTE5_T,		_OFFSET(iNIC_wep64Key16),		_SIZE(iNIC_wep64Key16)},
{MIB_INIC_WEP64_KEY2_6,			"INIC_WEP64_KEY2_6",		BYTE5_T,		_OFFSET(iNIC_wep64Key26),		_SIZE(iNIC_wep64Key26)},
{MIB_INIC_WEP64_KEY3_6,			"INIC_WEP64_KEY3_6",		BYTE5_T,		_OFFSET(iNIC_wep64Key36),		_SIZE(iNIC_wep64Key36)},
{MIB_INIC_WEP64_KEY4_6,			"INIC_WEP64_KEY4_6",		BYTE5_T,		_OFFSET(iNIC_wep64Key46),		_SIZE(iNIC_wep64Key46)},
{MIB_INIC_WEP64_KEY1_7,			"INIC_WEP64_KEY1_7",		BYTE5_T,		_OFFSET(iNIC_wep64Key17),		_SIZE(iNIC_wep64Key17)},
{MIB_INIC_WEP64_KEY2_7,			"INIC_WEP64_KEY2_7",		BYTE5_T,		_OFFSET(iNIC_wep64Key27),		_SIZE(iNIC_wep64Key27)},
{MIB_INIC_WEP64_KEY3_7,			"INIC_WEP64_KEY3_7",		BYTE5_T,		_OFFSET(iNIC_wep64Key37),		_SIZE(iNIC_wep64Key37)},
{MIB_INIC_WEP64_KEY4_7,			"INIC_WEP64_KEY4_7",		BYTE5_T,		_OFFSET(iNIC_wep64Key47),		_SIZE(iNIC_wep64Key47)},
{MIB_INIC_WEP128_KEY1_1,		"INIC_WEP128_KEY1_1",		BYTE13_T,		_OFFSET(iNIC_wep128Key11),		_SIZE(iNIC_wep128Key11)},
{MIB_INIC_WEP128_KEY2_1,		"INIC_WEP128_KEY2_1",		BYTE13_T,		_OFFSET(iNIC_wep128Key21),		_SIZE(iNIC_wep128Key21)},
{MIB_INIC_WEP128_KEY3_1,		"INIC_WEP128_KEY3_1",		BYTE13_T,		_OFFSET(iNIC_wep128Key31),		_SIZE(iNIC_wep128Key31)},
{MIB_INIC_WEP128_KEY4_1,		"INIC_WEP128_KEY4_1",		BYTE13_T,		_OFFSET(iNIC_wep128Key41),		_SIZE(iNIC_wep128Key41)},
{MIB_INIC_WEP128_KEY1_2,		"INIC_WEP128_KEY1_2",		BYTE13_T,		_OFFSET(iNIC_wep128Key12),		_SIZE(iNIC_wep128Key12)},
{MIB_INIC_WEP128_KEY2_2,		"INIC_WEP128_KEY2_2",		BYTE13_T,		_OFFSET(iNIC_wep128Key22),		_SIZE(iNIC_wep128Key22)},
{MIB_INIC_WEP128_KEY3_2,		"INIC_WEP128_KEY3_2",		BYTE13_T,		_OFFSET(iNIC_wep128Key32),		_SIZE(iNIC_wep128Key32)},
{MIB_INIC_WEP128_KEY4_2,		"INIC_WEP128_KEY4_2",		BYTE13_T,		_OFFSET(iNIC_wep128Key42),		_SIZE(iNIC_wep128Key42)},
{MIB_INIC_WEP128_KEY1_3,		"INIC_WEP128_KEY1_3",		BYTE13_T,		_OFFSET(iNIC_wep128Key13),		_SIZE(iNIC_wep128Key13)},
{MIB_INIC_WEP128_KEY2_3,		"INIC_WEP128_KEY2_3",		BYTE13_T,		_OFFSET(iNIC_wep128Key23),		_SIZE(iNIC_wep128Key23)},
{MIB_INIC_WEP128_KEY3_3,		"INIC_WEP128_KEY3_3",		BYTE13_T,		_OFFSET(iNIC_wep128Key33),		_SIZE(iNIC_wep128Key33)},
{MIB_INIC_WEP128_KEY4_3,		"INIC_WEP128_KEY4_3",		BYTE13_T,		_OFFSET(iNIC_wep128Key43),		_SIZE(iNIC_wep128Key43)},
{MIB_INIC_WEP128_KEY1_4,		"INIC_WEP128_KEY1_4",		BYTE13_T,		_OFFSET(iNIC_wep128Key14),		_SIZE(iNIC_wep128Key14)},
{MIB_INIC_WEP128_KEY2_4,		"INIC_WEP128_KEY2_4",		BYTE13_T,		_OFFSET(iNIC_wep128Key24),		_SIZE(iNIC_wep128Key24)},
{MIB_INIC_WEP128_KEY3_4,		"INIC_WEP128_KEY3_4",		BYTE13_T,		_OFFSET(iNIC_wep128Key34),		_SIZE(iNIC_wep128Key34)},
{MIB_INIC_WEP128_KEY4_4,		"INIC_WEP128_KEY4_4",		BYTE13_T,		_OFFSET(iNIC_wep128Key44),		_SIZE(iNIC_wep128Key44)},
{MIB_INIC_WEP128_KEY1_5,		"INIC_WEP128_KEY1_5",		BYTE13_T,		_OFFSET(iNIC_wep128Key15),		_SIZE(iNIC_wep128Key15)},
{MIB_INIC_WEP128_KEY2_5,		"INIC_WEP128_KEY2_5",		BYTE13_T,		_OFFSET(iNIC_wep128Key25),		_SIZE(iNIC_wep128Key25)},
{MIB_INIC_WEP128_KEY3_5,		"INIC_WEP128_KEY3_5",		BYTE13_T,		_OFFSET(iNIC_wep128Key35),		_SIZE(iNIC_wep128Key35)},
{MIB_INIC_WEP128_KEY4_5,		"INIC_WEP128_KEY4_5",		BYTE13_T,		_OFFSET(iNIC_wep128Key45),		_SIZE(iNIC_wep128Key45)},
{MIB_INIC_WEP128_KEY1_6,		"INIC_WEP128_KEY1_6",		BYTE13_T,		_OFFSET(iNIC_wep128Key16),		_SIZE(iNIC_wep128Key16)},
{MIB_INIC_WEP128_KEY2_6,		"INIC_WEP128_KEY2_6",		BYTE13_T,		_OFFSET(iNIC_wep128Key26),		_SIZE(iNIC_wep128Key26)},
{MIB_INIC_WEP128_KEY3_6,		"INIC_WEP128_KEY3_6",		BYTE13_T,		_OFFSET(iNIC_wep128Key36),		_SIZE(iNIC_wep128Key36)},
{MIB_INIC_WEP128_KEY4_6,		"INIC_WEP128_KEY4_6",		BYTE13_T,		_OFFSET(iNIC_wep128Key46),		_SIZE(iNIC_wep128Key46)},
{MIB_INIC_WEP128_KEY1_7,		"INIC_WEP128_KEY1_7",		BYTE13_T,		_OFFSET(iNIC_wep128Key17),		_SIZE(iNIC_wep128Key17)},
{MIB_INIC_WEP128_KEY2_7,		"INIC_WEP128_KEY2_7",		BYTE13_T,		_OFFSET(iNIC_wep128Key27),		_SIZE(iNIC_wep128Key27)},
{MIB_INIC_WEP128_KEY3_7,		"INIC_WEP128_KEY3_7",		BYTE13_T,		_OFFSET(iNIC_wep128Key37),		_SIZE(iNIC_wep128Key37)},
{MIB_INIC_WEP128_KEY4_7,		"INIC_WEP128_KEY4_7",		BYTE13_T,		_OFFSET(iNIC_wep128Key47),		_SIZE(iNIC_wep128Key47)},
{MIB_INIC_WPA_PSK_1,			"INIC_WPA_PSK_1",		STRING_T,		_OFFSET(iNIC_wpaPSK1),			_SIZE(iNIC_wpaPSK1)},
{MIB_INIC_WPA_PSK_2,			"INIC_WPA_PSK_2",		STRING_T,		_OFFSET(iNIC_wpaPSK2),			_SIZE(iNIC_wpaPSK2)},
{MIB_INIC_WPA_PSK_3,			"INIC_WPA_PSK_3",		STRING_T,		_OFFSET(iNIC_wpaPSK3),			_SIZE(iNIC_wpaPSK3)},
{MIB_INIC_WPA_PSK_4,			"INIC_WPA_PSK_4",		STRING_T,		_OFFSET(iNIC_wpaPSK4),			_SIZE(iNIC_wpaPSK4)},
{MIB_INIC_WPA_PSK_5,			"INIC_WPA_PSK_5",		STRING_T,		_OFFSET(iNIC_wpaPSK5),			_SIZE(iNIC_wpaPSK5)},
{MIB_INIC_WPA_PSK_6,			"INIC_WPA_PSK_6",		STRING_T,		_OFFSET(iNIC_wpaPSK6),			_SIZE(iNIC_wpaPSK6)},
{MIB_INIC_WPA_PSK_7,			"INIC_WPA_PSK_7",		STRING_T,		_OFFSET(iNIC_wpaPSK7),			_SIZE(iNIC_wpaPSK7)},
{MIB_INIC_SECURITY_MODE_1,		"INIC_SECURITY_MODE_1",		BYTE_T,			_OFFSET(iNIC_secMode1),			_SIZE(iNIC_secMode1)},
{MIB_INIC_SECURITY_MODE_2,		"INIC_SECURITY_MODE_2",		BYTE_T,			_OFFSET(iNIC_secMode2),			_SIZE(iNIC_secMode2)},
{MIB_INIC_SECURITY_MODE_3,		"INIC_SECURITY_MODE_3",		BYTE_T,			_OFFSET(iNIC_secMode3),			_SIZE(iNIC_secMode3)},
{MIB_INIC_SECURITY_MODE_4,		"INIC_SECURITY_MODE_4",		BYTE_T,			_OFFSET(iNIC_secMode4),			_SIZE(iNIC_secMode4)},
{MIB_INIC_SECURITY_MODE_5,		"INIC_SECURITY_MODE_5",		BYTE_T,			_OFFSET(iNIC_secMode5),			_SIZE(iNIC_secMode5)},
{MIB_INIC_SECURITY_MODE_6,		"INIC_SECURITY_MODE_6",		BYTE_T,			_OFFSET(iNIC_secMode6),			_SIZE(iNIC_secMode6)},
{MIB_INIC_SECURITY_MODE_7,		"INIC_SECURITY_MODE_7",		BYTE_T,			_OFFSET(iNIC_secMode7),			_SIZE(iNIC_secMode7)},
{MIB_INIC_ENABLE_1X_1,			"INIC_ENABLE_1X_1",	BYTE_T,			_OFFSET(iNIC_enable1X1),		_SIZE(iNIC_enable1X1)},
{MIB_INIC_ENABLE_1X_2,			"INIC_ENABLE_1X_2",	BYTE_T,			_OFFSET(iNIC_enable1X2),		_SIZE(iNIC_enable1X2)},
{MIB_INIC_ENABLE_1X_3,			"INIC_ENABLE_1X_3",	BYTE_T,			_OFFSET(iNIC_enable1X3),		_SIZE(iNIC_enable1X3)},
{MIB_INIC_ENABLE_1X_4,			"INIC_ENABLE_1X_4",	BYTE_T,			_OFFSET(iNIC_enable1X4),		_SIZE(iNIC_enable1X4)},
{MIB_INIC_ENABLE_1X_5,			"INIC_ENABLE_1X_5",	BYTE_T,			_OFFSET(iNIC_enable1X5),		_SIZE(iNIC_enable1X5)},
{MIB_INIC_ENABLE_1X_6,			"INIC_ENABLE_1X_6",	BYTE_T,			_OFFSET(iNIC_enable1X6),		_SIZE(iNIC_enable1X6)},
{MIB_INIC_ENABLE_1X_7,			"INIC_ENABLE_1X_7",	BYTE_T,			_OFFSET(iNIC_enable1X7),		_SIZE(iNIC_enable1X7)},
{MIB_INIC_WEP_1,			"INIC_WEP_1",			BYTE_T,			_OFFSET(iNIC_wep1),			_SIZE(iNIC_wep1)},
{MIB_INIC_WEP_2,			"INIC_WEP_2",			BYTE_T,			_OFFSET(iNIC_wep2),			_SIZE(iNIC_wep2)},
{MIB_INIC_WEP_3,			"INIC_WEP_3",			BYTE_T,			_OFFSET(iNIC_wep3),			_SIZE(iNIC_wep3)},
{MIB_INIC_WEP_4,			"INIC_WEP_4",			BYTE_T,			_OFFSET(iNIC_wep4),			_SIZE(iNIC_wep4)},
{MIB_INIC_WEP_5,			"INIC_WEP_5",			BYTE_T,			_OFFSET(iNIC_wep5),			_SIZE(iNIC_wep5)},
{MIB_INIC_WEP_6,			"INIC_WEP_6",			BYTE_T,			_OFFSET(iNIC_wep6),			_SIZE(iNIC_wep6)},
{MIB_INIC_WEP_7,			"INIC_WEP_7",			BYTE_T,			_OFFSET(iNIC_wep7),			_SIZE(iNIC_wep7)},
{MIB_INIC_WEP_KEY_TYPE_1,		"INIC_WEP_KEY_TYPE_1",		BYTE_T,			_OFFSET(iNIC_wepKeyType1),		_SIZE(iNIC_wepKeyType1)},
{MIB_INIC_WEP_KEY_TYPE_2,		"INIC_WEP_KEY_TYPE_2",		BYTE_T,			_OFFSET(iNIC_wepKeyType2),		_SIZE(iNIC_wepKeyType2)},
{MIB_INIC_WEP_KEY_TYPE_3,		"INIC_WEP_KEY_TYPE_3",		BYTE_T,			_OFFSET(iNIC_wepKeyType3),		_SIZE(iNIC_wepKeyType3)},
{MIB_INIC_WEP_KEY_TYPE_4,		"INIC_WEP_KEY_TYPE_4",		BYTE_T,			_OFFSET(iNIC_wepKeyType4),		_SIZE(iNIC_wepKeyType4)},
{MIB_INIC_WEP_KEY_TYPE_5,		"INIC_WEP_KEY_TYPE_5",		BYTE_T,			_OFFSET(iNIC_wepKeyType5),		_SIZE(iNIC_wepKeyType5)},
{MIB_INIC_WEP_KEY_TYPE_6,		"INIC_WEP_KEY_TYPE_6",		BYTE_T,			_OFFSET(iNIC_wepKeyType6),		_SIZE(iNIC_wepKeyType6)},
{MIB_INIC_WEP_KEY_TYPE_7,		"INIC_WEP_KEY_TYPE_7",		BYTE_T,			_OFFSET(iNIC_wepKeyType7),		_SIZE(iNIC_wepKeyType7)},
{MIB_INIC_WPA_CIPHER_SUITE_1,		"INIC_WPA_CIPHER_SUITE_1",	BYTE_T,			_OFFSET(iNIC_wpaCipher1),		_SIZE(iNIC_wpaCipher1)},
{MIB_INIC_WPA_CIPHER_SUITE_2,		"INIC_WPA_CIPHER_SUITE_2",	BYTE_T,			_OFFSET(iNIC_wpaCipher2),		_SIZE(iNIC_wpaCipher2)},
{MIB_INIC_WPA_CIPHER_SUITE_3,		"INIC_WPA_CIPHER_SUITE_3",	BYTE_T,			_OFFSET(iNIC_wpaCipher3),		_SIZE(iNIC_wpaCipher3)},
{MIB_INIC_WPA_CIPHER_SUITE_4,		"INIC_WPA_CIPHER_SUITE_4",	BYTE_T,			_OFFSET(iNIC_wpaCipher4),		_SIZE(iNIC_wpaCipher4)},
{MIB_INIC_WPA_CIPHER_SUITE_5,		"INIC_WPA_CIPHER_SUITE_5",	BYTE_T,			_OFFSET(iNIC_wpaCipher5),		_SIZE(iNIC_wpaCipher5)},
{MIB_INIC_WPA_CIPHER_SUITE_6,		"INIC_WPA_CIPHER_SUITE_6",	BYTE_T,			_OFFSET(iNIC_wpaCipher6),		_SIZE(iNIC_wpaCipher6)},
{MIB_INIC_WPA_CIPHER_SUITE_7,		"INIC_WPA_CIPHER_SUITE_7",	BYTE_T,			_OFFSET(iNIC_wpaCipher7),		_SIZE(iNIC_wpaCipher7)},
{MIB_INIC_WPA2_CIPHER_SUITE_1,		"INIC_WPA2_CIPHER_SUITE_1",	BYTE_T,			_OFFSET(iNIC_wpa2Cipher1),		_SIZE(iNIC_wpa2Cipher1)},
{MIB_INIC_WPA2_CIPHER_SUITE_2,		"INIC_WPA2_CIPHER_SUITE_2",	BYTE_T,			_OFFSET(iNIC_wpa2Cipher2),		_SIZE(iNIC_wpa2Cipher2)},
{MIB_INIC_WPA2_CIPHER_SUITE_3,		"INIC_WPA2_CIPHER_SUITE_3",	BYTE_T,			_OFFSET(iNIC_wpa2Cipher3),		_SIZE(iNIC_wpa2Cipher3)},
{MIB_INIC_WPA2_CIPHER_SUITE_4,		"INIC_WPA2_CIPHER_SUITE_4",	BYTE_T,			_OFFSET(iNIC_wpa2Cipher4),		_SIZE(iNIC_wpa2Cipher4)},
{MIB_INIC_WPA2_CIPHER_SUITE_5,		"INIC_WPA2_CIPHER_SUITE_5",	BYTE_T,			_OFFSET(iNIC_wpa2Cipher5),		_SIZE(iNIC_wpa2Cipher5)},
{MIB_INIC_WPA2_CIPHER_SUITE_6,		"INIC_WPA2_CIPHER_SUITE_6",	BYTE_T,			_OFFSET(iNIC_wpa2Cipher6),		_SIZE(iNIC_wpa2Cipher6)},
{MIB_INIC_WPA2_CIPHER_SUITE_7,		"INIC_WPA2_CIPHER_SUITE_7",	BYTE_T,			_OFFSET(iNIC_wpa2Cipher7),		_SIZE(iNIC_wpa2Cipher7)},
{MIB_INIC_WPA_PSK_FORMAT_1,		"INIC_PSK_FORMAT_1",	BYTE_T,			_OFFSET(iNIC_wpaPSKFormat1),		_SIZE(iNIC_wpaPSKFormat1)},
{MIB_INIC_WPA_PSK_FORMAT_2,		"INIC_PSK_FORMAT_2",	BYTE_T,			_OFFSET(iNIC_wpaPSKFormat2),		_SIZE(iNIC_wpaPSKFormat2)},
{MIB_INIC_WPA_PSK_FORMAT_3,		"INIC_PSK_FORMAT_3",	BYTE_T,			_OFFSET(iNIC_wpaPSKFormat3),		_SIZE(iNIC_wpaPSKFormat3)},
{MIB_INIC_WPA_PSK_FORMAT_4,		"INIC_PSK_FORMAT_4",	BYTE_T,			_OFFSET(iNIC_wpaPSKFormat4),		_SIZE(iNIC_wpaPSKFormat4)},
{MIB_INIC_WPA_PSK_FORMAT_5,		"INIC_PSK_FORMAT_5",	BYTE_T,			_OFFSET(iNIC_wpaPSKFormat5),		_SIZE(iNIC_wpaPSKFormat5)},
{MIB_INIC_WPA_PSK_FORMAT_6,		"INIC_PSK_FORMAT_6",	BYTE_T,			_OFFSET(iNIC_wpaPSKFormat6),		_SIZE(iNIC_wpaPSKFormat6)},
{MIB_INIC_WPA_PSK_FORMAT_7,		"INIC_PSK_FORMAT_7",	BYTE_T,			_OFFSET(iNIC_wpaPSKFormat7),		_SIZE(iNIC_wpaPSKFormat7)},
{MIB_INIC_WEP_DEFAULT_KEY_1,		"INIC_WEP_DEFAULT_KEY_1",	BYTE_T,			_OFFSET(iNIC_wepDefaultKey1),		_SIZE(iNIC_wepDefaultKey1)},
{MIB_INIC_WEP_DEFAULT_KEY_2,		"INIC_WEP_DEFAULT_KEY_2",	BYTE_T,			_OFFSET(iNIC_wepDefaultKey2),		_SIZE(iNIC_wepDefaultKey2)},
{MIB_INIC_WEP_DEFAULT_KEY_3,		"INIC_WEP_DEFAULT_KEY_3",	BYTE_T,			_OFFSET(iNIC_wepDefaultKey3),		_SIZE(iNIC_wepDefaultKey3)},
{MIB_INIC_WEP_DEFAULT_KEY_4,		"INIC_WEP_DEFAULT_KEY_4",	BYTE_T,			_OFFSET(iNIC_wepDefaultKey4),		_SIZE(iNIC_wepDefaultKey4)},
{MIB_INIC_WEP_DEFAULT_KEY_5,		"INIC_WEP_DEFAULT_KEY_5",	BYTE_T,			_OFFSET(iNIC_wepDefaultKey5),		_SIZE(iNIC_wepDefaultKey5)},
{MIB_INIC_WEP_DEFAULT_KEY_6,		"INIC_WEP_DEFAULT_KEY_6",	BYTE_T,			_OFFSET(iNIC_wepDefaultKey6),		_SIZE(iNIC_wepDefaultKey6)},
{MIB_INIC_WEP_DEFAULT_KEY_7,		"INIC_WEP_DEFAULT_KEY_7",	BYTE_T,			_OFFSET(iNIC_wepDefaultKey7),		_SIZE(iNIC_wepDefaultKey7)},
{MIB_INIC_SSID_MIRROR_1,		"INIC_SSID_MIRROR_1",		BYTE_T,			_OFFSET(iNIC_ssidMirror1),		_SIZE(iNIC_ssidMirror1)},
{MIB_INIC_SSID_MIRROR_2,		"INIC_SSID_MIRROR_2",		BYTE_T,			_OFFSET(iNIC_ssidMirror2),		_SIZE(iNIC_ssidMirror2)},
{MIB_INIC_SSID_MIRROR_3,		"INIC_SSID_MIRROR_3",		BYTE_T,			_OFFSET(iNIC_ssidMirror3),		_SIZE(iNIC_ssidMirror3)},
{MIB_INIC_SSID_MIRROR_4,		"INIC_SSID_MIRROR_4",		BYTE_T,			_OFFSET(iNIC_ssidMirror4),		_SIZE(iNIC_ssidMirror4)},
{MIB_INIC_SSID_MIRROR_5,		"INIC_SSID_MIRROR_5",		BYTE_T,			_OFFSET(iNIC_ssidMirror5),		_SIZE(iNIC_ssidMirror5)},
{MIB_INIC_SSID_MIRROR_6,		"INIC_SSID_MIRROR_6",		BYTE_T,			_OFFSET(iNIC_ssidMirror6),		_SIZE(iNIC_ssidMirror6)},
{MIB_INIC_SSID_MIRROR_7,		"INIC_SSID_MIRROR_7",		BYTE_T,			_OFFSET(iNIC_ssidMirror7),		_SIZE(iNIC_ssidMirror7)},
{MIB_INIC_FIX_RATE_1,			"INIC_FIX_RATE_1",		WORD_T,			_OFFSET(iNIC_fixedTxRate1),		_SIZE(iNIC_fixedTxRate1)},
{MIB_INIC_FIX_RATE_2,			"INIC_FIX_RATE_2",		WORD_T,			_OFFSET(iNIC_fixedTxRate2),		_SIZE(iNIC_fixedTxRate2)},
{MIB_INIC_FIX_RATE_3,			"INIC_FIX_RATE_3",		WORD_T,			_OFFSET(iNIC_fixedTxRate3),		_SIZE(iNIC_fixedTxRate3)},
{MIB_INIC_FIX_RATE_4,			"INIC_FIX_RATE_4",		WORD_T,			_OFFSET(iNIC_fixedTxRate4),		_SIZE(iNIC_fixedTxRate4)},
{MIB_INIC_FIX_RATE_5,			"INIC_FIX_RATE_5",		WORD_T,			_OFFSET(iNIC_fixedTxRate5),		_SIZE(iNIC_fixedTxRate5)},
{MIB_INIC_FIX_RATE_6,			"INIC_FIX_RATE_6",		WORD_T,			_OFFSET(iNIC_fixedTxRate6),		_SIZE(iNIC_fixedTxRate6)},
{MIB_INIC_FIX_RATE_7,			"INIC_FIX_RATE_7",		WORD_T,			_OFFSET(iNIC_fixedTxRate7),		_SIZE(iNIC_fixedTxRate7)},
{MIB_INIC_HIDDEN_SSID_1,		"INIC_HIDDEN_SSID_1",		BYTE_T,			_OFFSET(iNIC_hiddenSSID1),		_SIZE(iNIC_hiddenSSID1)},
{MIB_INIC_HIDDEN_SSID_2,		"INIC_HIDDEN_SSID_2",		BYTE_T,			_OFFSET(iNIC_hiddenSSID2),		_SIZE(iNIC_hiddenSSID2)},
{MIB_INIC_HIDDEN_SSID_3,		"INIC_HIDDEN_SSID_3",		BYTE_T,			_OFFSET(iNIC_hiddenSSID3),		_SIZE(iNIC_hiddenSSID3)},
{MIB_INIC_HIDDEN_SSID_4,		"INIC_HIDDEN_SSID_4",		BYTE_T,			_OFFSET(iNIC_hiddenSSID4),		_SIZE(iNIC_hiddenSSID4)},
{MIB_INIC_HIDDEN_SSID_5,		"INIC_HIDDEN_SSID_5",		BYTE_T,			_OFFSET(iNIC_hiddenSSID5),		_SIZE(iNIC_hiddenSSID5)},
{MIB_INIC_HIDDEN_SSID_6,		"INIC_HIDDEN_SSID_6",		BYTE_T,			_OFFSET(iNIC_hiddenSSID6),		_SIZE(iNIC_hiddenSSID6)},
{MIB_INIC_HIDDEN_SSID_7,		"INIC_HIDDEN_SSID_7",		BYTE_T,			_OFFSET(iNIC_hiddenSSID7),		_SIZE(iNIC_hiddenSSID7)},
{MIB_INIC_WMM_1,			"INIC_WMM_1",		BYTE_T,			_OFFSET(iNIC_wlanWmm1),			_SIZE(iNIC_wlanWmm1)},
{MIB_INIC_WMM_2,			"INIC_WMM_2",		BYTE_T,			_OFFSET(iNIC_wlanWmm2),			_SIZE(iNIC_wlanWmm2)},
{MIB_INIC_WMM_3,			"INIC_WMM_3",		BYTE_T,			_OFFSET(iNIC_wlanWmm3),			_SIZE(iNIC_wlanWmm3)},
{MIB_INIC_WMM_4,			"INIC_WMM_4",		BYTE_T,			_OFFSET(iNIC_wlanWmm4),			_SIZE(iNIC_wlanWmm4)},
{MIB_INIC_WMM_5,			"INIC_WMM_5",		BYTE_T,			_OFFSET(iNIC_wlanWmm5),			_SIZE(iNIC_wlanWmm5)},
{MIB_INIC_WMM_6,			"INIC_WMM_6",		BYTE_T,			_OFFSET(iNIC_wlanWmm6),			_SIZE(iNIC_wlanWmm6)},
{MIB_INIC_WMM_7,			"INIC_WMM_7",		BYTE_T,			_OFFSET(iNIC_wlanWmm7),			_SIZE(iNIC_wlanWmm7)},
{MIB_INIC_ENCRYPT_1,			"INIC_ENCRYPT_1",		BYTE_T,			_OFFSET(iNIC_encrypt1),			_SIZE(iNIC_encrypt1)},
{MIB_INIC_ENCRYPT_2,			"INIC_ENCRYPT_2",		BYTE_T,			_OFFSET(iNIC_encrypt2),			_SIZE(iNIC_encrypt2)},
{MIB_INIC_ENCRYPT_3,			"INIC_ENCRYPT_3",		BYTE_T,			_OFFSET(iNIC_encrypt3),			_SIZE(iNIC_encrypt3)},
{MIB_INIC_ENCRYPT_4,			"INIC_ENCRYPT_4",		BYTE_T,			_OFFSET(iNIC_encrypt4),			_SIZE(iNIC_encrypt4)},
{MIB_INIC_ENCRYPT_5,			"INIC_ENCRYPT_5",		BYTE_T,			_OFFSET(iNIC_encrypt5),			_SIZE(iNIC_encrypt5)},
{MIB_INIC_ENCRYPT_6,			"INIC_ENCRYPT_6",		BYTE_T,			_OFFSET(iNIC_encrypt6),			_SIZE(iNIC_encrypt6)},
{MIB_INIC_ENCRYPT_7,			"INIC_ENCRYPT_7",		BYTE_T,			_OFFSET(iNIC_encrypt7),			_SIZE(iNIC_encrypt7)},

#if defined(_INIC_ENVLAN_)
{MIB_INIC_VLANID_1,			"INIC_VLANID_1",		WORD_T,			_OFFSET(iNIC_vlanid1),			_SIZE(iNIC_vlanid1)},
{MIB_INIC_VLANID_2,			"INIC_VLANID_2",		WORD_T,			_OFFSET(iNIC_vlanid2),			_SIZE(iNIC_vlanid2)},
{MIB_INIC_VLANID_3,			"INIC_VLANID_3",		WORD_T,			_OFFSET(iNIC_vlanid3),			_SIZE(iNIC_vlanid3)},
{MIB_INIC_VLANID_4,			"INIC_VLANID_4",		WORD_T,			_OFFSET(iNIC_vlanid4),			_SIZE(iNIC_vlanid4)},
{MIB_INIC_VLANID_5,			"INIC_VLANID_5",		WORD_T,			_OFFSET(iNIC_vlanid5),			_SIZE(iNIC_vlanid5)},
{MIB_INIC_VLANID_6,			"INIC_VLANID_6",		WORD_T,			_OFFSET(iNIC_vlanid6),			_SIZE(iNIC_vlanid6)},
{MIB_INIC_VLANID_7,			"INIC_VLANID_7",		WORD_T,			_OFFSET(iNIC_vlanid7),			_SIZE(iNIC_vlanid7)},
#endif
#endif	//INIC_MSSID

#ifdef _POWERSAVINGLOGITEC_
{MIB_PS_SCH_ENABLED,			"PS_SCH_ENABLED",               BYTE_T,                 _OFFSET(oem.PsSchEnabled),  _SIZE(oem.PsSchEnabled)},
{MIB_PS_SCH_LEDSWITCH,			"PS_SCH_LEDSWITCH",             BYTE_T,                 _OFFSET(oem.PSLedswitch),   _SIZE(oem.PSLedswitch)},
{MIB_PS_SCH_WIREDLAN,			"PS_SCH_WIREDLAN",              BYTE_T,                 _OFFSET(oem.PSWiredLan),    _SIZE(oem.PSWiredLan)},
{MIB_PS_SCH_WIRELESSLAN,		"PS_SCH_WIRELESSLAN",		BYTE_T,                 _OFFSET(oem.PSWirelessLan), _SIZE(oem.PSWirelessLan)},
{MIB_PS_SCH_Day0,                       "PS_SCH_Day0",                  STRING_T,                       _OFFSET(oem.day0),                  _SIZE(oem.day0)},
{MIB_PS_SCH_Day1,                       "PS_SCH_Day1",                  STRING_T,                       _OFFSET(oem.day1),                  _SIZE(oem.day1)},
{MIB_PS_SCH_Day2,                       "PS_SCH_Day2",                  STRING_T,                       _OFFSET(oem.day2),                  _SIZE(oem.day2)},
{MIB_PS_SCH_Day3,                       "PS_SCH_Day3",                  STRING_T,                       _OFFSET(oem.day3),                  _SIZE(oem.day3)},
{MIB_PS_SCH_Day4,                       "PS_SCH_Day4",                  STRING_T,                       _OFFSET(oem.day4),                  _SIZE(oem.day4)},
{MIB_PS_SCH_Day5,                       "PS_SCH_Day5",                  STRING_T,                       _OFFSET(oem.day5),                  _SIZE(oem.day5)},
{MIB_PS_SCH_Day6,                       "PS_SCH_Day6",                  STRING_T,                       _OFFSET(oem.day6),                  _SIZE(oem.day6)},
#endif //_POWERSAVINGLOGITEC_

#ifdef _USB_NAS_
{MIB_NAS_PS_ENABLED,			"NAS_PS_ENABLED",		BYTE_T,			_OFFSET(oem.NAS_PS_Enabled),		_SIZE(oem.NAS_PS_Enabled)},
{MIB_NAS_PS_TIMER,			"NAS_PS_TIMER",			BYTE_T,			_OFFSET(oem.NAS_PS_Timer),		_SIZE(oem.NAS_PS_Timer)},

{MIB_NAS_ACCESS_ENABLED,		"NAS_ACCESS_ENABLED",		BYTE_T,			_OFFSET(oem.NAS_Access_Enabled),	_SIZE(oem.NAS_Access_Enabled)},

{MIB_NAS_USER_ENABLED,			"NAS_USER_ENABLED",		BYTE_T,			_OFFSET(oem.NAS_User_Enabled),		_SIZE(oem.NAS_User_Enabled)},
{MIB_NAS_USER_NUM,			"NAS_USER_NUM",			BYTE_T,			_OFFSET(oem.NAS_User_Num),		_SIZE(oem.NAS_User_Num)},
{MIB_NAS_USER,				"NAS_USER",			NAS_USER_ARRAY_T,	_OFFSET(oem.Nas_User),			_SIZE(oem.Nas_User)},

{MIB_NAS_FOLDER_ENABLED,		"NAS_FOLDER_ENABLED",		BYTE_T,			_OFFSET(oem.NAS_Folder_Enabled),	_SIZE(oem.NAS_Folder_Enabled)},
{MIB_NAS_FOLDER_NAME,			"NAS_FOLDER_NAME",		STRING_T,		_OFFSET(oem.NAS_Folder_Name),		_SIZE(oem.NAS_Folder_Name)},
{MIB_NAS_FOLDER_MEMO,			"NAS_FOLDER_MEMO",		STRING_T,		_OFFSET(oem.NAS_Folder_Memo),		_SIZE(oem.NAS_Folder_Memo)},
{MIB_NAS_FOLDER_GROUP_NAME,		"NAS_FOLDER_GROUP_NAME",	STRING_T,		_OFFSET(oem.NAS_Folder_Group_Name),	_SIZE(oem.NAS_Folder_Group_Name)},
{MIB_NAS_WAN_ENABLED,			"NAS_WAN_ENABLED",		BYTE_T,			_OFFSET(oem.NAS_WAN_Enabled),		_SIZE(oem.NAS_WAN_Enabled)},
{MIB_NAS_DLNA_ENABLED,			"NAS_DLNA_ENABLED",		BYTE_T,			_OFFSET(oem.NAS_DLNA_Enabled),		_SIZE(oem.NAS_DLNA_Enabled)},
#endif //_USB_NAS_

#ifdef _Customer_AUTO_FW_UPGRADE_
{MIB_SCS_ENABLE,		"SCS_ENABLE",		STRING_T,	_OFFSET(oem.SCS_Enable),	_SIZE(oem.SCS_Enable)},
{MIB_SC_FW_ACTION,		"SC_FW_ACTION",		STRING_T,	_OFFSET(oem.SC_FW_Action),	_SIZE(oem.SC_FW_Action)},
{MIB_SC_FW_ON_SERVER,		"SC_FW_ON_SERVER",	STRING_T,	_OFFSET(oem.SC_FW_OnServer),	_SIZE(oem.SC_FW_OnServer)},
{MIB_SC_FW_VER,			"SC_FW_VER",		STRING_T,	_OFFSET(oem.SC_FW_Ver),		_SIZE(oem.SC_FW_Ver)},
{MIB_SC_DNS,			"SC_DNS",		IA_T,		_OFFSET(oem.SC_DNS),		_SIZE(oem.SC_DNS)},
#endif
#ifdef _IQv2_
{MIB_WIZ_MODE,               "WIZ_MODE",          BYTE_T,         _OFFSET(wizMode),        _SIZE(wizMode)},
{MIB_IQSET_DISABLE,               "IQSET_DISABLE",          BYTE_T,         _OFFSET(IqDisable),        _SIZE(IqDisable)},
{MIB_WEPASCII_STRING,				"WEPASCII_STRING",				STRING_T,		_OFFSET(wepasciistring),		_SIZE(wepasciistring)},
{MIB_WEPASCII_STRING_5G,				"WEPASCII_STRING_5G",				STRING_T,		_OFFSET(wepasciistring5g),		_SIZE(wepasciistring5g)},
#endif

#ifdef _DHCP_SWITCH_
{MIB_DHCP_SWITCH,              "DHCP_SWITCH",                 BYTE_T,                 _OFFSET(dhcpswitch),   _SIZE(dhcpswitch)},
{MIB_DHCP_SWITCH_5G,              "DHCP_SWITCH_5G",                 BYTE_T,                 _OFFSET(dhcpswitch5g),   _SIZE(dhcpswitch5g)},
{MIB_IP_ADDR_5G,				"IP_ADDR_5G",			IA_T,			_OFFSET(ipAddr5g),		_SIZE(ipAddr5g)},
{MIB_SUBNET_MASK_5G,			"SUBNET_MASK_5G",			IA_T,			_OFFSET(subnetMask5g),		_SIZE(subnetMask5g)},
{MIB_DEFAULT_GATEWAY_5G,			"DEFAULT_GATEWAY_5G",		IA_T,			_OFFSET(defaultGateway5g),	_SIZE(defaultGateway5g)},
#endif

#ifdef _WANTTL_
{MIB_TTL_TYPE,				"TTL_TYPE",			BYTE_T,			_OFFSET(ttlType),		_SIZE(ttlType)},
{MIB_TTL_VALUE,				"TTL_VALUE",			WORD_T,			_OFFSET(ttlValue),		_SIZE(ttlValue)},
#endif

#ifdef _INDEPEND_MTU_
{MIB_INDEPEND_MTU,				"INDEPEND_MTU",			WORD_T,			_OFFSET(inMTU),		_SIZE(inMTU)},
#endif

#ifdef _DNS_PROXY_
{MIB_DNS_PROXY_ENABLE,       "DNS_PROXY_ENABLE",      BYTE_T,     _OFFSET(dnsproxy_enable),       _SIZE(dnsproxy_enable)},
{MIB_DNS_PROXY_URL,			"DNS_PROXY_URL",		STRING_T,		_OFFSET(dnsproxyurll),		_SIZE(dnsproxyurll)},
#endif

#ifdef _EDIT_DNSPROXYURL_  
{MIB_PROXYURL_CUSTOM_ENABLED,          "PROXYURL_CUSTOM_ENABLED",     BYTE_T,                 _OFFSET(proxyurlCustomEnabled), _SIZE(proxyurlCustomEnabled)},
{MIB_PROXYURL_NUM,			"PROXYURL_NUM",		BYTE_T,			_OFFSET(proxyurlNum),	_SIZE(proxyurlNum)},
{MIB_PROXYURL_TBL,					"PROXYURL_TBL",		DNSPROXYURL_ARRAY_T,	_OFFSET(proxyurlArray),	_SIZE(proxyurlArray)},
#endif

#ifdef _MSSID_NOFORWARD_ 
{MIB_WLAN_NOFORWARD1,			"WLAN_NOFORWARD1",		BYTE_T,			_OFFSET(wlanForward1),		_SIZE(wlanForward1)},
{MIB_WLAN_NOFORWARD2,			"WLAN_NOFORWARD2",		BYTE_T,			_OFFSET(wlanForward2),		_SIZE(wlanForward2)},
{MIB_WLAN_NOFORWARD3,			"WLAN_NOFORWARD3",		BYTE_T,			_OFFSET(wlanForward3),		_SIZE(wlanForward3)},
{MIB_WLAN_NOFORWARD4,			"WLAN_NOFORWARD4",		BYTE_T,			_OFFSET(wlanForward4),		_SIZE(wlanForward4)},
{MIB_WLAN_NOFORWARD5,			"WLAN_NOFORWARD5",		BYTE_T,			_OFFSET(wlanForward5),		_SIZE(wlanForward5)},
{MIB_WLAN_NOFORWARD6,			"WLAN_NOFORWARD6",		BYTE_T,			_OFFSET(wlanForward6),		_SIZE(wlanForward6)},
{MIB_WLAN_NOFORWARD7,			"WLAN_NOFORWARD7",		BYTE_T,			_OFFSET(wlanForward7),		_SIZE(wlanForward7)},
#endif
#ifdef _INICMSSID_NOFORWARD_ 
{MIB_INIC_NOFORWARD1,			"INIC_NOFORWARD1",		BYTE_T,			_OFFSET(iNICForward1),		_SIZE(iNICForward1)},
{MIB_INIC_NOFORWARD2,			"INIC_NOFORWARD2",		BYTE_T,			_OFFSET(iNICForward2),		_SIZE(iNICForward2)},
{MIB_INIC_NOFORWARD3,			"INIC_NOFORWARD3",		BYTE_T,			_OFFSET(iNICForward3),		_SIZE(iNICForward3)},
{MIB_INIC_NOFORWARD4,			"INIC_NOFORWARD4",		BYTE_T,			_OFFSET(iNICForward4),		_SIZE(iNICForward4)},
{MIB_INIC_NOFORWARD5,			"INIC_NOFORWARD5",		BYTE_T,			_OFFSET(iNICForward5),		_SIZE(iNICForward5)},
{MIB_INIC_NOFORWARD6,			"INIC_NOFORWARD6",		BYTE_T,			_OFFSET(iNICForward6),		_SIZE(iNICForward6)},
{MIB_INIC_NOFORWARD7,			"INIC_NOFORWARD7",		BYTE_T,			_OFFSET(iNICForward7),		_SIZE(iNICForward7)},
#endif

#ifdef _MULTIPLE_WLAN_RS_SETTING_   //EDX patrick add
{MIB_WLAN_RS_IP_1,                "RS_IP_1",                IA_T,                   _OFFSET(rsIpAddr1),       _SIZE(rsIpAddr1)},
{MIB_WLAN_RS_IP_2,                "RS_IP_2",                IA_T,                   _OFFSET(rsIpAddr2),       _SIZE(rsIpAddr2)},
{MIB_WLAN_RS_IP_3,                "RS_IP_3",                IA_T,                   _OFFSET(rsIpAddr3),       _SIZE(rsIpAddr3)},
{MIB_WLAN_RS_IP_4,                "RS_IP_4",                IA_T,                   _OFFSET(rsIpAddr4),       _SIZE(rsIpAddr4)},
{MIB_WLAN_RS_IP_5,                "RS_IP_5",                IA_T,                   _OFFSET(rsIpAddr5),       _SIZE(rsIpAddr5)},
{MIB_WLAN_RS_IP_6,                "RS_IP_6",                IA_T,                   _OFFSET(rsIpAddr6),       _SIZE(rsIpAddr6)},
{MIB_WLAN_RS_IP_7,                "RS_IP_7",                IA_T,                   _OFFSET(rsIpAddr7),       _SIZE(rsIpAddr7)},
{MIB_WLAN_RS_PORT_1,                      "RS_PORT_1",                      WORD_T,                 _OFFSET(rsPort1),                _SIZE(rsPort1)},
{MIB_WLAN_RS_PORT_2,                      "RS_PORT_2",                      WORD_T,                 _OFFSET(rsPort2),                _SIZE(rsPort2)},
{MIB_WLAN_RS_PORT_3,                      "RS_PORT_3",                      WORD_T,                 _OFFSET(rsPort3),                _SIZE(rsPort3)},
{MIB_WLAN_RS_PORT_4,                      "RS_PORT_4",                      WORD_T,                 _OFFSET(rsPort4),                _SIZE(rsPort4)},
{MIB_WLAN_RS_PORT_5,                      "RS_PORT_5",                      WORD_T,                 _OFFSET(rsPort5),                _SIZE(rsPort5)},
{MIB_WLAN_RS_PORT_6,                      "RS_PORT_6",                      WORD_T,                 _OFFSET(rsPort6),                _SIZE(rsPort6)},
{MIB_WLAN_RS_PORT_7,                      "RS_PORT_7",                      WORD_T,                 _OFFSET(rsPort7),                _SIZE(rsPort7)},
{MIB_WLAN_RS_PASSWORD_1,                  "RS_PASSWORD_1",                  STRING_T,               _OFFSET(rsPassword1),            _SIZE(rsPassword1)},
{MIB_WLAN_RS_PASSWORD_2,                  "RS_PASSWORD_2",                  STRING_T,               _OFFSET(rsPassword2),            _SIZE(rsPassword2)},
{MIB_WLAN_RS_PASSWORD_3,                  "RS_PASSWORD_3",                  STRING_T,               _OFFSET(rsPassword3),            _SIZE(rsPassword3)},
{MIB_WLAN_RS_PASSWORD_4,                  "RS_PASSWORD_4",                  STRING_T,               _OFFSET(rsPassword4),            _SIZE(rsPassword4)},
{MIB_WLAN_RS_PASSWORD_5,                  "RS_PASSWORD_5",                  STRING_T,               _OFFSET(rsPassword5),            _SIZE(rsPassword5)},
{MIB_WLAN_RS_PASSWORD_6,                  "RS_PASSWORD_6",                  STRING_T,               _OFFSET(rsPassword6),            _SIZE(rsPassword6)},
{MIB_WLAN_RS_PASSWORD_7,                  "RS_PASSWORD_7",                  STRING_T,               _OFFSET(rsPassword7),            _SIZE(rsPassword7)},
{MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME_1,     "ACCOUNT_RS_INTERVAL_TIME_1",     WORD_T,                 _OFFSET(accountRsIntervalTime1), _SIZE(accountRsIntervalTime1)},
{MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME_2,     "ACCOUNT_RS_INTERVAL_TIME_2",     WORD_T,                 _OFFSET(accountRsIntervalTime2), _SIZE(accountRsIntervalTime2)},
{MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME_3,     "ACCOUNT_RS_INTERVAL_TIME_3",     WORD_T,                 _OFFSET(accountRsIntervalTime3), _SIZE(accountRsIntervalTime3)},
{MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME_4,     "ACCOUNT_RS_INTERVAL_TIME_4",     WORD_T,                 _OFFSET(accountRsIntervalTime4), _SIZE(accountRsIntervalTime4)},
{MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME_5,     "ACCOUNT_RS_INTERVAL_TIME_5",     WORD_T,                 _OFFSET(accountRsIntervalTime5), _SIZE(accountRsIntervalTime5)},
{MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME_6,     "ACCOUNT_RS_INTERVAL_TIME_6",     WORD_T,                 _OFFSET(accountRsIntervalTime6), _SIZE(accountRsIntervalTime6)},
{MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME_7,     "ACCOUNT_RS_INTERVAL_TIME_7",     WORD_T,                 _OFFSET(accountRsIntervalTime7), _SIZE(accountRsIntervalTime7)},
{MIB_WLAN_RS_INTERVAL_TIME_1,             "RS_INTERVAL_TIME_1",             WORD_T,                 _OFFSET(rsIntervalTime1),        _SIZE(rsIntervalTime1)},
{MIB_WLAN_RS_INTERVAL_TIME_2,             "RS_INTERVAL_TIME_2",             WORD_T,                 _OFFSET(rsIntervalTime2),        _SIZE(rsIntervalTime2)},
{MIB_WLAN_RS_INTERVAL_TIME_3,             "RS_INTERVAL_TIME_3",             WORD_T,                 _OFFSET(rsIntervalTime3),        _SIZE(rsIntervalTime3)},
{MIB_WLAN_RS_INTERVAL_TIME_4,             "RS_INTERVAL_TIME_4",             WORD_T,                 _OFFSET(rsIntervalTime4),        _SIZE(rsIntervalTime4)},
{MIB_WLAN_RS_INTERVAL_TIME_5,             "RS_INTERVAL_TIME_5",             WORD_T,                 _OFFSET(rsIntervalTime5),        _SIZE(rsIntervalTime5)},
{MIB_WLAN_RS_INTERVAL_TIME_6,             "RS_INTERVAL_TIME_6",             WORD_T,                 _OFFSET(rsIntervalTime6),        _SIZE(rsIntervalTime6)},
{MIB_WLAN_RS_INTERVAL_TIME_7,             "RS_INTERVAL_TIME_7",             WORD_T,                 _OFFSET(rsIntervalTime7),        _SIZE(rsIntervalTime7)},

#endif
#ifdef _MULTIPLE_INIC_RS_SETTING_
{MIB_INIC_RS_IP_1,                "INIC_RS_IP_1",                IA_T,                   _OFFSET(rs5GIpAddr1),       _SIZE(rs5GIpAddr1)},
{MIB_INIC_RS_IP_2,                "INIC_RS_IP_2",                IA_T,                   _OFFSET(rs5GIpAddr2),       _SIZE(rs5GIpAddr2)},
{MIB_INIC_RS_IP_3,                "INIC_RS_IP_3",                IA_T,                   _OFFSET(rs5GIpAddr3),       _SIZE(rs5GIpAddr3)},
{MIB_INIC_RS_IP_4,                "INIC_RS_IP_4",                IA_T,                   _OFFSET(rs5GIpAddr4),       _SIZE(rs5GIpAddr4)},
{MIB_INIC_RS_IP_5,                "INIC_RS_IP_5",                IA_T,                   _OFFSET(rs5GIpAddr5),       _SIZE(rs5GIpAddr5)},
{MIB_INIC_RS_IP_6,                "INIC_RS_IP_6",                IA_T,                   _OFFSET(rs5GIpAddr6),       _SIZE(rs5GIpAddr6)},
{MIB_INIC_RS_IP_7,                "INIC_RS_IP_7",                IA_T,                   _OFFSET(rs5GIpAddr7),       _SIZE(rs5GIpAddr7)},
{MIB_INIC_RS_PORT_1,                      "INIC_RS_PORT_1",                      WORD_T,                 _OFFSET(rs5GPort1),                _SIZE(rs5GPort1)},
{MIB_INIC_RS_PORT_2,                      "INIC_RS_PORT_2",                      WORD_T,                 _OFFSET(rs5GPort2),                _SIZE(rs5GPort2)},
{MIB_INIC_RS_PORT_3,                      "INIC_RS_PORT_3",                      WORD_T,                 _OFFSET(rs5GPort3),                _SIZE(rs5GPort3)},
{MIB_INIC_RS_PORT_4,                      "INIC_RS_PORT_4",                      WORD_T,                 _OFFSET(rs5GPort4),                _SIZE(rs5GPort4)},
{MIB_INIC_RS_PORT_5,                      "INIC_RS_PORT_5",                      WORD_T,                 _OFFSET(rs5GPort5),                _SIZE(rs5GPort5)},
{MIB_INIC_RS_PORT_6,                      "INIC_RS_PORT_6",                      WORD_T,                 _OFFSET(rs5GPort6),                _SIZE(rs5GPort6)},
{MIB_INIC_RS_PORT_7,                      "INIC_RS_PORT_7",                      WORD_T,                 _OFFSET(rs5GPort7),                _SIZE(rs5GPort7)},
{MIB_INIC_RS_PASSWORD_1,                  "INIC_RS_PASSWORD_1",                  STRING_T,               _OFFSET(rs5GPassword1),            _SIZE(rs5GPassword1)},
{MIB_INIC_RS_PASSWORD_2,                  "INIC_RS_PASSWORD_2",                  STRING_T,               _OFFSET(rs5GPassword2),            _SIZE(rs5GPassword2)},
{MIB_INIC_RS_PASSWORD_3,                  "INIC_RS_PASSWORD_3",                  STRING_T,               _OFFSET(rs5GPassword3),            _SIZE(rs5GPassword3)},
{MIB_INIC_RS_PASSWORD_4,                  "INIC_RS_PASSWORD_4",                  STRING_T,               _OFFSET(rs5GPassword4),            _SIZE(rs5GPassword4)},
{MIB_INIC_RS_PASSWORD_5,                  "INIC_RS_PASSWORD_5",                  STRING_T,               _OFFSET(rs5GPassword5),            _SIZE(rs5GPassword5)},
{MIB_INIC_RS_PASSWORD_6,                  "INIC_RS_PASSWORD_6",                  STRING_T,               _OFFSET(rs5GPassword6),            _SIZE(rs5GPassword6)},
{MIB_INIC_RS_PASSWORD_7,                  "INIC_RS_PASSWORD_7",                  STRING_T,               _OFFSET(rs5GPassword7),            _SIZE(rs5GPassword7)},
{MIB_INIC_ACCOUNT_RS_INTERVAL_TIME_1,     "INIC_ACCOUNT_RS_INTERVAL_TIME_1",     WORD_T,                 _OFFSET(accountRs5GIntervalTime1), _SIZE(accountRs5GIntervalTime1)},
{MIB_INIC_ACCOUNT_RS_INTERVAL_TIME_2,     "INIC_ACCOUNT_RS_INTERVAL_TIME_2",     WORD_T,                 _OFFSET(accountRs5GIntervalTime2), _SIZE(accountRs5GIntervalTime2)},
{MIB_INIC_ACCOUNT_RS_INTERVAL_TIME_3,     "INIC_ACCOUNT_RS_INTERVAL_TIME_3",     WORD_T,                 _OFFSET(accountRs5GIntervalTime3), _SIZE(accountRs5GIntervalTime3)},
{MIB_INIC_ACCOUNT_RS_INTERVAL_TIME_4,     "INIC_ACCOUNT_RS_INTERVAL_TIME_4",     WORD_T,                 _OFFSET(accountRs5GIntervalTime4), _SIZE(accountRs5GIntervalTime4)},
{MIB_INIC_ACCOUNT_RS_INTERVAL_TIME_5,     "INIC_ACCOUNT_RS_INTERVAL_TIME_5",     WORD_T,                 _OFFSET(accountRs5GIntervalTime5), _SIZE(accountRs5GIntervalTime5)},
{MIB_INIC_ACCOUNT_RS_INTERVAL_TIME_6,     "INIC_ACCOUNT_RS_INTERVAL_TIME_6",     WORD_T,                 _OFFSET(accountRs5GIntervalTime6), _SIZE(accountRs5GIntervalTime6)},
{MIB_INIC_ACCOUNT_RS_INTERVAL_TIME_7,     "INIC_ACCOUNT_RS_INTERVAL_TIME_7",     WORD_T,                 _OFFSET(accountRs5GIntervalTime7), _SIZE(accountRs5GIntervalTime7)},
{MIB_INIC_RS_INTERVAL_TIME_1,             "INIC_RS_INTERVAL_TIME_1",             WORD_T,                 _OFFSET(rs5GIntervalTime1),        _SIZE(rs5GIntervalTime1)},
{MIB_INIC_RS_INTERVAL_TIME_2,             "INIC_RS_INTERVAL_TIME_2",             WORD_T,                 _OFFSET(rs5GIntervalTime2),        _SIZE(rs5GIntervalTime2)},
{MIB_INIC_RS_INTERVAL_TIME_3,             "INIC_RS_INTERVAL_TIME_3",             WORD_T,                 _OFFSET(rs5GIntervalTime3),        _SIZE(rs5GIntervalTime3)},
{MIB_INIC_RS_INTERVAL_TIME_4,             "INIC_RS_INTERVAL_TIME_4",             WORD_T,                 _OFFSET(rs5GIntervalTime4),        _SIZE(rs5GIntervalTime4)},
{MIB_INIC_RS_INTERVAL_TIME_5,             "INIC_RS_INTERVAL_TIME_5",             WORD_T,                 _OFFSET(rs5GIntervalTime5),        _SIZE(rs5GIntervalTime5)},
{MIB_INIC_RS_INTERVAL_TIME_6,             "INIC_RS_INTERVAL_TIME_6",             WORD_T,                 _OFFSET(rs5GIntervalTime6),        _SIZE(rs5GIntervalTime6)},
{MIB_INIC_RS_INTERVAL_TIME_7,             "INIC_RS_INTERVAL_TIME_7",             WORD_T,                 _OFFSET(rs5GIntervalTime7),        _SIZE(rs5GIntervalTime7)},
#endif
#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
{MIB_WLAN_SCH_ENABLED,	"WLAN_SCH_ENABLED",	BYTE_T,						_OFFSET(WlanSchEnabled),		_SIZE(WlanSchEnabled)},
{MIB_INIC_SCH_ENABLED,	"INIC_SCH_ENABLED",	BYTE_T,						_OFFSET(iNIC_SchEnabled),		_SIZE(iNIC_SchEnabled)},
{MIB_WIRE_SCH_NUM,			"WIRE_SCH_NUM",			BYTE_T,						_OFFSET(WirelessSchNum),		_SIZE(WirelessSchNum)},
{MIB_WIRE_SCH,					"WIRE_SCH",					WIRE_SCH_ARRAY_T,	_OFFSET(WirelessSchArray),	_SIZE(WirelessSchArray)},
#endif

#if defined(_CROSSBAND_)
{MIB_CROSSBAND_ENABLE,  "CROSSBAND_ENABLE",	BYTE_T,	_OFFSET(crossbandEnable),		_SIZE(crossbandEnable)},
#endif

#ifdef _DDNS_UID_BY_MAC_ //EDX, Robert Add 2015_04_09
{MIB_DDNS_UID,			"DDNS_UID",			STRING_T,			_OFFSET(ddnsUid),		_SIZE(ddnsUid)},
#endif

#ifdef _OPENVPN_ //EDX, Robert Add 2015_04_09
{MIB_OPENVPN_ENABLED,						"OPENVPN_ENABLED",				BYTE_T,			_OFFSET(openvpnEnabled),				_SIZE(openvpnEnabled)},
{MIB_OPENVPN_USER_NUM,					"OPENVPN_USER_NUM",				BYTE_T,			_OFFSET(openvpnUserNum),	_SIZE(openvpnUserNum)},
{MIB_OPENVPN_USER_TBL,					"OPENVPN_USER_TBL",				OPENVPN_USER_ARRAY_T,	_OFFSET(openvpnUserArray),	_SIZE(openvpnUserArray)},

{MIB_OPENVPN_ADV_SETUP,				"OPENVPN_ADV_SETUP",			BYTE_T,			_OFFSET(openvpnAdvSetup),			_SIZE(openvpnAdvSetup)},
//===== OpenVPN Server .config Settings =====
//Interface -> Type 0:TAP 1:TUN
{MIB_OPENVPN_INTER_TYPE,				"OPENVPN_INTER_TYPE",			BYTE_T,			_OFFSET(openvpnInterType),			_SIZE(openvpnInterType)},
//Protocol -> 0:TCP 1:UDP
{MIB_OPENVPN_PROTOCOL,					"OPENVPN_PROTOCOL",				BYTE_T,			_OFFSET(openvpnProtocol),				_SIZE(openvpnProtocol)},
//Server Port -> Default:443
{MIB_OPENVPN_SER_PORT,					"OPENVPN_SER_PORT",				WORD_T,			_OFFSET(openvpnserPort),				_SIZE(openvpnserPort)},
//Authorization Mode -> 0:TLS 1:Static key
{MIB_OPENVPN_AUTH_MODE,					"OPENVPN_AUTH_MODE",			BYTE_T,			_OFFSET(openvpnauthMode),				_SIZE(openvpnauthMode)},
//UsernamePassword Auth. Only -> 0:NO 1:YES
{MIB_OPENVPN_ONLY_AUTH_USER,		"OPENVPN_ONLY_AUTH_USER",	BYTE_T,			_OFFSET(openvpnonlyAuthUser),		_SIZE(openvpnonlyAuthUser)},
//VPN Subnet/Netmask (TUN)
{MIB_OPENVPN_SUBNET,						"OPENVPN_SUBNET",					IA_T,				_OFFSET(openvpnSubnet),					_SIZE(openvpnSubnet)},
{MIB_OPENVPN_NETMASK,						"OPENVPN_NETMASK",				IA_T,				_OFFSET(openvpnNetmask),				_SIZE(openvpnNetmask)},
//VPN START-IP/END-IP (TAP)
{MIB_OPENVPN_START_IP,					"OPENVPN_START_IP",				IA_T,				_OFFSET(openvpnStartIP),				_SIZE(openvpnStartIP)},
{MIB_OPENVPN_END_IP,						"OPENVPN_END_IP",					IA_T,				_OFFSET(openvpnEndIP),					_SIZE(openvpnEndIP)},
//Direct Client Traffic -> 0:NO 1:YES
{MIB_OPENVPN_REDIRECT_TRAFFIC,	"OPENVPN_REDIRECT_TRAFFIC",	BYTE_T,		_OFFSET(openvpnRedirectTraffic),	_SIZE(openvpnRedirectTraffic)},
//Push To LAN To Clients -> 0:NO 1:YES
{MIB_OPENVPN_PUSH_LAN,					"OPENVPN_PUSH_LAN",				BYTE_T,			_OFFSET(openvpnPushLAN),					_SIZE(openvpnPushLAN)},
//Respond DNS -> 0:NO 1:YES
{MIB_OPENVPN_RES_DNS,						"OPENVPN_RES_DNS",				BYTE_T,			_OFFSET(openvpnResDNS),						_SIZE(openvpnResDNS)},
#endif	

#ifdef _TEXTFILE_CONFIG_
{MIB_TMPSTRING,                         "TMPSTRING",                            STRING_T,               _OFFSET(tmpstring),             _SIZE(tmpstring)},
{MIB_TMPSTRING_5G,                              "TMPSTRING_5G",                         STRING_T,               _OFFSET(tmpstring5g),           _SIZE(tmpstring5g)},
#endif

#ifdef _DUAL_WAN_IGMP_
{MIB_DUAL_WAN_IGMP,			"DUAL_WAN_IGMP",		BYTE_T,			_OFFSET(Dual_WAN_IGMP),		_SIZE(Dual_WAN_IGMP)},
#endif

#ifdef _ADV_CONTROL_ //EDX patrick
{MIB_ADVCONTROL_ENABLE, "ADVCONTROL_ENABLE", BYTE_T, _OFFSET(manualavdcontrol_enable), _SIZE(manualavdcontrol_enable)},
#endif

#ifdef _WIFI_ROMAING_
{MIB_WIFIROMAING_SID,             	"WIFIROMAING_SID",                         STRING_T,               _OFFSET(wifiromaing_sid),                  _SIZE(wifiromaing_sid)},
{MIB_WIFIROMAING_SEQ,			"WIFIROMAING_SEQ",            BYTE_T,         _OFFSET(wifiromaing_seq),      _SIZE(wifiromaing_seq)},
{MIB_WIFIROMAING_SEQ5,                    "WIFIROMAING_SEQ5",            BYTE_T,         _OFFSET(wifiromaing_seq5),      _SIZE(wifiromaing_seq5)},
{MIB_WIFIROMAING_MASTER_BSSID,            "WIFIROMAING_MASTER_BSSID",                  BYTE6_T,                _OFFSET(wifiromaing_MasterBssid),             _SIZE(wifiromaing_MasterBssid)},
{MIB_WIFIROMAING_SNAP_INFO,               "WIFIROMAING_SNAP_INFO",            STRING_T,         _OFFSET(wifiromaing_snap_info),      _SIZE(wifiromaing_snap_info)},
{MIB_SINGLESIGNON,                       "SINGLESIGNON",                 BYTE_T,                 _OFFSET(singlesignon),            _SIZE(singlesignon)},
{MIB_WIFIROMAING_WIRELESS_TYPE,           "WIFIROMAING_WIRELESS_TYPE",            STRING_T,         _OFFSET(wifiromaing_wireless_type),      _SIZE(wifiromaing_wireless_type)},
{MIB_WIFIROMAING_CONNREFSITEBSSID,            "WIFIROMAING_CONNREFSITEBSSID",                  BYTE6_T,                _OFFSET(wifiromaing_Connrefsitebssid),             _SIZE(wifiromaing_Connrefsitebssid)},
{MIB_WIFIROMAING_STATUS,			"WIFIROMAING_STATUS",            BYTE_T,         _OFFSET(wifiromaing_status),      _SIZE(wifiromaing_status)},
{MIB_SINGLESIGNON_FREEZE,                       "SINGLESIGNON_FREEZE",                 BYTE_T,                 _OFFSET(singlesignon_freeze),            _SIZE(singlesignon_freeze)},
{MIB_WIFIROMAING_SYS_SID,             	"WIFIROMAING_SYS_SID",                         STRING_T,               _OFFSET(wifiromaing_sys_sid),                  _SIZE(wifiromaing_sys_sid)},
#endif
{0}
};

mib_table_entry_T hwmib_table[]={
{MIB_HW_BOARD_VER,			"HW_BOARD_ID",			BYTE_T,			_OFFSET_HW(boardVer),		_SIZE_HW(boardVer)},
{MIB_HW_NIC0_ADDR,			"HW_NIC0_ADDR",			BYTE6_T,		_OFFSET_HW(nic0Addr),		_SIZE_HW(nic0Addr)},
{MIB_HW_NIC1_ADDR,			"HW_NIC1_ADDR",			BYTE6_T,		_OFFSET_HW(nic1Addr),		_SIZE_HW(nic1Addr)},
{MIB_HW_WLAN_ADDR,			"HW_WLAN_ADDR",			BYTE6_T,		_OFFSET_HW(wlanAddr),		_SIZE_HW(wlanAddr)},
{MIB_HW_INIC_ADDR,			"HW_INIC_ADDR",			BYTE6_T,		_OFFSET_HW(inicAddr),		_SIZE_HW(inicAddr)},
{MIB_HW_REG_DOMAIN,			"HW_REG_DOMAIN",		BYTE_T,			_OFFSET_HW(regDomain),		_SIZE_HW(regDomain)},
#ifdef _TX_POWER_CONTROL_
{MIB_HW_TXPOWER_SW,			"HW_TXPOWER_SW",			BYTE_T,			_OFFSET_HW(txpower_sw),		_SIZE_HW(txpower_sw)},
#else
{MIB_HW_RF_TYPE,			"HW_RF_TYPE",			BYTE_T,			_OFFSET_HW(rfType),		_SIZE_HW(rfType)},
#endif
{MIB_HW_TX_POWER_CCK,			"HW_TX_POWER_CCK",		BYTE_ARRAY_T,		_OFFSET_HW(txPowerCCK),		_SIZE_HW(txPowerCCK)},
{MIB_HW_TX_POWER_OFDM,			"HW_TX_POWER_OFDM",		BYTE_ARRAY_T,		_OFFSET_HW(txPowerOFDM),	_SIZE_HW(txPowerOFDM)},
{MIB_HW_ANT_DIVERSITY,			"HW_ANT_DIVERSITY",		BYTE_T,			_OFFSET_HW(antDiversity),	_SIZE_HW(antDiversity)},
{MIB_HW_TX_ANT,                         "HW_TX_ANT",                    BYTE_T,                 _OFFSET_HW(txAnt),              _SIZE_HW(txAnt)},
#ifdef _TX_POWER_CONTROL_
{MIB_HW_TX_DELTAR0,                     "HW_TX_DELTAR0",                BYTE_T,                 _OFFSET_HW(hwtxdeltar0),        _SIZE_HW(hwtxdeltar0)},
{MIB_HW_TX_DELTAR1,                     "HW_TX_DELTAR1",                BYTE_T,                 _OFFSET_HW(hwtxdeltar1),        _SIZE_HW(hwtxdeltar1)},
{MIB_HW_TX_DELTAR2,                     "HW_TX_DELTAR2",                BYTE_T,                 _OFFSET_HW(hwtxdeltar2),        _SIZE_HW(hwtxdeltar2)},
{MIB_TX_POWER,			 	 "TX_POWER",			BYTE_T,			_OFFSET_HW(tx_power),		_SIZE_HW(tx_power)},
#else
{MIB_HW_CCA_MODE,			"HW_CCA_MODE",			BYTE_T,			_OFFSET_HW(ccaMode),		_SIZE_HW(ccaMode)},
{MIB_HW_LED_TYPE,			"HW_WLAN_LED_TYPE",		BYTE_T,			_OFFSET_HW(ledType),		_SIZE_HW(ledType)},
{MIB_HW_INIT_GAIN,			"HW_INIT_GAIN",			BYTE_T,			_OFFSET_HW(initGain),		_SIZE_HW(initGain)},
#endif
{MIB_HW_MODEL_NAME,			"HW_MODEL_NAME",		STRING_T,			_OFFSET_HW(model),		_SIZE_HW(model)},

{MIB_HW_COUNTRY_REG,			"HW_COUNTRY_REG",		BYTE_T,			_OFFSET_HW(countryReg),		_SIZE_HW(countryReg)},
{MIB_HW_COUNTRY_REG_ABAND,		"HW_COUNTRY_REG_ABAND",		BYTE_T,			_OFFSET_HW(countryRegABand),	_SIZE_HW(countryRegABand)},
{MIB_HW_COUNTRY_CODE,			"HW_COUNTRY_CODE",		STRING_T,		_OFFSET_HW(countryCode),	_SIZE_HW(countryCode)},
#if defined(_AUTOWPA_KEY_)
{MIB_HW_AUTOWPA_KEY,          "HW_AUTOWPA_KEY",        BYTE_T,         _OFFSET_HW(AutoWPAKey), _SIZE_HW(AutoWPAKey)},
{MIB_HW_AUTOWPA_NUMBER,          "HW_AUTOWPA_NUMBER",        BYTE6_T,         _OFFSET_HW(AutoWPANumber), _SIZE_HW(AutoWPANumber)},
#endif
#if defined(_SITECOM_AUTO_FW_UPGRADE_)
{MIB_HW_SC_ACTIVATION_GUI,	"HW_SC_ACTIVATION_GUI",		STRING_T,	_OFFSET_HW(SCActivationGUI),	_SIZE_HW(SCActivationGUI)},
{MIB_HW_SC_FEATURE,          	"HW_SC_FEATURE",		STRING_T,	_OFFSET_HW(SCFeature),		_SIZE_HW(SCFeature)},
{MIB_HW_SC_FEATURE_ENABLE,      "HW_SC_FEATURE_ENABLE",		STRING_T,	_OFFSET_HW(SCFeatureEnable),	_SIZE_HW(SCFeatureEnable)},
{MIB_HW_SC_NOT_AFTER_EPOCH,   	"HW_SC_NOT_AFTER_EPOCH",	STRING_T,	_OFFSET_HW(SCNotAfterEpoch),	_SIZE_HW(SCNotAfterEpoch)},
{MIB_HW_SC_SECRET_KEY,         	"HW_SC_SECRET_KEY",		STRING_T,	_OFFSET_HW(SCSecretKey),	_SIZE_HW(SCSecretKey)},
{MIB_HW_SC_MAC,         	"HW_SC_MAC",			STRING_T,	_OFFSET_HW(SCMAC),		_SIZE_HW(SCMAC)},
#endif
{0}
};
