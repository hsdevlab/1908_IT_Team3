#include "main_ui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Main_UI w;
    w.show();

    return a.exec();
}
