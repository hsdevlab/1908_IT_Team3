#include "cluster_ui.h"
#include "ui_cluster_ui.h"

Cluster_UI::Cluster_UI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cluster_UI)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
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
//    painter.scale(1.0, 1.0);
    QTime time = QTime::currentTime();
    static const QPoint needle[5] = {
        QPoint(1, 3),
        QPoint(-1, 3),
        QPoint(-1, -60),
        QPoint(0, -70),
        QPoint(1, -60)
    };
    static const QPoint speedMeter[14] = {
        QPoint(-84, 72),
        QPoint(-108, 40),
        QPoint(-114, 5),
        QPoint(-108, -31),
        QPoint(-89, -64),
        QPoint(-65, -89),
        QPoint(-30, -101),
        QPoint(10, -101),
        QPoint(43, -89),
        QPoint(78, -64),
        QPoint(96, -31),
        QPoint(102, 5),
        QPoint(96, 40),
        QPoint(78, 72)
    };
    QLCDNumber *speed_lcd = new QLCDNumber(3, this);
    QLCDNumber *mileage_lcd = new QLCDNumber(6, this);
    QLCDNumber *current_lcd = new QLCDNumber(6, this);
    speed_lcd->setSegmentStyle(QLCDNumber::Outline);
    mileage_lcd->setSegmentStyle(QLCDNumber::Outline);
    current_lcd->setSegmentStyle(QLCDNumber::Outline);
    speed_lcd->display(100);

    QColor needleColor(255, 255, 255);
    QColor needle2Color(50, 50, 50);
    QColor cautionColor(255, 30, 30);
    QColor cautionColor2(70, 50, 50);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(this->width() / 2, this->height() / 2);
    painter.rotate(140.0);
    painter.scale(side / 300.0, side / 300.0);
    painter.setPen(Qt::NoPen);
    painter.setBrush(needleColor);
    painter.save();

    painter.rotate(time.second() * 6.0 + (time.msec() * 0.006));
    painter.drawConvexPolygon(needle, 5);
    painter.restore();
    painter.save();
    painter.setPen(needleColor);
    for (int i = 0; i < 21; i++) {
        painter.drawLine(90, 0, 100, 0);
        painter.rotate(10.0);
    }
    painter.setPen(cautionColor);
    for (int i = 21; i < 27; i++) {
        painter.drawLine(90, 0, 100, 0);
        painter.rotate(10.0);
    }
    painter.restore();
    painter.save();
    QFont font = QFont("맑은 고딕");
    font.setBold(true);
    font.setWeight(20);
    painter.setFont(font);
    painter.setPen(needleColor);
    painter.rotate(-140.0);
    for(int i = 0; i < 11; i++){
        painter.drawText(speedMeter[i], QString("%3").arg(i * 20.0));
    }
    painter.setPen(cautionColor);
    for(int i = 11; i < 14; i++){
        painter.drawText(speedMeter[i], QString("%3").arg(i * 20.0));
    }

    painter.restore();
    painter.setPen(needle2Color);
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

    if(!doors->load(":images/doors_off.png")) {
        qDebug() << "이미지 불러오기 에러: doors_off";
        exit(-1);
    }
    if(!fuel->load(":images/fuel_off.png")) {
        qDebug() << "이미지 불러오기 에러: fuel_off";
        exit(-1);
    }
    if(!seatbelt->load(":images/seatbelt_off.png")) {
        qDebug() << "이미지 불러오기 에러: seatbelt_off";
        exit(-1);
    }
    if(!turn_left->load(":images/turn_left_off.png")) {
        qDebug() << "이미지 불러오기 에러: turn_left_off";
        exit(-1);
    }
    if(!turn_right->load(":images/turn_right_off.png")) {
        qDebug() << "이미지 불러오기 에러: turn_right_off";
        exit(-1);
    }

    painter.scale(0.2, 0.2);
    painter.rotate(-40.0);
    painter.drawPixmap(800, 180, *doors);
    painter.drawPixmap(-1000, -100, *fuel);
    painter.drawPixmap(900, 180, *seatbelt);
    painter.drawPixmap(-550, -650, *turn_left);
    painter.drawPixmap(450, -650, *turn_right);
}

Cluster_UI::~Cluster_UI()
{
    delete ui;
}
