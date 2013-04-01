#ifndef __MagnetSend_H__
#define __MagnetSend_H__

#include "MagnetCore.h"
//  Ask peer if it's still alive
//
void send_beacon_request (cached_env *cached, peer_t *peer);
void send_beacon_response (cached_env *cached, peer_t *peer);
//  Ask peer for a list of its channels
void send_channels_request (cached_env *cached, peer_t *peer);
//  Reply to peer with list of channels
void send_channels_response (cached_env *cached, peer_t *peer);
void send_discovery_message (cached_env *cached, peer_t *peer);
void broadcast_halt (cached_env *cached);
void send_nodename_message(cached_env *cached, peer_t *peer, bool bReq);
void send_channels_update (cached_env *cached, peer_t *peer);
#endif
