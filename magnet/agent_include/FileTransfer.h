#ifndef _FileTransfer_H_
#define _FileTransfer_H_

#include <string>
#include <map>
using namespace std;

typedef enum _TransferState
{
    STATE_UNKNOWN,
    STATE_SEND_WAITING,
    STATE_RECEIVE_WAITING,
    STATE_SENDING,
    STATE_RECEIVING,
    STATE_REQ_CANCEL,        
    STATE_TCANCELED,
    STATE_TFAILED,
    STATE_COMPLETED
}TransferState;

class FileState
{
public:
    string mExchangeId;
    string mFileName;
    long mIngSize;
    long mTotalSize;        
    TransferState mState;        

    void set(const string& exID, const string& name, long ingsize, long totsize, TransferState state) 
        { mExchangeId=exID; mFileName=name; mIngSize=ingsize; mTotalSize=totsize; mState=state; }
};

class FileTransfer
{
public:
    class NotificationInfo
    {
    public:
        string taskId;
        int index;
        int filesCount;
    };

    static NotificationInfo *getInfo(string str);
    static string getTaskId(string str);
    static string createExchangeId(const string &taskId, int contentIndex, int numberOfFiles);
    
private:
    static const string TASK_ID;
    static const string CONTENT_INDEX;
    static const string NUMBER_OF_FILES;

    static string flatten(map <string,string> &localMap);
    static void unflatten(map <string,string> &localMap, const string &params);
};

#endif