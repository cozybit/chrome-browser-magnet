#ifndef __FileTransferFetch_H__
#define __FileTransferFetch_H__
#include "MagnetCore.h"

file_fetch_t* file_fetch_new (zmsg_t *msg, const char *tmp_path, long chunk_timeout, int chunk_retries, long chunk_size, int*error_code);
 void file_fetch_destroy (file_fetch_t **fetch);
 void file_fetch_properties_destroy (file_fetch_properties_t **p);
 void file_fetch_add_duplicate (file_fetch_t *fetch, zmsg_t *msg);
 void file_fetch_properties_clear (file_fetch_properties_t *p);
 magnet_peer_t * peer_get (zhash_t *peer_connections, zctx_t *ctx, const char *ip, const char *port);
void peer_attach_fetch (magnet_peer_t *peer, file_fetch_t *fetch);
void peer_detach_fetch (magnet_peer_t *peer, file_fetch_t *fetch);
file_fetch_t* peer_find_fetch (magnet_peer_t *peer, const char *path, const char *hash, size_t siz);
file_fetch_t *peer_find_fetch (magnet_peer_t *peer, const char *path, const char *hash, size_t size);
void peer_set_all_fail(zhash_t *peer_connections);

#endif
