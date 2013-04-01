/* 
*    ChannelManager Functions
*    ChannelManager <-> MagnetCore
*/
#ifndef __MeshMessaging_H__
#define __MeshMessaging_H__
#include "MeshMessageReceiver.h"
#include "MagnetCore.h"

#include "log.h"

#include <string>
#include <list>

#if (defined (__WINDOWS__))

#include <process.h>
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <pthread.h>
#endif

using namespace std;

typedef enum _NodeEvent
{
    CORE_LISTENING, 
    APPEARED, 
    DISAPPEARED,    
}NodeEvent;

typedef enum _MeshMsgState
{
    STATE_MESH_NONE,
    STATE_MESH_RUN,
    STATE_MESH_STOPING,
    STATE_MESH_STOPED,
} MeshMsgState;
    
class MeshMessaging 
{

public:
    MeshMessaging(MeshMessageReceiver *messageReceiver);
    ~MeshMessaging();

    void init();
#if (defined (__WINDOWS__))
    void start(bool restart);
#elif (defined (__UNIX__))
    int start(bool restart);
#endif
    bool stop();
    void release();    

    int sendData(string toNode, string toChannel, string type, list<unsigned char*>* payload, int *parts_len);
    int shareFile(string toNode, string toChannel, string type, string pathToFile, string exchangeId, long timeoutMsec);

    int shareFile(string toNode, string toChannel, string type, string pathToFile, string exchangeId);
    void acceptFile(string exchangeId, long chunk_timeout, int chunk_retries, long chunk_size);
    void rejectFile(string exchangeId);
    void cancelFile(string exchangeId);
    void joinChannel(string name, int nSecurity);
    void leaveChannel(string name);
    int setIncomingTempFolder(string path);
    void setLivenessTimeout(long timeoutMsec);
    void discoverNode(string nodeName, string ip, int port, bool monitorNode);
    void undiscoverNode(string nodename);
    void useUdpDiscovery(bool value);
    void usePeriodicBeacon(bool value);
    void setPeriodicBeaconTimeOut(long value);
    void useNodeExpiry(bool value);
    void setUsingInterface(string interfaceInfo);
    string getActiveInterface();
	int getDiscoveryPortNumber();
    string getDiscoveryIP();
    
private :
    
#if (defined __ANDROID__ || defined __UNIX__)
    static void* run(void* arg);
#else
	unsigned int __stdcall run (void* arg);
#endif

    //MeshMessageReceiver messageReceiver;
#if (defined (__WINDOWS__))
    HANDLE worker;
    HANDLE mutex;
#elif (defined (__UNIX__))
    pthread_t worker;
    pthread_mutex_t mutex;
#endif

    MeshMsgState mState;

    void _lock(int line);
    void _unlock(int line);  
};

namespace meshmessageing
{
    void onActiveIface(string name, string ip, string mac);
    void onCoreListening();
    void onCoreStopped();
    void recvData(string fromNode, string fromChannel, string type, list<unsigned char*> *payload, int *sizes);
    void recvError(string errorMessage) ;    

    void recvFileNotify(string fromNode, string fromChannel, string originalName, string hash, string exchangeId, string type, long fileSize, string msg1) ;    

    void recvFileChunk(string fromNode, string fromChannel, string originalName, string hash, string exchangeId, string type, long fileSize, long offset) ;    

    void recvFileComplete(string fromNode, string fromChannel, string originalName, string hash, string exchangeId, string type, long fileSize, string tmpPath) ;    

    void recvFileFailed(string fromNode, string fromChannel, string originalName, string hash, string exchangeId, int reason) ;    

    void sendFileChunk (string toNode, string toChannel, string filename, string hash, string exchangeId, string type, long fileSize, long offset, long chunkSize);    

    void sendFileComplete (string toNode, string toChannel, string filename, string hash, string exchangeId, string type, long fileSize);    

    void onNodeAppeared(string fromNode) ;    

    void onNodeDisappeared(string fromNode) ;    

    void onNodeJoined(string fromNode, string channel, string fromIp) ;    

    void onNodeParted(string fromNode, string channel, int reason) ;    
}

#endif
