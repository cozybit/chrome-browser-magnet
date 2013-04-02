#pragma once
#include "FileSender.h"
#include <string>
#include <map>
#include <vector>
#include <set>
#include <queue>
#include "stringconverter.h"
#include "FileTransfer.h"
#include "MeshMessaging.h"
#include "MagnetListener.h"

class ReceiveTask;

using namespace std;
class FileReceiver
{
    public:
    static const int CHUNK_SIZE_THRESHOLD = 500 * 1024;
    static const int CHUNK_SIZE = 300 * 1024;
	
    class ReceiveTask
    {
        class Item 
        {
        public :
            Item(const string &filePath, const string &exchangeId, long filesize);

            string getExchangeId() { return mExchangeId; }
            string getFileName() { return mFileName; }        
            TransferState getState() { return mState; }
            void setState(TransferState  state) { mState=state; }
            void updateInfo(long size, TransferState state);

        private :
            string mExchangeId;
            string mFileName;
            long mIngSize;
            long mTotalSize;        
            TransferState mState;
        };
        
    public:
        ReceiveTask(MeshMessaging *meshmessaging, MagnetCallbacks::MagnetChannelCallback * channelCB, FileReceiver *task, const string &fromNode, const string &channel);
        ~ReceiveTask();

        TransferState getStatusFirstItem();
        string getNode() { return mNode; }   
        string getChannel() { return mChannel; }       
        bool updateInfo(const string &exchangeId, long transferredsize, TransferState state);
        string getLastExchangeId();    
        void dispatch();
        void accept( long chunkTimeoutMsec, int chunkRetries, long chunkSize);
        void reject();
        void cancel();
        void addItem(const string &filePath, const string &exchangeId, long filesize);
        void notifyMsg(int reason);
    
    private:
        MeshMessaging *mMeshMessaging;
        MagnetCallbacks::MagnetChannelCallback * mChannelCB;
        FileReceiver* mParent;
    
        map<string, Item*> mItems;          // key:taskID, content:ALL,         
		
        string mNode;		
        string mChannel;        
        string mTaskId;
        
        int mSentFileCount;
    };
    

    FileReceiver(MeshMessaging *meshmessaging, MagnetCallbacks::MagnetChannelCallback * channelCB);
    ~FileReceiver();

    void onNodeJoined(const string &nodeName, const string &channel);
    void onNodeLeft(const string &node, const string &channel);
    void onFileFailed(const string &fromNode, const string &fromChannel, const string &originalName, const string &hash, const string &exchangeId, int reason);
    void onFileNotified(const string &fromNode, const string &channel, const string &filename, const string &hash, const string &type, const string &exchangeId, long fileSize, const string &msg1, bool isNotify);
    void onChunkReceived(const string &fromNode, const string &fromChannel, const string &originalName, const string &hash, const string &exchangeId, const string &type, long fileSize, long offset);
    void onFileReceived(const string &fromNode, const string &fromChannel, const string &originalName, const string &hash, const string &exchangeId, const string &type, const string &filename, long fileSize);

    bool acceptFile(const string &channel, const string &exchangeId, long chunkTimeoutMsec, int chunkRetries, long chunkSize);
    bool rejectFile(const string &channel, const string &exchangeId);
    bool cancelFile(const string &channel, const string &exchangeId);
    vector<FileState*> getReceiverStatus(const string &taskId);
    void notifyDisconnect(const string& node, const string& channel);

private:
    void removeTaskofNodes(const string &node, const string &channel, const string &taskId);
    void notifyAllTasks(const string &node, const string &channel, int reason);
    void cancelAllTasks(const string &node, const string &channel);
    void cancelTask(const string &node, const string &channel, const string &taskId);
    void updateInfo(const string &exchangeId, long transferredsize, TransferState state);
    void _lock(int line);
    void _unlock(int line);    
    
private:
    MeshMessaging *mMeshMessaging;
    MagnetCallbacks::MagnetChannelCallback * mChannelCB;
    
#if (defined (__WINDOWS__))
    HANDLE mutex;
#else
    pthread_mutex_t mutex;
#endif

    multimap<string, string> mNodes;       // key : node+channel, value : taskID
    map<string, ReceiveTask*> mTasks;   // key : taskID, value : ReceiveTask object
	
};
