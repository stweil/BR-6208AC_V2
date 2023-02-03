#ifndef _MYTEST_H
#define _MYTEST_H

#define LOCAL_AGENT_VER "0.2"
#define LIFE_LOCAL_AGENT_VER "0.1"

#include <linux/ip.h>
#include <linux/udp.h>
#include "cmdx.h"

struct packet_header {
	struct iphdr ip;
	struct udphdr udp;
};

#endif

