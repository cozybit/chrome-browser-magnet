#ifndef _MagnetInterface_H_
#define _MagnetInterface_H_
/* 
*    Magnet Interface Functions
*    App -> Magnet
*/

/*============================================================================================
*
* Include Files
*
============================================================================================*/
#include "FileTransfer.h"
#include "MagnetListener.h"
#include "vector"

#ifdef USE_JNI
#include <jni.h>
#endif

using namespace std;
/*============================================================================================
*
* Defininations
*
============================================================================================*/


/**
    * ���׳� ���� �ʱ�ȭ.
    *
    * @param[in] customAction  ���� �̸�.
*/
namespace MagnetInterface
{
#ifdef USE_JNI
    int MagnetInit( MagnetCallbacks::MagnetCoreCallback * coreCB, 
        MagnetCallbacks::MagnetChannelCallback * channelCB, 
        string tmpPath,
        JNIEnv * env);
    #else
    int MagnetInit( MagnetCallbacks::MagnetCoreCallback * coreCB, 
        MagnetCallbacks::MagnetChannelCallback * channelCB, 
        string tmpPath);
    #endif
    
#ifdef USE_JNI
    int MagnetInit( MagnetCallbacks::MagnetCoreCallback * coreCB, 
        MagnetCallbacks::MagnetChannelCallback * channelCB, 
        string tmpPath,
        string securityKeyPath,
        JNIEnv * env);
    #else
    int MagnetInit( MagnetCallbacks::MagnetCoreCallback * coreCB, 
        MagnetCallbacks::MagnetChannelCallback * channelCB, 
        string tmpPath,
        string securityKeyPath);
    #endif
    
    /**
        * ���׳� ���� ����.
        *
        * @return true ���׳� ���񽺰� ���������� �����, false ���׳� ���� �������� ����.
    */
    bool MagnetRelease(void);
    
    /**
    	* ���׼� ���� ����.
    	*
    	* @return true ���׼� ���񽺰� ���������� ���۵�, false ���׳� ���񽺰� ���������� ���۵��� ����.
    */
    bool MagnetStart(void);
    
    /**
    	* ���׳� ���� ����.
    	*
    	* @return true ���׳� ���񽺰� ���������� �����, false ���׳� ���� �������� ����.
    */
    bool MagnetStop(void);
    
    /**
        * ���ϴ� ä�ο� ����.
        *
        * @param[in] channel �����ϰ��� �ϴ� ä�� �̸�.
        * @param[in] nSecurity Security Channel ��� ���� ���� 1 - Security Channel / 0 - Nonsecurity Channel.
    */
    int joinChannel(string channel, int nSecurity);
    
    /**
        * ���ӵ� ä�ο��� ���� ����.
        *
        * @param[in] channel ���� ���� �ϰ��� �ϴ� ä�� �̸�.
    */
    void leaveChannel(string channel);
    
    /**
        * ���ӵ� ä�� Ȥ�� ä�γ��� Ư�� node�� ���ؼ� ���� ����.
        *
        * @param[in] toNode ������ node �̸�.
        * @param[in] toChannel ������ ä�� �̸�.
        * @param[in] type ����ڰ� ������ ���� ����.
        * @param[in] path ���� ���� ���.
        * @param[in] exchangeId ���� ���� �ĺ���.
        * @param[in] ���� ���� ���� �ð�. ������ ������ ������ -1�� ����.
    */
    string shareFile(string toNode, string toChannel, string type, string path, long timeoutMsec);

    /**
        * ���ӵ� ä�� Ȥ�� ä�γ��� Ư�� node�� ���ؼ� ���� ����.
        *
        * @param[in] toNode ������ node �̸�.
        * @param[in] toChannel ������ ä�� �̸�.
        * @param[in] type ����ڰ� ������ ���� ����.
        * @param[in] path ���� ���� ���.
        * @param[in] exchangeId ���� ���� �ĺ���.
        * @param[in] ���� ���� ���� �ð�. ������ ������ ������ -1�� ����.
    */    
    string multishareFile(string toNode, string toChannel, string type, vector<string> &filePaths, long timeoutMsec);
    
    /**
        * ���� ���� ��û�� �޾Ƶ���.
        *
        * @param[in] coreTransactionId ���� ���� transaction identifier.
        * @param[in] chunktimeoutMsec ���� ���� ���� �ð�. ������ ������ ������ -1�� ����.
        * @param[in] chunkRetries Number of times to retry chunk in addition to the initial try. ������ ������ ������ -1�� ����.
        * @param[in] chunkSize : Size of chunk to request. ������ ������ ������ -1�� ����.
    */
    bool acceptFile(string channel, string coreTransactionId, long chunktimeoutMsec, int chunkRetries, long chunkSize);
    
    /**
        * ���� ���� ��û �ź�.
        *
        * @param[in] coreTransactionId ���� ���� transaction identifier.
    */
    bool rejectFile(string channel, string exchangeId);
    
    /**
        * �������� ���� ���� ���.
        *
        * @param[in] exchangeId ���� ���� �ĺ���.
        * @param[in] coreTransactionId ���� ���� transaction identifier.
    */
    bool cancelFile(string channel, string exchangeId);
    
    /**
        * ���ӵ� ä�� Ȥ�� ä�γ��� Ư�� node�� ���ؼ� �޽��� ����.
        *
        * @param[in] toNode ������ node �̸�.
        * @param[in] toChannel ������ channel �̸�.
        * @param[in] type ����ڰ� ������ �޽��� ����.
        * @param[in] payload �����ϰ��� �ϴ� �޽��� ����.
    */
    
    vector<FileState*> getTransferStatusSender(string taskId);
    
    vector<FileState*> getTransferStatusReceiver(string taskId);
    
    int sendData(string toNode, string toChannel, string type, list<unsigned char *>* payload, int* parts_len);
    
    /**
    * Magnet Manager���� �ٷ� Channel List return �ϵ���
    * @return Channel List
    */
    list<string> getJoinedChannelList(void);

    /**
    * Magnet Manager���� �ٷ� Channel List return �ϵ���
    * @param channel Channel Name
    * @return Node List
    */
    list<string> getConnectedNodeList(string channel);

    /**
        * Sets Liveliness Timeout.
        *
        * @param[in] timeoutMsec : TimeOut
    */
    void setLivenessTimeout(long timeoutMsec);
    
    /**
        * Discover Node.
        *
        * @param[in] ip Ž�� �ϰ��� �ϴ� IP �ּ�.
        * @param[in] port Ž���ϰ����ϴ� node�� ��Ʈ ����.
        * @param[in] monitorNode ����͸� ���� ����.
    */
    void discoverNode(string nodeName, string ip, int port, bool monitorNode);
    
    /**
        * Undiscover node.
        *
        * @param[in] nodeName : Node name
    */
    void undiscoverNode(string nodeName);
    
    /**
        * Use UDP Discovery.
        *
        * @param[in] value : true/false
    */
    void setUdpDiscover(bool value);
    
    /**
        * Set node expiration check.
        *
        * @param[in] value : true/false
    */
    void setNodeExpiry(bool value);
    
    /**
        * Use Periodic Beacon.
        *
        * @param[in] value : true/false
    */
    void setPeriodicBeacon(bool value);

    /**
        * Set Periodic Beacon time-out.
        *
        * @param[in] value : msec
    */
    void setPeriodicBeaconTimeOut(long value);
    
    /**
        * Application���� ��� �� Network Interface ����.
        *
        * @param[in] value : true/false
    */
    void setUsingInterface(string interfaceInfo);
    
    /**
        * Network ���� ���� ����.
        *
        * @param[in] state : OFF[0], CONNECTED[1], DISCONNECTED[2]
    */
    void setConnectivityState(int state);
    
    /**
        * Application���� Core�� Activate�� Network Interface��û.
        *
        * @return : Activate�� Network interface name.
    */
    string getUsingInterfaceName(void);

    /**
        * Application���� �ڽ��� Port Number��û.
    	* @return Port Number.
    
    */
    int getDiscoveryPortNumber(void);
    
    /**
        * Application���� �ڽ��� IP��û.
    	* @return IP Address.
    
    */
    string getDiscoveryIP(void);
    
    /**
        * Application���� Ư�� Node�� IP Address�� ��û.
        * @param[in] nodeName : IP Address�� �˰��� �ϴ� ����� �̸�.
        * @return : IP Address.
    */
    string getNodeIpAddress(string nodeName, string channel);

    /**
        * Application���� Ư�� Node�� Port Number ��û.
        * @param[in] nodeName : Port Number�� ����� �ϴ� Node Name
        * @return Port Number.
    */
    int getNodePort(string nodeName, string channel);
    
    /**
        * check samsung device
        * @return : true/false
    */
    bool getAvailableDevice();

}
#endif
