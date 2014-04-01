#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static void callBack(quint16 opcode, void *msg);

private slots:
    void on_powerOnBtn_clicked();
    void on_powerOffBtn_clicked();
    void on_listWidget_currentRowChanged(int currentRow);

    void on_excuteBtn_clicked();

    void on_warmResetBtn_clicked();

private:
    Ui::MainWindow *ui;
    int m_currRow;
    void setParamVisible(quint16 count);
};

enum
{
    ENUM_NO_USE1 = 0,       /* TX */
    ENUM_TXSTART,
    ENUM_TXDATA1,
    ENUM_TXDATA2,
    ENUM_TXDATA3,
    ENUM_TXDATA4,
    ENUM_NO_USE2,           /* RX */
    ENUM_RXSTART1,
    ENUM_RXSTART2,
    ENUM_RXDATA1,
    ENUM_RXDATA2,
    ENUM_BITERR1,
    ENUM_NO_USE3,           /* CONFIG */
    ENUM_CFGFREQ,
    ENUM_CFGPKT,
    ENUM_CFGTXIF,
    ENUM_CFGHOPPING
};

#endif // MAINWINDOW_H
