#ifndef __APP_CMD_H__
#define __APP_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "bt_types.h"

/******************************************************************************
**  Constants & Macros
******************************************************************************/
#define APP_MAX_NAME_LEN    30


/******************************************************************************
**  Type definitions
******************************************************************************/
typedef struct
{
    uint8   name[APP_MAX_NAME_LEN];
}BT_THREAD_DATA_T;

typedef struct
{
    uint16 testId;
    uint16 param[5];
}BT_RADIOTEST_COMMAN_REQ_T;


/******************************************************************************
**  Functions
******************************************************************************/
uint8 SendMessage(uint16 opcode, void *msg);

#ifdef __cplusplus
}
#endif

#endif /* __APP_CMD_H__ */
