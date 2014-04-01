#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "../../inc/bt_if.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    BtTestRegisterHandlerCbReq(&MainWindow::callBack);
    setParamVisible(0);

    /* set input format */
    QRegExp decExp("^\\d+$");                   /* only accept number 0~9 */
    QRegExp hexExp("^[a-fA-F0-9]+$");           /* only accept hex input */
    QValidator *hexValidator = new QRegExpValidator(hexExp, this);
    ui->param1Edit->setValidator(hexValidator);
    ui->param2Edit->setValidator(hexValidator);
    ui->param3Edit->setValidator(hexValidator);
    ui->param4Edit->setValidator(hexValidator);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::callBack(quint16 opcode, void *msg)
{
    BT_RADIOTEST_COMMAN_CFM_T *cfm = (BT_RADIOTEST_COMMAN_CFM_T *)msg;
    //qDebug("opcode=0x%.2X, status=0x%.2X", opcode, cfm->status);
}

void MainWindow::on_powerOnBtn_clicked()
{
    BtTestPowerOnReq("/dev/ttyUSB0");
}

void MainWindow::on_powerOffBtn_clicked()
{
    BtTestPowerOffReq();
}


void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    m_currRow = currentRow;
    switch(currentRow)
    {
    case ENUM_NO_USE1:
    case ENUM_NO_USE2:
    case ENUM_NO_USE3:
    {
        setParamVisible(0);
        break;
    }
    case ENUM_TXSTART:
    {
        ui->param1->setText("L0 Freq. (MHz)");
        ui->param1Edit->setText("2441");
        ui->param2->setText("Power (Ext, Int)");
        ui->param2Edit->setText("65530");
        ui->param3->setText("Modulat'n Freq.");
        ui->param3Edit->setText("0");
        setParamVisible(3);
        break;
    }
    case ENUM_TXDATA1:
    case ENUM_TXDATA3:
    case ENUM_TXDATA4:
    {
        ui->param1->setText("L0 Freq. (MHz)");
        ui->param1Edit->setText("2441");
        ui->param2->setText("Power (Ext, Int)");
        ui->param2Edit->setText("65530");
        setParamVisible(2);
        break;
    }
    case ENUM_TXDATA2:
    {
        ui->param1->setText("Country Code");
        ui->param1Edit->setText("0");
        ui->param2->setText("Power (Ext, Int)");
        ui->param2Edit->setText("65530");
        setParamVisible(2);
        break;
    }
    case ENUM_RXSTART1:
    case ENUM_RXSTART2:
    case ENUM_RXDATA1:
    {
        ui->param1->setText("L0 Freq. (MHz)");
        ui->param1Edit->setText("2441");
        ui->param2->setText("hi-sied");
        ui->param2Edit->setText("1");
        ui->param3->setText("Rx Attenuation");
        ui->param3Edit->setText("0");
        setParamVisible(3);
        break;
    }
    case ENUM_RXDATA2:
    {
        ui->param1->setText("Country Code");
        ui->param1Edit->setText("0");
        ui->param2->setText("hi-sied");
        ui->param2Edit->setText("1");
        ui->param3->setText("Rx Attenuation");
        ui->param3Edit->setText("0");
        setParamVisible(3);
        break;
    }
    case ENUM_BITERR1:
    {
        ui->param1->setText("L0 Freq. (MHz)");
        ui->param1Edit->setText("2441");
        ui->param2->setText("hi-sied");
        ui->param2Edit->setText("1");
        ui->param3->setText("Rx Attenuation");
        ui->param3Edit->setText("0");
        ui->param4->setText("Sample Size");
        ui->param4Edit->setText("10000");
        setParamVisible(4);
        break;
    }
    case ENUM_CFGFREQ:
    {
        ui->param1->setText("TX/RX Int (us)");
        ui->param1Edit->setText("1250");
        ui->param2->setText("Loopback (us)");
        ui->param2Edit->setText("1875");
        ui->param3->setText("Report Int (s)");
        ui->param3Edit->setText("1");
        setParamVisible(3);
        break;
    }
    case ENUM_CFGPKT:
    {
        ui->param1->setText("Packet Type");
        ui->param1Edit->setText("4");
        ui->param2->setText("Packet Size");
        ui->param2Edit->setText("27");
        setParamVisible(2);
        break;
    }
    case ENUM_CFGTXIF:
    {
        ui->param1->setText("IF Offset");
        ui->param1Edit->setText("0");
        setParamVisible(1);
        break;
    }
    case ENUM_CFGHOPPING:
    {
        ui->param1->setText("Channels 31-00 (0x)");
        ui->param1Edit->setText("FFFFFFFF");
        ui->param2->setText("Channels 63-32 (0x)");
        ui->param2Edit->setText("FFFFFFFF");
        ui->param3->setText("Channels 78-64 (0x)");
        ui->param3Edit->setText("7FFF");
        setParamVisible(3);
        break;
    }
    default:
        break;
    }
}


void MainWindow::setParamVisible(quint16 count)
{
    ui->param1->setVisible(false);
    ui->param1Edit->setVisible(false);
    ui->param2->setVisible(false);
    ui->param2Edit->setVisible(false);
    ui->param3->setVisible(false);
    ui->param3Edit->setVisible(false);
    ui->param4->setVisible(false);
    ui->param4Edit->setVisible(false);

    if(count >=1)
    {
        ui->param1->setVisible(true);
        ui->param1Edit->setVisible(true);
    }
    if(count >= 2)
    {
        ui->param2->setVisible(true);
        ui->param2Edit->setVisible(true);
    }

    if(count >=3)
    {
        ui->param3->setVisible(true);
        ui->param3Edit->setVisible(true);
    }

    if(count>=4)
    {
        ui->param4->setVisible(true);
        ui->param4Edit->setVisible(true);
    }
}


void MainWindow::on_excuteBtn_clicked()
{
    quint16 param1 = ui->param1Edit->text().toInt();
    quint16 param2 = ui->param2Edit->text().toInt();
    quint16 param3 = ui->param3Edit->text().toInt();
    switch(m_currRow)
    {
    case ENUM_TXSTART:
        BtTestTxStartReq(param1, param2, param3);
        break;
    case ENUM_TXDATA1:
        BtTestTxDataReq(RADIOTEST_TX_DATA1, param1, param2);
        break;
    case ENUM_TXDATA2:
        BtTestTxData2Req(param1, param2);
        break;
    case ENUM_TXDATA3:
        BtTestTxDataReq(RADIOTEST_TX_DATA3, param1, param2);
        break;
    case ENUM_TXDATA4:
        BtTestTxDataReq(RADIOTEST_TX_DATA4, param1, param2);
        break;
    case ENUM_RXSTART1:
        BtTestRxStartReq(RADIOTEST_RX_START1, param1, param2, param3);
        break;
    case ENUM_RXSTART2:
        BtTestRxStartReq(RADIOTEST_RX_START2, param1, param2, param3);
        break;
    case ENUM_RXDATA1:
        BtTestRxData1Req(param1, param2, param3);
        break;
    case ENUM_RXDATA2:
        BtTestRxData2Req(param1, param2, param3);
        break;
    case ENUM_BITERR1:
        BtTestBER1Req(param1, param2, param3);
        break;
    case ENUM_CFGFREQ:
        BtTestConfigFreqReq(param1,param2);
        break;
    case ENUM_CFGPKT:
        BtTestConfigPacketReq(param1, param2);
        break;
    case ENUM_CFGTXIF:
        BtTestConfigTxIfReq(param1);
        break;
    case ENUM_CFGHOPPING:
    {
        param1 = ui->param1Edit->text().toInt(0,16);
        param2 = ui->param2Edit->text().toInt(0,16);
        param3 = ui->param3Edit->text().toInt(0,16);
        BtTestConfigHoppingSequence(param1, param2, param3);
        break;
    }
    default:
        break;
    }
}

void MainWindow::on_warmResetBtn_clicked()
{
    BtTestWarmResetReq();
}
