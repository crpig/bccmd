#include "app_private.h"


void BtTestCommonCfm(uint16 opcode, uint16 result)
{
    MAKE_MESSAGE(BT_RADIOTEST_COMMAN_CFM);
    msg->status = result;
    SendConfirm(opcode, msg);
}

void BtTestRssiInd(uint16 rssi)
{
    MAKE_MESSAGE(BT_RADIOTEST_RSSI_IND);
    msg->rssi = rssi;
    SendConfirm(BT_RADIOTEST_RSSI_IND, msg);
}

void BtTestRxPktStatusInd(uint16 n_pkts, uint16 good_pkts, uint16 corr_pkts, uint16 rssi, bool rssi_valid)
{
    MAKE_MESSAGE(BT_RADIOTEST_RX_PKT_STATUS_IND);
    msg->n_pkts = n_pkts;
    msg->n_good_pkts = good_pkts;
    msg->n_corr_pkts = corr_pkts;
    msg->rssi = rssi;
    msg->rssi_valid = rssi_valid;
    SendConfirm(BT_RADIOTEST_RX_PKT_STATUS_IND, msg);
}

void SendConfirm(uint16 opcode, void *msg)
{
    BT_cfmlog(opcode, msg);

    if(opcode == theApp->pendingCmd)
    {
        theApp->pendingCmd = BT_INVALID_OPCODE;
    }

    testCb(opcode, msg);

    if(msg != NULL)
    {
        free(msg);
    }
}

