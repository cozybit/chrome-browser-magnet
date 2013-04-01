/* 
*    ChannelManager Functions
*    ChannelManager <-> MagnetCore
*/

#ifndef _MeshMessageReceiver_H_
#define _MeshMessageReceiver_H_

#include <string>
#include<queue>
#include<list>
#ifdef USE_JNI
#include<jni.h>
#endif

#if (defined __ANDROID__ || defined __unix__)
#include <pthread.h>
#else

#include <process.h>
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#endif

using namespace std;

enum MagnetEvents
{
	EVENT_DATA_RECEIVED=0,
	EVENT_ERROR_RECEIVED,
	EVENT_FILE_NOTIFIED,
	EVENT_CHUNK_RECEIVED,
	EVENT_FILE_RECEIVED,
	EVENT_FILE_FAILED, //5
	EVENT_CHUNK_SENDED,
	EVENT_FILE_SENDED,
	EVENT_NODE_LISTENING,
	EVENT_NODE_APPEARED,
	EVENT_NODE_DISAPPEARED	, //10
	EVENT_NODE_JOINED,
	EVENT_NODE_PARTED,
	EVENT_ACTIVE_NETIFACE,
	EVENT_CORE_STOPPED, 
	EVENT_CORE_START_FAILURE, //15
};

class MagnetEventData
{
    public:
        void setEvent(MagnetEvents event)  { mEvent = event; }
        MagnetEvents getEvent() { return mEvent; }

        void setFromNode(std::string Node) { mNode = Node; }
        std::string getFromNode( ) { return mNode; }

        void setFromChannel(std::string Channel) { mChannel = Channel; }
        std::string getFromChannel( ) { return mChannel; }
		
        void setFromIp(std::string ipAddress) { mIpAddress = ipAddress; }
        std::string getFromIp( ) { return mIpAddress; }

        void setOriginalName(std::string originalName) { mOriginalName = originalName; }
        std::string getOriginalName( ) { return mOriginalName; }

        void setHash(std::string hash) { mHash = hash; }
        std::string getHash( ) { return mHash; }

        void setExchangeId(std::string exchangeId) { mExchangeId = exchangeId; }
        std::string getExchangeId( ) { return mExchangeId; }

        void setType(std::string type) { mType = type; }
        std::string getType( ) { return mType; }	

        void setTmpPath(std::string tmpPath) { mTmpPath = tmpPath; }
        std::string getTmpPath( ) { return mTmpPath; }	

        void setIntCoreTransactionId(int coreTransactionId) { mIntCoreTransactionId = coreTransactionId; }
        int getIntCoreTransactionId( ) { return mIntCoreTransactionId; }
		
        void setMsg1(string msg1) { mMsg1 = msg1; }
        std::string getMsg1( ) { return mMsg1; }

        void setErrorCode(std::string  errorCode) { mErrorMsg = errorCode; }
        std::string getErrorCode( ) { return mErrorMsg; }	

        void setFileSize(long fileSize) { mFileSize= fileSize; }
        long getFileSize( ) { return mFileSize; }	

        void setOffset(long offset) { mOffset = offset; }
        long getOffset( ) { return mOffset; }		

        void setChunkSize(long chunkSize) { mChunkSize = chunkSize; }
        long getChunkSize( ) { return mChunkSize; }	

        void setReason(int reason) { mReason = reason; }
        int getReason( ) { return mReason; }	

        void setIfaceName(std::string name) { mIfaceName = name; }
        std::string getIfaceName( ) { return mIfaceName; }
        
        void setIfaceIP(std::string ip) { mIfaceIP = ip; }
        std::string getIfaceIP( ) { return mIfaceIP; }
        
        void setIfaceMac(std::string mac) { mIfaceMac = mac; }
        std::string getIfaceMac( ) { return mIfaceMac; }

        void setPayload(std::list<unsigned char *> * pPayload) { mPayload = pPayload; } // ???
        std::list<unsigned char *>* getPayload( ) { return mPayload; }

        void setNodes(std::list<string> * pNodes) { mNodes = pNodes; } // ???
        std::list<string>* getNodes( ) { return mNodes; }

        void setResult(int ret) { mResult = ret; }
        int getResult() { return mResult; }

        void setPayloadSizes(int *s) { mSizes = s; }
        int*   getPayloadSizes() { return mSizes; }
    private:
        MagnetEvents mEvent;
        std::string mNode;
        std::string mChannel;
        std::string mIpAddress;
        std::string mOriginalName;
        std::string mHash;
        std::string mExchangeId;
        std::string mType;
        std::string mTmpPath;			
        std::string mMsg1;			
        std::string mErrorMsg;
        std::string mIfaceName;
        std::string mIfaceIP;
        std::string mIfaceMac;
        long mFileSize;
        long mOffset;
        long mChunkSize;
        int   mReason;
        int mIntCoreTransactionId;
        std::list<unsigned char *> *mPayload;
        int   *mSizes;
        std::list<std::string> *mNodes;
        int mResult;
};

class MagnetChannelManager;
    
class MeshMessageReceiver
{
public:
#ifdef USE_JNI
    MeshMessageReceiver(MagnetChannelManager *pCM, JNIEnv* env);
static JNIEnv* mEnv;
#else
    MeshMessageReceiver(MagnetChannelManager *pCM);
#endif
    ~MeshMessageReceiver();

    static queue <MagnetEventData*> *mQueue;    
#if (defined __ANDROID__ || defined __unix__)
    pthread_t mThread; 
    static pthread_mutex_t mutex;
    static pthread_mutex_t mutex_cond;
    static pthread_cond_t mCond;
#else
    HANDLE mThread;
    static HANDLE mutex;
    static HANDLE mutex_cond;	
    static CONDITION_VARIABLE mCond;
#endif
    static bool m_isStoped_Core;

    static bool mRunning;
    static int mNodeCount;
    
    static void Exit() { mRunning = false; }
    static int isRunning() { return mRunning; }  

    void stop();
    void start();

    void push(MagnetEventData *pData);     

    static void Thread_cond_Signal(void);
    static bool isQueueEmpty();
#ifdef __ANDROID__	
private:
#else
public:
#endif
    static int MessMessage_Proc(MagnetChannelManager *pCM, MagnetEventData *pData);    
    static void* threadLoop(void* arg);   
    static void _lock(int line);
    static void _unlock(int line);
private:
    MagnetChannelManager* p_ChannelManager;

 };

#endif
