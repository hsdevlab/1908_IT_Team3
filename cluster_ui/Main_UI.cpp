#include "main_ui.h"
#include "ui_main_ui.h"

Main_UI::Main_UI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_UI)
{
    ui->setupUi(this);
    cluster_ui = new Cluster_UI("qrc:qml/main.qml", this);
    cluster_ui->show();
}

Main_UI::~Main_UI()
{
    delete ui;
}
