#pragma once

#include <stdint.h>
//four Bytes(high -> low):31bit:1 error 0:OK module name(15-30bits) + module seq(1Bytes) + module error type(1Bytes)
//0xB74E0001 => 'GN' 00 01 :GNSS　module system error
enum MOUDLE_EN
{
    MODULE_GNSS     = 0x474E, /* gnss module */
    MODULE_RADAR    = 0x5241, /* radar module */
    MODULE_RPICAN   = 0x5250, /* rpican module */
    MODULE_V2X      = 0x5632, /* v2x module*/
    MODULE_END 
};

enum SocketCanStatus
{
    STATUS_OK =  0,
    STATUS_SOCKET_CREATE_ERROR = 2,
    STATUS_INTERFACE_NAME_TO_IDX_ERROR =  3,
    STATUS_MTU_ERROR =  4, /// maximum transfer unit
    STATUS_CANFD_NOT_SUPPORTED = 5, /// Flexible data-rate is not supported on this interface
    STATUS_ENABLE_FD_SUPPORT_ERROR = 6, /// Error on enabling fexible-data-rate support
    STATUS_WRITE_ERROR =  7,
    STATUS_READ_ERROR =  8,
    STATUS_BIND_ERROR = 9,
};


enum ERR_EN
{
    ERR_OK          = 0,  /* OK */
    ERR_SYS         = 1,  /*  */
    ERR_NODEV       = 2,  /*  */
    ERR_NOFILE      = 2,  /*  */
    ERR_TIMEOUT     = 3,  /*  */
    ERR_INVAL       = 4,  /* param or data invalid */
    ERR_NOFUN       = 5,  /*  */
    ERR_BUSY        = 6,  /*  */
    ERR_NOINIT      = 7,  /*  */
    ERR_NOMEM       = 8,  /*  */
    ERR_NODISK      = 9,  /*  */
    ERR_NORECORD    = 10, /*  */
    ERR_CFG         = 11, /*  */
    ERR_NOCFG       = 12, /*  */
    ERR_DEVUNSTAB   = 13, /*  */
    ERR_DISCONNECT  = 14, /*  */
    ERR_INTERFACE_TO_IDX = 16,
    ERR_NOT_SUPPORT = 18,
    ERR_OTHER       = 80  /*  */

};

inline int error_code(int module, int errnum)
{
    return ((1<< 31) + ((module & 0xFFFF)<< 16) + (0 << 8) + (errnum & 0xFF));
}