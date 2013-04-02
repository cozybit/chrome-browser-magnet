#ifndef _MAGNET_H_
# define _MAGNET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>

/* Export the functions */
#if defined _WIN32
#   if !defined MAGNET_STATIC
#       if defined DLL_EXPORT
#           define MAGNET_EXPORT __declspec(dllexport)
#       else
#           define MAGNET_EXPORT __declspec(dllimport)
#       endif
#   endif
#endif

#ifndef BOOL
typedef int BOOL;
#endif
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef streq
#define streq(a,b) (*(a) == *(b) && strcmp ((a),(b)) == 0)
#endif
/* In any other case just define export as empty */
#ifndef MAGNET_EXPORT
#define MAGNET_EXPORT
#endif

/**
 * @defgroup Magnet_Definition_For_Opaque_Structure Magnet Type Definition for opaque structure
 */

/**
 * @addtogroup Magnet_Definition_For_Opaque_Structure
 * @{
 */

/**
 * @brief Opaque structure for magnet header
 */
typedef struct _stMagnetHeader stMagnetHeader;

/**
 * @brief Opaque structure holding the callbacks
 */
typedef struct _stMagnetListener stMagnetListener;

/**
 * @brief A single data item inside the payload
 */
typedef struct _stMagnetData stMagnetData;

/**
 * @brief A magnet data payload
 */
typedef struct _stMagnetPayload stMagnetPayload;

/**
 * @brief File transfer info passed to callbacks
 */
typedef struct _stMagnetTransferInfo stMagnetTransferInfo;

/**
 * @}
 */

/**
 * @defgroup Magnet_Definition_For_Callbacks Magnet Type Definition for callback funtions
 */

/**
 * @addtogroup Magnet_Definition_For_Callbacks
 * @{
 */

/**
 * @brief Type definition function pointer for user data free callback
 *
 * @param[out] data pointer to the user data
 */
typedef void (magnetUserdataFreeCB) (void *  data);

/**
 * @brief Type definition function pointer for core listening callback
 *
 * This callback will happen when core is fully started and ready to listen
 *
 * @param[out] localName My nodename
 */
typedef void (magnetOnListeningCB) (const char *  localName);

/**
 * @brief Type definition function pointer for core stopped callback
 *
 * This callback will be received when the magnet core is fully stopped
 *
 */
typedef void (magnetOnStoppedCB) ();

/**
 * @brief Type definition function pointer for on peers come callback
 *
 * Called when the first peer join magnet public channel
 *
 */
typedef void (magnetOnPeersCB) ();

/**
 * @brief Type definition function pointer for no peers in magnet callback
 *
 * Called when last node leave the public channel
 *
 */
typedef void (magnetOnNoPeersCB) ();

/**
 * @brief Type definition function pointer for failed core start callback
 *
 * If core fails to start, call this method
 *
 * @param[out] reason Reason of failure. Refer to MagnetErrorCode.h about the code
 */
 typedef void (magnetOnServiceErrorCB) (int reason);

/**
 * @brief Type definition function pointer for node joined event
 *
 * Called when other peer joined to a channel
 * param header contains information about the joined node
 *
 * @param[out] header Opaque structure for magnet header
 *                     has information of peer's nodename and channel
 */
 typedef void (magnetOnJoinCB) (stMagnetHeader *  header);

/**
 * @brief Type definition function pointer for peer leaved event
 *
 * Called when other peer leaved from a channel
 * param header contains information of the left node
 *
 * @param[out] header Opaque structure for magnet header
 *                     has information of peer's nodename and channel
 */
 typedef void (magnetOnLeaveCB) (stMagnetHeader *  header);

/**
 * @brief Type definition function pointer for receive data event
 *
 * When data is received, call this method
 *
 *              has information of peer's nodename, channel, and data type d
 * @param[out] payload magnet data structure
 */
 typedef void (magnetOnDataReceivedCB) (stMagnetHeader *  header, stMagnetPayload *  payload);

/**
 * @brief Type definition function pointer for request file share event
 *
 * When a request from peer within channel is received, call this method
 *
 * @param[out] header Opaque structure for magnet header
 *                     has information of peer's nodename, channel, and file type of user defined
 * @param[out] info File transfer info
 *                     has information of file transfer(filename, hash, exchange id, filesize)
 */
 typedef void (magnetOnFileNotifiedCB) (stMagnetHeader *  header, stMagnetTransferInfo *  info);

/**
 * @brief Type definition function pointer for received chunk event
 *
 * When received chunk from share side, call this method
 *
 * @param[out] header Opaque structure for magnet header
 *                     has information of peer's nodename, channel, and file type of user defined
 * @param[out] info File transfer info
 *                     has information of file transfer(filename, hash, exchange id, filesize, offset)
 */
 typedef void (magnetOnChunkReceivedCB) (stMagnetHeader *  header, stMagnetTransferInfo *  info);

/**
 * @brief Type definition function pointer for file received event
 *
 * When a file received completely, call this method
 *
 * @param[out] header Opaque structure for magnet header
 *                     has information of peer's nodename, channel, and file type of user defined
 * @param[out] info File transfer info
 *                     has information of file transfer(filename, hash, exchange id, filesize, received path(include temporary filename))
 */
 typedef void (magnetOnFileReceivedCB) (stMagnetHeader *  header, stMagnetTransferInfo *  info);

/**
 * @brief Type definition function pointer for file transfer fail event
 *
 * When file transfer fails, call this method
 *
 * @param[out] header Opaque structure for magnet header
 *                     has information of peer's nodename, channel, and file type of user defined
 * @param[out] info File transfer info
 *                     has information of file transfer(filename, hash, exchange id, error reason)
 */
 typedef void (magnetOnFileFailedCB) (stMagnetHeader *  header, stMagnetTransferInfo *  info);

/**
 * @brief Type definition function pointer for sent chunk
 *
 * When chunk is sent to receiver, call this mehod
 *
 * @param[out] header Opaque structure for magnet header
 *                     has information of peer's nodename, channel, and file type of user defined
 * @param[out] info File transfer info
 *                     has information of file transfer(filename, hash, exchange id, filesize, offset, chunksize)
 */
 typedef void (magnetOnChunkSentCB) (stMagnetHeader *  header, stMagnetTransferInfo *  info);

/**
 * @brief Type definition function pointer for sent file
 *
 * When file is sent completely, call this method
 *
 * @param[out] header Opaque structure for magnet header
 *                     has information of peer's nodename, channel, and file type of user defined
 * @param[out] info File transfer info
 *                     has information of file transfer(filename, hash, exchange id, filesize,)
 */
 typedef void (magnetOnFileSentCB) (stMagnetHeader *  header, stMagnetTransferInfo *  info);

/**
 * @}
 */

/**
 * @defgroup Magnet_Interface Magnet Interface API
 */

/**
 * @addtogroup Magnet_Interface
 * @{
 */

/**
 * @brief Allocates and initializes a new magnet instance.
 *
 * A Magnet instance must be freed with MagnetRelease() after the caller
 * is done with it.
 *
 * @param[in] tmpPath Path where temporary files are stored. Must be accessible and writable by the current process
 *
 * @return The newly allocated magnet instance
 */
MAGNET_EXPORT
    BOOL MagnetInit (const char *  tmpPath);

/**
 * @brief Runs the magnet instance.
 *
 * This function will block until the handle is running.
 * It is safe to run multiple magnet handles in separate threads
 *
 * @return Boolean indicating whether the instance was successfully started
 */
MAGNET_EXPORT
    BOOL MagnetStart ();

/**
 * @brief Stops the specified magnet instance.
 *
 * This function does not destroy the magnet instance. It is safe to start/stop the handle
 * several times during it's lifecycle
 *
 * @return Boolean indicating whether the instance was successfully stopped
 */
MAGNET_EXPORT
     BOOL MagnetStop ();

/**
 * @brief Fully stops the magnet instance and release if it's running.
 *
 * This function also frees the memory associated with the magnet instance.
 * Accessing the instance after it's destroyed will case undefined behaviour
 *
 * @return true is destroy success, other is release fail
 */
MAGNET_EXPORT
    BOOL MagnetRelease ();

/**
 * @brief Joins a given channel
 *
 * @param[in] channel The name of the channel to join to
 *
 * @return Returns a boolean indicating whether joining the channel succeeded
*/
MAGNET_EXPORT
    BOOL MagnetJoinChannel (const char *  channel);

/**
 * @brief Leaves a given channel
 *
 * @param[in] channel The name of the channel to leave
 *
 * @return Returns a boolean indicating whether leaving the channel succeeded
 */
MAGNET_EXPORT
    BOOL MagnetLeaveChannel (const char *  channel);

/**
 * @brief Shares a file to a node or a channel
 *
 * Shares a file with other nodes in the network. The source file must be readable by the
 * current process.
 *
 * @param[in] header    The receiver information of destination peer and channel
 * @param[in] filePath    Path to the file to share
 * @param[in] timeoutMsec    Idle timeout for the fileshare. If the share is idle more than N msec it will be removed
 *
 * @return Exchange id for the this share. The exchange id can be used to uniquely identify this share on the sender.
 *         The caller must deallocate the returned string using free() function.
 */
MAGNET_EXPORT
    char* MagnetShareFile (stMagnetHeader *  header, const char *  filePath, long timeoutMsec);

/**
 * @brief Accept a file share from remote node
 *
 * Accepts a file being shared by a remote node.
 * Usually this function is called from magnetOnFileNotifiedCB callback.
 *
 * @param[in] channel         The name of channel
 * @param[in] exchangeId   The exchange id from the share notification
 * @param[in] chunkTimeoutMsec Timeout for each file chunk
 * @param[in] chunkRetries How many times to retry a chunk that reaches the timeout before giving up
 * @param[in] chunkSize     How large chunks to request
 *
 * @return Returns a boolean indicating whether accepting the file succeeded
 */
MAGNET_EXPORT
    BOOL MagnetAcceptFile (const char *  channel, const char *  exchangeId, long chunkTimeoutMsec, int chunkRetries, long chunkSize);

/**
 * @brief Rejects a file share from remote node
 *
 * Rejects a file being shared by a remote node.
 * Usually this function is called from magnetOnFileNotifiedCB callback.
 *
 * @param[in] channel        The name of channel
 * @param[in] exchangeId  The exchange id from the share notification
 *
 * @return Returns a boolean indicating whether rejecting the file succeeded
 */
MAGNET_EXPORT
    BOOL MagnetRejectFile (const char *  channel, const char *  exchangeId);

/**
 * @brief Cancels an active file transfers
 *
 * Cancels an on-going file transfer
 *
 * @param[in] channel        The name of channel
 * @param[in] exchangeId The exchange id from the share notification
 *
 * @return Returns a boolean indicating whether canceling the file succeeded
 */
MAGNET_EXPORT
    BOOL MagnetCancelFile (const char *  channel, const char *  exchangeId);

/**
 * @brief Sends data to a remote node or channel
 *
 * Sends data to a remote node on the network. It's important to note that
 * even if this function returns true, there are no guarantees that the data
 * has been reached the remote node. In cases where there are higher reliability
 * requirements, the use of manual acknowledgement is highly recommended.
 *
 * @param[in] header   The node to send the message to. Passing NULL as nodeName will cause data to be sent to all nodes on the 'channel' member
 * @param[in] payload  The payload to send. The payload is automatically destroyed after sending. In case the payload needs to be used
 *                                multiple times MagnetPayloadDup() can be used to duplicate the payload
 *
 * @return Returns a boolean indicating if the data was successfully queued for sending
 */
MAGNET_EXPORT
    BOOL MagnetSendData (stMagnetHeader *  header, stMagnetPayload **  payload);

/**
 * @}
 */

/**
 * @defgroup Magnet_Getter_Setter Magnet getter and setter
 */

/**
 * @addtogroup Magnet_Getter_Setter
 * @{
 */

/**
 * @brief Sets the userData member
 *
 * The userData member of the stMagnetHandle. The purpose of this parameter is to allow passing
 * additional information to callback functions. For example in case of JNI this could be a pointer
 * to the Java environment. If set userData is called multiple times the free function for previous
 * data will be invoked
 *
 *
 * @param[in] userData  Opaque blob of data to associated with the handle
 * @param[in] func        A free-function for the userData. If this parameter is non-NULL the handle will
 *                                 automatically free the userData during destruction.
 */
MAGNET_EXPORT
    void MagnetSetUserData (void *  userData, magnetUserdataFreeCB *  func);

/**
 * @brief Gets the userData member
 *
 * Returns the userData member of the handle set with MagnetSetUserData()
 *
 * @return Data associated with MagnetSetUserData() or NULL if none is set
 */
MAGNET_EXPORT
    void *  MagnetGetUserData ();

/**
 * @brief Set the listener callbacks on the this handle.
 *
 * The instance will take ownership of the callback structure and will take care of freeing it
 * during the destruction of the handle.
 *
 * @param[in] listener Magnet listener structure
 */
MAGNET_EXPORT
     void MagnetSetListener (stMagnetListener *  listener);

/**
 * @brief Get the listener callback structure associated.
 *
 * The returned buffer is valid until the handle associated with it is destroyed
 *
 * @return The core listener callback structure. The caller must not free this
 */
MAGNET_EXPORT
     stMagnetListener *  MagnetGetListener ();

/**
 * @brief Sets the path where temporary files are stored.
 *
 * The path for temporary files needs to be set before the instance is started using MagnetStart()
 *
 * @param[in] tmpPath Path where temporary files are stored. Must be accessible and writable by the current process
 *
 * @return Boolean indicating whether setting temporary path succeeded
 */
MAGNET_EXPORT
    BOOL MagnetSetTmpPath (const char *  tmpPath);

/**
 * @brief Gets the path where temporary files are being stored
 *
 * @return Path where temporary files are stored. The returned buffer is valid
 *             until the associated handle is destroyed
 */
MAGNET_EXPORT
    const char *  MagnetGetTmpPath ();

/**
 * @brief Get list of joined channels
 *
 * Returns a list of channels where the current instance has joined
 *
 * @param[out] numChannels Number of channels in the returned array
 *
 * @return Channels where the current instance has joined. The caller must deallocate this using MagnetFreeChannels().
 */
MAGNET_EXPORT
    char **  MagnetGetJoinedChannels (size_t *  numChannels);

/**
 * @brief Frees a channel list returned from MagneGtetJoinedChannels()
 *
 * @param[in] channels Channels got by MagnetGetJoinedChannels()
 * @param[in] numChannels Number of channels in the returned array
 */
MAGNET_EXPORT
    void MagnetFreeChannels (char **  channels, size_t numChannels);

/**
 * @brief Get connected nodes
 *
 * Get currently connected nodes in a channel
 *
 * @param[in]   channel      The name of channel from which to query the nodes. Passing public channel
 *                                      will return all nodes known in the network
 * @param[out] numNodes  Number of nodes in the returned array
 *
 * @return Nodes in the requested channel. The caller must deallocate this using MagnetFreeConnectedNodes()
 */
MAGNET_EXPORT
    char **  MagnetGetConnectedNodes (const char *  channel, size_t *  numNodes);

/**
 * @brief Frees a channel list returned from MagnetGetConnectedNodes()
 *
 * @param[in] nodes Nodes got by MagnetGetConnectedNodes()
 * @param[in] numNodes Number of nodes in the returned array
 */
MAGNET_EXPORT
    void MagnetFreeConnectedNodes (char **  nodes, size_t numNodes);

/**
 * @brief Setting timeout for node expiry
 *
 * @param[in] timeoutMsec Time for node expiry timeout
 */
MAGNET_EXPORT
    void MagnetSetLivenessTimeout (long timeoutMsec);

/**
 * @brief Setting use or not udp discover
 *
 * @param[in] value true is using udp discover
 */
MAGNET_EXPORT
    void MagnetSetUdpDiscover (BOOL value);

/**
 * @brief Setting use or not node expiry
 *
 * @param[in] value True is using node expiry
 */
MAGNET_EXPORT
    void MagnetSetNodeExpiry (BOOL value);


/**
 * @brief This function is still unstable
 *
 * @param[in] networkInterfaceInfo Network interface info name:ip
 *                   name - interface name
 *                   ip - ipv4 ip address
 *                   - eth0;*
 *                   - *;10.0.0.2
 *                   Star will signify "any", for example the second example reads "using interface eth0 and any ip".
 *                   This would bind to first ip on the matching nic.
 *
 * @return Returns false if the format does not match
 */
MAGNET_EXPORT
    BOOL MagnetSetUsingNetworkInterface (const char *  networkInterfaceInfo);

/**
 * @brief Get activated network interface
 *
 * @return Return Using network interface name
 */
MAGNET_EXPORT
    const char *  MagnetGetUsingInterface ();

/**
 * @brief Set network available state
 *
 * @param[in] connected True is connected, False is disconnected
 */
MAGNET_EXPORT
    void MagnetSetConnectedState (BOOL connected);

/**
 * @brief Get current magnet state
 *
 * @return Returns magnet state
 */
MAGNET_EXPORT
    int MagnetGetState();

/**
 * @brief Get my port
 *
 * @return the TCP port used for messaging
 */
MAGNET_EXPORT
    int MagnetGetMyPortNumber();

/**
 * @brief Get my ip
 *
 * @return the IP address of the interface where magnet instance is bounded.
 */
MAGNET_EXPORT
    const char *  MagnetGetMyIP();

/**
 * @brief Get peers IP address by nodemae
 *
 * @param[in] nodeName  Target nodename
 * @param[in] channel  Target channel
 *
 * @return IP address
 */
MAGNET_EXPORT
    const char *  MagnetGetNodeIpAddress(const char *  nodeName, const char *  channel);

/**
 * @}
 */

/**
 * @defgroup Magnet_Payload_API Magnet API for Magnet payload
 */

/**
 * @addtogroup Magnet_Payload_API
 * @{
 */

/**
 * @brief Allocated and initialize a new magnet payload
 *
 * Allocates and initializes a new payload object. The object must be
 * freed with MagnetPayloadDestroy() after the caller is done with it
 *
 * @return Returns a pointer to the newly allocated payload
 */
MAGNET_EXPORT
    stMagnetPayload *  MagnetPayloadInit ();

/**
 * @brief Creates a copy of the payload and the data inside it
 *
 * @param[in] payload Payload to be copied
 */
MAGNET_EXPORT
    stMagnetPayload *  MagnetPayloadDup (stMagnetPayload *  payload);

/**
 * @brief Get number of data items in the payload
 *
 * Each payload composes of one or more data items. This function will
 * return the count of data items in the payload.
 *
 * @param[in] payload Payload to be counted
 *
 * @return The amount of data items in the payload object
 */
MAGNET_EXPORT
    size_t MagnetPayloadCount (stMagnetPayload *  payload);

/**
 * @brief Appends a data item into the payload
 *
 * This function will add a data item into the payload. The payload takes
 * ownership of the stMagnetData object
 *
 * @param[in] payload Payload to be taken
 * @param[in] data      Data to be appended
 *
 * @return Returns a boolean whether adding the data succeeded
 */
MAGNET_EXPORT
    BOOL MagnetPayloadAppend (stMagnetPayload *  payload, stMagnetData *  data);

/**
 * @brief Appends a binary data into the payload
 *
 * This function will add a blob of data into the payload. The payload
 * object will automatically wrap the data into stMagnetData object.
 *
 * @param[in] payload  Payload to be taken
 * @param[in] data       Data to be appended
 * @param[in] dataSize Size of data
 *
 * @return Returns a boolean whether adding the data succeeded
 */
MAGNET_EXPORT
    BOOL MagnetPayloadAppendBlob (stMagnetPayload *  payload, unsigned char *  data, size_t dataSize);

/**
 * @brief Rewinds the internal iterator to first element in the payload
 *
 * This function rewinds the iterator to the first element. The common usage
 * of this function is:
 *
 *        stMagnetData *d;
 *        for (d = MagnetPayloadFirst (p) ;d != NULL; d = MagnetPayloadNext (p)) {
 *
 *            size_t siz;
 *            unsigned char *contents = MagnetDataGetContents (d, &siz);
 *
 *            if (contents) {
 *                  // Do stuff
 *            }
 *        }
 *
 * @param[in] payload  Payload
 *
 * @return Returns first element in the payload or NULL if the payload is empty
 *
 */
MAGNET_EXPORT
    stMagnetData *  MagnetPayloadFirst (stMagnetPayload *  payload);

/**
 * @brief Moves the internal pointer to next data element
 *
 * Moves the internal pointer in the payload to the next data element. The next
 * next data element is returned or NULL if there is no next element.
 * See MagnetPayloadFirst() for example usage.
 *
 * @param[in] payload  Payload
 *
 * @return The next data element in the payload
 */
MAGNET_EXPORT
    stMagnetData *  MagnetPayloadNext (stMagnetPayload *  payload);

/**
 * @brief Destroy the payload and all data elements in it
 *
 * This function frees all memory associated with the payload including
 * all data elements inside it
 *
 * @param[in] payload  Payload
 */
MAGNET_EXPORT
    void MagnetPayloadDestroy (stMagnetPayload **  payload);

/**
 * @}
 */

/**
 * @defgroup Magnet_Data_API Magnet API for Magnet data structure
 */

/**
 * @addtogroup Magnet_Data_API
 * @{
 */

/**
 * @brief Initializes new stMagnetData structure
 *
 * Initializes a new data structure. The new structure will be empty
 * until MagnetDataSetContents() is used to set the data. The alternative
 * function MagnetDataInitAndSet() allows creating and initializing a
 * data structure in the same call
 *
 * @return Pointer to the created data structure
 */
MAGNET_EXPORT
    stMagnetData *  MagnetDataInit ();

/**
 * @brief Initialize and set data
 *
 * A convenience function for doing MagnetDataInit() and MagnetDataSetContents()
 * in one call
 *
 * @param[in] content       Data to be set to Magnet data
 * @param[in] contentSize Size of content
 *
 * @return Pointer to the created data structure
 */
MAGNET_EXPORT
    stMagnetData *  MagnetDataInitAndSet (unsigned char *  content, size_t contentSize);

/**
 * @brief Set the contents of the data object
 *
 * Set the data contents of the structure. Calling this function on a structure that
 * already has data will cause the previous data to be freed and replaced with new data.
 * The structure will take a copy of the data
 *
 * @param[in] data               Magnet data
 * @param[in] content           Data to be initialized
 * @param[in] contentSize     Size of data
 *
 * The contents can be destroyed by passing NULL as data and 0 as dataSize
 *
 */
MAGNET_EXPORT
    void MagnetDataSetContents (stMagnetData *  data, unsigned char *  content, size_t contentSize);

/**
 * @brief Get the contents of the data object
 *
 * Returns a pointer to the data member of the structure. The second argument will be set
 * to the size of the data member.
 *
 * @param[in] data  Magnet data
 *
 * @return Returns a pointer to the content data member of the structure. The caller must not free this directly
 */
MAGNET_EXPORT
    unsigned char *  MagnetDataGetContents (stMagnetData *  data);

/**
 * @brief Get the size of the data object
 *
 * Returns a size of data.
 *
 * @param[in] data  Magnet data
 *
 * @return Returns a size of data
 */
MAGNET_EXPORT
    size_t MagnetDataGetSize (stMagnetData *  data);

/**
 * @brief Get the contents of the data object and contents size
 *
 * Returns a pointer to the contents
 *
 * @param[in]   data     Magnet data
 * @param[out] contentSize  Size of data
 *
 * @return Returns a pointer to the data
 */
MAGNET_EXPORT
    unsigned char *  MagnetDataGet (stMagnetData *  data, size_t *  contentSize);

/**
 * @brief Destroys the data structure and frees memory associated with it
 *
 * Destroys the stMagnetData structure and frees the data member if it's set
 * to a non-NULL value
 *
 * @param[in]  data   Magnet data
 */
MAGNET_EXPORT
    void MagnetDataDestroy (stMagnetData **  data);

/**
 * @}
 */

/**
 * @defgroup Magnet_Callback_Initialize_API Magnet API for initializing callback structure
 */

/**
 * @addtogroup Magnet_Callback_Initialize_API
 * @{
 */

/**
 * @brief Allocate and initialize a new core callback structure
 *
 * Initializes a new listener callback structure and sets it's members to NULL.
 *
 * @return The new listener callback structure
 */
MAGNET_EXPORT
    stMagnetListener *  MagnetListenerInit ();

/**
 * @brief Set magnet core listening callback function to listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 * @param[in] func      Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnListeningCB (stMagnetListener *  listener, magnetOnListeningCB *  func);

/**
 * @brief Get magnet core listening callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */
MAGNET_EXPORT
    magnetOnListeningCB *  MagnetListenerGetOnListeningCB (stMagnetListener *  listener);

/**
 * @brief Set magnet core stopeed callback function to listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 * @param[in] func      Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnStoppedCB (stMagnetListener *  listener, magnetOnStoppedCB *  func);

/**
 * @brief Get magnet core stopeed callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */
MAGNET_EXPORT
    magnetOnStoppedCB *  MagnetListenerGetOnStoppedCB (stMagnetListener *  listener);

/**
 * @brief Set magnet on peers callback function to listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 * @param[in] func      Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnPeersCB (stMagnetListener *  listener, magnetOnPeersCB *  func);

/**
 * @brief Get magnet on peers callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */
MAGNET_EXPORT
    magnetOnPeersCB *  MagnetListenerGetOnPeersCB (stMagnetListener *  listener);

/**
 * @brief Set magnet no peers callback function to listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 * @param[in] func      Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnNoPeersCB (stMagnetListener *  listener, magnetOnNoPeersCB *  func);

/**
 * @brief Get magnet no peers callback function from listener
 *

 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */
MAGNET_EXPORT
    magnetOnNoPeersCB *  MagnetListenerGetOnNoPeersCB (stMagnetListener *  listener);

/**
 * @brief Set core start error callback function to listener
 *
 * @param[in] listener   Magnet listener created with MagnetListenerInit()
 * @param[in] func       Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnServiceErrorCB (stMagnetListener *  listener,  magnetOnServiceErrorCB *  func);

/**
 * @brief Get core start error callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */MAGNET_EXPORT
    magnetOnServiceErrorCB *  MagnetListenerGetOnServiceErrorCB (stMagnetListener *  listener);

/**
 * @brief Set peer joined callback function to listener
 *
 * @param[in] listener      Magnet listener created with MagnetListenerInit()
 * @param[in] func   Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnJoinCB (stMagnetListener *  listener, magnetOnJoinCB *  func);

/**
 * @brief Get peer joined callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */MAGNET_EXPORT
    magnetOnJoinCB *  MagnetListenerGetOnJoinCB (stMagnetListener *  listener);

/**
 * @brief Set peer leaved callback function to listener
 *
 * @param[in] listener      Magnet listener created with MagnetListenerInit()
 * @param[in] func   Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnLeaveCB (stMagnetListener *  listener, magnetOnLeaveCB *  func);

/**
 * @brief Get peer left callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */MAGNET_EXPORT
    magnetOnLeaveCB *  MagnetListenerGetOnLeaveCB (stMagnetListener *  listener);

/**
 * @brief Set receive data callback function to listener
 *
 * @param[in] listener      Magnet listener created with MagnetListenerInit()
 * @param[in] func   Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnDataReceivedCB (stMagnetListener *  listener, magnetOnDataReceivedCB *  func);

/**
 * @brief Get receive data callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 * @return Returns a pointer of callback function
 */
 MAGNET_EXPORT
    magnetOnDataReceivedCB *  MagnetListenerGetOnDataReceivedCB (stMagnetListener *  listener);

/**
 * @brief Set request file share notification callback function to listener
 *
 * @param[in] listener      Magnet listener created with MagnetListenerInit()
 * @param[in] func   Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnFileNotifiedCB (stMagnetListener *  listener, magnetOnFileNotifiedCB *  func);

/**
 * @brief Get request file share notification callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */
 MAGNET_EXPORT
    magnetOnFileNotifiedCB *  MagnetListenerGetOnFileNotifiedCB (stMagnetListener *  listener);

/**
 * @brief Set complete file received callback function to listener
 *
 * @param[in] listener      Magnet listener created with MagnetListenerInit()
 * @param[in] func   Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnFileReceivedCB (stMagnetListener *  listener, magnetOnFileReceivedCB *  func);

/**
 * @brief Get complete file received callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */
 MAGNET_EXPORT
    magnetOnFileReceivedCB *  MagnetListenerGetOnFileReceivedCB (stMagnetListener *  listener);

/**
 * @brief Set file chunk receive callback function to listener
 *
 * @param[in] listener      Magnet listener created with MagnetListenerInit()
 * @param[in] func   Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnChunkReceivedCB (stMagnetListener *  listener, magnetOnChunkReceivedCB *  func);

/**
 * @brief Get file chunk receive callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */
 MAGNET_EXPORT
    magnetOnChunkReceivedCB *  MagnetListenerGetOnChunkReceivedCB (stMagnetListener *  listener);

/**
 * @brief Set file transfer fail callback function to listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 * @param[in] func   Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnFileFailedCB (stMagnetListener *  listener, magnetOnFileFailedCB *  func);

/**
 * @brief Get file transfer fail callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */
 MAGNET_EXPORT
    magnetOnFileFailedCB *  MagnetListenerGetOnFileFailedCB (stMagnetListener *  listener);

/**
 * @brief Set sent file chunk callback function to listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 * @param[in] func   Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnChunkSentCB (stMagnetListener *  listener, magnetOnChunkSentCB *  func);

/**
 * @brief Get sent file chunk callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */
 MAGNET_EXPORT
    magnetOnChunkSentCB *  MagnetListenerGetOnChunkSentCB (stMagnetListener *  listener);

/**
 * @brief Set complete file sent callback function to listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 * @param[in] func   Magnet callback function to set
 */
MAGNET_EXPORT
    void MagnetListenerSetOnFileSentCB (stMagnetListener *  listener, magnetOnFileSentCB *  func);

/**
 * @brief Get complete file sent callback function from listener
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 * @return Returns a pointer of callback function
 */
 MAGNET_EXPORT
    magnetOnFileSentCB *  MagnetListenerGetOnFileSentCB (stMagnetListener *  listener);

/**
 * @brief Deallocate a listener callback structure.
 *
 * It's important to note that when callbacks are associated with a magnet instance
 * using MagnetSetListener () the instance will take ownership of this structure and
 * takes care of freeing it during the destruction of it.
 *
 * @param[in] listener  Magnet listener created with MagnetListenerInit()
 *
 */
MAGNET_EXPORT
    void MagnetListenerDestroy (stMagnetListener **  listener);

/**
 * @}
 */

/**
 * @defgroup Magnet_Transfer_Information_API Magnet API for Magnet transfer information structure
 */

/**
 * @addtogroup Magnet_Transfer_Information_API
 * @{
 */

/**
 * @brief Initializes Magnet transfer information structure
 *
 * @return Pointer to the created transfer information data structure
 */
MAGNET_EXPORT
    stMagnetTransferInfo *  MagnetTransferInfoInit ();

/**
 * @brief Set filename to file transfer info structure
 *
 * @param info File transfer info
 * @param fileName Filename to set
 */
MAGNET_EXPORT
    void MagnetTransferInfoSetFilename (stMagnetTransferInfo *  info, const char *  fileName);

/**
 * @brief Get filename from file transfer info structure
 *
 * @param info File transfer info
 *
 * @return Returns filename
 */
MAGNET_EXPORT
    const char *  MagnetTransferInfoGetFilename (stMagnetTransferInfo *  info);

/**
 * @brief Set file hash to file transfer info structure
 *
 * @param info File transfer info
 * @param hash File hash to set
 */
MAGNET_EXPORT
    void MagnetTransferInfoSetHash (stMagnetTransferInfo *  info, const char *  hash);

/**
 * @brief Get file hash from file transfer info structure
 *
 * @param info File transfer info
 *
 * @return Returns file hash
 */
MAGNET_EXPORT
    const char *  MagnetTransferInfoGetHash (stMagnetTransferInfo *  info);

/**
 * @brief Set transfer exchange ID to file transfer info structure
 *
 * @param info File transfer info
 * @param exchangeId Transfer exchange ID to set
 */
MAGNET_EXPORT
    void MagnetTransferInfoSetExchangeId (stMagnetTransferInfo *  info, const char *  exchangeId);

/**
 * @brief Get Transfer exchange ID from file transfer info structure
 *
 * @param info File transfer info
 *
 * @return Returns transfer exchange ID
 */
MAGNET_EXPORT
    const char *  MagnetTransferInfoGetExchangeId (stMagnetTransferInfo *  info);

/**
 * @brief Set temporary file save path to file transfer info structure
 *
 * @param info File transfer info
 * @param tmpPath Temporary file save path to set
 */
MAGNET_EXPORT
    void MagnetTransferInfoSetTmpPath (stMagnetTransferInfo *  info, const char *  tmpPath);

/**
 * @brief Get temporary file save path from file transfer info structure
 *
 * @param info File transfer info
 *
 * @return Returns temporary file save path
 */
MAGNET_EXPORT
    const char *  MagnetTransferInfoGetTmpPath (stMagnetTransferInfo *  info);

/**
 * @brief Set file size to file transfer info structure
 *
 * @param info File transfer info
 * @param fileSize File size to set
 */
MAGNET_EXPORT
    void MagnetTransferInfoSetFileSize (stMagnetTransferInfo *  info, size_t fileSize);

/**
 * @brief Get file size from file transfer info structure
 *
 * @param info File transfer info
 *
 * @return Returns file size
 */
MAGNET_EXPORT
    size_t MagnetTransferInfoGetFileSize (stMagnetTransferInfo *  info);

/**
 * @brief Set file offset to file transfer info structure
 *
 * @param info File transfer info
 * @param offset File offset to set
 */
MAGNET_EXPORT
    void MagnetTransferInfoSetOffset (stMagnetTransferInfo *  info, size_t offset);

/**
 * @brief Get file offset from file transfer info structure
 *
 * @param info File transfer info
 *
 * @return Returns file offset
 */
MAGNET_EXPORT
    size_t MagnetTransferInfoGetOffset (stMagnetTransferInfo *  info);

/**
 * @brief Set file chunksize to file transfer info structure
 *
 * @param info File transfer info
 * @param chunkSize File chunksize to set
 */
MAGNET_EXPORT
    void MagnetTransferInfoSetChunkSize (stMagnetTransferInfo *  info, size_t chunkSize);

/**
 * @brief Get file chunksize from file transfer info structure
 *
 * @param info File transfer info
 *
 * @return Returns file chunksize
 */
MAGNET_EXPORT
    size_t MagnetTransferInfoGetChunkSize (stMagnetTransferInfo *  info);

/**
 * @brief Set file transfer fail reason to file transfer info structure
 *
 * @param info File transfer info
 * @param reason file transfer fail reason. Refet to the MagnetErrorCode.h about file trasnfer
 */
MAGNET_EXPORT
    void MagnetTransferInfoSetReason (stMagnetTransferInfo *  info, int reason);

/**
 * @brief Get file transfer fail reason from file transfer info structure
 *
 * @param info File transfer info
 *
 * @return Returns file transfer fail reason
 */
MAGNET_EXPORT
    int MagnetTransferInfoGetReason (stMagnetTransferInfo *  info);

/**
 * @brief Memory free and delete file transfer structure
 *
 * @param p Pointer of file transfer info structure
 */
MAGNET_EXPORT
    void MagnetTransferInfoDestroy (stMagnetTransferInfo **  p);

/**
 * @}
 */

/**
 * @defgroup Magnet_Header_API Magnet API for Magnet header structure
 */

/**
 * @addtogroup Magnet_Header_API
 * @{
 */

/**
 * @brief Allocate stMagnetHeader type memory
 *
 * @return Returns stMagnetHeader type memory
 */
MAGNET_EXPORT
    stMagnetHeader *  MagnetHeaderInit ();

/**
 * @brief Duplicate header
 *
 * @return Returns Duplicate stMagnetHeader
 */
MAGNET_EXPORT
    stMagnetHeader *  MagnetHeaderDup (stMagnetHeader *  header);

/**
 * @brief Set nodename to magnet header structure
 *
 * @param header Opaque structure for magnet header
 * @param nodeName Node name
 */
MAGNET_EXPORT
    void MagnetHeaderSetNodeName (stMagnetHeader *  header, const char *  nodeName);

/**
 * @brief Get nodename from magnet header structure
 *
 * @param header Opaque structure for magnet header
 *
 * @return Returns node name
 */
MAGNET_EXPORT
    const char *  MagnetHeaderGetNodeName (stMagnetHeader *  header);

/**
 * @brief Set channel to magnet header structure
 *
 * @param header Opaque structure for magnet header
 * @param channel channel name
 */
MAGNET_EXPORT
    void MagnetHeaderSetChannel (stMagnetHeader *  header, const char *  channel);

/**
 * @brief Get channel from magnet header structure
 *
 * @param header Opaque structure for magnet header
 *
 * @return Returns channel name
 */
MAGNET_EXPORT
    const char *  MagnetHeaderGetChannel (stMagnetHeader *  header);

/**
 * @brief Set message or file type to magnet header structure
 *
 * @param header Opaque structure for magnet header
 * @param type Message or file type defined by user
 */
MAGNET_EXPORT
    void MagnetHeaderSetType (stMagnetHeader *  header, const char *  type);

/**
 * @brief Get nodename from magnet header structure
 *
 * @param header Opaque structure for magnet header
 *
 * @return Returns type
 */
MAGNET_EXPORT
    const char *  MagnetHeaderGetType (stMagnetHeader *  header);

/**
 * @brief Memory free and delete magnet header structure
 *
 * @param header Pointer of magnet header structure
 */
MAGNET_EXPORT
    void MagnetHeaderDestroy (stMagnetHeader **  header);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* _MAGNET_API_H_ */
