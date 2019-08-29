/********************************************************************************
** Form generated from reading UI file 'cluster_ui.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLUSTER_UI_H
#define UI_CLUSTER_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Cluster_UI
{
public:
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Cluster_UI)
    {
        if (Cluster_UI->objectName().isEmpty())
            Cluster_UI->setObjectName(QString::fromUtf8("Cluster_UI"));
        Cluster_UI->resize(1059, 632);
        centralWidget = new QWidget(Cluster_UI);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        Cluster_UI->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Cluster_UI);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Cluster_UI->setStatusBar(statusBar);

        retranslateUi(Cluster_UI);

        QMetaObject::connectSlotsByName(Cluster_UI);
    } // setupUi

    void retranslateUi(QMainWindow *Cluster_UI)
    {
        Cluster_UI->setWindowTitle(QApplication::translate("Cluster_UI", "Cluster_UI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Cluster_UI: public Ui_Cluster_UI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLUSTER_UI_H
