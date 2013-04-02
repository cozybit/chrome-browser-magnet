#ifndef _MagnetListener_H_
#define _MagnetListener_H_
#include <string>
#include <list>

#include "Magnet.h"

typedef struct _stMagnetHandle stMagnetHandle;

namespace MagnetCallbacks
{
    using std::string;
    using std::list;

    class MagnetCoreCallback
    {
    private:
        stMagnetHandle *mHandle;

    public:
        virtual void onCoreListeningCB(const string &localName);

        virtual void onCoreStoppedCB(void);

        virtual void onMagnetPeersCB(void);

        virtual void onMagnetNoPeersCB(void);

        virtual void onActiveInterfaceCB(const string &name);

		virtual void onNotifyServiceErrorCB(int reason);

        // Set the listener on the interface so that we can
        // proxy the calls to the C functions
        void setMagnetHandle (stMagnetHandle *handle)
        {
            mHandle = handle;
        }
    };

    class MagnetChannelCallback
    {
    private:
        stMagnetHandle *mHandle;

    public:

        virtual void onJoinedEventCB(const string &fromNode, const string &fromChannel);

        virtual void onLeaveEventCB(const string &fromNode, const string &fromChannel, const int reason);

        virtual void onDataReceivedCB(const string &fromNode, const string &fromChannel, const string &type, list<unsigned char*> *payload, int*sizes);

        virtual void onFileNotifiedCB(const string &fromNode, const string &fromChannel, const string &originalName, const string &hash, const string &exchangeId, const string &type, long fileSize, const string &msg1);

        virtual void onChunkReceivedCB(const string &fromNode, const string &fromChannel, const string &originalName, const string &hash, const string &exchangeId,
            const string &type, long fileSize, long offset);

        virtual void onFileReceivedCB(const string &fromNode, const string &fromChannel, const string &originalName, const string &hash,
            const string &exchangeId, const string &type, long fileSize, const string &tmpPath);

        virtual void onFileFailedCB(const string &fromNode, const string &fromChannel, const string &originalName, const string &hash,
            const string &exchangeId, int reason);

        virtual void onChunkSentCB(const string &destination, const string &channel, const string &fileName, const string &hash, const string &exchangeId,
            const string &type, long fileSize, long offset, long chunkSize);

        virtual void onFileSentCB(const string &destination, const string &channel, const string &fileName, const string &hash, const string &exchangeId, const string &type, long fileSize);

        void setMagnetHandle (stMagnetHandle *handle)
        {
            mHandle = handle;
        }
    };
}
#endif