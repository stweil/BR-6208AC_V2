/*
 *      Include file of mibtbl.c
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: mibtbl.h,v 1.1.1.1 2005/03/09 04:52:51 rex Exp $
 *
 */

#ifndef INCLUDE_MIBTBL_H
#define INCLUDE_MIBTBL_H

#include "apmib.h"

#ifdef WIN32
#ifdef FIELD_OFFSET
#undef FIELD_OFFSET
#endif
#endif

#define FIELD_OFFSET(type, field)	((unsigned long)(long *)&(((type *)0)->field))
#define _OFFSET(field)			((int)FIELD_OFFSET(APMIB_T,field))
#define _SIZE(field)			sizeof(((APMIB_T *)0)->field)

#define _OFFSET_HW(field)		((int)FIELD_OFFSET(HW_SETTING_T,field))
#define _SIZE_HW(field)			sizeof(((HW_SETTING_T *)0)->field)


typedef enum { BYTE_T, WORD_T, STRING_T, BYTE5_T, BYTE6_T,BYTE8_T, BYTE13_T, BYTE16_T, IA_T, WLAC_ARRAY_T,

#if defined(_MULTIPLE_WLAN_ACCESS_CONTROL_ )
	WLAC_ARRAY2_T,
	WLAC_ARRAY3_T,
	WLAC_ARRAY4_T,
#endif

#ifdef _EDIT_DNSPROXYURL_  //EDX patrick add
	DNSPROXYURL_ARRAY_T,
#endif

#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
    WIRE_SCH_ARRAY_T,
#endif

#ifdef _OPENVPN_
    OPENVPN_USER_ARRAY_T,
#endif
		SDHCP_ARRAY_T,
#ifdef _EZ_QOS_
		EZ_QOS_ARRAY_T,
#endif
#ifdef HOME_GATEWAY
		PORTFW_ARRAY_T, IPFILTER_ARRAY_T, PORTFILTER_ARRAY_T, MACFILTER_ARRAY_T, TRIGGERPORT_ARRAY_T, VSER_ARRAY_T, DMZ_ARRAY_T, URLB_ARRAY_T, ACPC_ARRAY_T, SROUT_ARRAY_T,WAN1_QOS_ARRAY_T,FTP_ARRAY_T,
#endif
#ifndef HOME_GATEWAY
		RSER_USR_ARRAY_T, RSER_CLT_ARRAY_T,
#endif
#ifdef WLAN_WPA
//		BYTE32_T,
#endif

		BYTE_ARRAY_T,INDEX_T,INDEXDOS_T,
#ifdef _LAN_WAN_ACCESS_
		INDEXRA_T,
#endif
#ifdef WLAN_WDS
                WDS_ARRAY_T,
#endif

#if defined(_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ )
		INIC_AC_ARRAY2_T,
		INIC_AC_ARRAY3_T,
		INIC_AC_ARRAY4_T,
#endif
		INIC_AC_ARRAY_T,

#ifdef _USB_NAS_
		NAS_USER_ARRAY_T,
#endif //_USB_NAS_

		DWORD_T

	     } TYPE_T;

// MIB value, id mapping table
typedef struct _mib_table_entry {
	int id;
	char name[32];
	TYPE_T type;
	int offset;
	int size;
} mib_table_entry_T;

extern mib_table_entry_T mib_table[], hwmib_table[];

#endif // INCLUDE_MIBTBL_H
