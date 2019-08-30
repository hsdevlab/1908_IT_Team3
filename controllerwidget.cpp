#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    socket = new QTcpSocket(this);
    SocketConn();
    QTimer* timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),SLOT(sendMSG()));
    timer->start(50);

    QTimer* musictimer = new QTimer(this);
    connect(musictimer,SIGNAL(timeout()),SLOT(timesetting()));
    musictimer->start(50);


    Volume = new QDial(this);
    Volume->setRange(0, 100);
    Volume->setValue(30);
    Volume->setGeometry(130, 520, 120, 120);
    Volume->setNotchesVisible(true);
    connect(Volume, SIGNAL(valueChanged(int)),this, SLOT(musicvolume(int)));
    V_lbl = new QLabel(QString("%1").arg(volume),this);
    V_lbl->setGeometry(185,560,50,40);



    Music[0] = new QPushButton(QString("Play"), this);
    Music[0]->setGeometry(70, 450, 100, 20);
    Music[1] = new QPushButton(QString("Stop"), this);
    Music[1]->setGeometry(195, 450, 100, 20);
    Music[2] = new QPushButton(QString("<<"),this);
    Music[2]->setGeometry(25,450,30,20);
    Music[3] = new QPushButton(QString(">>"),this);
    Music[3]->setGeometry(310,450,30,20);

    connect(Music[0],SIGNAL(clicked()),this,SLOT(musicplay()));
    connect(Music[1],SIGNAL(clicked()),this,SLOT(musicstop()));
    connect(Music[2],SIGNAL(clicked()),this,SLOT(prevsong()));
    connect(Music[3],SIGNAL(clicked()),this,SLOT(nextsong()));


    M_lbl = new QLabel(QString("Music"),this);
    M_lbl->setGeometry(149,462,120,50);

    playtime = new QLabel("00:00",this);
    playtime->setGeometry(150,485,60,40);
    durationtime = new QLabel("00.00",this);
    durationtime->setGeometry(150,500,60,40);

    mp = new QMediaPlayer(this);
    mpl = new QMediaPlaylist();

    mpl->addMedia(QUrl::fromLocalFile(QFileInfo("C:\\Kalimba.mp3").absoluteFilePath()));
    mpl->addMedia(QUrl::fromLocalFile(QFileInfo("C:\\Maid with the Flaxen Hair.mp3").absoluteFilePath()));
    mpl->addMedia(QUrl::fromLocalFile(QFileInfo("C:\\Sleep Away.mp3").absoluteFilePath()));
    //mpl->addMedia(QUrl::fromLocalFile(QFileInfo("C:\\Coldplay - All I Can Think About Is You.mp3").absoluteFilePath()));
    mpl->addMedia(QUrl::fromLocalFile(QFileInfo("C:\\Elton John - Rocket Man.mp3").absoluteFilePath()));

    musiclist.push_back("Kalimba");
    singerlist.push_back("mwkim");
    musiclist.push_back("Maid with the Flaxen Hair");
    singerlist.push_back("jmyang");
    musiclist.push_back("Sleep Away");
    singerlist.push_back("Sarah Brightman");
    musiclist.push_back("Rocket Man");
    singerlist.push_back("Elton John");


   //mpv = new QVideoWidget(this);
    //mpv->setGeometry(400,200, 200, 200);
    //mpv->setAspectRatioMode(Qt::IgnoreAspectRatio);


    mp->setVolume(30);
    mp->setPlaybackRate(1);
    //mp->setVideoOutput(mpv);

    mpl->setCurrentIndex(0);
    mp->setPlaylist(mpl);


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
        if(i==2)
            lbl[i] = new QLabel(QString("P"), this);
        else
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
        DO_lbl[i] = new QLabel(QString("CLOSED"),this);
        DO_lbl[i]->setGeometry(115,ypos + 10,50,40);
        ypos += 45;
    }
    nametag[4] = new QLabel(QString("DOOR"),this);
    nametag[4]->setGeometry(110,377,50,50);

    ypos = 200;
    for(int i = 0; i<2; i++){
        TS_lbl[i] = new QLabel(QString("OFF"),this);
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

    //connect(mp,SIGNAL(positionChanged(qint32)),this,SLOT(printduation(qint32)));
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
    msg.append(QString("%1").arg(musicOn));
    msg.append('|');
    msg.append(singer);
    msg.append('|');
    msg.append(musicname);
    msg.append('|');
/*  msg.append(QString("%1").arg(dur_min));
    msg.append('|');
    msg.append(QString("%1").arg(dur_sec));
    msg.append('|');
    msg.append(QString("%1").arg(min));
    msg.append('|');
    msg.append(QString("%1").arg(sec));
    msg.append('|');
*/
    msg.append(QString("%1").arg(mp->duration()/1000));
    msg.append('|');
    msg.append(QString("%1").arg(mp->position()/1000));
    msg.append('|');
    //qDebug() << msg;

}
void Widget::valueChanged1(int value)
{
    lbl[0]->setText(QString("%1").arg(value));
    accelator = value;
    qDebug() << QString("%1").arg(accelator);
    //ToQString();
    //qDebug()<<msg;
}


/*void Widget::printduation(qint32 a)
{
    qDebug() << a;
}*/

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
        DO_lbl[0]->setText(QString("CLOSED"));
        doorOpen[0] = false;
    }
    else{
        DO_lbl[0]->setText(QString("OPEN"));
        doorOpen[0] = true;
    }

    qDebug() << QString("%1").arg(doorOpen[0]);

}

void Widget::valueChanged6(int value)
{
    if(value == 0){
        DO_lbl[1]->setText(QString("CLOSED"));
        doorOpen[1] = false;
    }
    else{
        DO_lbl[1]->setText(QString("OPEN"));
        doorOpen[1] = true;
    }

    qDebug() << QString("%1").arg(doorOpen[1]);

}

void Widget::valueChanged7(int value)
{
    if(value == 0){
        DO_lbl[2]->setText(QString("CLOSED"));
        doorOpen[2] = false;
    }
    else{
        DO_lbl[2]->setText(QString("OPEN"));
        doorOpen[2] = true;
    }

    qDebug() << QString("%1").arg(doorOpen[0]);

}

void Widget::valueChanged8(int value)
{
    if(value == 0){
        DO_lbl[3]->setText(QString("CLOSED"));
        doorOpen[3] = false;
    }
    else{
        DO_lbl[3]->setText(QString("OPEN"));
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
void Widget::musicplay(){
    musicOn = true;
    mp->play();
    musicname = musiclist[mpl->currentIndex()];
    singer = singerlist[mpl->currentIndex()];
    M_lbl->setText(musicname);
    //qDebug()<<singer;
    long temp = mp->duration();
    qDebug() << temp;
    temp /= 1000;
    dur_min = temp/60;
    dur_sec = temp%60;
    durationtime->setText(QString(QString("%1").arg(dur_min)+":"+QString("%1").arg(dur_sec)));

    qDebug() << dur_min;

    //qDebug()<<mp->duration();
    //qDebug()<<mp->position();

}
void Widget::musicstop(){
    musicOn = false;
    //mp->stop();
    mp->pause();
    M_lbl->setText("paused");
}

void Widget::musicvolume(int value){
    volume = value;
    V_lbl->setText(QString("%1").arg(value));
    mp->setVolume(value);
    //and turn up the volume of the music using ~~
}

void Widget::prevsong(){
    if(curidx == 0)
        curidx = MUSIC;

    curidx = (curidx - 1);

    mpl->setCurrentIndex(curidx);
    mp->play();

    musicname = musiclist[mpl->currentIndex()];
    singer = singerlist[mpl->currentIndex()];
    M_lbl->setText(musicname);

    qDebug() << dur_min;
}

void Widget::nextsong(){
    curidx = (curidx + 1)%MUSIC;

    mpl->setCurrentIndex(curidx);
    mp->play();

    musicname = musiclist[mpl->currentIndex()];
    singer = singerlist[mpl->currentIndex()];
    M_lbl->setText(musicname);


}

void Widget::timesetting(){
    long t1 = mp->position();
    long t2 = mp->duration();
    t1 /= 1000;
    min = t1/60;
    sec = t1%60;

    t2 /= 1000;
    if(musicOn == false){
        dur_min = 0;
        dur_sec = 0;
    }
    else{
        dur_min = t2/60;
        dur_sec = t2%60;
    }
    //qDebug()<<"???";
    playtime->setText(QString(QString("%1").arg(min)+":"+QString("%1").arg(sec)));
    durationtime->setText(QString(QString("%1").arg(dur_min)+":"+QString("%1").arg(dur_sec)));
}


void Widget::SocketConn(){
     socket->connectToHost("192.168.100.147", 5050);
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
