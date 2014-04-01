#include "app_private.h"

/******************************************************************************
**  Global variables
******************************************************************************/
APP_INST_DATA_T *theApp = NULL;
APP_MSG_T       *app_msg = NULL;
pthread_t       testThread = 0;
handlerCbFunc   testCb = NULL;

/******************************************************************************
**  Static Functions
******************************************************************************/
static void workHandler(uint16 opcode, void *msg);
static void handle_warmReset(void);
static void handle_radioTestTx(uint16 opcode, void *msg);
static void handle_radioTestRx(uint16 opcode, void *msg);
static void handle_radioTestCfg(uint16 opcode, void *msg);
static void handle_rssi(uint8 *value, uint16 length);
static void handle_rxpktstatus(uint8 *value, uint16 length);

void *workThread(void *param)
{
    BT_THREAD_DATA_T *data = param;
    uint16_t tmp[4];

    DEBUG("The device name = %s\n", data->name);

    /* Init Inst Data */
    theApp = malloc(sizeof(APP_INST_DATA_T));
    memset(theApp, 0, sizeof(APP_INST_DATA_T));
    theApp->pendingCmd = BT_INVALID_OPCODE;
    app_msg = malloc(sizeof(APP_MSG_T));
    app_msg->opcode = BT_INVALID_OPCODE;
    app_msg->msg = NULL;

    pthread_mutex_init(&theApp->mutex, NULL);

    /* Save Deivce Name */
    memcpy(theApp->device, data->name, strlen(data->name) + 1);

    /* open device first */
    if(csr_open_bcsp(theApp->device, APP_BT_BAUDRATE))
    {
        DEBUG("BCSP Init Error\n");

        pthread_mutex_destroy(&theApp->mutex);

        free(data);
        data = NULL;

        if (theApp != NULL)
        {
            free(theApp);
            theApp = NULL;
        }

        if (app_msg != NULL)
        {
            free(app_msg);
            app_msg = NULL;
        }

        testThread = 0;
        BtTestCommonCfm(BT_RADIOTEST_POWER_ON_CFM, BT_RESULT_FAILED);

        /* assert(0); */
        return NULL;
    }

    /* set ANA FREQ */
    tmp[0] = APP_BT_ANA_FREQ;
    BT_writePskey(CSR_PSKEY_ANA_FREQ, 1, tmp);

    BT_warmReset();

    theApp->runing = TRUE;
    BtTestCommonCfm(BT_RADIOTEST_POWER_ON_CFM, BT_RESULT_SUCCESS);

    while(theApp->runing)
    {
        pthread_mutex_lock(&theApp->mutex);
        if(app_msg->opcode != BT_INVALID_OPCODE)
        {
            workHandler(app_msg->opcode, app_msg->msg);
            app_msg->opcode = BT_INVALID_OPCODE;
            app_msg->msg = NULL;
        }
        pthread_mutex_unlock(&theApp->mutex);


        if(theApp->receivePending)
        {
            uint16 varid = 0, length = 0;
            uint8 value[254];
            if(csr_read_hq(&varid, value, &length) == 0)
            {
                if(varid == HQ_RADIOTEST_RSSI)
                {
                    handle_rssi(value, length);
                }
                else if(varid == HQ_RADIOTEST_RX_PKT_STATUS)
                {
                    handle_rxpktstatus(value, length);
                }
                else
                {
                    DEBUG("Unknown varid = 0x%.2X", varid);
                }
            }
        }


        usleep(5000);
    }

    /* Power off */
    BT_warmReset();         /* stop current operation */
    csr_close_bcsp();

    if (data != NULL)
    {
        free(data);
    }

    BtTestCommonCfm(BT_RADIOTEST_POWER_OFF_CFM, BT_RESULT_SUCCESS);

    pthread_mutex_destroy(&theApp->mutex);
    if (theApp != NULL)
    {
        free(theApp);
        theApp = NULL;
    }

    if (app_msg != NULL)
    {
        free(app_msg);
        app_msg = NULL;
    }

    testThread = 0;

    return NULL;
}


void workHandler(uint16 opcode, void *msg)
{
    if(theApp->pendingCmd != BT_INVALID_OPCODE)
    {
        BtTestCommonCfm(opcode, BT_RESULT_BUSY);
        return ;
    }

    theApp->pendingCmd = opcode;

    DEBUG("Receive opcode = 0x%.2X", opcode);
    switch (opcode)
    {
        case BT_RADIOTEST_WARM_RESET_REQ:
            handle_warmReset();
            break;
        case BT_RADIOTEST_TX_START_REQ:
        case BT_RADIOTEST_TX_DATA_REQ:
        case BT_RADIOTEST_TX_DATA2_REQ:
            handle_radioTestTx(opcode, msg);
            break;
        case BT_RADIOTEST_RX_START_REQ:
        case BT_RADIOTEST_RX_DATA1_REQ:
        case BT_RADIOTEST_RX_DATA2_REQ:
        case BT_RADIOTEST_BER1_REQ:
            handle_radioTestRx(opcode, msg);
            break;
        case BT_RADIOTEST_CONFIG_FREQ_REQ:
        case BT_RADIOTEST_CONFIG_PACKET_REQ:
        case BT_RADIOTEST_CONFIG_TX_IF_REQ:
        case BT_RADIOTEST_CONFIG_HOPPING_SEQ_REQ:
            handle_radioTestCfg(opcode, msg);
            break;
        default:
            DEBUG("Unknown opcode=%.2X", opcode);
            break;
    }

    if (msg != NULL)
    {
        free(msg);
    }
}


void handle_warmReset(void)
{
    if(BT_warmReset())
    {
        BtTestCommonCfm(BT_RADIOTEST_WARM_RESET_CFM, BT_RESULT_SUCCESS);
    }
    else
    {
        BtTestCommonCfm(BT_RADIOTEST_WARM_RESET_CFM, BT_RESULT_FAILED);
    }
}

void handle_radioTestTx(uint16 opcode, void *msg)
{
    BT_RADIOTEST_COMMAN_REQ_T *req = msg;
    uint8 array[8];
    memset(array, 0, sizeof(array));
    array[0] = req->testId & 0xFF;
    array[1] = req->testId >> 8;
    array[2] = req->param[0] & 0xFF;
    array[3] = req->param[0] >> 8;
    array[4] = req->param[1] & 0xFF;
    array[5] = req->param[1] >> 8;
    array[6] = req->param[2] & 0xFF;
    array[7] = req->param[2] >> 8;
    if(csr_write_bccmd(CSR_VARID_RADIOTEST, array, 8))
    {
        DEBUG("Send Test=%d Error", req->testId);
        /* assert(0); */
        BtTestCommonCfm(opcode, BT_RESULT_FAILED);
    }

    BtTestCommonCfm(opcode, BT_RESULT_SUCCESS);
}

void handle_radioTestRx(uint16 opcode, void *msg)
{
    BT_RADIOTEST_COMMAN_REQ_T *req = msg;
    uint8 array[8];
    memset(array, 0, sizeof(array));
    array[0] = req->testId & 0xFF;
    array[1] = req->testId >> 8;
    array[2] = req->param[0] & 0xFF;
    array[3] = req->param[0] >> 8;
    array[4] = req->param[1] & 0xFF;
    array[5] = req->param[1] >> 8;
    array[6] = req->param[2] & 0xFF;
    array[7] = req->param[2] >> 8;
    if(csr_write_bccmd(CSR_VARID_RADIOTEST, array, 8))
    {
        DEBUG("Send Test=%d Error", req->testId);
        /* assert(0); */
        BtTestCommonCfm(opcode, BT_RESULT_FAILED);
    }

    BtTestCommonCfm(opcode, BT_RESULT_SUCCESS);

    if((req->testId == TESTID_RXSTART2) || (req->testId == TESTID_RXDATA1) ||
        (req->testId == TESTID_RXDATA2))
    {
        theApp->receivePending = TRUE;
    }
}

void handle_radioTestCfg(uint16 opcode, void *msg)
{
    BT_RADIOTEST_COMMAN_REQ_T *req = msg;
    uint8 array[12];
    memset(array, 0, sizeof(array));
    array[0] = req->testId & 0xFF;
    array[1] = req->testId >> 8;
    array[2] = req->param[0] & 0xFF;
    array[3] = req->param[0] >> 8;
    array[4] = req->param[1] & 0xFF;
    array[5] = req->param[1] >> 8;
    array[6] = req->param[2] & 0xFF;
    array[7] = req->param[2] >> 8;
    if (opcode != BT_RADIOTEST_CONFIG_HOPPING_SEQ_REQ)
    {
        if(csr_write_bccmd(CSR_VARID_RADIOTEST, array, 8))
        {
            DEBUG("Send Test=%d Error", req->testId);
            /* assert(0); */
            BtTestCommonCfm(opcode, BT_RESULT_FAILED);
        }

        BtTestCommonCfm(opcode, BT_RESULT_SUCCESS);
    }
    else
    {
            array[8]  = req->param[3] & 0xFF;
            array[9]  = req->param[3] >> 8;
            array[10] = req->param[4] & 0xFF;
            array[11] = req->param[4] >> 8;

            if(csr_write_bccmd(CSR_VARID_RADIOTEST, array, 12))
            {
                DEBUG("Send Test=%d Error", req->testId);
                /* assert(0); */
                BtTestCommonCfm(opcode, BT_RESULT_FAILED);
            }

        BtTestCommonCfm(opcode, BT_RESULT_SUCCESS);
    }
}

void handle_rssi(uint8 *value, uint16 length)
{
    uint16 rssi;
    if(length < 2)
    {
        DEBUG("%s received wrong length", __FUNCTION__);
        return ;
    }

    rssi = value[0] + (value[1]<<8);
    BtTestRssiInd(rssi);
}

void handle_rxpktstatus(uint8 *value, uint16 length)
{
    uint16 n_pkts, good_pkts, corr_pkts, rssi;
    bool rssi_valid;
    if(length < 10)
    {
        DEBUG("%s received wrong length", __FUNCTION__);
        return ;
    }

    n_pkts      = value[0] + (value[1]<<8);
    good_pkts   = value[2] + (value[3]<<8);
    corr_pkts   = value[4] + (value[5]<<8);
    rssi        = value[6] + (value[7]<<8);
    if((value[8] + (value[9]<<8)) == 1)
    {
        rssi_valid = TRUE;
    }
    else
    {
        rssi_valid = FALSE;
    }

    BtTestRxPktStatusInd(n_pkts, good_pkts, corr_pkts, rssi, rssi_valid);
}
