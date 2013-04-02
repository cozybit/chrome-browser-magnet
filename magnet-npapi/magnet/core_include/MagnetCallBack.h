#include "MagnetCore.h"

void call_rcv_data (cached_env *cached, magnet_msg_t *mag);
void call_rcv_notify (struct cached_env *cached, magnet_file_msg_t *mag);
void call_rcv_chunk (struct cached_env *cached, zmsg_t *msg);
void call_rcv_file (struct cached_env *cached, zmsg_t *msg);
void call_rcv_file_failed (struct cached_env *cached, zmsg_t *msg, int error);
void call_sent_chunk (struct cached_env *cached, zmsg_t *msg);
void call_sent_file (struct cached_env *cached, zmsg_t *msg);
void call_disappear (cached_env *cached, peer_t *peer);
void call_joined_channel (cached_env *cached, const char *nodename_c, const char *channel_c, const char *ipaddress_c);
void call_parted_channel (cached_env *cached, const char *nodename_c, const char *channel_c, const int reason);
