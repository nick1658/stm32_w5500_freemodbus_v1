/******************** (C) COPYRIGHT 2013 **************************
 * 文件名  ：mbport.h
 * 描述    ：STM32 Modbus 移植测试程序         
 * 实验平台：STM32F103VCT6+W5500 开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：Zhangsz
 * 编写日期：2013-10-03
**********************************************************************************/

#ifndef _MB_PORT_H
#define _MB_PORT_H



/* ----------------------- Type definitions ---------------------------------*/

typedef enum
{
    EV_READY,                   /*!< Startup finished. */
    EV_FRAME_RECEIVED,          /*!< Frame received. */
    EV_EXECUTE,                 /*!< Execute function. */
    EV_FRAME_SENT               /*!< Frame sent. */
} eMBEventType;

/*! \ingroup modbus
 * \brief Parity used for characters in serial mode.
 *
 * The parity which should be applied to the characters sent over the serial
 * link. Please note that this values are actually passed to the porting
 * layer and therefore not all parity modes might be available.
 */
typedef enum
{
    MB_PAR_NONE,                /*!< No parity. */
    MB_PAR_ODD,                 /*!< Odd parity. */
    MB_PAR_EVEN                 /*!< Even parity. */
} eMBParity;

/* ----------------------- Supporting functions -----------------------------*/
BOOL            xMBPortEventInit( void );

BOOL            xMBPortEventPost( eMBEventType eEvent );

BOOL            xMBPortEventGet(  /*@out@ */ eMBEventType * eEvent );

/* ----------------------- Serial port functions ----------------------------*/

BOOL            xMBPortSerialInit( UCHAR ucPort, ULONG ulBaudRate,
                                   UCHAR ucDataBits, eMBParity eParity );

void            vMBPortClose( void );

void            xMBPortSerialClose( void );

void            vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable );

BOOL            xMBPortSerialGetByte( CHAR * pucByte );

BOOL            xMBPortSerialPutByte( CHAR ucByte );

/* ----------------------- Timers functions ---------------------------------*/
BOOL            xMBPortTimersInit( USHORT usTimeOut50us );

void            xMBPortTimersClose( void );

void            vMBPortTimersEnable( void );

void            vMBPortTimersDisable( void );

void            vMBPortTimersDelay( USHORT usTimeOutMS );

/* ----------------------- Callback for the protocol stack ------------------*/

/*!
 * \brief Callback function for the porting layer when a new byte is
 *   available.
 *
 * Depending upon the mode this callback function is used by the RTU or
 * ASCII transmission layers. In any case a call to xMBPortSerialGetByte()
 * must immediately return a new character.
 *
 * \return <code>TRUE</code> if a event was posted to the queue because
 *   a new byte was received. The port implementation should wake up the
 *   tasks which are currently blocked on the eventqueue.
 */
extern          BOOL( *pxMBFrameCBByteReceived ) ( void );

extern          BOOL( *pxMBFrameCBTransmitterEmpty ) ( void );

extern          BOOL( *pxMBPortCBTimerExpired ) ( void );

/* ----------------------- TCP port functions -------------------------------*/
BOOL            xMBTCPPortInit( USHORT usTCPPort );

void            vMBTCPPortClose( void );

void            vMBTCPPortDisable( void );

BOOL            xMBTCPPortGetRequest( UCHAR **ppucMBTCPFrame, USHORT * usTCPLength );

BOOL            xMBTCPPortSendResponse( const UCHAR *pucMBTCPFrame, USHORT usTCPLength );


#endif
