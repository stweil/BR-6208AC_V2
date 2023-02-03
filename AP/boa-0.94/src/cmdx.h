/*****************************************************************************
 **                                                                          **
 **                       Edimax Proprietary Document                        **
 **                               AP Wizard                                  **
 **                    Copyright (c) 2008-2009 Edimax Inc.                   **
 **                                                                          **
 **   No.3,Wu-Chuan 3rd Road, Wu-Ku Lndustrial Pack. Taipei Hsien, Taiwan    **
 **               TEL : 886-2-7739-6888 FAX : 886-2-7739-6887                **
 **         E-mail:sales@edimax.com.tw      http://www.edimax.com            **
 **                                                                          **
 *****************************************************************************/


#ifndef CMDX_H
#define CMDX_H

#define UDP_PORT 					0x5050 //Plug local port: 0x5050

#ifdef _WIFI_ROMAING_
	#define LIFE_UDP_PORT                         0x6262 //Life Plug local port: 0x6262
#else
	#define LIFE_UDP_PORT				0x6161 //Life Plug local port: 0x6161
#endif

#define MAX_BROADCAST_STRING_SIZE   1514
#define PROTOCOL_VER	"000000"
#define WEBSERVER_PORT	80

/*****************************************************************************
 **  FILE NAME:
 **      cmdx.h
 **
 **  HISTORY:
 **      12/10/2008 create
 **
 **  FUNCTION:
 **      This file contains the data structures for get system info.
 **
 *****************************************************************************/

/************************************************
 * Type defines                                  *
 *************************************************/
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

/************************************************
 * Constant defines                              *
 *************************************************/
#define MAC_ID_LEN			6
#define SIGNATURE_LEN_AGENT		12
#define IP_ADDR_LEN			4
#define HEADER_SIZE			22

/************************************************
 * FunctionOp commands defines                   *
 *************************************************/

/* FuncType COMMANDS =================== */
enum FuncTypes {
	REQUEST, RESPONSE, FAULT,
};

/* MainCmd COMMANDS ==================== */
//EDXW #define SEARCH_AP           0xA0
#define SEARCH_PLUG         0xA1
//#define SET_CMD             0xB0

/************************************************
 * SEARCH_GB                                     *
 *************************************************/
typedef struct _STRU_Header {
	uint8 MacID[MAC_ID_LEN];          // app 帶 0xFFFFFFFFFFFF, Plug 回真實 ID
	uint8 Signature[SIGNATURE_LEN_AGENT];   // 字串 "EDIMAX" 不足 12 碼後面補 0x00, 文字最多11碼
	uint8 FuncType;                   // app帶 REQUEST, Plug 回 RESPONSE
	uint8 Function;				   	  // SEARCH_PLUG
	uint8 CheckSum[2];                // CheckSum[0] = ~FuncType, CheckSum[1] = ~Function;
} STRU_Header, *PSTRU_Header;

typedef struct _STRU_Reply_Search {
	STRU_Header Header;
	uint8 SP_ModelName[14];           // 字串 Shakim 定義, 後面補滿 0x00, 文字最多 13 碼
	uint8 SP_FW_Ver[8];               // 字串 "1.00" 不足 8 碼後面補滿 0x00, 文字最多 7 碼
	uint8 SP_Name[128];               // App 定的 Name, 可用 unicode 存, 最多存 63 個字, 目前備用
	uint16 SP_Web_Port;
	uint8 SP_IPAddr[IP_ADDR_LEN];
	uint8 SP_Subnet[IP_ADDR_LEN];
	uint8 SP_Gateway[IP_ADDR_LEN];
} STRU_Reply_Search, *PSTRU_Reply_Search;

/************************************************
 * LIFE_SEARCH_GB                                     *
 *************************************************/
enum {
	SEARCH_BROADCAST = 1, SEARCH_UNICAST = 2, SEARCH_SETUP = 3
};

typedef struct {
	uint8 MacID[MAC_ID_LEN];	// app 帶 0xFFFFFFFFFFFF, Plug 回真實 ID
	uint8 FuncType;			// 0 = request(APP to Device), 1 = response(Device to APP), 2 = fault
	uint8 Function;	// 0x01: Search (Broadcast reply),0x02: Search (Unicast reply), 0x03: Setup (Broadcast / Unicast  來  Broadcast/ Unicast reply),  only accept myself MAC
	uint8 CheckSum[2];			// CheckSum[0] = ~FuncType, CheckSum[1] = ~Function;
	char DataLen[4];			// JSON data length (0 ~ 9999(string))
} LIFE_HEADER, *PLIFE_HEADER;

typedef struct {
	LIFE_HEADER Header;
	char data[MAX_BROADCAST_STRING_SIZE - sizeof(LIFE_HEADER)];
} LIFE_STRU;

#endif //CMDX_H

