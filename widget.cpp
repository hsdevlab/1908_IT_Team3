#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    socket = new QTcpSocket(this);
    SocketConn();
    QTimer* timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),SLOT(sendMSG()));
    timer->start(50);

    int xpos = 20;
    int ypos;
    for(int i = 0 ; i < 3 ; i++)
    {

        slider[i] = new QSlider(Qt::Vertical, this);
        slider[i]->setGeometry(xpos, 20, 30, 80);
        xpos += 80;
        if(i != 2)
            slider[i]->setRange(0, 3);
        else
            slider[i]->setRange(0,3);
        slider[i]->setValue(0);

    }

    SB_slider = new QSlider(Qt::Horizontal, this);
    SB_slider->setGeometry(10,200,50,20);
    SB_slider->setRange(0,1);
    SB_slider->setValue(0);

    ypos = 200;
    for(int i=0;i<4; i++){
        DO_slider[i] = new QSlider(Qt::Horizontal,this);
        DO_slider[i]->setGeometry(100,ypos,50,20);
        DO_slider[i]->setRange(0,1);
        DO_slider[i]->setValue(0);
        ypos += 45;
    }
    ypos = 200;
    for(int i =0; i<2; i++){
        TS_slider[i] = new QSlider(Qt::Horizontal,this);
        TS_slider[i]->setGeometry(210,ypos,50,20);
        TS_slider[i]->setRange(0,1);
        TS_slider[i]->setValue(0);
        ypos += 45;
    }

    xpos = 20;
    for(int i = 0 ; i < 3 ; i++)
    {
        lbl[i] = new QLabel(QString("%1").arg(slider[i]->value()), this);
        lbl[i]->setGeometry(xpos+10, 100, 30, 40);

        if(i == 0)
            nametag[i] = new QLabel(QString("ACCEL"),this);
        else if(i==1)
            nametag[i] = new QLabel(QString("BRAKE"),this);
        else if(i==2)
            nametag[i] = new QLabel(QString("GEAR"),this);

        nametag[i]->setGeometry(xpos,120,50,50);
        xpos += 80;
    }
    nametag[3] = new QLabel(QString("BELT"),this);
    nametag[3]->setGeometry(20,260,50,20);

    ypos = 200;
    for(int i = 0; i<4; i++){
        DO_lbl[i] = new QLabel(QString("%1").arg(DO_slider[i]->value()),this);
        DO_lbl[i]->setGeometry(115,ypos + 10,30,40);
        ypos += 45;
    }
    nametag[4] = new QLabel(QString("DOOR"),this);
    nametag[4]->setGeometry(110,377,50,50);

    ypos = 200;
    for(int i = 0; i<2; i++){
        TS_lbl[i] = new QLabel(QString("%1").arg(DO_slider[i]->value()),this);
        TS_lbl[i]->setGeometry(225,ypos + 10,30,40);
        ypos += 45;
    }
    nametag[5] = new QLabel(QString("TSIG"),this);
    nametag[5]->setGeometry(220,287,50,50);


    xpos -= 80;
    gearlbl[0] = new QLabel(QString("P"),this);
    gearlbl[0]->setGeometry(xpos+40,75,25,40);

    gearlbl[1] = new QLabel(QString("R"),this);
    gearlbl[1]->setGeometry(xpos+40,52,25,40);

    gearlbl[2] = new QLabel(QString("N"),this);
    gearlbl[2]->setGeometry(xpos+40,28,25,40);

    gearlbl[3] = new QLabel(QString("D"),this);
    gearlbl[3]->setGeometry(xpos+40,5,25,40);

    DO_lbl2[0] = new QLabel(QString("F/L"),this);
    DO_lbl2[0]->setGeometry(170,190,30,40);

    DO_lbl2[1] = new QLabel(QString("F/R"),this);
    DO_lbl2[1]->setGeometry(170,235,30,40);

    DO_lbl2[2] = new QLabel(QString("R/L"),this);
    DO_lbl2[2]->setGeometry(170,280,30,40);

    DO_lbl2[3] = new QLabel(QString("R/R"),this);
    DO_lbl2[3]->setGeometry(170,325,30,40);

    TS_lbl2[0] = new QLabel(QString("LEFT"),this);
    TS_lbl2[0]->setGeometry(280,190,30,40);

    TS_lbl2[1] = new QLabel(QString("RIGHT"),this);
    TS_lbl2[1]->setGeometry(280,235,30,40);

    SB_lbl = new QLabel(QString("OFF"),this);
    SB_lbl->setGeometry(25,220,30,40);

    connect(slider[0], SIGNAL(valueChanged(int)), this,
                       SLOT(valueChanged1(int)));

    connect(slider[1], SIGNAL(valueChanged(int)), this,
                       SLOT(valueChanged2(int)));

    connect(slider[2], SIGNAL(valueChanged(int)), this,
                       SLOT(valueChanged3(int)));

    connect(SB_slider, SIGNAL(valueChanged(int)), this,
                       SLOT(valueChanged4(int)));

    connect(DO_slider[0], SIGNAL(valueChanged(int)), this,
                       SLOT(valueChanged5(int)));
    connect(DO_slider[1], SIGNAL(valueChanged(int)), this,
                       SLOT(valueChanged6(int)));
    connect(DO_slider[2], SIGNAL(valueChanged(int)), this,
                       SLOT(valueChanged7(int)));
    connect(DO_slider[3], SIGNAL(valueChanged(int)), this,
                       SLOT(valueChanged8(int)));

    connect(TS_slider[0], SIGNAL(valueChanged(int)), this,
                       SLOT(valueChanged9(int)));
    connect(TS_slider[1], SIGNAL(valueChanged(int)), this,
                       SLOT(valueChanged10(int)));

    //ToQString();
    //qDebug()<<msg;
}

void Widget::ToQString(void){
    msg = "";
    for(int i =0; i<2; i++)
        msg.append(QString("%1").arg(turnSignal[i]));
    for(int i =0; i<4; i++)
        msg.append(QString("%1").arg(doorOpen[i]));
    msg.append(QString("%1").arg(seatbeltOn));
    msg.append(QString("%1").arg(accelator));
    msg.append(QString("%1").arg(brake));
    msg.append(QString("%1").arg(gear));
    qDebug() << msg;

}
void Widget::valueChanged1(int value)
{
    lbl[0]->setText(QString("%1").arg(value));
    accelator = value;
    qDebug() << QString("%1").arg(accelator);
    //ToQString();
    //qDebug()<<msg;
}

void Widget::valueChanged2(int value)
{
    lbl[1]->setText(QString("%1").arg(value));
    brake = value;
    qDebug() << QString("%1").arg(brake);
    //slider[4]->setValue(slider[1]->value());
}

void Widget::valueChanged3(int value)
{
    //lbl[2]->setText(QString("%1").arg(value));
    if(value == 0)
        lbl[2]->setText(QString("P"));
    if(value == 1)
        lbl[2]->setText(QString("R"));
    if(value == 2)
        lbl[2]->setText(QString("N"));
    if(value == 3)
        lbl[2]->setText(QString("D"));

    gear = value;
    qDebug() << QString("%1").arg(gear);
    //slider[4]->setValue(slider[1]->value());
}
void Widget::valueChanged4(int value)
{
    if(value == 0){
        SB_lbl->setText(QString("OFF"));
        seatbeltOn = false;
    }
    else{
        SB_lbl->setText(QString("ON"));
        seatbeltOn = true;
    }

    qDebug() << QString("%1").arg(seatbeltOn);

}
void Widget::valueChanged5(int value)
{
    if(value == 0){
        DO_lbl[0]->setText(QString("OFF"));
        doorOpen[0] = false;
    }
    else{
        DO_lbl[0]->setText(QString("ON"));
        doorOpen[0] = true;
    }

    qDebug() << QString("%1").arg(doorOpen[0]);

}

void Widget::valueChanged6(int value)
{
    if(value == 0){
        DO_lbl[1]->setText(QString("OFF"));
        doorOpen[1] = false;
    }
    else{
        DO_lbl[1]->setText(QString("ON"));
        doorOpen[1] = true;
    }

    qDebug() << QString("%1").arg(doorOpen[1]);

}

void Widget::valueChanged7(int value)
{
    if(value == 0){
        DO_lbl[2]->setText(QString("OFF"));
        doorOpen[2] = false;
    }
    else{
        DO_lbl[2]->setText(QString("ON"));
        doorOpen[2] = true;
    }

    qDebug() << QString("%1").arg(doorOpen[0]);

}

void Widget::valueChanged8(int value)
{
    if(value == 0){
        DO_lbl[3]->setText(QString("OFF"));
        doorOpen[3] = false;
    }
    else{
        DO_lbl[3]->setText(QString("ON"));
        doorOpen[3] = true;
    }

    qDebug() << QString("%1").arg(doorOpen[3]);

}

void Widget::valueChanged9(int value)
{
    if(value == 0){
        TS_lbl[0]->setText(QString("OFF"));
        turnSignal[0] = false;
    }
    else{
        TS_lbl[0]->setText(QString("ON"));
        turnSignal[0] = true;
    }

    qDebug() << QString("%1").arg(turnSignal[0]);

}

void Widget::valueChanged10(int value)
{
    if(value == 0){
        TS_lbl[1]->setText(QString("OFF"));
        turnSignal[1] = false;
    }
    else{
        TS_lbl[1]->setText(QString("ON"));
        turnSignal[1] = true;
    }

    qDebug() << QString("%1").arg(turnSignal[1]);

}
void Widget::SocketConn(){
     socket->connectToHost("192.168.100.147", 6060);
}
void Widget::sendMSG(){
    ToQString();
    //qDebug() << "msg";
    //qDebug() << msg;
    socket->write(msg.toUtf8(), msg.length() + 1);
}
Widget::~Widget()
{

}
