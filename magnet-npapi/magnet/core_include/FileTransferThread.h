#ifndef __FileTransferThread_h__
#define __FileTransferThread_h__

#include "MagnetCore.h"

void *transfer_thread_new (zctx_t *ctx);
void transfer_run_fn (void *args, zctx_t *ctx, void *pipe);

#endif
