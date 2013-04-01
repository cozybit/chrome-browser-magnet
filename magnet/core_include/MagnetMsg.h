#ifndef __MagnetMsg_H__
#define __MagnetMsg_H__

#include "MagnetCore.h"

//  Destroy Magnet message
void magnet_msg_destroy (magnet_msg_t **mag);
//  Destroy Magnet file message
void magnet_file_msg_destroy (magnet_file_msg_t **fmag);
void magnet_file_msg_free (void *item);
magnet_file_msg_t * magnet_file_msg_parse (magnet_msg_t *mag);
// Parses zmsg_t to magnet_msg_t. The magnet_msg_t will take ownership
// of the zmsg_t and will free it during destruction
magnet_msg_t * magnet_msg_parse (zmsg_t *msg);
// 2013.02.19 Merge from cozybit : magnet-fix-capi
// Unparses a magnet_msg_t and creates a zmsg_t
zmsg_t * magnet_msg_unparse (magnet_msg_t *mag);
zmsg_t * zmsg_new_magnet_head (char *origin, magnet_dst_type_t dst_t,
                               char *destination, char *channel,
                               magnet_frm_type_t frm_t);
// 2013.02.19 Merge from cozybit : magnet-fix-capi
// Creates zmsg_t from magnet_msg_t
zmsg_t * magnet_file_msg_unparse (magnet_file_msg_t *fmag);
bool parse_heartbeat_message (char *message, discovery_t *discovery, cached_env *cached);

#endif