#ifndef __FileTransferIO_H__
#define __FileTransferIO_H__
#include "MagnetCore.h"

size_t handle_dealer_activity (zhash_t *peer_connections, zmq_pollitem_t *pollset, size_t poll_size, int check_expire);
zmq_pollitem_t * shrink_sparse_pollset (zmq_pollitem_t **pollset, size_t *poll_size, size_t deleted);
void expire_active_transfers (void *pipe, zhash_t *outgoing_transfers);
void send_finish_message (void *pipe, file_fetch_t *fetch, const char *status);
zmsg_t* handle_head_request (zhash_t *outgoing_transfers, zmsg_t *request);
zmsg_t* handle_get_request (void *pipe, zhash_t *outgoing_transfers, zmsg_t *request);
void file_notify_fail_msg_to_core(void* pipe, char* channel, char* node, char* type, char* exchangeid, char* path, char *hash, size_t offset, int reason);

extern void file_transfer_properties_destroy (file_transfer_properties_t **properties);
#endif
