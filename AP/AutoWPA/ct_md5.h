/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************/


/****************************************************************************
    Module Name:
    MD5

    Abstract:
    RFC1321: The MD5 Message-Digest Algorithm
    
    Revision History:
    Who         When            What
    --------    ----------      ------------------------------------------
    Eddy        2008/11/24      Create md5
***************************************************************************/

/* Algorithm options */

typedef unsigned int	UINT32;
typedef unsigned long long	UINT64;
typedef unsigned char UINT8;
typedef unsigned int UINT;

#define MD5_BLOCK_SIZE    64 /* 512 bits = 64 bytes */
#define MD5_DIGEST_SIZE   16 /* 128 bits = 16 bytes */
typedef struct {
    UINT32 HashValue[4];
    UINT64 MessageLen;
    UINT8  Block[MD5_BLOCK_SIZE];
    UINT   BlockLen;
} MD5_CTX_STRUC, *PMD5_CTX_STRUC;

void MD5_Init ( MD5_CTX_STRUC *pMD5_CTX );
void MD5_Hash ( MD5_CTX_STRUC *pMD5_CTX );
void MD5_Append ( MD5_CTX_STRUC *pMD5_CTX, const UINT8 Message[], UINT MessageLen);
void MD5_End ( MD5_CTX_STRUC *pMD5_CTX, UINT8 DigestMessage[]);
void RT_MD5 ( const UINT8 Message[], UINT MessageLen, UINT8 DigestMessage[]);
