#include "cluster_ui.h"

Cluster_UI::Cluster_UI(QString source, QWidget *parent)
    : m_source(source), QQuickWidget (parent)
{
    qDebug() << m_source;
    this->setSource(m_source);
    this->show();
}
