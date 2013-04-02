#pragma once
#include "FileReceiver.h"
#include <string>
#include <map>
#include <vector>
#include <set>
#include "stringconverter.h"
#include "FileTransfer.h"
#include "MeshMessaging.h"
#include "MagnetListener.h"

using namespace std;
class FileInfo;

static int gFileSenderSeq = 10000;

class FileSender
{
public:

    class SendTask
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
        SendTask(MeshMessaging *meshmessaging, MagnetCallbacks::MagnetChannelCallback * mChannelCB, FileSender *task, const string &toNode, const string &toChannel, const string &type, const string &filePath, long timeout, const string &taskId);
        SendTask(MeshMessaging *meshmessaging, MagnetCallbacks::MagnetChannelCallback * mChannelCB, FileSender *task, const string &toNode, const string &toChannel, const string &type,  vector<string> filePaths, long timeout, const string &taskId);        
        ~SendTask();

        string getNode() { return mNode; }   
        string getChannel() { return mChannel; }       
        bool updateInfo(const string &exchangeId, long transferredsize, TransferState state);
        string getLastExchangeId();    
        void dispatch();
        void cancel();
        void notifyMsg(int reason);        

    private:
        MeshMessaging *mMeshMessaging;    
        MagnetCallbacks::MagnetChannelCallback * mChannelCB;
        FileSender* mParent;
            
        map<string, Item*> mItems; // key:taskID, content:ALL, 
        
        string mNode;        
        string mChannel;        
        string mTaskId;
        string mType;
        long   mTimeoutMsec;
        int mSentFileCount;
    };

        
    FileSender(MeshMessaging *meshmessaging, MagnetCallbacks::MagnetChannelCallback * mChannelCB);
    ~FileSender();

    void setLocalNodeName(const string &localNode) { mLocalNode = localNode; }
    void onNodeJoined(const string &nodeName, const string &channel);
    void onNodeLeft(const string &node, const string &channel);
    void onFileFailed(const string &fromNode, const string &fromChannel, const string &originalName, const string &hash, const string &exchangeId, int reason);    
    void onChunkSent(const string &toNode, const string & toChannel, const string & filename, const string & hash, const string & exchangeId, const string & type, long fileSize, long offset, long chunkSize);
    void onFileSent(const string toNode, const string toChannel, const string filename, const string hash, const string exchangeId, const string type, const long fileSize);

    string shareFile(const string &toNode, const string &toChannel, const string &type, const string &filePath, long timeoutMsec);
    string multishareFile(const string &toNode, const string &toChannel, const string &type, vector<string> filePaths, long timeoutMsec);
    bool   cancelFile(const string &channel, const string &exchangeId);
    vector<FileState*> getSenderStatus(const string &taskId);
    void notifyDisconnect(const string& node, const string& channel);
    
private:
    void removeTaskofNodes(const string &node, const string &channel, const string &taskId);
    void notifyAllTasks(const string &node, const string &channel, int reason);        
    void cancelAllTasks(const string &node, const string &channel);
    void cancelTask(const string &node, const string &channel, const string &taskId );
    void updateInfo(const string &exchangeId, long transferredsize, TransferState state);            
    void _lock(int line);
    void _unlock(int line);  
    
private:
    static const long SHARE_FILE_TIMEOUT_MILISECONDS;
    
    MeshMessaging *mMeshMessaging;
    MagnetCallbacks::MagnetChannelCallback * mChannelCB;
    
#if (defined (__WINDOWS__))
    HANDLE mutex;
#else
    pthread_mutex_t mutex;
#endif
        
    string mLocalNode;
    
    multimap<string, string> mNodes;     // key:node+channel, content:taskID
    map<string, SendTask*> mTasks;         // key:taskID, content:sendtask object
    
    void removeNode(const string &node);
    void updateInfo(const string &exchangeId, long long transferred, TransferState state);
    
protected:
};

