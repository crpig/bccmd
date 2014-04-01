#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csr.h"

static uint8_t array[20];
static char deviceName[20];

int bt_WritePskey(uint16_t pskey, uint16_t length, uint16_t value[])
{
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
    printf("0x%.2x 0x%.2x\n", array[6], array[7]);
    if(csr_write_bcsp(CSR_VARID_PS, array, (length + 3)*2))
    {
        printf("Write Pskey 0x%.4X Error\n", pskey);
        return -1;
    }

    printf("Write Pskey 0x%.4X Success\n", pskey);
    return 0;
}

int bt_WarmReset(void)
{
    if(csr_write_bcsp(CSR_VARID_WARM_RESET, NULL, 0))
    {
        printf("Warm Reset Error\n");
        return -1;
    }

    csr_close_bcsp();

    if(csr_open_bcsp((char *)deviceName, B921600))
    {
        printf("[WarmReset]BCSP Init Error\n");
        return -1;
    }

    return 0;
}

int BT_OpenUart(char *device)
{
    uint16_t tmp[4];

    /* Save Deivce Name */
    memcpy(deviceName, device, strlen(device));

    /* open device first */
    if(csr_open_bcsp(device, B921600))
    {
        printf("BCSP Init Error\n");
        return -1;
    }

    /* set ANA FREQ */
    tmp[0] = 0x6590;
    bt_WritePskey(CSR_PSKEY_ANA_FREQ, 1, tmp);

    /* set ANA FTRIM */
    //tmp[0] = 0x001E;
    //bt_WritePskey(CSR_PSKEY_ANA_FTRIM, 1, tmp);

    /* set host interface */
    //tmp[0] = 0x0001;
    //bt_WritePskey(CSR_PSKEY_HOST_INTERFACE, 1, tmp);

    #if 0
    memset(array, 0, sizeof(array));
    array[0] = CSR_PSKEY_ANA_FREQ & 0xFF;
    array[1] = CSR_PSKEY_ANA_FREQ >> 8;     /* PS Key */
    array[2] = 0x01;
    array[3] = 0x00;                        /* Length */
    array[4] = 0x08;
    array[5] = 0x00;                        /* Stores */
    array[6] = 0x90;
    array[7] = 0x65;                        /* PS Value */
    if(csr_write_bcsp(CSR_VARID_PS, array, 8))
    {
        printf("Set ANA FREQ Error\n");
        return -1;
    }
    #endif

    bt_WarmReset();




    /* start RadioTest at 2441MHz */
    memset(array, 0, sizeof(array));
    array[0] = 0x01;
    array[1] = 0x00;    /* ID = 0x0001 */
    array[2] = 0x89;
    array[3] = 0x09;    /* FREQ = 0x0989 */
    array[4] = 0x32;
    array[5] = 0xFF;    /* LEVEL = 0xFF32 */
    array[6] = 0x00;
    array[7] = 0x00;    /* Mod_Freq = 0x0000 */
    if(csr_write_bcsp(CSR_VARID_RADIOTEST, array, 8))
    {
        printf("TXStart Error\n");
        return -1;
    }

    /* Open Uart & Start TX Success */
    printf("BT_OpenUart Success\n");
    return 0;
}

int BT_ChangeFreq(uint16_t freq)
{
    static uint16_t freqLast = 0x0989;

    if (freq == 0)
    {
        freq = freqLast;
    }
    else
    {
        freqLast = freq;
    }

    if ((freq < 2402) || (freq > 2495))
    {
        printf("Freq error\n");
        freqLast = 0x0989;
        return -1;
    }

    printf("change freq to %d\n", freq);
    array[0] = 0x01;
    array[1] = 0x00;    /* ID = 0x0001 */
    array[2] = freq & 0xFF;
    array[3] = freq >> 8;
    array[4] = 0x32;
    array[5] = 0xFF;    /* LEVEL = 0xFF32 */
    array[6] = 0x00;
    array[7] = 0x00;    /* Mod_Freq = 0x0000 */
    if(csr_write_bcsp(CSR_VARID_RADIOTEST, array, 8))
    {
        printf("TXStart Error\n");
        return -1;
    }

    printf("BT_ChangeFreq Success\n");
    return 0;
}

int BT_ChangeFreqTrim(uint16_t freqTrim)
{
    uint16_t tmp[1];
    if ((freqTrim < 0 ) || (freqTrim > 63))
    {
        printf("FreqTrim error\n");
        return -1;
    }

    printf("change freq trim to %d\n", freqTrim);

    tmp[0] = freqTrim;
    if (bt_WritePskey(CSR_PSKEY_ANA_FTRIM, 1, tmp))
    {
        return -1;
    }

    if (bt_WarmReset())
    {
        return -1;
    }

    BT_ChangeFreq(0);

    return 0;
}

int BT_CloseUart()
{
    if(csr_write_bcsp(CSR_VARID_WARM_RESET, NULL, 0))
    {
        printf("Warm Reset Error\n");
    }
    csr_close_bcsp();
    printf("BT_CloseUart Success\n");
    return 0;
}

void bt_Usage(void)
{
    printf("==============================\n");
    printf("1. Start Test\n");
    printf("2. Change freq\n");
    printf("3. Change freqTrim\n");
    printf("4. Stop Test\n");
    printf("==============================\n");
}

void bt_getFreq()
{
    int freq;
    printf("Please Input FREQ(2402~2495)\n");
    scanf("%d", &freq);
    BT_ChangeFreq(freq);
}

void bt_getFreqTrim()
{
    int freqTrim;
    printf("Please Input Freq Trim(0~63)\n");
    scanf("%d", &freqTrim);
    BT_ChangeFreqTrim(freqTrim);
}


int main(void)
{
    int choice;
    char runing = 1;

    while(runing)
    {
        bt_Usage();
        scanf("%d", &choice);
        switch(choice)
        {
            case 1:
                BT_OpenUart("/dev/ttyUSB0");
                break;
            case 2:
                bt_getFreq();
                break;
            case 3:
                bt_getFreqTrim();
                break;
            case 4:
                BT_CloseUart();
                runing = 0;
                break;
            default:
                printf("wrong input!!!!!!\n");
        }
    }
}
