#ifndef _MagnetErrorCode_H_
#define _MagnetErrorCode_H_

/**
 * @defgroup Magnet_Core_Macros Magnet Error Codes about magnet start
 */

/**
 * @addtogroup Magnet_Core_Macros
 * @{
 */

/**
 * @brief Default value for the magnet core created
 */
#define CORE_CREATE                             1000;

/**
 * @brief The system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process PTHREAD_THREADS_MAX would be exceeded
 */
#define CORE_CREATE_FAIL_EAGAIN		            1001;

/**
 * @brief The value specified by attribute is invalid
 */
#define CORE_CREATE_FAIL_EINVAL                 1002;

/**
 * @brief The caller does not have appropriate permission to set the required scheduling parameters or scheduling policy
 */
#define CORE_CREATE_FAIL_EPERM                  1003;

/**
 * @brief Unknown failure 
 */
#define CORE_CREATE_FAIL_UNKNOWN                1004;

/**
 * @} 
 */

/**
 * @defgroup Magnet_File_Share_Macros Magnet Error Codes about file transfer
 */

/**
 * @addtogroup Magnet_File_Share_Macros
 * @{
 */

/**
 * @brief Default value for the magnet file transfer 
 */
#define FILE_SHARE                              5000;

/**
 * @brief There is not peer information when file transfer is started 
 */
#define FILE_SHARE_ERROR_PEER_NOT_FOUND         5001;

/**
 * @brief Fail to allocate the memory to send file 
 */
#define FILE_SHARE_ERROR_MEMORY_ALLOC           5002;

/**
 * @brief Fail to create file that is sent 
 */
#define FILE_SHARE_ERROR_FILE_CREATE_FAIL       5003;

/**
 * @brief The file size is larger than UINT size
 */
#define FILE_SHARE_ERROR_INVALID_FILE_SIZE      5004;

/**
 * @brief Transfer message has an error 
 */
#define FILE_SHARE_ERROR_INVALID_TRANSFER_MSG   5005;

/**
 * @brief Time out is occured when file transfer is in progress 
 */
#define FILE_SHARE_ERROR_TRANSFER_TIME_OUT      5006;

/**
 * @brief Internal error is occrued when file transfer is preparing 
 */
#define FILE_SHARE_ERROR_TRANSFER_PREPARE       5007;

/**
 * @brief Receiver reject the file transfer 
 */
#define FILE_SHARE_ERROR_REJECT_FILE_TRANSFER   5011;

/**
 * @brief Receiver cancel the file transfer 
 */
#define FILE_SHARE_ERROR_CANCEL_FILE_TRANSFER   5012;

/**
 * @} 
 */

#endif
