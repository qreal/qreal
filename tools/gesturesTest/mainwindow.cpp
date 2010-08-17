#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paintmanager.h"
#include "adopter.h"
#include "QDebug"
#include "QFileDialog"
#include "QMouseEvent"

static const QString xmlDir = "../../unreal/trunk/qrxml";

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(loadFile()));
	connect(ui->listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
			this, SLOT(drawIdealGesture()));
	connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui->actionCheck_gestures, SIGNAL(triggered()), this, SLOT(checkGestures()));
	mPaintManager = new PaintManager(ui->graphicsView);
	mGesturesManager = new GesturesManager();
}

void MainWindow::drawIdealGesture()
{
	QString currentItem = ui->listWidget->currentItem()->text();
	mPaintManager->drawGesture(mGesturesManager->getPath(currentItem));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent * event)
{
	mPath.push_back(event->pos());
	this->update();
}

void MainWindow::mousePressEvent(QMouseEvent * event)
{
	mPath.clear();
	mPath.push_back(event->pos());
	this->update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent * event)
{
	mPath.push_back(event->pos());
	ui->tbQtAlg->setText(mGesturesManager->qtRecognize(mPath));
	ui->tbRectAlg->setText(mGesturesManager->rectRecognize(mPath));
	this->update();
	if (ui->listWidget->currentItem() == NULL)
		return;
	QString currentItem = ui->listWidget->currentItem()->text();
	if (currentItem.isEmpty())
		return;
	mGesturesMap[currentItem].second.push_back(Adopter::pathToString(mPath));
}

void MainWindow::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	PaintManager::drawPath(&painter, mPath);
}

void MainWindow::loadFile()
{
	this->mGesturesMap = XmlParser::parseXml();
	QList<GestureObject> gestures;
	foreach (QString name, mGesturesMap.keys())
	{
		gestures.push_back(GestureObject(name, mGesturesMap[name].first));
		ui->listWidget->addItem(name);
	}
	mGesturesManager->setIdealGestres(gestures);
}

void MainWindow::save()
{
	QMap<QString, QPair<QString, QList<QString> > > gesturesMap;
	foreach (QString name, mGesturesMap.keys())
	{
		QPair<QString, QList<QString> > pair(
				Adopter::pathToString(mGesturesMap[name].first),
				mGesturesMap[name].second);
		gesturesMap.insert(name, pair);
	}
	XmlParser::save(gesturesMap);
}

void MainWindow::checkGestures()
{
	int allGestures = 0;
	int rectGestures = 0;
	int qtGestures = 0;
	foreach (QString object, this->mGesturesMap.keys())
	{
		foreach (QString pathStr, mGesturesMap[object].second)
		{
			allGestures++;
			QList<QPoint> path = Adopter::stringToPath(pathStr);
			if (object == mGesturesManager->qtRecognize(path))
				qtGestures++;
			if (object == mGesturesManager->rectRecognize(path))
				rectGestures++;
		}
	}
	ui->tbGesturesNum->setText(QString("%1").arg(allGestures));
	ui->teQtNum->setText(QString("%1").arg(qtGestures));
	ui->teRectNum->setText(QString("%1").arg(rectGestures));
}
