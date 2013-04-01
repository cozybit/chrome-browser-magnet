#ifndef __MagnetUdp_H__
#define __MagnetUdp_H__

#include "MagnetCore.h"

int create_udp_mcast_recv_socket(sockaddr_in *bcast_sa);
int mcast_send (int fd, struct sockaddr_in *bcast_sa, char *message);
ssize_t mcast_recv (int fd, char *buffer, char *sender_ip);

#endif
