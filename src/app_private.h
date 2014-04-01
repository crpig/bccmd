#ifndef __APP_PRIVATE_H__
#define __APP_PRIVATE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "csr.h"
#include "bt_types.h"
#include "bt_if.h"
#include "bt_types.h"
#include "app_cmd.h"
#include "app_evt.h"
#include "app_bcsp.h"
#include "app_handler.h"
#include "app_utils.h"
#include "app_logcat.h"

/******************************************************************************
**  Constants & Macros
******************************************************************************/
#define APP_BT_ANA_FREQ     0x6590      /* 26M */
#define APP_BT_BAUDRATE     B921600

/* Definition For bccmd RadioTest TestID */
#define TESTID_TXSTART          1
#define TESTID_RXSTART1         2
#define TESTID_RXSTART2         3
#define TESTID_TXDATA1          4
#define TESTID_TXDATA2          5
#define TESTID_TXDATA3          6
#define TESTID_TXDATA4          7
#define TESTID_RXDATA1          8
#define TESTID_RXDATA2          9
#define TESTID_BER1             19
#define TESTID_BER2             20
#define TESTID_CONFIG_FREQ      22
#define TESTID_CONFIG_PKT       23
#define TESTID_CONFIG_BIT_ERR   24
#define TESTID_CONFIG_TX_IF     27
#define TESTID_CONFIG_HOPPING   45

/* Definition For HQ RadioTest Results */
#define HQ_RADIOTEST_RSSI           0x0801
#define HQ_RADIOTEST_RX_PKT_STATUS  0x1001
#define HQ_RADIOTEST_BIT_ERR        0x1006

#define MAKE_MESSAGE(TYPE)  \
    TYPE##_T *msg = malloc(sizeof(TYPE##_T));\
    memset(msg, 0, sizeof(TYPE##_T));

/******************************************************************************
**  Type definitions
******************************************************************************/
typedef struct
{
    bool            runing;                         /* thread state */
    char            device[APP_MAX_NAME_LEN];       /* Uart Name */
    uint16          pendingCmd;                     /* Pending cmd */
    bool            receivePending;                 /* pending when receive hq ind */
    pthread_mutex_t mutex;
} APP_INST_DATA_T;

typedef struct
{
    uint16  opcode;
    void    *msg;
} APP_MSG_T;


/******************************************************************************
**  Extern variables and functions
******************************************************************************/
extern APP_INST_DATA_T  *theApp;
extern APP_MSG_T        *app_msg;
extern pthread_t        testThread;
extern handlerCbFunc    testCb;


#ifdef __cplusplus
}
#endif


#endif /* __APP_PRIVATE_H__ */

