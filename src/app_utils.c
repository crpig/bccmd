#include "app_private.h"

uint8_t BT_writePskey(uint16_t pskey, uint16_t length, uint16_t value[])
{
    uint8_t array[20];
    memset(array, 0, sizeof(array));
    char i = 0;
    array[0] = pskey & 0xFF;
    array[1] = pskey >> 8;          /* PS Key */
    array[2] = length & 0xFF;
    array[3] = length >> 8;         /* Length (word) */
    array[4] = 0x08;
    array[5] = 0x00;                /* Stores PSRAM */
    for (i=0; i<length; i++)
    {
        array[6+i*2] = value[i] & 0xFF;
        array[7+i*2] = value[i] >> 8;   /* PS Value */
    }

    if(csr_write_bccmd(CSR_VARID_PS, array, (length + 3)*2))
    {
        DEBUG("Write Pskey 0x%.4X Error", pskey);
        /* assert(0); */
        return FALSE;
    }

    return TRUE;
}

uint8_t BT_warmReset(void)
{
    if (csr_write_bccmd(CSR_VARID_WARM_RESET, NULL,  0))
    {
        DEBUG("Warm Reset Error");
        /* assert(0); */
        return FALSE;
    }

    theApp->receivePending = FALSE;

    /* We need to reinit chip again */
    csr_close_bcsp();

    if(csr_open_bcsp(theApp->device, APP_BT_BAUDRATE))
    {
        /* assert(0); */
        return FALSE;
    }

    return TRUE;
}

void BT_reqlog(uint16_t opcode, void *msg)
{
    BT_RADIOTEST_COMMAN_REQ_T *req = msg;
    switch (opcode)
    {
        case BT_RADIOTEST_WARM_RESET_REQ:
            DEBUG_API("BT_RADIOTEST_WARM_RESET_REQ");
            break;
        case BT_RADIOTEST_TX_START_REQ:
            DEBUG_API("BT_RADIOTEST_TX_START_REQ freq=%d, gain=%d, mod_freq=%d", req->param[0], req->param[1], req->param[2]);
            break;
        case BT_RADIOTEST_RX_START_REQ:
            DEBUG_API("BT_RADIOTEST_RX_START_REQ id=%d, freq=%d, hiside=%d, attn=%d", req->testId, req->param[0],
                        req->param[1], req->param[2]);
            break;
        case BT_RADIOTEST_TX_DATA_REQ:
            DEBUG_API("BT_RADIOTEST_TX_DATA_REQ id=%d, freq=%d, gain=%d", req->testId, req->param[0], req->param[1]);
            break;
        case BT_RADIOTEST_TX_DATA2_REQ:
            DEBUG_API("BT_RADIOTEST_TX_DATA2_REQ cc=%d, gain=%d", req->param[0], req->param[1]);
            break;
        case BT_RADIOTEST_RX_DATA1_REQ:
            DEBUG_API("BT_RADIOTEST_RX_DATA1_REQ freq=%d, hiside=%d, attn=%d", req->param[0], req->param[1], req->param[2]);
            break;
        case BT_RADIOTEST_RX_DATA2_REQ:
            DEBUG_API("BT_RADIOTEST_RX_DATA2_REQ cc=%d, hiside=%d, attn=%d", req->param[0], req->param[1], req->param[2]);
            break;
        case BT_RADIOTEST_BER1_REQ:
            DEBUG_API("BT_RADIOTEST_BER1_REQ freq=%d, hiside=%d, attn=%d", req->param[0], req->param[1], req->param[2]);
            break;
        case BT_RADIOTEST_CONFIG_FREQ_REQ:
            DEBUG_API("BT_RADIOTEST_CONFIG_FREQ_REQ txrx_freq=%d, report_freq=%d", req->param[0], req->param[1]);
            break;
        case BT_RADIOTEST_CONFIG_PACKET_REQ:
            DEBUG_API("BT_RADIOTEST_CONFIG_PACKET_REQ pkt_type=%d, pkt_size=%d", req->param[0], req->param[1]);
            break;
        case BT_RADIOTEST_CONFIG_TX_IF_REQ:
            DEBUG_API("BT_RADIOTEST_CONFIG_TX_IF_REQ offset_half_mhz=%d", req->param[0]);
            break;
        case BT_RADIOTEST_CONFIG_HOPPING_SEQ_REQ:
            DEBUG_API("BT_RADIOTEST_CONFIG_HOPPING_SEQ_REQ chan00_15=0x%.4X, chan16_31=0x%.4X, chan32_47=0x%.4X, chan48_63=0x%.4X, chan64_78=0x%.4X",
                        req->param[0], req->param[1], req->param[2], req->param[3], req->param[4], req->param[5]);
            break;
        default:
            DEBUG_API("[%s] Wrong Opcode=%d", __FUNCTION__, opcode);
            break;
    }

}

void BT_cfmlog(uint16_t opcode, void *msg)
{
    BT_RADIOTEST_COMMAN_CFM_T *cfm = msg;
    uint16 status = cfm->status;
    switch (opcode)
    {
        case BT_RADIOTEST_POWER_ON_CFM:
            DEBUG_API("BT_RADIOTEST_POWER_ON_CFM status=%d", status);
            break;
        case BT_RADIOTEST_POWER_OFF_CFM:
            DEBUG_API("BT_RADIOTEST_POWER_OFF_CFM status=%d", status);
            break;
        case BT_RADIOTEST_WARM_RESET_CFM:
            DEBUG_API("BT_RADIOTEST_WARM_RESET_CFM status=%d", status);
            break;
        case BT_RADIOTEST_TX_START_CFM:
            DEBUG_API("BT_RADIOTEST_TX_START_CFM status=%d", status);
            break;
        case BT_RADIOTEST_RX_START_CFM:
            DEBUG_API("BT_RADIOTEST_RX_START_CFM status=%d", status);
            break;
        case BT_RADIOTEST_TX_DATA_CFM:
            DEBUG_API("BT_RADIOTEST_TX_DATA_CFM status=%d", status);
            break;
        case BT_RADIOTEST_TX_DATA2_CFM:
            DEBUG_API("BT_RADIOTEST_TX_DATA2_CFM status=%d", status);
            break;
        case BT_RADIOTEST_RX_DATA1_CFM:
            DEBUG_API("BT_RADIOTEST_RX_DATA1_CFM status=%d", status);
            break;
        case BT_RADIOTEST_RX_DATA2_CFM:
            DEBUG_API("BT_RADIOTEST_RX_DATA2_CFM status=%d", status);
            break;
        case BT_RADIOTEST_BER1_CFM:
            DEBUG_API("BT_RADIOTEST_BER1_CFM status=%d", status);
            break;
        case BT_RADIOTEST_CONFIG_FCFM_CFM:
            DEBUG_API("BT_RADIOTEST_CONFIG_FCFM_CFM status=%d", status);
            break;
        case BT_RADIOTEST_CONFIG_PACKET_CFM:
            DEBUG_API("BT_RADIOTEST_CONFIG_PACKET_CFM status=%d", status);
            break;
        case BT_RADIOTEST_CONFIG_TX_IF_CFM:
            DEBUG_API("BT_RADIOTEST_CONFIG_TX_IF_CFM status=%d", status);
            break;
        case BT_RADIOTEST_CONFIG_HOPPING_SEQ_CFM:
            DEBUG_API("BT_RADIOTEST_CONFIG_HOPPING_SEQ_CFM status=%d", status);
            break;
        case BT_RADIOTEST_RX_PKT_STATUS_IND:
            {
                BT_RADIOTEST_RX_PKT_STATUS_IND_T *ind = msg;
                DEBUG_API("BT_RADIOTEST_RX_PKT_STATUS_IND n_pkts=%d, n_good_pkts, n_corr_pkts, rssi=%d, rssi_valid=%d",
                            ind->n_pkts, ind->n_good_pkts, ind->n_corr_pkts, ind->rssi, ind->rssi_valid);
            }
            break;
        case BT_RADIOTEST_RSSI_IND:
            {
                BT_RADIOTEST_RSSI_IND_T *ind = msg;
                DEBUG_API("BT_RADIOTEST_RSSI_IND rssi=%d", ind->rssi);
            }
            break;
        default:
            DEBUG_API("[%s] Wrong Opcode=0x%.2X", __FUNCTION__, opcode);
            break;
    }
}