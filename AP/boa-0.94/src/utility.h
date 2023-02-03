/*
 *      Include file of utility.c
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: utility.h,v 1.1.1.1 2005/03/09 04:52:51 rex Exp $
 *
 */

#ifndef INCLUDE_UTILITY_H
#define INCLUDE_UTILITY_H

//typedef enum { IP_ADDR, SUBNET_MASK, DEFAULT_GATEWAY, HW_ADDR } ADDR_T;
typedef enum { IP_ADDR, SUBNET_MASK, DEFAULT_GATEWAY, HW_ADDR, DHCPGATEWAYIP_ADDR, DHCPNAMESERVER_ADDR } ADDR_T;

/* type define */
struct user_net_device_stats {
    unsigned long long rx_packets;	/* total packets received       */
    unsigned long long tx_packets;	/* total packets transmitted    */
    unsigned long long rx_bytes;	/* total bytes received         */
    unsigned long long tx_bytes;	/* total bytes transmitted      */
    unsigned long rx_errors;	/* bad packets received         */
    unsigned long tx_errors;	/* packet transmit problems     */
    unsigned long rx_dropped;	/* no space in linux buffers    */
    unsigned long tx_dropped;	/* no space available in linux  */
    unsigned long rx_multicast;	/* multicast packets received   */
    unsigned long rx_compressed;
    unsigned long tx_compressed;
    unsigned long collisions;

    /* detailed rx_errors: */
    unsigned long rx_length_errors;
    unsigned long rx_over_errors;	/* receiver ring buff overflow  */
    unsigned long rx_crc_errors;	/* recved pkt with crc error    */
    unsigned long rx_frame_errors;	/* recv'd frame alignment error */
    unsigned long rx_fifo_errors;	/* recv'r fifo overrun          */
    unsigned long rx_missed_errors;	/* receiver missed packet     */
    /* detailed tx_errors */
    unsigned long tx_aborted_errors;
    unsigned long tx_carrier_errors;
    unsigned long tx_fifo_errors;
    unsigned long tx_heartbeat_errors;
    unsigned long tx_window_errors;
};

/* Entry info scanned by site survey */

#define SSID_LEN	32
#define	MAX_BSS_DESC	64

typedef struct _OCTET_STRING {
    unsigned char *Octet;
    unsigned short Length;
} OCTET_STRING;


typedef enum _BssType {
    infrastructure = 1,
    independent = 2,
} BssType;


typedef	struct _IbssParms {
    unsigned short	atimWin;
} IbssParms;


typedef enum _Capability {
    cESS 		= 0x01,
    cIBSS		= 0x02,
    cPollable		= 0x04,
    cPollReq		= 0x01,
    cPrivacy		= 0x10,
    cShortPreamble	= 0x20,
} Capability;


typedef enum _Synchronization_Sta_State{
    STATE_Min			= 0,
    STATE_No_Bss		= 1,
    STATE_Bss			= 2,
    STATE_Ibss_Active	= 3,
    STATE_Ibss_Idle		= 4,
    STATE_Act_Receive	= 5,
    STATE_Pas_Listen	= 6,
    STATE_Act_Listen	= 7,
    STATE_Join_Wait_Beacon = 8,
    STATE_Max			= 9
} Synchronization_Sta_State;


typedef enum _wlan_mac_state {
    STATE_DISABLED=0, STATE_IDLE, STATE_SCANNING, STATE_STARTED, STATE_CONNECTED
} wlan_mac_state;

typedef enum _wlan_wds_state {
    STATE_WDS_EMPTY=0, STATE_WDS_DISABLED, STATE_WDS_ACTIVE
} wlan_wds_state;

typedef struct _bss_info {
    unsigned char state;
    unsigned char channel;
    unsigned char txRate;
    unsigned char bssid[6];
    unsigned char rssi, sq;	// RSSI  and signal strength    
    unsigned char ssid[SSID_LEN+1];
} bss_info;

typedef struct _WDS_INFO {
	unsigned char	state;
	unsigned char	addr[6];
	unsigned long	tx_packets;
	unsigned long	rx_packets;
	unsigned long	tx_errors;
	unsigned char	txOperaRate;
} WDS_INFO_T, *WDS_INFO_Tp;
/*
typedef struct _BssDscr {
    unsigned char bdBssId[6];
    unsigned char bdSsIdBuf[SSID_LEN];
    OCTET_STRING  bdSsId;
    BssType bdType;
    unsigned short bdBcnPer;			// beacon period in Time Units
    unsigned char bdDtimPer;			// DTIM period in beacon periods
    unsigned long bdTstamp[2];			// 8 Octets from ProbeRsp/Beacon
    IbssParms bdIbssParms;			// empty if infrastructure BSS
    unsigned short bdCap;				// capability information
    unsigned char ChannelNumber;			// channel number
    unsigned char bdBrates;			// bit 0,1,2,3 ==> 1, 2, 5.5, 11 Mbps
    unsigned char bdSupportRates;			// bit 0,1,2,3 ==> 1, 2, 5.5, 11 Mbps
    unsigned char bdsa[6];			// SA address
    unsigned char rssi, sq;			// RSSI and signal strength
    unsigned char network;
} BssDscr, *pBssDscr;
*/

typedef struct _BssDscr {
    unsigned char bdBssId[6];
    unsigned char bdSsIdBuf[SSID_LEN];
    OCTET_STRING  bdSsId;
    BssType bdType;
    unsigned short bdBcnPer;            // beacon period in Time Units
    unsigned char bdDtimPer;            // DTIM period in beacon periods
    unsigned long bdTstamp[2];          // 8 Octets from ProbeRsp/Beacon
    IbssParms bdIbssParms;          // empty if infrastructure BSS
    unsigned short bdCap;               // capability information
    unsigned char ChannelNumber;            // channel number
    unsigned long bdBrates;
    unsigned long bdSupportRates;
    unsigned char bdsa[6];          // SA address
    unsigned char rssi, sq;         // RSSI and signal strength
    unsigned char network;          // 1: 11B, 2: 11G, 4:11G
} BssDscr, *pBssDscr;

typedef struct _SITE_SURVEY
{
	unsigned char          channel[5];
	unsigned char          ssid[34];
	unsigned char          slength[3];
	unsigned char          bssid[21];
	unsigned char          webstatus[9];
	unsigned char          authmode[15];
	unsigned char          signal[10];
	unsigned char          rssi[7];
	unsigned char          wirelessmode[8];
	unsigned char          networktype[4];
	unsigned char          extch[7];
}	SITE_SURVEY;

typedef struct _SITE_SURVEY5G
{
	unsigned char          channel[5];
	unsigned char          ssid[34];
	unsigned char          slength[3];
	unsigned char          bssid[21];
	unsigned char          webstatus[9];
	unsigned char          authmode[15];
	unsigned char          signal[10];
	unsigned char          rssi[7];
	unsigned char          wirelessmode[8];
	unsigned char          networktype[4];
	unsigned char          extch[7];
	#if 1
	unsigned char          ch_width[7];
	#endif
}	SITE_SURVEY5G;



typedef struct _sitesurvey_status {
    unsigned char number;
    SITE_SURVEY bssdb[MAX_BSS_DESC];
} SS_STATUS_T, *SS_STATUS_Tp;


typedef struct _sitesurvey_status5g {
    unsigned char number;
    SITE_SURVEY5G bssdb[MAX_BSS_DESC];
} SS_STATUS_T5G, *SS_STATUS_Tp5G;

//2007.03.08 Kyle
//fixed inet_ntoa,inet_aton problem
 char *fixed_inet_ntoa(char *in) ;
 int fixed_inet_aton(const  char *cp,char *inp) ;
 //int fixed_inet_aton(const char *cp,struct in_addr *inp) ;
 
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_) 
	static char *StrReplace(char *Str, char *OldStr, char *NewStr);
	char *CharFilter(char *Str);
	char *CharFilter2(char *Str);
#endif
int getWlStaNum( char *interface, int *num );
int getWlStaInfo( char *interface,  WLAN_STA_INFO_Tp pInfo );
int getInicStaInfo( char *interface,  INIC_STA_INFO_Tp pInfo );
int getInAddr(char *interface, ADDR_T type, void *pAddr);
int getDefaultRoute(char *interface, struct in_addr *route);
int getWlSiteSurveyResult(char *interface, unsigned char *pStatus, int iSize );
int getWlSiteSurveyResult_1(char *interface, unsigned char *pStatus );
int getWlSiteSurveyRequest(char *interface, char *pStatus);
int getWlJoinRequest(char *interface, pBssDscr pBss, unsigned char *res);
int getWlJoinResult(char *interface, unsigned char *res);
int getWlBssInfo(char *interface, bss_info *pInfo);
extern pid_t find_pid_by_name( char* pidName);
int getWdsInfo(char *interface, char *pInfo);
int getAPChannel (char *interface, char *pInfo);
int get_file (char *file, char *ptr_out, int out_size);
#if 0
int setInAddr(char *interface, ADDR_T type, void *addr);
int addDefaultRoute(char *dev, void *route);
int deleteDefaultRoute(char *dev, void *route);
#endif
int getStats(char *interface, struct user_net_device_stats *pStats, int op_mode);



#endif // INCLUDE_UTILITY_H
