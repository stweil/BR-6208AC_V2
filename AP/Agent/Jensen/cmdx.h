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
typedef unsigned char   nuint8;
typedef unsigned short  nuint16;
typedef unsigned long   nuint32;

#define AGENT_PORT			13379

/************************************************
* Constant defines                              *
*************************************************/
#define MAC_ID_LEN          6
#define SIGNATURE_LEN       6
#define IP_ADDRESS_LEN      4
#define HEADER_SIZE			16

/************************************************
* FunctionOp commands defines                   *
*************************************************/

/* FuncType COMMANDS =================== */
enum FuncTypes
{
   REQUEST,
   RESPONSE,
   FAULT,
};

/* MainCmd COMMANDS ==================== */
#define SEARCH_AP           0xA0

/************************************************
* SEARCH_GB                                     *
*************************************************/
typedef struct _STRU_Header
{
    nuint8          MacID[MAC_ID_LEN];
    nuint8          Signature[SIGNATURE_LEN];
    nuint8          FuncType;
    nuint8          Function;
    nuint8          CheckSum[2];
} STRU_Header, *PSTRU_Header;

typedef struct _STRU_Reply_Search
{
    STRU_Header     Header;
    nuint8          VendorID;
    nuint8          DeviceModel[23];
    nuint8          IPAddr[IP_ADDRESS_LEN];
    nuint8          Subnet[IP_ADDRESS_LEN];
    nuint8          Reserve[256];   
} STRU_Reply_Search;

#endif

