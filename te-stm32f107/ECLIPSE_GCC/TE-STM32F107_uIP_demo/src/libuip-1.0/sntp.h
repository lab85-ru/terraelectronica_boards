//
//  $Id: sntp.h 331 2008-11-09 16:59:47Z jcw $
//  $Revision: 331 $
//  $Author: jcw $
//  $Date: 2008-11-09 11:59:47 -0500 (Sun, 09 Nov 2008) $
//  $HeadURL: http://tinymicros.com/svn_public/arm/lpc2148_demo/trunk/uip/apps/sntp/sntp.h $
//

#ifndef _SNTP_H_
#define _SNTP_H_


#define SNTP_SERVER_PORT 123

#include <time.h>

#include "uip_timer.h"

//
//
//
#define SNTP_TIMEOUT 10
#define SNTP_RETRIES 10

//
//
//
struct sntpState_t
{
  struct pt pt;
  struct uip_udp_conn *conn;
  struct timer timer;
  int retry;
  void (*onSyncCallback) (time_t *epochSeconds);
} ;



typedef struct sntpState_t uip_udp_appstate_sntp;

//
//
//
int sntpSync (uip_ipaddr_t *ipaddr, void (*syncedCallback) (time_t *epochSeconds));
void sntp_appcall (void);

#endif
