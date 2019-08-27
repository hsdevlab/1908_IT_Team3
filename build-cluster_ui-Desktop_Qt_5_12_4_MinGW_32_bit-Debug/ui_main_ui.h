/********************************************************************************
** Form generated from reading UI file 'main_ui.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_UI_H
#define UI_MAIN_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Main_UI
{
public:

    void setupUi(QWidget *Main_UI)
    {
        if (Main_UI->objectName().isEmpty())
            Main_UI->setObjectName(QString::fromUtf8("Main_UI"));
        Main_UI->resize(400, 300);

        retranslateUi(Main_UI);

        QMetaObject::connectSlotsByName(Main_UI);
    } // setupUi

    void retranslateUi(QWidget *Main_UI)
    {
        Main_UI->setWindowTitle(QApplication::translate("Main_UI", "Main_UI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Main_UI: public Ui_Main_UI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_UI_H
