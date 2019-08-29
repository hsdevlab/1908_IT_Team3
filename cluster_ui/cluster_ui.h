#ifndef CLUSTER_UI_H
#define CLUSTER_UI_H

#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QLCDNumber>

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
};

#endif // CLUSTER_UI_H
