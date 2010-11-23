#include "adopter.h"
#include "serializer.h"
#include "mousegestures.h"
#include "ui_mousegestures.h"
#include "pathcorrector.h"
#include "validpathcreator.h"
#include "GeometricForms.h"
#include "paintmanager.h"
#include "multistrokekeybuilder.h"


//todo:: что-то форма чересчур поумнела... надо бы ее тупой сделать
static const QString xmlDir = "../../unreal/trunk/qrxml";

MouseGestures::MouseGestures(QWidget *parent)
		: QMainWindow(parent), ui(new Ui::MouseGestures)
{
	ui->setupUi(this);
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(loadFile()));
	connect(ui->twObjectPathTable, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)), this, SLOT(drawGesture()));
	connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
	mPaintManager = new PaintManager(ui->gvGesture);
	mGesturesManager = new MultistrokeGesturesManager();
	mRecognizer = new AbstractRecognizer(mGesturesManager);
}

void MouseGestures::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
	QAction *rotate = new QAction("Rotate path", this);
	connect(rotate, SIGNAL(triggered()), this, SLOT(rotatePath()));
	menu.addAction(rotate);
	QAction *increase = new QAction("Increase path", this);
	connect(increase, SIGNAL(triggered()), this, SLOT(increasePath()));
	menu.addAction(increase);
	QAction *decrease = new QAction("Decrease path", this);
	connect(decrease, SIGNAL(triggered()), this, SLOT(decreasePath()));
	menu.addAction(decrease);
	menu.exec(event->globalPos());
}

void MouseGestures::rotatePath()
{
	QString pathStr = ui->twObjectPathTable->currentItem()->text();
	QList<QPoint> path = Adopter::stringToPath(pathStr);
	path = PathCorrector::rotate(path);
	ui->twObjectPathTable->currentItem()->setText(Adopter::pathToString(path));
}

void MouseGestures::increasePath(double koef)
{
	QString pathStr = ui->twObjectPathTable->currentItem()->text();
	QList<QPoint> path = Adopter::stringToPath(pathStr);
	path = PathCorrector::increase(path, koef);
	ui->twObjectPathTable->currentItem()->setText(Adopter::pathToString(path));
}

void MouseGestures::increasePath()
{
	increasePath(2);
}

void MouseGestures::decreasePath()
{
	increasePath(0.5);
}

void MouseGestures::save()
{
	//TODO:: save different gestures
	Objects objects;
	Serializer serializer(mFileName);
	serializer.serialize(objects);
}

void MouseGestures::changePath()
{
//	int currentRow = ui->twObjectPathTable->currentRow();
//	QString name = ui->twObjectPathTable->item(currentRow, objectColumn)->text();
//	QString pathStr = ui->twObjectPathTable->item(currentRow, pathColumn)->text();
//	QList<QPoint> path = Adopter::stringToPath(pathStr);
//	if (path.isEmpty())
//	{
//		QMessageBox msgBox;
//		msgBox.setText("Invalid string. Path cannot be modified.");
//		msgBox.exec();
//	}
}

void MouseGestures::loadFile()
{
	//save();
	mFileName = QFileDialog::getOpenFileName(this,
											 tr("Open Xml"), xmlDir,
											 tr("Xml files (*.xml)"));
	int rowCount = ui->twObjectPathTable->rowCount();
	for (int i = 0; i < rowCount; i++)
	{
		ui->twObjectPathTable->removeRow(0);
	}
	Serializer serializer(mFileName);
	EntityVector entities = serializer.deserialize();
	mGesturesManager = new MultistrokeGesturesManager();
	mRecognizer = new AbstractRecognizer(mGesturesManager, entities);
//	addEntities(entities);
	showTable();
//	this->mKeyBulder = new MultistrokeGesturesManager(entities);
}

void MouseGestures::showTable()
{
	this->disconnect(ui->twObjectPathTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(changePath()));
//	for (int i = 0; i < this->mKeyObjectTable.size(); i ++)
//	{
//		KeyObjectItem object = this->mKeyObjectTable.at(i);
//		int rowCount = ui->twObjectPathTable->rowCount();
//		ui->twObjectPathTable->setRowCount(rowCount + 1);
//		QTableWidgetItem *item = new QTableWidgetItem(object.object);
//		item->setFlags(Qt::NoItemFlags);
//		ui->twObjectPathTable->setItem(rowCount, objectColumn, item);
//		QString text = Adopter::pathToString(object.correctPath);
//		item = new QTableWidgetItem(text);
//		ui->twObjectPathTable->setItem(rowCount, pathColumn, item);
//		text = object.key;
//		item = new QTableWidgetItem(text);
//		item->setFlags(Qt::NoItemFlags);
//		ui->twObjectPathTable->setItem(rowCount, keyColumn, item);
//	}
	foreach (QString object, mRecognizer->getObjects())
	{
		int rowCount = ui->twObjectPathTable->rowCount();
		ui->twObjectPathTable->setRowCount(rowCount + 1);
		QTableWidgetItem *item = new QTableWidgetItem(object);
		ui->twObjectPathTable->setItem(rowCount, 0, item);
	}
	connect(ui->twObjectPathTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(changePath()));
}

void MouseGestures::mouseMoveEvent(QMouseEvent * event)
{
	mRecognizer->mouseMove(event->pos());
	this->update();
}

void MouseGestures::mouseReleaseEvent(QMouseEvent *event)
{
//    mCorrectPath = PathCorrector::correctPath(mMousePath);
//    QString object = mKeyObjectTable.getObject(mCorrectPath);
//    ui->teObject->setText(object);
	mRecognizer->mouseRelease(event->pos());
	this->update();
}

void MouseGestures::keyPressEvent(QKeyEvent * event)
{
	if (event->key() != 16777220)
		return;
	//mCorrectPath = PathCorrector::correctPath(mMousePath);
	ui->teObject->setText(mRecognizer->recognizeObject());
	mCorrectPath.clear();
	this->update();
}

void MouseGestures::mousePressEvent(QMouseEvent * event)
{
	mRecognizer->mousePress(event->pos());
	this->update();
}

void MouseGestures::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	PaintManager::drawPath(&painter, mRecognizer->getGesture());
	painter.setPen(Qt::red);
	//PaintManager::drawPath(&painter, mCorrectPath);
}

void MouseGestures::drawGesture()
{
	QString name = ui->twObjectPathTable->currentItem()->text();
	mRecognizer->drawGesture(name, mPaintManager);
}

MouseGestures::~MouseGestures()
{
	delete ui;
}
