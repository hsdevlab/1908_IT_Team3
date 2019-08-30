#include "cluster_ui.h"
#include "ui_cluster_ui.h"

Cluster_UI::Cluster_UI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cluster_UI)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    socket->connectToHost("192.168.100.147", 5050);
    connect(socket, SIGNAL(readyRead()), this, SLOT(getData()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(50);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(1, 15, 30));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    setWindowTitle(tr("Cluster UI"));
    showMaximized();
}

void Cluster_UI::paintEvent(QPaintEvent *){
    QPainter painter(this);
    int side = qMin(width(), height());
    double speedValue = speedSignal.toDouble();
    double fuelValue = (fuelSignal.toDouble() / FUEL_MAX) * 100;
    QDateTime currTime = QDateTime::currentDateTime();
    QString time_format = "yyyy-MM-dd\n HH:mm:ss";

    static const QPoint speedNeedle[5] = {
        QPoint(1, 3),
        QPoint(-1, 3),
        QPoint(-1, -60),
        QPoint(0, -70),
        QPoint(1, -60)
    };
    static const QPoint gearNeedle[3] = {
        QPoint(-1, 0),
        QPoint(0, -20),
        QPoint(1, 0)
    };
    static const QPoint fuelNeedle[5] = {
        QPoint(-1, 0),
        QPoint(0, -20),
        QPoint(1, 0)
    };
    static const QPoint speedMeter[14] = {
        QPoint(-66, 57),
        QPoint(-81, 33),
        QPoint(-87, 5),
        QPoint(-81, -25),
        QPoint(-66, -50),
        QPoint(-49, -66),
        QPoint(-24, -75),
        QPoint(4, -75),
        QPoint(27, -66),
        QPoint(48, -49),
        QPoint(63, -25),
        QPoint(68, 5),
        QPoint(63, 33),
        QPoint(49, 57)
    };
    static const QPoint gearMeter[4] = {
        QPoint(-3, -22),
        QPoint(20, -10),
        QPoint(20, 17),
        QPoint(-3, 31)
    };

    static const QString gearValue[4] = {
        "P", "R", "N", "D"
    };

    QColor needleColor(255., 255, 255);
    QColor meterColor(43., 165, 189);
    QColor meter2Color(0, 57, 78);
    QColor cautionColor(255, 30, 30);
    QColor cautionColor2(70, 50, 50);
    QColor digitalColor(125, 180, 220);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(this->width() / 2, this->height() / 2);
    painter.scale(side / 300.0, side / 300.0);

    QRectF speed_lcd = QRectF(QPointF(-15, 46), QPointF(18, 30));
    QRectF kmh = QRectF(QPointF(-15, 21), QPointF(18, 30));
    QRectF total_mileage_lcd = QRectF(QPointF(-330, 100), QPointF(18, 30));
    QRectF current_mileage_lcd = QRectF(QPointF(-410, 100), QPointF(18, 30));
    QRectF current_time = QRectF(QPointF(313, 10), QPointF(18, 30));
    QRectF total_text = QRectF(QPointF(-330, 120), QPointF(18, 30));
    QRectF current_text = QRectF(QPointF(-410, 120), QPointF(18, 30));
    painter.setPen(QColor(255, 255, 255));
    QFont speedFont = QFont("DS-Digital");
    speedFont.setBold(true);
    speedFont.setWeight(20);
    QFont defaultFont = QFont("Impact");
    defaultFont.setBold(true);
    defaultFont.setWeight(20);
    QFont musicFont("나눔바른고딕", 6);
    musicFont.setBold(true);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setFont(defaultFont);
    painter.drawText(kmh, Qt::AlignCenter, "km/h");
    painter.setFont(musicFont);
    painter.drawText(total_text, Qt::AlignCenter, "주행 거리");
    painter.drawText(current_text, Qt::AlignCenter, "주행 시간");
    painter.setFont(speedFont);
    painter.setPen(meterColor);

    painter.drawText(speed_lcd, Qt::AlignCenter, QString("%1").arg((int)speedValue));
    painter.drawText(current_mileage_lcd, Qt::AlignCenter, currentMileageSignal + "sec");
    painter.drawText(total_mileage_lcd, Qt::AlignCenter, totalMileageSignal + "km");
    painter.drawText(current_time, Qt::AlignCenter, currTime.toString(time_format));    
    painter.save();

    //연료
    painter.translate(-180, 20);
    painter.setPen(needleColor);
    painter.setBrush(needleColor);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();

    painter.setRenderHint(QPainter::Antialiasing);
    painter.rotate(180 + (fuelValue * -1.8));
    painter.drawConvexPolygon(fuelNeedle, 5);
    painter.setPen(meterColor);
    painter.setBrush(meterColor);
    painter.drawEllipse(-2, -2, 4, 4);
    painter.restore();
    painter.setPen(meterColor);
    painter.rotate(-90.0);
    for(int i = 0; i < 4; i++){
        painter.drawLine(30, 0, 25, 0);
        painter.rotate(45.0);
    }
    painter.setPen(cautionColor);
    painter.drawLine(30, 0, 25, 0);
    painter.rotate(-180.0);
    painter.setPen(meter2Color);
    for(int i = 0; i <= 27; i++){
        if((i % 9) != 0)
            painter.drawLine(30, 0, 25, 0);
        painter.rotate(5.0);
    }
    painter.setPen(cautionColor2);
    for(int i = 0; i < 8; i++){
        painter.drawLine(30, 0, 25, 0);
        painter.rotate(5.0);
    }
    painter.restore();
    painter.save();

    //기어
    painter.translate(-180, -50);
    painter.setPen(needleColor);
    painter.setBrush(needleColor);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();

    painter.rotate((gearSignal.toInt() * 60));
    painter.drawConvexPolygon(gearNeedle, 5);
    painter.setPen(meterColor);
    painter.setBrush(meterColor);
    painter.drawEllipse(-2, -2, 4, 4);
    painter.restore();
    painter.setPen(meterColor);
    painter.setFont(defaultFont);
    for(int i = 0; i < 4; i++){
        painter.drawText(gearMeter[i], gearValue[i]);
    }
    //뮤직 슬라이더
    painter.translate(315, 20);

    painter.setPen(needleColor);
    QRectF song = QRectF(QPointF(0, -20), QPointF(100, 20));
    QRectF singer = QRectF(QPointF(0, -10), QPointF(100, 20));
    painter.setFont(musicFont);
    painter.drawText(song, songNameSignal);
    musicFont.setBold(false);
    musicFont.setPointSize(4);
    painter.setFont(musicFont);
    painter.drawText(singer, singerNameSignal);
    painter.rotate(270);
    painter.drawLine(0, 0, 0, 60);
    painter.setPen(meterColor);
    int musicTime = (int)(60.0 * (currPlayTimeSignal.toDouble() / totalPlayTimeSignal.toDouble()));
    painter.drawLine(0, 0, 0, musicTime);
    painter.drawEllipse(-2, musicTime, 4, 4);
    painter.restore();

    //속도계
    painter.rotate(140.0);
    painter.setPen(Qt::NoPen);
    painter.setBrush(meterColor);
    painter.save();

    //속도계 표시 (기본 90.0도 = 0Km)
    painter.setPen(needleColor);
    painter.setBrush(needleColor);
    painter.rotate(90.0 + speedValue);
    painter.drawConvexPolygon(speedNeedle, 5);
    painter.setPen(meterColor);
    painter.setBrush(meterColor);
    painter.drawEllipse(-3, -1, 8, 8);

    painter.restore();
    painter.save();
    painter.setPen(meterColor);
    for (int i = 0; i < 21; i++) {
        painter.drawLine(89, 0, 97, 0);
        painter.rotate(10.0);
    }
    painter.setPen(cautionColor);
    for (int i = 21; i < 27; i++) {
        painter.drawLine(89, 0, 97, 0);
        painter.rotate(10.0);
    }
    painter.restore();
    painter.save();
    painter.setFont(defaultFont);
    painter.setPen(meterColor);
    painter.rotate(-140.0);
    for(int i = 0; i < 11; i++){
        painter.drawText(speedMeter[i], QString("%1").arg(i * 20.0));
    }
    painter.setPen(cautionColor);
    for(int i = 11; i < 14; i++){
        painter.drawText(speedMeter[i], QString("%1").arg(i * 20.0));
    }

    painter.restore();
    painter.setPen(meter2Color);
    for (int i = 0; i < 200; i++) {
        if((i % 10) != 0){
            painter.drawLine(90, 0, 96, 0);
        }
        painter.rotate(1.0);
    }
    painter.setPen(cautionColor2);
    for (int i = 200; i < 260; i++) {
        if((i % 10) != 0){
            painter.drawLine(90, 0, 96, 0);
        }
        painter.rotate(1.0);
    }


    /*이미지*/
    doors = new QPixmap();
    fuel = new QPixmap();
    seatbelt = new QPixmap();
    turn_left = new QPixmap();
    turn_right = new QPixmap();
    gearIcon = new QPixmap();
    mediaStatIcon = new QPixmap();
    musicIcon = new QPixmap();

    if(!doors->load(":images/doors_" + doorsSignal + ".png")) {
        qDebug() << "이미지 불러오기 에러: :images/doors" + doorsSignal + ".png";
    }
    if(fuelValue >= 25){
        if(!fuel->load(":images/fuel_off.png"))
            qDebug() << "이미지 불러오기 에러: fuel_off.png";
    }
    else{
        if(!fuel->load(":images/fuel_on.png"))
            qDebug() << "이미지 불러오기 에러: fuel_on.png";
    }
    if(!seatbelt->load(":images/seatbelt_" + seatBeltSignal + ".png")) {
        qDebug() << "이미지 불러오기 에러: seatbelt" + seatBeltSignal + ".png";
    }

    //깜빡이
    if(turnLeftSignal == "on"){
        if(left_blinkCount % 25 < 12){
            left_on = false;
            if(!turn_left->load(":images/turn_left_off.png")) {
                qDebug() << "이미지 불러오기 에러:turn_left_off.png";
            }
        }
        else{
            left_on = true;
            if(!turn_left->load(":images/turn_left_on.png")) {
                qDebug() << "이미지 불러오기 에러:turn_left_on.png";
            }
        }
        left_blinkCount++;
    }
    else{
        if(!turn_left->load(":images/turn_left_off.png")) {
            qDebug() << "이미지 불러오기 에러: turn_left_off.png";
        }
    }

    if(turnRightSignal == "on"){
        if(right_blinkCount % 25 < 12){
            right_on = false;
            if(!turn_right->load(":images/turn_right_off.png")) {
                qDebug() << "이미지 불러오기 에러:turn_right_off.png";
            }
        }
        else{
            right_on = true;
            if(!turn_right->load(":images/turn_right_on.png")) {
                qDebug() << "이미지 불러오기 에러:turn_right_on.png";
            }
        }
        right_blinkCount++;
    }
    else{
        if(!turn_right->load(":images/turn_right_off.png")) {
            qDebug() << "이미지 불러오기 에러: turn_right_off.png";
        }
    }

    if(!gearIcon->load(":images/gear.png")) {
        qDebug() << "이미지 불러오기 에러: gear.png";
    }

    if(mediaStatSignal == "off"){
        if(!mediaStatIcon->load(":images/media_start.png")) {
            qDebug() << "이미지 불러오기 에러: media_start.png";
        }
    }
    else{
        if(!mediaStatIcon->load(":images/media_pause.png")) {
            qDebug() << "이미지 불러오기 에러: gear.png";
        }
    }

    if(!musicIcon->load(":images/music_icon.png")){
        qDebug() << "이미지 불러오기 에러: music.png";
    }


    painter.scale(0.2, 0.2);
    painter.rotate(-40.0);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(700, 275, *doors);
    painter.drawPixmap(-1050, 80, *fuel);
    painter.drawPixmap(900, 275, *seatbelt);
    painter.drawPixmap(-550, -650, *turn_left);
    painter.drawPixmap(450, -650, *turn_right);
    painter.drawPixmap(-1050, -280, *gearIcon);
    painter.drawPixmap(805, -130, *mediaStatIcon);
    painter.drawPixmap(677, -550, *musicIcon);
}

void Cluster_UI::getData(){
    while(socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug() << line;

        QStringList data = line.split("|");
        if(data.length() >= 18){
            turnLeftSignal = data[0].at(0) == "1" ? "on" : "off";
            turnRightSignal = data[0].at(1) == "1" ? "on" : "off";

            if(turnLeftSignal == "off"){
                left_blinkCount = 0;
                left_on = false;
            }

            if(turnRightSignal == "off"){
                right_blinkCount = 0;
                right_on = false;
            }

            doorsSignal = data[1].toInt() > 0 ? "on" : "off";
            seatBeltSignal = data[2] == "1" ? "on" : "off";
            brakeSignal = data[4].toInt() > 0 ? "on" : "off";
            gearSignal = data[5]; //0123: PRND
            speedSignal = data[6];
            fuelSignal = data[8];
            totalMileageSignal = data[9];
            currentMileageSignal = data[10];
            fuelEconomySignal = data[11];
            mediaStatSignal = data[14] == "1" ? "on" : "off";
            singerNameSignal = data[15];
            songNameSignal = data[16];
            totalPlayTimeSignal = data[17];
            currPlayTimeSignal = data[18];
        }
        else{
            turnLeftSignal = "off";
            turnRightSignal ="off";

            if(turnLeftSignal == "off"){
                left_blinkCount = 0;
                left_on = false;
            }

            if(turnRightSignal == "off"){
                right_blinkCount = 0;
                right_on = false;
            }

            doorsSignal = "off";
            seatBeltSignal = "off";
            brakeSignal = "off";
            gearSignal = "0";
            speedSignal = "0";
            fuelSignal = "0";
            currentMileageSignal = "0.0";
            totalMileageSignal = "0.0";
            fuelEconomySignal = "0.0";
            mediaStatSignal = "off";
            singerNameSignal = "-";
            songNameSignal = "Untitled";
            totalPlayTimeSignal = "1";
            currPlayTimeSignal = "0";
        }
    }
}
Cluster_UI::~Cluster_UI()
{
    delete ui;
}
