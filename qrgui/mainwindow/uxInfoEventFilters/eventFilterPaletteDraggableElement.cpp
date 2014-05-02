#include "eventFilterPaletteDraggableElement.h"

#include "qrutils/uxInfo/uxInfo.h"

#include <QtCore/QEvent>
#include <QtWidgets/QAction>

#include <QtCore/QDebug>
#include "mainwindow/palette/draggableElement.h"

EventFilterPaletteDraggableElement::EventFilterPaletteDraggableElement(QObject *parent)
	: QObject(parent)
{
}

bool EventFilterPaletteDraggableElement::eventFilter(QObject *object, QEvent *event)
{
	gui::DraggableElement *element = static_cast<gui::DraggableElement *>(object);
	QString name = "none";
	if (element) {
		name = element->text();
	}
	QString userAction;
	if (event->type() == QEvent::Enter) {
		userAction = QString::fromUtf8("Навести на элемент в палитре — название элемента: ") + name + "|";
		utils::UXInfo::instance()->reportUserAction(userAction);
	}
	else if (event->type() == QEvent::Leave) {
		userAction = QString::fromUtf8("Убрать наведение с элемента в палитре — название элемента: ") + name + "|";
		utils::UXInfo::instance()->reportUserAction(userAction);
	}
	else if (event->type() == QEvent::MouseButtonPress) {
		userAction = QString::fromUtf8("Нажать на элемент в палитре — название элемента: ") + name + "|";//кнопка мыши: " + mouseButton(event);
		utils::UXInfo::instance()->reportUserAction(userAction);
	}
	else if (event->type() == QEvent::MouseButtonRelease) {
		userAction = QString::fromUtf8("Отпустить элемент в палитре — название элемента: ") + name + "|";
		utils::UXInfo::instance()->reportUserAction(userAction);
	}
	return false;
}

QString const EventFilterPaletteDraggableElement::mouseButton(QEvent *event)
{
	QString button;
	QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
	if (mouseEvent) {
		if (mouseEvent->button() == Qt::RightButton) {
			button = " right button";
		}
		else {
			button = " left button";
		}
	}
	return button;
}
