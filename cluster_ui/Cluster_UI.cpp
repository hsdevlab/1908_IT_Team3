#include "cluster_ui.h"
#include "ui_cluster_ui.h"

Cluster_UI::Cluster_UI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cluster_UI)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    socket->connectToHost("192.168.100.147", 6060);
    connect(socket, SIGNAL(readyRead()), this, SLOT(getData()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(30);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(1, 2, 20));
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

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(this->width() / 2, this->height() / 2);
    painter.scale(side / 300.0, side / 300.0);

    QRectF speed_lcd = QRectF(QPointF(-15, 46), QPointF(18, 30));
    QRectF total_mileage_lcd = QRectF(QPointF(-330, 46), QPointF(18, 30));
    QRectF current_mileage_lcd = QRectF(QPointF(-410, 46), QPointF(18, 30));
    painter.setPen(QColor(255, 255, 255));
    QFont speedFont = QFont("DS-Digital");
    speedFont.setBold(true);
    speedFont.setWeight(20);
    QFont defaultFont = QFont("Impact");
    defaultFont.setBold(true);
    defaultFont.setWeight(20);
    painter.setFont(speedFont);

    painter.drawText(speed_lcd, Qt::AlignCenter, QString("%1").arg((int)speedValue));
    painter.drawText(current_mileage_lcd, Qt::AlignCenter, currentMileageSignal);
    painter.drawText(total_mileage_lcd, Qt::AlignCenter, totalMileageSignal);
    painter.save();

    //연료
    painter.translate(-180, 0);
    painter.setPen(needleColor);
    painter.setBrush(needleColor);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();

    painter.rotate((fuelValue * 1.8) + 180.0);
    painter.drawConvexPolygon(fuelNeedle, 5);
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
    painter.translate(-180, -70);
    painter.setPen(needleColor);
    painter.setBrush(needleColor);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();

    painter.rotate((gearSignal.toInt() * 45));
    painter.drawConvexPolygon(gearNeedle, 5);
    painter.restore();
    painter.setPen(meterColor);
    painter.setFont(defaultFont);
    for(int i = 0; i < 4; i++){
        painter.drawText(gearMeter[i], gearValue[i]);
    }
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
    if(!turn_left->load(":images/turn_left_" + turnLeftSignal + ".png")) {
        qDebug() << "이미지 불러오기 에러: turn_left_" + turnLeftSignal + ".png";
    }
    if(!turn_right->load(":images/turn_right_" + turnRightSignal + ".png")) {
        qDebug() << "이미지 불러오기 에러: turn_right_" + turnRightSignal + ".png";
    }
    if(!gearIcon->load(":images/gear.png")) {
        qDebug() << "이미지 불러오기 에러: gear.png";
    }

    painter.scale(0.2, 0.2);
    painter.rotate(-40.0);
    painter.drawPixmap(800, 180, *doors);
    painter.drawPixmap(-1000, -20, *fuel);
    painter.drawPixmap(900, 180, *seatbelt);
    painter.drawPixmap(-550, -650, *turn_left);
    painter.drawPixmap(450, -650, *turn_right);
    painter.drawPixmap(-1000, -380, *gearIcon);
}

void Cluster_UI::getData(){
    while(socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug() << line;

        QStringList data = line.split("|");

        turnLeftSignal = data[0].at(0) == "1" ? "on" : "off";
        turnRightSignal = data[0].at(1) == "1" ? "on" : "off";
        doorsSignal = data[1].toInt() > 0 ? "on" : "off";
        seatBeltSignal = data[2] == "1" ? "on" : "off";
        brakeSignal = data[4].toInt() > 0 ? "on" : "off";
        gearSignal = (QString)data[5].toInt(); //0123: PRND
        speedSignal = data[6];
        fuelSignal = data[7];
        currentMileageSignal = (QString)data[8].toInt();
        totalMileageSignal = (QString)data[8].toInt();
        fuelEconomySignal = data[10];

        qDebug() << "speed: " << speedSignal << " fuel: " << fuelSignal;
    }
}
Cluster_UI::~Cluster_UI()
{
    delete ui;
}
