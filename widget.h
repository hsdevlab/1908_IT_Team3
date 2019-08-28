#ifndef WIDGET_H
#define WIDGET_H

#include <QSlider>
#include <QLabel>
#include <QDebug>
#include <QtNetwork/QTcpSocket>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    bool turnSignal[2]; // {R, L}
    bool doorOpen[4]; // {FL, FR, RL, RR}
    bool seatbeltOn;
    int accelator; // 0/1/2/3
    int brake; // 0/1/2/3
    int gear; // 0(P)/1(R)/2(N)/3(D)

    QString msg;

private:
    QTcpSocket *socket;
    QSlider *slider[3];

    QSlider *SB_slider;
    QSlider *DO_slider[4];
    QSlider *TS_slider[2];

    QLabel *lbl[3];
    QLabel *DO_lbl[4];
    QLabel *TS_lbl[2];
    QLabel *nametag[6];
    QLabel *gearlbl[4];
    QLabel *DO_lbl2[4];
    QLabel *TS_lbl2[2];
    QLabel *SB_lbl;
    //void readyRead();
    //void connected(QString msg);
    void ToQString();

private slots:
    void valueChanged1(int value);//ACCEL
    void valueChanged2(int value);//BRAKE
    void valueChanged3(int value);//GEAR
    void valueChanged4(int value);//SEATBELT
    void valueChanged5(int value);//DOOROPEN FL
    void valueChanged6(int value);//FR
    void valueChanged7(int value);//RL
    void valueChanged8(int value);//RR
    void valueChanged9(int value);//LEFT TSIG
    void valueChanged10(int value);//RIGHT TSIG
    //void readyRead();
    //void connected(QString msg);
};

#endif // WIDGET_H
