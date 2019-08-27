#ifndef MAIN_UI_H
#define MAIN_UI_H

#include <QWidget>
#include <QQuickWidget>
#include <cluster_ui.h>

namespace Ui {
class Main_UI;
}

class Main_UI : public QWidget
{
    Q_OBJECT

public:
    explicit Main_UI(QWidget *parent = nullptr);
    ~Main_UI();

private:
    Ui::Main_UI *ui;
    Cluster_UI *cluster_ui;
};

#endif // MAIN_UI_H
