#ifndef CLUSTER_UI_H
#define CLUSTER_UI_H

#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QtNetwork/QTcpSocket>

#define FUEL_MAX 65.0

namespace Ui {
class Cluster_UI;
}

class Cluster_UI : public QMainWindow
{
    Q_OBJECT

public:
    explicit Cluster_UI(QWidget *parent = nullptr);
    ~Cluster_UI();

private:
    Ui::Cluster_UI *ui;
    void paintEvent(QPaintEvent*);
    QPixmap *doors;
    QPixmap *fuel;
    QPixmap *seatbelt;
    QPixmap *turn_left;
    QPixmap *turn_right;
    QPixmap *gearIcon;
    QPixmap *mediaStatIcon;
    QPixmap *musicIcon;
    QTcpSocket *socket;

    QString turnLeftSignal = "off";
    QString turnRightSignal = "off";
    QString speedSignal = "0.0";
    QString doorsSignal = "off";
    QString brakeSignal = "off";
    QString seatBeltSignal = "off";
    QString fuelSignal = "65.0";
    QString currentMileageSignal = "0";
    QString totalMileageSignal = "0";
    QString gearSignal = "0";
    QString fuelEconomySignal = "0";
    QString singerNameSignal = "";
    QString songNameSignal = "";
    QString mediaStatSignal = "off";
    QString totalPlayTimeSignal = "1";
    QString currPlayTimeSignal = "0";

    int left_blinkCount = 0;
    int right_blinkCount = 0;
    bool left_on = false;
    bool right_on = false;

public slots:
    void getData();
};

#endif // CLUSTER_UI_H
