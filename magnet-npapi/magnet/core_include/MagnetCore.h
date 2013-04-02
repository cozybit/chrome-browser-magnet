#pragma once
#include <string>

#ifdef ZMQ_STATIC_LIBRARY
#include "zmq.h"
#include "czmq.h"
#else
#include <zmq.h>
#include <czmq.h>
#endif

#if (defined (__WINDOWS__))
#include <winsock2.h>
#elif (defined (__UNIX__) || defined (__ANDROID__))
#if defined (__ANDROID__)
#include <linux/wireless.h>
#endif
#include <netinet/in.h>
#include <net/if.h>
#else
    #error
#endif

#include "log.h"
#include "constants.h"
#include "MagnetStruct.h"
#include "MagnetUtil.h"
#include "MagnetMsg.h"
#include "MagnetUdp.h"
#include "MagnetPeer.h"
#include "MagnetSend.h"
#include "MagnetCallBack.h"
#include "FileTransferThread.h"
#include "FileTransferIO.h"
#include "FileTransferFetch.h"

enum {
    DISAPPEAR_NONE = 0,
    // expected leave
    DISAPPEAR_EXPECTED_LEAVE = 1,
    // unexpected leave
    DISAPPEAR_UNEXPECTED_LEAVE = 2,
    // case of network re-connect or ip-based discoverNode
    DISAPPEAR_RECONNECT = 3,
    // call undiscoverNode
    DISAPPEAR_UNDISCOVER = 4,
    // received "HALT" msg from remote side node.
    DISAPPEAR_RECV_HALT = 5
};

void notify_node_disappear (cached_env *cached, peer_t *peer);
int getMagnetSeqNum(void);

// 2013.02.19 Merge from cozybit : magnet-fix-capi
#ifndef tblsize(x)
#define tblsize(x)      (sizeof (x) / sizeof ((x) [0]))
#endif
// 2013.02.19 Merge from cozybit : magnet-fix-capi

#define update_mtime(my_struct) (my_struct->m_time = zclock_time ())

using namespace std;

extern char tmp_path_default [PATH_MAX];


static int debug_verbose = 1;

extern net_interface_info selected_info;

extern char router_ip[INET_ADDRSTRLEN];
extern bool isEmulator;
static int fixedPubPortChecksumBase = 100000;

class magnet_core_t
{
    public:
        magnet_core_t();
        ~magnet_core_t();
		
        static void print_zmsg(char *p, zmsg_t *msg);		
        int init();       
        void start();
        int stop();
        int release();
        void setUsingInterface(string interfaceInfo);
        char* getActiveInterface();
        string getLocalName();
        int getDiscovery_PortNumber();
        string getDiscovery_IP();
        void set_UDP_DiscoverMode(bool value);
        void set_Node_Expiry(bool value);
        void set_PeriodicBeacon(bool value);
        void set_PeriodicBeacon_TimeOut(long value);
        /*
        void stop();
        void release();
        */
    private:
        struct cached_env mCached;
        zctx_t *mCtx;
};

//twkim temporary
#ifdef WINAPI_PARTITION_PHONE_RESTRICTED
#define zsocket_disconnect(socket, fmt, ...)
#endif

#include "MeshMessaging.h"