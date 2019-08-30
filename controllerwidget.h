#ifndef WIDGET_H
#define WIDGET_H

#include <QSlider>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QtNetwork/QTcpSocket>
#include <QDial>
#include <QtWidgets/QPushButton>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QtMultimedia>
#include <QVideoWidget>
#define MUSIC 4

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    bool turnSignal[2] ={0,}; // {R, L}
    bool doorOpen[4]={0,}; // {FL, FR, RL, RR}
    bool seatbeltOn = 0;
    int accelator = 0; // 0/1/2/3
    int brake = 0; // 0/1/2/3
    int gear = 0; // 0(P)/1(R)/2(N)/3(D)
    QString musicname = "none";
    QString singer = "none";
    bool musicOn = false;
    int volume = 30;
    int curidx = 0;
    int min=0;
    int sec=0;
    int dur_min=0;
    int dur_sec=0;
//DATA data;
QString msg = "";
private:
    QTcpSocket *socket;
    QDial *Volume;
    QSlider *slider[3];


    QMediaPlayer *mp;
    QMediaPlaylist *mpl;
    QVideoWidget *mpv;
    QStringList musiclist;
    QStringList singerlist;

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
    QLabel *V_lbl;
    QLabel *M_lbl;
    QLabel *playtime;
    QLabel *durationtime;
    QPushButton *Music[4];
    //void readyRead();
    //void connected(QString msg);
    void ToQString();
    void SocketConn();

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
    void sendMSG();
    void musicplay();
    void musicstop();
    void musicvolume(int value);
    void nextsong();
    void prevsong();
    void timesetting();
    //void printduation(qint32);


};

#endif // WIDGET_H
