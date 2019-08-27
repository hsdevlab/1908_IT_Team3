#ifndef CLUSTER_UI_H
#define CLUSTER_UI_H

#include <QQuickWidget>
#include <QtCore/QtCore>
#include <QDebug>

class Cluster_UI : public QQuickWidget
{
    Q_OBJECT
public:
    Cluster_UI(QString source, QWidget *parent = nullptr);

private:
    QString m_source;
};

#endif // CLUSTER_UI_H
