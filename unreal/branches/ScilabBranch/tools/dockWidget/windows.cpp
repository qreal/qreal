#include "windows.h"
#include "ui_windows.h"
#include <QDebug.h>
#include <QtGui>
#include <stdlib.h>
#include "math.h"

Windows::Windows(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::Windows)
{

	ui->setupUi(this);
	m_timeLine = new QTimeLine(1000, ui->dockWidget);
	connect(m_timeLine, SIGNAL(frameChanged(int)), SLOT(setSlidePosition(int)));
	connect(m_timeLine, SIGNAL(stateChanged(QTimeLine::State)), SLOT(changeState()));
	connect(m_timeLine, SIGNAL(finished()), m_timeLine, SLOT(toggleDirection()));
	connect(ui->pushButton, SIGNAL(clicked()), m_timeLine, SLOT(start()));
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonPressed()));
}

void Windows::resizeEvent(QResizeEvent*)
{
	ui->dockWidget->resize(ui->dockWidget->width(), height()-PANEL_MARGIN);
}

void Windows::changeState()
{
	ui->dockWidget->show();
	ui->dockWidget->raise();
}

void Windows::setSlidePosition(int pos)
{
	switch(dockWidgetArea(ui->dockWidget)){
	case Qt::LeftDockWidgetArea:{
			ui->dockWidget->move(- pos , ui->dockWidget->pos().y());
			break;
		}
	case Qt::RightDockWidgetArea:{
			ui->dockWidget->move(startX + pos , ui->dockWidget->pos().y());
			break;
		}
	case Qt::TopDockWidgetArea:{
			ui->dockWidget->move(ui->dockWidget->pos().x() , -pos);
			break;
		}
	case Qt::BottomDockWidgetArea:{
			ui->dockWidget->move(ui->dockWidget->pos().x() , startY + pos);
			break;
		}
	default:
		break;
	}
}

void Windows::buttonPressed()
{
	qDebug() << "width" << ui->dockWidget->width() << "frameGeometry().width" << ui->dockWidget->frameGeometry().width();
	qDebug() << "height" << ui->dockWidget->height() << "frameGeometry().height" << ui->dockWidget->frameGeometry().height();
	m_timeLine->setFrameRange(0, ui->dockWidget->width());
	switch(dockWidgetArea(ui->dockWidget)){
	case Qt::LeftDockWidgetArea:{
			m_timeLine->setFrameRange(0, ui->dockWidget->frameGeometry().width());
		}
	case Qt::RightDockWidgetArea:{
			m_timeLine->setFrameRange(0, ui->dockWidget->frameGeometry().width());
		}
	case Qt::TopDockWidgetArea:{
			m_timeLine->setFrameRange(0, ui->dockWidget->frameGeometry().height() + 20);
		}
	case Qt::BottomDockWidgetArea:{
			m_timeLine->setFrameRange(0, ui->dockWidget->frameGeometry().height() + 20);
		}
	default:
		break;
	}
	if(m_timeLine->direction() == 0){
		startX = ui->dockWidget->pos().x();
		startY = ui->dockWidget->pos().y();
	}
	m_timeLine->start();
}


Windows::~Windows()
{
	delete ui;
}

void Windows::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}


