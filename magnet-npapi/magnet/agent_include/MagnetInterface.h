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
    * 마그넷 서비스 초기화.
    *
    * @param[in] customAction  서비스 이름.
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
        * 마그넷 서비스 해제.
        *
        * @return true 마그넷 서비스가 성공적으로 종료됨, false 마그넷 서비스 종료하지 못함.
    */
    bool MagnetRelease(void);
    
    /**
    	* 마그셋 서비스 시작.
    	*
    	* @return true 마그셋 서비스가 성공적으로 시작됨, false 마그넷 서비스가 성공적으로 시작되지 못함.
    */
    bool MagnetStart(void);
    
    /**
    	* 마그넷 서비스 종료.
    	*
    	* @return true 마그넷 서비스가 성공적으로 종료됨, false 마그넷 서비스 종료하지 못함.
    */
    bool MagnetStop(void);
    
    /**
        * 원하는 채널에 접속.
        *
        * @param[in] channel 접속하고자 하는 채널 이름.
        * @param[in] nSecurity Security Channel 사용 여부 설정 1 - Security Channel / 0 - Nonsecurity Channel.
    */
    int joinChannel(string channel, int nSecurity);
    
    /**
        * 접속된 채널에서 접속 해제.
        *
        * @param[in] channel 접속 해제 하고자 하는 채널 이름.
    */
    void leaveChannel(string channel);
    
    /**
        * 접속된 채널 혹은 채널내의 특정 node에 대해서 파일 공유.
        *
        * @param[in] toNode 목적지 node 이름.
        * @param[in] toChannel 목적지 채널 이름.
        * @param[in] type 사용자가 지정한 파일 유형.
        * @param[in] path 파일 절대 경로.
        * @param[in] exchangeId 파일 공유 식별자.
        * @param[in] 파일 공유 만료 시간. 설정을 원하지 않으면 -1로 설정.
    */
    string shareFile(string toNode, string toChannel, string type, string path, long timeoutMsec);

    /**
        * 접속된 채널 혹은 채널내의 특정 node에 대해서 파일 공유.
        *
        * @param[in] toNode 목적지 node 이름.
        * @param[in] toChannel 목적지 채널 이름.
        * @param[in] type 사용자가 지정한 파일 유형.
        * @param[in] path 파일 절대 경로.
        * @param[in] exchangeId 파일 공유 식별자.
        * @param[in] 파일 공유 만료 시간. 설정을 원하지 않으면 -1로 설정.
    */    
    string multishareFile(string toNode, string toChannel, string type, vector<string> &filePaths, long timeoutMsec);
    
    /**
        * 파일 공유 요청을 받아들임.
        *
        * @param[in] coreTransactionId 파일 공유 transaction identifier.
        * @param[in] chunktimeoutMsec 파일 공유 만료 시간. 설정을 원하지 않으면 -1로 설정.
        * @param[in] chunkRetries Number of times to retry chunk in addition to the initial try. 설정을 원하지 않으면 -1로 설정.
        * @param[in] chunkSize : Size of chunk to request. 설정을 원하지 않으면 -1로 설정.
    */
    bool acceptFile(string channel, string coreTransactionId, long chunktimeoutMsec, int chunkRetries, long chunkSize);
    
    /**
        * 파일 공유 요청 거부.
        *
        * @param[in] coreTransactionId 파일 공유 transaction identifier.
    */
    bool rejectFile(string channel, string exchangeId);
    
    /**
        * 공유중인 파일 전송 취소.
        *
        * @param[in] exchangeId 파일 공유 식별자.
        * @param[in] coreTransactionId 파일 공유 transaction identifier.
    */
    bool cancelFile(string channel, string exchangeId);
    
    /**
        * 접속된 채널 혹은 채널내의 특정 node에 대해서 메시지 전송.
        *
        * @param[in] toNode 목적지 node 이름.
        * @param[in] toChannel 목적지 channel 이름.
        * @param[in] type 사용자가 지정한 메시지 유형.
        * @param[in] payload 전달하고자 하는 메시지 내용.
    */
    
    vector<FileState*> getTransferStatusSender(string taskId);
    
    vector<FileState*> getTransferStatusReceiver(string taskId);
    
    int sendData(string toNode, string toChannel, string type, list<unsigned char *>* payload, int* parts_len);
    
    /**
    * Magnet Manager에서 바로 Channel List return 하도록
    * @return Channel List
    */
    list<string> getJoinedChannelList(void);

    /**
    * Magnet Manager에서 바로 Channel List return 하도록
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
        * @param[in] ip 탐색 하고자 하는 IP 주소.
        * @param[in] port 탐색하고자하는 node의 포트 정보.
        * @param[in] monitorNode 모니터링 여부 설정.
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
        * Application에서 사용 할 Network Interface 설정.
        *
        * @param[in] value : true/false
    */
    void setUsingInterface(string interfaceInfo);
    
    /**
        * Network 접속 상태 설정.
        *
        * @param[in] state : OFF[0], CONNECTED[1], DISCONNECTED[2]
    */
    void setConnectivityState(int state);
    
    /**
        * Application에서 Core로 Activate된 Network Interface요청.
        *
        * @return : Activate된 Network interface name.
    */
    string getUsingInterfaceName(void);

    /**
        * Application에서 자신의 Port Number요청.
    	* @return Port Number.
    
    */
    int getDiscoveryPortNumber(void);
    
    /**
        * Application에서 자신의 IP요청.
    	* @return IP Address.
    
    */
    string getDiscoveryIP(void);
    
    /**
        * Application에서 특정 Node의 IP Address를 요청.
        * @param[in] nodeName : IP Address를 알고자 하는 노드의 이름.
        * @return : IP Address.
    */
    string getNodeIpAddress(string nodeName, string channel);

    /**
        * Application에서 특정 Node의 Port Number 요청.
        * @param[in] nodeName : Port Number를 얻고자 하는 Node Name
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
