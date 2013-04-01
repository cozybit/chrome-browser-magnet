/**
 * This is the main "configuration" file for constants
 */

#ifndef _CONSTANTS_H_
# define _CONSTANTS_H_
#   if (defined (__WINDOWS__))
#include <windows.h>
#include <WinSock.h>
#include <tchar.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#define PATH_MAX 255
#define strdup _strdup
#define strncpy strncpy_s
#define open _open
#define close _close
#define fdopen _fdopen
#define unlink _unlink
#define getpid _getpid
#	else
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <czmq.h>
#	endif
// Some magic constants
#define CHECKSUM_LEN            22
#define NODENAME_LEN            16
#define NODENAME_BUFSIZE        24

//  For UDP heartbeating subprotocol
#define HEARTBEAT_PROTOCOL      "02"
#define HEARTBEAT_PORT          9006
#define HEARTBEAT_INTERVAL      1000    //  msecs
#define HEARTBEAT_MAXSIZE       255     //  Maximum size of discovery message
#define MAX_HEARTBEAT_FIELD_CNT 	16   //  optional fields are added for emulator case only

#define EMULATOR_PEER_IP      "EIP:"   // Emulator Peer IP Address
#define EMULATOR_ROUTER_IP "ERI:"   // Emulator Router IP Address

#define MAX_IFACE 256
#define MAX_NAME_LEN 256
#define MAC_STR_LEN 18

// Name in the log
#define APP_NAME1 "libmagnet"

// How long it takes to expire active transfer
#define TRANSFER_EXPIRY_MSEC 180000

// How often to check for expired entries
#define TRANSFER_EXPIRY_CHECK_MSEC 10000

// The chunk size, default to 100k
// Hack: make large enough to hold a SmartMeeting slide :) 
#define TRANSFER_CHUNK_SIZE 305600

// Transfer chunk retry
#define TRANSFER_CHUNK_RETRIES 2

// How long to wait for a chunk, 10 secs by default
#define TRANSFER_CHUNK_RETRY_MSEC 20000

// The last six 'X' are replaced with random string
#define TMP_FILE_PATTERN "magnet.XXXXXX"

// Message type for internal messages
#define MAGNET_INTERNAL_MIME_TYPE "application/magnet-internal"
#define MAGNET_INTERNAL_MIME_TYPE_LEN 27

// The public channel
#define MAGNET_CORE_PUBLIC_CHANNEL "Magnet"

// interface type for the android emulator AVD
const char INTERFACE_NAME_AVD[] 				= "eth0";
const char ANDROID_AVD_IP_ADDRESS []				= "10.0.2.15";
const char ANDROID_AVD_BCAST_INTERFACE_ADDR[] 	= "10.0.2.255";

// How often to retry the channel request
#define MAGNET_CHANNEL_RETRY_MSEC 2000

// How long before peers timeout
#define EXPIRATION_TIME_IN_MSECS 15000

// After how much idle time send beacon
#define MAGNET_BEACON_TIMEOUT 5000

// Whether to send head request in xfer
#define MAGNET_SKIP_HEAD 1

// Whether to do zero copy from mmap to network
#define MAGNET_ZERO_COPY 0

// How many times to retry channel list
#define MAGNET_CHANNEL_LIST_RETRIES 5

#ifndef MAX
#    define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#    define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define FOREACH_CONSTNESS const

#endif /* _CONSTANTS_H_ */

