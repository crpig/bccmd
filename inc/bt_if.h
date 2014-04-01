#ifndef __BT_HOST_INTERFACE_H__
#define __BT_HOST_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "bt_types.h"

/******************************************************************************
**  Constants & Macros
******************************************************************************/
#define BT_INVALID_OPCODE      0xFFFF

#define COMMAND(x)      (x)
#define EVENT(x)        (x | 0x8000)


#define BT_RADIOTEST_POWER_ON_REQ           COMMAND(0x01)
#define BT_RADIOTEST_POWER_OFF_REQ          COMMAND(0x02)
#define BT_RADIOTEST_WARM_RESET_REQ         COMMAND(0x03)
#define BT_RADIOTEST_TX_START_REQ           COMMAND(0x04)
#define BT_RADIOTEST_RX_START_REQ           COMMAND(0x05)
#define BT_RADIOTEST_TX_DATA_REQ            COMMAND(0x06)
#define BT_RADIOTEST_TX_DATA2_REQ           COMMAND(0x07)
#define BT_RADIOTEST_RX_DATA1_REQ           COMMAND(0x08)
#define BT_RADIOTEST_RX_DATA2_REQ           COMMAND(0x09)
#define BT_RADIOTEST_BER1_REQ               COMMAND(0x0A)

#define BT_RADIOTEST_CONFIG_FREQ_REQ        COMMAND(0x20)
#define BT_RADIOTEST_CONFIG_PACKET_REQ      COMMAND(0x21)
#define BT_RADIOTEST_CONFIG_TX_IF_REQ       COMMAND(0x22)
#define BT_RADIOTEST_CONFIG_HOPPING_SEQ_REQ COMMAND(0x23)

#define BT_RADIOTEST_RSSI_IND               EVENT(0x24)
#define BT_RADIOTEST_RX_PKT_STATUS_IND      EVENT(0x25)
#define BT_RADIOTEST_BIT_ERR_IND            EVENT(0x26)

#define BT_RADIOTEST_COMMAN_REQ
#define BT_RADIOTEST_COMMAN_CFM

#define BT_RADIOTEST_POWER_ON_CFM           BT_RADIOTEST_POWER_ON_REQ
#define BT_RADIOTEST_POWER_OFF_CFM          BT_RADIOTEST_POWER_OFF_REQ
#define BT_RADIOTEST_WARM_RESET_CFM         BT_RADIOTEST_WARM_RESET_REQ
#define BT_RADIOTEST_TX_START_CFM           BT_RADIOTEST_TX_START_REQ
#define BT_RADIOTEST_RX_START_CFM           BT_RADIOTEST_RX_START_REQ
#define BT_RADIOTEST_TX_DATA_CFM            BT_RADIOTEST_TX_DATA_REQ
#define BT_RADIOTEST_TX_DATA2_CFM           BT_RADIOTEST_TX_DATA2_REQ
#define BT_RADIOTEST_RX_DATA1_CFM           BT_RADIOTEST_RX_DATA1_REQ
#define BT_RADIOTEST_RX_DATA2_CFM           BT_RADIOTEST_RX_DATA2_REQ
#define BT_RADIOTEST_BER1_CFM               BT_RADIOTEST_BER1_REQ

#define BT_RADIOTEST_CONFIG_FCFM_CFM        BT_RADIOTEST_CONFIG_FREQ_REQ
#define BT_RADIOTEST_CONFIG_PACKET_CFM      BT_RADIOTEST_CONFIG_PACKET_REQ
#define BT_RADIOTEST_CONFIG_TX_IF_CFM       BT_RADIOTEST_CONFIG_TX_IF_REQ
#define BT_RADIOTEST_CONFIG_HOPPING_SEQ_CFM BT_RADIOTEST_CONFIG_HOPPING_SEQ_REQ


/* RESULT */
#define BT_RESULT_SUCCESS       0x00
#define BT_RESULT_FAILED        0x01
#define BT_RESULT_BUSY          0x02

/* For TX DATA */
#define RADIOTEST_TX_DATA1      0x01
#define RADIOTEST_TX_DATA3      0x03
#define RADIOTEST_TX_DATA4      0x04

/* For RX Start */
#define RADIOTEST_RX_START1     0x01
#define RADIOTEST_RX_START2     0x02



/******************************************************************************
**  Type definitions
******************************************************************************/
typedef void (*handlerCbFunc)(uint16 opcode, void *msg);

typedef struct
{
    uint16 status;
}BT_RADIOTEST_COMMAN_CFM_T;

typedef struct
{
    uint16 rssi;            /* Received Signal Strength Indication */
}BT_RADIOTEST_RSSI_IND_T;

typedef struct
{
    uint16 n_pkts;          /* Total packets received */
    uint16 n_good_pkts;     /* The number of good packets */
    uint16 n_corr_pkts;     /* The number of fixed packets */
    uint16 rssi;            /* Received Signal Strength Indication */
    bool   rssi_valid;      /* rssi is valid only when this is TRUE */
}BT_RADIOTEST_RX_PKT_STATUS_IND_T;

/******************************************************************************
**  Functions
******************************************************************************/
/*****************************************************************************
**
** @brief   : Register the confirm callback
**
** @param   : handlerCbFunc func
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestRegisterHandlerCbReq(handlerCbFunc func);

/*****************************************************************************
**
** @brief   : BtTestPowerOnReq
**
** @param   : char *device     For tmap  "/dev/ttymxc1"
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestPowerOnReq(char *device);

/*****************************************************************************
**
** @brief   : BtTestPowerOffReq
**
** @param   : void
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestPowerOffReq(void);

/*****************************************************************************
**
** @brief   : BtTestWarmResetReq
**
** @param   : void
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestWarmResetReq(void);


/******************************** Radio Test ******************************************/
/*****************************************************************************
**
** @brief   : BtTestTxStartReq
**
** @param   : uint16 freq
** @param   : uint16 gain
** @param   : int16 mod_freq
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestTxStartReq(uint16 freq, uint16 gain, int16 mod_freq);

/*****************************************************************************
**
** @brief   : BtTestRxStartReq
**
** @param   : uint8 id          RADIOTEST_RX_START1/RADIOTEST_RX_START2
** @param   : uint16 freq
** @param   : bool highside
** @param   : uint16 attn
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestRxStartReq(uint8 id, uint16 freq, bool highside, uint16 attn);

/*****************************************************************************
**
** @brief   : BtTestTxDataReq
**
** @param   : uint8 id          RADIOTEST_TX_DATA1/RADIOTEST_TX_DATA3/RADIOTEST_TX_DATA4
** @param   : uint16 freq
** @param   : uint16 gain
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestTxDataReq(uint8 id, uint16 freq, uint16 gain);

/*****************************************************************************
**
** @brief   : BtTestTxData2Req
**
** @param   : uint16 cc
** @param   : uint16 gain
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestTxData2Req(uint16 cc, uint16 gain);

/*****************************************************************************
**
** @brief   : BtTestRxData1Req
**
** @param   : uint16 freq
** @param   : bool highside
** @param   : uint16 attn
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestRxData1Req(uint16 freq, bool highside, uint16 attn);

/*****************************************************************************
**
** @brief   : BtTestRxData2Req
**
** @param   : uint16 cc
** @param   : bool highside
** @param   : uint16 attn
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestRxData2Req(uint16 cc, bool highside, uint16 attn);

/*****************************************************************************
**
** @brief   : BtTestBER1Req
**
** @param   : uint16 freq
** @param   : bool highside
** @param   : uint16 attn
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestBER1Req(uint16 freq, bool highside, uint16 attn);
/******************************** Radio Test ******************************************/


/******************************** Configuration ***************************************/
/*****************************************************************************
**
** @brief   : BtTestConfigFreqReq
**
** @param   : uint16 txrx_freq
** @param   : uint16 report_freq
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestConfigFreqReq(uint16 txrx_freq, uint16 report_freq);

/*****************************************************************************
**
** @brief   : BtTestConfigPacketReq
**
** @param   : uint16 pkt_type
** @param   : uint16 pkt_size
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestConfigPacketReq(uint16 pkt_type, uint16 pkt_size);

/*****************************************************************************
**
** @brief   : BtTestConfigTxIfReq
**
** @param   : int16 offset_half_mhz
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestConfigTxIfReq(int16 offset_half_mhz);

/*****************************************************************************
**
** @brief   : BtTestConfigHoppingSequence
**
** @param   : uint32 chan00_31
** @param   : uint32 chan32_63
** @param   : uint16 chan64_78
**
** @return  : TRUE:Success   FALSE:Failed
**
*****************************************************************************/
uint8 BtTestConfigHoppingSequence(uint32 chan00_31, uint32 chan32_63, uint16 chan64_78);

/******************************** Configuration ***************************************/




#ifdef __cplusplus
}
#endif


#endif /* __BT_HOST_INTERFACE_H__ */
