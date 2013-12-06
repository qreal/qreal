#include "filterObject.h"
#include <QtCore/QDebug>
#include <QEvent>
#include <QAction>

#include "../../qrutils/uxInfo/uxInfo.h"

FilterObject::FilterObject(QObject *parent) :
  QObject(parent)
{
}

bool FilterObject::eventFilter(QObject *object, QEvent *event)
{
  Q_UNUSED(object);
  Q_UNUSED(event);
  //qDebug() << "eventFilter" << object  << " " << event->type();
  return false;
}

void FilterObject::triggeredActionActivated()
{
  QAction* action = static_cast<QAction*>(sender());
  if (action) {
    utils::UXInfo::reportMenuElements(action->text());
  }
}

void FilterObject::toggledActionActivated(bool status)
{
  QString statusText = (status) ? "checked" : "unchecked";
  QAction* action = static_cast<QAction*>(sender());
  if (action) {
    QString const statusForUStatistics = "status change on: " + statusText;
    utils::UXInfo::reportMenuElements(action->text(), statusForUStatistics);
  }
}

 void FilterObject::setStatusCollectUsabilityStatistics(bool status)
{
  utils::UXInfo::setStatus(status);
}
 
void FilterObject::reportTestStarted()
{
  utils::UXInfo::reportTestStarted();
}
 
void FilterObject::reportTestFinished()
{
  utils::UXInfo::reportTestFinished();
}
