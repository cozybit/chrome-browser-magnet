#ifndef __MagnetStruct_H__
#define __MagnetStruct_H__

struct cached_env {
	zhash_t *peers;
	zhash_t *channels;
	zhash_t *pending_f;
	int checksum;
	char nodename [NODENAME_LEN + 1];
	void *publisher;                //  Publishes to channels
	void *subscriber;               //  Subscribes to channels
	bool use_udp_discovery;
	bool use_node_expiry;
	long liveness_timeout;
	char *ipaddress;                //  My IP address
	int pub_port;                   //  My PUB port
	int pull_port;                  //  My PULL port, or zero
	bool use_periodic_beacon;       //  sending periodic beacon
	long periodic_beacon_timeout;
	zctx_t *ctx;
};

// Peer info type
typedef struct peer_info_s {
    zctx_t *context;                //  ZeroMQ context
    int64_t last_seen;
    int64_t last_channel_request;
    zhash_t *channels;
    zhash_t *shared_channels;
    int waiting_beacon;
    int channel_list_retries;
    void *outgoing;                 //  PUSH socket to peer
    //  Information from UDP heartbeat packet
    int pub_port;                   //  Peer PUB port
    int pull_port;                  //  Peer PULL port, or zero
    char *ipaddress;                //  Peer IP address
    char *nodename;                 //  Peer nodename
    char *checksum;                 //  Checksum of all channels
    bool monitor_node;
    bool bRecreateOutgoing;         //  need to Re-Create Outgoing Socket
    int reasonOfParted;             //  reason of parted channel
} peer_t;

typedef struct _discovery_t {
    int pub_port;
    int pull_port;
    char ipaddress [INET_ADDRSTRLEN];
    char nodename [NODENAME_BUFSIZE+1];
    char checksum [CHECKSUM_LEN + 1];
    bool monitor_node;
} discovery_t;

// 2013.02.19 Merge from cozybit : magnet-fix-capi
typedef enum _magnet_dst_type_t {
    MAGNET_DST_UNKNOWN,
    MAGNET_DST_DIRECT,
    MAGNET_DST_CHANNEL,
    MAGNET_DST_GLOBAL
} magnet_dst_type_t;

typedef enum _magnet_frm_type_t {
    MAGNET_FRM_UNKNOWN,
    MAGNET_FRM_DATA,
    MAGNET_FRM_FILE,
    MAGNET_FRM_INTERNAL,
    MAGNET_FRM_NOTIFY // THIS SHOULD BE AN INTERNAL FRAME!
} magnet_frm_type_t;

typedef struct _magnet_msg_t {
    magnet_dst_type_t dst_t;
    char *destination;
    char *channel;
    char *origin;
    magnet_frm_type_t frm_t;
    zmsg_t *payload;
} magnet_msg_t;
// 2013.02.19 Merge from cozybit : magnet-fix-capi

typedef struct _magnet_file_msg_t {
    magnet_msg_t *header;
    char *mime_type;// 2013.02.19 Merge from cozybit : magnet-fix-capi
    char *exchange_id;
    char *path;
    char *hash;
    size_t filesize;
    char *ip;
    int port;
    char *msg1;
} magnet_file_msg_t;

typedef struct _channel_peer_arg {
    char *channel;          // The name of channel to match
    zlist_t *peers;         // List of nodes on channel
} channel_peer_arg;

typedef struct _net_interface_info {
    char name[MAX_NAME_LEN];
    char ip[INET_ADDRSTRLEN];
} net_interface_info;

typedef struct _magnet_mmap_t {	
#if (defined (__WINDOWS__))
    HANDLE fd;
#elif (defined (__UNIX__))
    int fd;
#else
    #error
#endif
    size_t size;
    void *data;
    char *path;
    char *hash;
    int ref_cnt;
#if (defined (__WINDOWS__))
    HANDLE mmap;
#endif
} magnet_mmap_t;

typedef struct _magnet_peer_t {

    zctx_t *ctx;        // context for the socket

    void *socket;       // DEALER socket connected to remote peer

    int64_t m_time;     // Last time something happened on this socket

    zhash_t *fetches;   // Hash of active fetches keyed with
                        // name-checksum pair
                        
    zlist_t *chunk_order; // Order to receive chunks in

    int has_pollitem;   // does the peer have pollset entry

    char peer_key [48];
    char poll_key [48];

    char *ip;          // DALER socket ip
    char *port;        // DALER socket port

} magnet_peer_t;

typedef struct _file_transfer_properties_t {
    char *exchange_id;
    char *type;
    char *destination;
    char *channel;
    int64_t m_time;        // Last access to this struct, used for timing out transfers    
    size_t offset;         // The current offset    
} file_transfer_properties_t;

typedef struct _file_transfer_t {
    magnet_mmap_t *m;      // mmap data
    char *key;             // The key of the transfer in outgoing_transfer table
    long timeout;          // Timeout
    size_t filesize;       // File size
    zlist_t *properties;   // transfer properties, keep a list if same file is shared multiple times
} file_transfer_t;

typedef struct _file_fetch_properties_t {

    char *tmp_name;    // Temp name, this is overridden for duplicates

    char *msg_channel; // Channel where the file was sent to
    char *msg_origin;  // Origin of the transmission
    char *msg_type;    // Mime-type
    char *msg_exch_id; // Exchange id used
    int canceled;

} file_fetch_properties_t;

// Used as a simple buffer for chunks arriving out of order
typedef struct _file_fetch_buffer_t {
    size_t offset;
    zframe_t *data;
} file_fetch_buffer_t;

// DONE:     state == 0
// ACTIVE:   state > STATE_ACTIVE_MIN && state < STATE_ACTIVE_MAX
// FAILURES: state >= STATE_FAILED
typedef enum _transfer_state_t {
    STATE_DONE = 0,
    STATE_NEED_SEND = 1,
    STATE_WAITING_REPLY = 2,
    STATE_FAILED = 20,
    STATE_CANCELED = 21
} transfer_state_t;

typedef enum _transfer_error_t{
    MAGNET_EVENT_NONE = 0,
    MAGNET_EVENT_PEER_NOT_FOUND=1,
    MAGNET_EVENT_MEMORY_ALLOC=2,
    MAGNET_EVENT_FILE_CREATE=3,
    MAGNET_EVENT_FILE_SIZE=4,
    MAGNET_EVENT_FAIL_MSG = 5,
    MAGNET_EVENT_TIMEOUT = 6, //window에서 ERROR_TIMEOUT 이 사용되므로 ERROR_TIMEOUT_1을 사용함
    MAGNET_EVENT_PREPARE = 7,
    MAGNET_EVENT_REJECT=11,
    MAGNET_EVENT_CANCEL=12,    
    MAGNET_EVENT_UNKNOWN = 99,    
}transfer_error_t;

typedef struct _file_fetch_t {

    // File properties
    char *path;         // Path of the remote file
    char *hash;         // The file hash
    char *tmp_path;     // The configured temp path
    FILE *handle;       // Local file handle for the transfer

    // Transfer properties
    size_t fsize;       // The size of the file
    size_t offset;      // The offset where the read is
    size_t chunk_size;  // what size chunk to request

    file_fetch_buffer_t chunk_buffer; // Buffer for chunks that were received out of order

    // State related params
    int head_sent;      // Has HEAD been sent for this request 1/0
    int retries;        // How many failures in a row
    transfer_state_t state;  // Fetch state
    int64_t m_time;     // Last time something happened on this fetch

    long chunk_timeout;  // Timeout for each chunk
    int chunk_retries;  // How many times to retry chunk in addition to the initial try

    // Magnet properties for sending notifications
    file_fetch_properties_t properties;

    // List of duplicate requests
    zlist_t *duplicates;

} file_fetch_t;

typedef struct _transfer_cancel_arg_t {
    zhash_t *outgoing_transfers;
    char *exchange_id;
    char *action;
    void *pipe;
} transfer_cancel_arg_t;

typedef struct _fetch_cancel_arg_t {
    zhash_t *peer_connections;
    char *exchange_id;
    magnet_peer_t *peer;
    char *action;	
    void *pipe;	
} fetch_cancel_arg_t;

typedef struct _transfer_fail_arg_t {
    zhash_t *outgoing_transfers;
    void *pipe;
} transfer_fail_arg_t;

typedef enum _channel_zhash_operator_t {
    CHANNEL_DIFF,
    CHANNEL_INTERSECT
} channel_zhash_operator_t;

typedef struct _channel_zhash_arg_t {
    zhash_t *compare;
    zhash_t *result;
    channel_zhash_operator_t mode;
} channel_zhash_arg_t;

typedef struct _notify_callback_arg_t {
    cached_env *env;
    peer_t *peer;
} notify_callback_arg_t;

#endif
