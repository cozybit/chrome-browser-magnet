#ifndef __MagnetUtil_H__
#define __MagnetUtil_H__

#include "MagnetCore.h"

int select_interface (struct sockaddr_in *ip_sa, struct sockaddr_in *bcast_sa, char *buffer);
int backoff_time (size_t nodes);

int msnprintf (char **retval, int max_size, const char *format, ...);
int magnet_stat (struct stat *sb, const char *path);
int magnet_fcopy (FILE *source, FILE *destination);
size_t magnet_filesize (const char *path);
magnet_mmap_t* magnet_prepare_file_share (const char *path);
void magnet_mmap_destroy (magnet_mmap_t **m);
//char* magnet_create_hash (const char *buffer, size_t buffer_size);
int magnet_verify_hash (const char *buffer, size_t buffer_size, const char *compare_hash);
//void magnet_madvise_seq (void *ptr, size_t position, size_t advise_size);
// Generate a random port between the range of 10000-30000
int magnet_random_port (void);
// Get basename from an absolute path
char* magnet_basename (char *path);
int magnet_zhash_clear (zhash_t *hash);
int magnet_zhash_append (zhash_t *target, zhash_t *source);
zmsg_t* magnet_recv_nowait (void *pipe);
int create_temp_file (const char *path, FILE **handle, char **name);
void makeEmulNodeName(char *temp_buffer);
void makeNodeName(unsigned char *mac, char *buffer);
void NodeNametoMac(unsigned char *mac, char *buffer);
bool isValidLetter(char c);
#if (defined (__WINDOWS__))
int __pc_log_write(int prio, const char *tag, const char *msg);
int __pc_log_print(int prio, const char *tag, const char *fmt, ...);
#endif
#if (defined (__WINDOWS__))
void CharToWChar(const char* szSource, WCHAR* szDes);
void WCharToChar(WCHAR* wszSrc, char* szDes);
int ANSIToUTF8(char *srcANSI, char *destUTF8);
char* UTF8toANSI(char *srcUTF8);
#endif
#endif
