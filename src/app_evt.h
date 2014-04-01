#ifndef __APP_EVT_H__
#define __APP_EVT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "bt_types.h"

void BtTestCommonCfm(uint16 opcode, uint16 result);
void BtTestRssiInd(uint16 rssi);
void BtTestRxPktStatusInd(uint16 n_pkts, uint16 good_pkts, uint16 corr_pkts, uint16 rssi, bool rssi_valid);

void SendConfirm(uint16 opcode, void *msg);

#ifdef __cplusplus
}
#endif

#endif /* __APP_EVT_H__ */

