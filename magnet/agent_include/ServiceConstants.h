#ifndef _SERVICE_CONSTANTS_H_
#define _SERVICE_CONSTANTS_H_

#include <iostream>
#include <string>

using namespace std;


    const int SUPPORTED_SDK_VERSIONS[] = {6,};
    const string PUBLIC_MAGNET_CHANNEL = "Magnet";

     // interface type for the android emulator AVD 
    const string MAGNET_SERVICE_INTERFACE_NAME_AVD = "eth0";	 
    const string MAGNET_SERVICE_PACKAGE_NAME = "com.samsung.magnet";

    typedef enum _ServiceEvent{
        LISTENING,
        PEERS,
        NO_PEERS,
        JOIN,
        LEAVE,
        STOPPED,
        START_FAILURE,
    } ServiceEvent;

    const int REASON_MAGNET_CORE_NOT_INIT = -1;
    const int REASON_MAGNET_CORE_FAILED = -2;
    	
    const int REASON_MAGNET_CORE_TIMEOUT = -3;
        
    const int REASON_TRANSACTION_EXPIRED = -4;
    	
    const int REASON_CHANNEL_NOT_FOUND = -5;

    const int REASON_FILE_NOT_FOUND = -6;

    const int REASON_TRANSACTION_ONGOING = 1;

    const int CORE_CREATE = 1000;
    const int CORE_CREATE_FAIL_EAGAIN = CORE_CREATE + 1;
    const int CORE_CREATE_FAIL_EINVAL = CORE_CREATE + 2;
    const int CORE_CREATE_FAIL_EPERM = CORE_CREATE + 3;
    const int CORE_CREATE_FAIL_UNKNOWN = CORE_CREATE + 4;

    const int JOIN_CHANNEL = 2000;
    const int CHANNEL_NAME_EMPTY_IN_JOIN = JOIN_CHANNEL + 1;
    const int INVALID_CHANNEL_NAME_IN_JOIN = JOIN_CHANNEL + 2;

    const int DISCOVERY_IN_CHANNEL = 3000;
    const int INVALID_IP_ADDRESS_IN_DISCOVERY = DISCOVERY_IN_CHANNEL + 1;
    const int INVALID_NODE_NAME_IN_DISCOVERY = DISCOVERY_IN_CHANNEL + 2;
    const int INVALID_PORT_NUMBER_IN_DISCOVERY = DISCOVERY_IN_CHANNEL + 3;

    const int SEND_DATA_IN_CHANNEL = 4000;
    const int CHANNEL_NAME_EMPTY_IN_SEND_DATA = SEND_DATA_IN_CHANNEL + 1;
    const int MESSAGE_TYPE_EMPTY_IN_SEND_DATA = SEND_DATA_IN_CHANNEL + 2;


    const int SHARE_FILE_IN_CHANNEL = 5000;
    const int PEER_NOT_FOUND      = SHARE_FILE_IN_CHANNEL + 1;
    const int MEMORY_ALLOC_FAIL = SHARE_FILE_IN_CHANNEL + 2;
    const int FILE_CREATE_FAIL     = SHARE_FILE_IN_CHANNEL + 3;	
    const int INVALID_FILE_SIZE_1    = SHARE_FILE_IN_CHANNEL + 4;  //INVALID_FILE_SIZE 는 window 에서 default로 사용하는것임 
    const int INVALID_TRANSFER_MSG = SHARE_FILE_IN_CHANNEL + 5;	
    const int TRANSFER_TIME_OUT = SHARE_FILE_IN_CHANNEL + 6;	
    const int TRANSFER_SHARE_PREPARE = SHARE_FILE_IN_CHANNEL + 7;	
	
    const int FILE_REJECT = SHARE_FILE_IN_CHANNEL + 11;	
    const int FILE_CANCEL = SHARE_FILE_IN_CHANNEL + 12;
#endif
