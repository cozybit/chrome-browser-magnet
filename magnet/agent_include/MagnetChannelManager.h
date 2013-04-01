#include "MeshMessaging.h"
#include "log.h"
#include "FileSender.h"
#include "FileReceiver.h"
#include "ServiceConstants.h"
#include <iostream>
#include <string>

#include <map>
#include <list>
#ifdef USE_JNI
#include <jni.h>
#endif

using namespace std;

class MagnetChannelManager
{

public:
    MagnetChannelManager();
    ~MagnetChannelManager();
#ifdef USE_JNI
    void init(JNIEnv* env, MagnetCallbacks::MagnetCoreCallback * coreCB, MagnetCallbacks::MagnetChannelCallback * channelCB);
#else
    void init(MagnetCallbacks::MagnetCoreCallback * coreCB, MagnetCallbacks::MagnetChannelCallback * channelCB);
#endif
    void release();
    bool start();
    bool stop();
    
    typedef enum _ConnStates
    {        
        CONNECTED,
        DISCONNECTED,
        START,
        OFF,
    }ConnStates;
    
    class MagnetChannel
    {
    public:
        MagnetChannel(MeshMessaging* mMeshMessaging, string mCahnnel, bool mConnectivity, MagnetChannelManager* mChannelManager, int nSecurity);
        ~MagnetChannel();
        
        int sendData(string destination, string toChannel, string type, list<unsigned char *>* payload, int *parts_len);
        void leaveChannel();
        string getNodeIpAddress(string nodename); //twkim_teporary
        void updateNodes(string nodename, string ip, bool join); //twkim_teporary
        list<string> getConnectedNodeList(string channel);
        map<string, string> &getNodeMap() { return mNodeIpAdress; }
        
    private:
        string mChannel;
        map<string, string> mNodeIpAdress;      // key:nodename, value:ip
        MagnetChannelManager* mChannelManager;
        MeshMessaging* mMeshMessaging;
        
    };

    void onDestroy();
    int joinChannel(string Channel,  int Security);
    MagnetChannel* getChannel(string Channel);
    void leaveChannel(string Channel);
    bool destroyCore(void);
#ifdef __ANDROID__
    int createCore(void);
#else
    void createCore(void);
#endif
    void rejoinChannels(void);

    int sendData(string destination, string channel, string type, list<unsigned char *>* payload, int *parts_len);
    string shareFile(string toNode, string toChannel, string type, string path, long timeoutMsec);
    string multishareFile(string toNode, string toChannel, string type, vector<string> &filePaths, long timeoutMsec);
    bool acceptFile(string channel, string exchangeId, long chunkTimeoutMsec, int chunkRetries, long chunkSize);
    bool rejectFile(string channel, string exchangeId);
    bool cancelFile(string channel, string exchangeId);	
    vector<FileState*> getSenderStatus(const string &taskId);
    vector<FileState*> getReceiverStatus(const string &taskId);

    list<string> getConnectedNodeList(string channel);
    void setLivenessTimeout(long timeoutMsec);
    void discoverNode(string nodeName, string ip, int port, bool monitorNode);
    void undiscoverNode(string nodename);
    void useUdpDiscovery(bool value);
    void useNodeExpiry(bool value);
    void usePeriodicBeacon(bool value);
    void setPeriodicBeaconTimeOut(long value);    
    void setConnectivityState(int state);
    void setTmpFolder(string path);
    void setUsingInterface(string interfaceInfo);
    string getActiveInterface();
    
    MagnetCallbacks::MagnetCoreCallback * mCoreCB;
    MagnetCallbacks::MagnetChannelCallback * mChannelCB;    
        
    int getDiscoveryPortNumber();
    string getDiscoveryIP();
    string getNodeIpAddress(string nodeName, string channel);
    int getNodePort(string nodeName, string channel);

    void setTmpFolderPathStr(string tmpPath);
    void setSecureKeyPathStr(string keyPath);
    list<string> getJoinedChannelList();
    
private:
    map<string, MagnetChannel*> mChannels;
    vector<string>* m_pChList;

    int nodeCount;
    ConnStates mConnState;
    bool mConnectivity;
    bool mServiceHasPeers;
    string mTmpPath;
    string m_strPathSecurityKey;

    MeshMessaging* mMeshMessaging;
    MeshMessageReceiver* mMeshReceiver;

    FileSender *mFileSender;
    FileReceiver *mFileReceiver;

#if (defined (__WINDOWS__))
    HANDLE mutex;
#else
    pthread_mutex_t mutex;
#endif

    void _lock(int line);
    void _unlock(int line);
    void handleConnectivity(int state);
    void deleteIncomingTmpFiles(void);
    void deleteIncomingTmpFolder(void);
    void joinAllChannels();
    
public:
    void notifyData(string fromNode, string fromChannel, string type, list<unsigned char*> *payload, int *sizes);
    void notifyFile(string fromNode, string fromChannel, string originalName, string hash, string exchangeId, string type, long fileSize, string msg1);
    void notifyChunkReceived(string fromNode, string fromChannel, string originalName, string hash, string exchangeId, string type, long fileSize, long offset);
    void notifyFileReceived(string fromNode, string fromChannel, string originalName, string hash, string exchangeId, string type, string pathToFile, long fileSize);
    void notifyFileFailed(string fromNode, string fromChannel, string originalName, string hash, string exchangeId, int reason);
    void notifyChunkSent(string toNode, string toChannel, string filename, string hash, string exchangeId, string type, long fileSize, long offset, long chunkSize);
    void notifyFileSent(const string toNode, const string toChannel, const string filename, const string hash, const string exchangeId, const string type, const long fileSize);
    void notifyEvent(string nodename, ServiceEvent e, string channel, int reason);
    void notifyJoinedEvent(string nodename, ServiceEvent e, string channel, string ipAddress);
    void notifyCreateFailure(int reason);
    
};
