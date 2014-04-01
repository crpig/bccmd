#include "app_private.h"

uint8 BtTestRegisterHandlerCbReq(handlerCbFunc func)
{
    testCb = func;
    return TRUE;
}

uint8 BtTestPowerOnReq(char *device)
{
    int error = 0;
    BT_THREAD_DATA_T *data = NULL;

    DEBUG_API("BT_RADIOTEST_POWER_ON_REQ device=%s", device);
    if(testThread != 0)
    {
        return FALSE;
    }

    if (strlen(device) >= APP_MAX_NAME_LEN)
    {
        /* Name is too long */
        return FALSE;
    }

    /* Copy device name */
    data = malloc(sizeof(BT_THREAD_DATA_T));
    memset(data, 0, sizeof(BT_THREAD_DATA_T));
    memcpy(data->name, device, strlen(device));

    error = pthread_create(&testThread, NULL, workThread, data);

    if (error != 0x00)
    {
        DEBUG("Create thread error %d", error);
        free(data);
        return FALSE;
    }

    return TRUE;
}

uint8 BtTestPowerOffReq(void)
{
    if(theApp != NULL)
    {
        theApp->runing = FALSE;
    }

    DEBUG_API("BT_RADIOTEST_POWER_OFF_REQ");

    return TRUE;
}

uint8 BtTestWarmResetReq(void)
{
    return SendMessage(BT_RADIOTEST_WARM_RESET_REQ, NULL);
}



/******************************** Radio Test ******************************************/
uint8 BtTestTxStartReq(uint16 freq, uint16 gain, int16 mod_freq)
{
    MAKE_MESSAGE(BT_RADIOTEST_COMMAN_REQ);
    msg->testId = TESTID_TXSTART;
    msg->param[0] = freq;
    msg->param[1] = gain;
    msg->param[2] = mod_freq;
    return SendMessage(BT_RADIOTEST_TX_START_REQ, msg);
}

uint8 BtTestRxStartReq(uint8 id, uint16 freq, bool highside, uint16 attn)
{
    MAKE_MESSAGE(BT_RADIOTEST_COMMAN_REQ);
    if(0x01 == id)
    {
        msg->testId = TESTID_RXSTART1;
    }
    else if(0x02 == id)
    {
        msg->testId = TESTID_RXSTART2;
    }
    else
    {
        free(msg);
        return FALSE;
    }

    msg->param[0] = freq;
    msg->param[1] = highside;
    msg->param[2] = attn;
    return SendMessage(BT_RADIOTEST_RX_START_REQ, msg);
}

uint8 BtTestTxDataReq(uint8 id, uint16 freq, uint16 gain)
{
    MAKE_MESSAGE(BT_RADIOTEST_COMMAN_REQ);
    if(0x01 == id)
    {
        msg->testId = TESTID_TXDATA1;
    }
    else if(0x03 == id)
    {
        msg->testId = TESTID_TXDATA3;
    }
    else if(0x04 == id)
    {
        msg->testId = TESTID_TXDATA4;
    }
    else
    {
        free(msg);
        return FALSE;
    }

    msg->param[0] = freq;
    msg->param[1] = gain;
    return SendMessage(BT_RADIOTEST_TX_DATA_REQ, msg);
}

uint8 BtTestTxData2Req(uint16 cc, uint16 gain)
{
    MAKE_MESSAGE(BT_RADIOTEST_COMMAN_REQ);
    msg->testId = TESTID_TXDATA2;
    msg->param[0] = cc;
    msg->param[1] = gain;

    return SendMessage(BT_RADIOTEST_TX_DATA2_REQ, msg);
}

uint8 BtTestRxData1Req(uint16 freq, bool highside, uint16 attn)
{
    MAKE_MESSAGE(BT_RADIOTEST_COMMAN_REQ);
    msg->testId = TESTID_RXDATA1;
    msg->param[0] = freq;
    msg->param[1] = highside;
    msg->param[2] = attn;

    return SendMessage(BT_RADIOTEST_RX_DATA1_REQ, msg);
}

uint8 BtTestRxData2Req(uint16 cc, bool highside, uint16 attn)
{
    MAKE_MESSAGE(BT_RADIOTEST_COMMAN_REQ);
    msg->testId = TESTID_RXDATA2;
    msg->param[0] = cc;
    msg->param[1] = highside;
    msg->param[2] = attn;

    return SendMessage(BT_RADIOTEST_RX_DATA2_REQ, msg);
}

uint8 BtTestBER1Req(uint16 freq, bool highside, uint16 attn)
{
    MAKE_MESSAGE(BT_RADIOTEST_COMMAN_REQ);
    msg->testId = TESTID_BER1;
    msg->param[0] = freq;
    msg->param[1] = highside;
    msg->param[2] = attn;

    return SendMessage(BT_RADIOTEST_BER1_REQ, msg);
}


/******************************** Configuration ***************************************/
uint8 BtTestConfigFreqReq(uint16 txrx_freq, uint16 report_freq)
{
    MAKE_MESSAGE(BT_RADIOTEST_COMMAN_REQ);
    msg->testId = TESTID_CONFIG_FREQ;
    msg->param[0] = txrx_freq;
    msg->param[1] = report_freq;

    return SendMessage(BT_RADIOTEST_CONFIG_FREQ_REQ, msg);
}

uint8 BtTestConfigPacketReq(uint16 pkt_type, uint16 pkt_size)
{
    MAKE_MESSAGE(BT_RADIOTEST_COMMAN_REQ);
    msg->testId = TESTID_CONFIG_PKT;
    msg->param[0] = pkt_type;
    msg->param[1] = pkt_size;

    return SendMessage(BT_RADIOTEST_CONFIG_PACKET_REQ, msg);
}

uint8 BtTestConfigTxIfReq(int16 offset_half_mhz)
{
    MAKE_MESSAGE(BT_RADIOTEST_COMMAN_REQ);
    msg->testId = TESTID_CONFIG_TX_IF;
    msg->param[0] = offset_half_mhz;

    return SendMessage(BT_RADIOTEST_CONFIG_TX_IF_REQ, msg);
}

uint8 BtTestConfigHoppingSequence(uint32 chan00_31, uint32 chan32_63, uint16 chan64_78)
{
    MAKE_MESSAGE(BT_RADIOTEST_COMMAN_REQ);
    msg->testId = TESTID_CONFIG_HOPPING;
    msg->param[0] = chan00_31 & 0x0000FFFF;
    msg->param[1] = chan00_31 >> 16;
    msg->param[2] = chan32_63 & 0x0000FFFF;
    msg->param[3] = chan32_63 >> 16;
    msg->param[4] = chan64_78;

    return SendMessage(BT_RADIOTEST_CONFIG_HOPPING_SEQ_REQ, msg);
}



uint8 SendMessage(uint16 opcode, void *msg)
{
    if (testThread == 0)
    {
        return FALSE;
    }

    pthread_mutex_lock(&theApp->mutex);
    if(app_msg->opcode == BT_INVALID_OPCODE)
    {
        app_msg->opcode = opcode;
        app_msg->msg = msg;
        pthread_mutex_unlock(&theApp->mutex);
        BT_reqlog(opcode, msg);
        return TRUE;
    }
    else
    {
        pthread_mutex_unlock(&theApp->mutex);
        DEBUG("%s status busy", __FUNCTION__);
        if(msg != NULL)
        {
            free(msg);
        }
        return FALSE;
    }
}