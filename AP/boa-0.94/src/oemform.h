/*
 *      Include file of form handler
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: apform.h,v 1.2 2005/05/13 07:52:54 tommy Exp $
 *
 */


#ifndef _INCLUDE_OEMFORM_H
#define _INCLUDE_OEMFORM_H

#include "apmib.h"

typedef enum {

MIB_OEM_START = APMIB_MIB_END,

#ifdef _Customer_
#ifdef _POWERSAVINGLOGITEC_
MIB_PS_SCH_ENABLED,
MIB_PS_SCH_LEDSWITCH,
MIB_PS_SCH_WIREDLAN,
MIB_PS_SCH_WIRELESSLAN,
MIB_PS_SCH_Day0,
MIB_PS_SCH_Day1,
MIB_PS_SCH_Day2,
MIB_PS_SCH_Day3,
MIB_PS_SCH_Day4,
MIB_PS_SCH_Day5,
MIB_PS_SCH_Day6,
#endif //_POWERSAVINGLOGITEC_

#ifdef _USB_NAS_
MIB_NAS_PS_ENABLED,
MIB_NAS_PS_TIMER,
MIB_NAS_ACCESS_ENABLED,
MIB_NAS_USER_ADD,
MIB_NAS_USER_DEL,
MIB_NAS_USER_EDIT,
MIB_NAS_USER_DELALL,
MIB_NAS_USER_ENABLED,
MIB_NAS_USER_NUM,
MIB_NAS_USER,
MIB_NAS_FOLDER_ENABLED,
MIB_NAS_FOLDER_NAME,
MIB_NAS_FOLDER_MEMO,
MIB_NAS_FOLDER_GROUP_NAME,
MIB_NAS_WAN_ENABLED,
MIB_NAS_DLNA_ENABLED,
#endif //_USB_NAS_

#endif //_Logitec_
#ifdef _Customer_AUTO_FW_UPGRADE_
MIB_SCS_ENABLE,
MIB_SC_FW_ACTION,
MIB_SC_FW_ON_SERVER,
MIB_SC_FW_VER,
MIB_SC_DNS,
#endif
MIB_OEM_END,
};

#ifdef _Customer_
#ifdef _USB_NAS_
#define MAX_NAS_USER_NUM	4
typedef struct nas_user_entry {
        unsigned char name[16];
        unsigned char password[16];
	unsigned char memo[12];
	unsigned char access;		//0: no, 1: read, 2:full
} __PACK__ NAS_USER_T, *NAS_USER_Tp;
#endif //_USB_NAS_

typedef struct oem_struct_header {
#ifdef _POWERSAVINGLOGITEC_
	unsigned char PsSchEnabled;
	unsigned char PSLedswitch;
	unsigned char PSWiredLan;
	unsigned char PSWirelessLan;
	unsigned char day0[50]; // Sun
	unsigned char day1[50]; // Mon
	unsigned char day2[50]; // Tue
	unsigned char day3[50]; // Wed
	unsigned char day4[50]; // Thu
	unsigned char day5[50]; // Fri
	unsigned char day6[50]; // Sat
#endif //_POWERSAVINGLOGITEC_

#ifdef _USB_NAS_
	unsigned char NAS_PS_Enabled;
	unsigned char NAS_PS_Timer;
	
	unsigned char NAS_Access_Enabled;

	unsigned char NAS_Folder_Enabled;
	unsigned char NAS_Folder_Name[16];
	unsigned char NAS_Folder_Memo[16];
	unsigned char NAS_Folder_Group_Name[16];

	unsigned char NAS_User_Enabled;
        unsigned char NAS_User_Num;
	NAS_USER_T Nas_User[4];

	unsigned char NAS_WAN_Enabled;
	unsigned char NAS_DLNA_Enabled;
#endif //_USB_NAS_

} __PACK__ OEM_STRUCTURE_T, *OEM_STRUCTURE_Tp;

#else	//_Logitec_
typedef struct oem_struct_header {
#ifdef _Customer_AUTO_FW_UPGRADE_
	unsigned char SCS_Enable[64];
	unsigned char SC_FW_Action[64];
	unsigned char SC_FW_OnServer[64];
	unsigned char SC_FW_Ver[64];
	unsigned char SC_DNS[4];
#endif

} __PACK__ OEM_STRUCTURE_T, *OEM_STRUCTURE_Tp;
#endif	//_Logitec_

#endif	//_INCLUDE_OEMFORM_H
