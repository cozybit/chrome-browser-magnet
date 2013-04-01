#ifndef __MagnetPeer_H__
#define __MagnetPeer_H__

#include "MagnetCore.h"

void set_reconnect_backoff (void *socket);
void peer_free (void *item);
void handle_peer_updates (cached_env *cached, peer_t *peer, int64_t time_now, int update_channels);
peer_t * peer_new (cached_env *cached, zctx_t *context, discovery_t *discovery, void *subscriber);
void expire_peers (cached_env *cached);

#endif