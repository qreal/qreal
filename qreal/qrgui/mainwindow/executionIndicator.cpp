#include "executionIndicator.h"

#include <QApplication>
#include <QDesktopWidget>

using namespace qReal::gui;

ExecutionIndicator::ExecutionIndicator(QWidget *parent, const QString &message) : QDialog(parent)
{
	mLabel = new QLabel(message);
	mLayout = new QGridLayout(this);
	mLayout->addWidget(mLabel,0,0);
	QRect rect(QApplication::desktop()->screenGeometry());
	move(rect.width()/2, rect.height()/2);
	setWindowModality(Qt::WindowModal);
	setModal(true);
	setWindowFlags(Qt::FramelessWindowHint);
}
