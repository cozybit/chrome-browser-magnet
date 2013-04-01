#ifndef __NATIVE_H_INCLUDE__
#define __NATIVE_H_INCLUDE__
#include "MagnetCore.h"

int join_channel (string channel, int security);
int leave_channel (string channel);
int send_data (string destination, string channel, string type, list<unsigned char*>* payload, int *parts_len);
int share_file (string destination, string channel, string type, string path, string exchange_id, long timeout_msec);
void accept_file(string exchange_id, long chunk_timeout_msec, int chunk_retries, long chunk_size);
void reject_file(string exchange_id);
void cancel_file(string exchange_id);
int set_tmp_folder(string path);
void set_liveness_timeout(long timeout_msec);
void discover_node(string nodeName, string ip, int port, bool monitor_node);
void undiscover_node(string nodename);
void set_UseUdpDiscovery(bool value);
void use_node_expiry(bool value);
void set_UsePeriodicBeacon(bool value);
void set_PeriodicBeaconTimeOut(long value);
#endif
