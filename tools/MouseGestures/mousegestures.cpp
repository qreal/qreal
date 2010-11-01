#include "adopter.h"
#include "serializer.h"
#include "mousegestures.h"
#include "ui_mousegestures.h"
#include "pathcorrector.h"
#include "validpathcreator.h"
#include "GeometricForms.h"
#include "paintmanager.h"


//todo:: что-то форма чересчур поумнела... надо бы ее тупой сделать
static const QString xmlDir = "../../unreal/trunk/qrxml";

MouseGestures::MouseGestures(QWidget *parent)
		: QMainWindow(parent), ui(new Ui::MouseGestures)
{
	ui->setupUi(this);
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(loadFile()));
	connect(ui->twObjectPathTable, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)), this, SLOT(drawGesture()));
	connect(ui->bSave, SIGNAL(clicked()), this, SLOT(save()));
	mPaintManager = new PaintManager(ui->gvGesture);
	showTable();
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
	Objects objects = mKeyObjectTable.getObjects();
	Serializer serializer(mFileName);
	serializer.serialize(objects);
}

void MouseGestures::changePath()
{
	int currentRow = ui->twObjectPathTable->currentRow();
	QString name = ui->twObjectPathTable->item(currentRow, objectColumn)->text();
	QString pathStr = ui->twObjectPathTable->item(currentRow, pathColumn)->text();
	QList<QPoint> path = Adopter::stringToPath(pathStr);
	if (!path.isEmpty())
		mKeyObjectTable.setPath(name, path);
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Invalid string. Path cannot be modified.");
		msgBox.exec();
		path = mKeyObjectTable.getPath(name);
		pathStr = Adopter::pathToString(path);
		ui->twObjectPathTable->item(currentRow, pathColumn)->setText(pathStr);
	}
	QString key = mKeyObjectTable.getKey(name);
	ui->twObjectPathTable->item(currentRow, keyColumn)->setText(key);
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
	mKeyObjectTable.clear();
	addEntities(entities);
	showTable();
	this->mKeyBulder = new MultistrokeKeyBulder(entities);
}

void MouseGestures::showTable()
{
	this->disconnect(ui->twObjectPathTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(changePath()));
	for (int i = 0; i < this->mKeyObjectTable.size(); i ++)
	{
		KeyObjectItem object = this->mKeyObjectTable.at(i);
		int rowCount = ui->twObjectPathTable->rowCount();
		ui->twObjectPathTable->setRowCount(rowCount + 1);
		QTableWidgetItem *item = new QTableWidgetItem(object.object);
		item->setFlags(Qt::NoItemFlags);
		ui->twObjectPathTable->setItem(rowCount, objectColumn, item);
		QString text = Adopter::pathToString(object.correctPath);
		item = new QTableWidgetItem(text);
		ui->twObjectPathTable->setItem(rowCount, pathColumn, item);
		text = object.key;
		item = new QTableWidgetItem(text);
		item->setFlags(Qt::NoItemFlags);
		ui->twObjectPathTable->setItem(rowCount, keyColumn, item);
	}
	connect(ui->twObjectPathTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(changePath()));
}

void MouseGestures::addEntities(EntityVector const & entities)
{
	foreach (Entity entity, entities)
	{
		ValidPathCreator pathCreator;
		QList<QPoint> path = pathCreator.createPath(entity.second);
		mKeyObjectTable.add(entity.first, path);
	}
}

void MouseGestures::mouseMoveEvent(QMouseEvent * event)
{
	mMousePath.push_back(event->pos());
	this->update();
}

void MouseGestures::mouseReleaseEvent(QMouseEvent *event)
{
	mMousePath.push_back(event->pos());
//    mCorrectPath = PathCorrector::correctPath(mMousePath);
//    QString object = mKeyObjectTable.getObject(mCorrectPath);
//    ui->teObject->setText(object);
	this->update();
}

void MouseGestures::keyPressEvent(QKeyEvent * event)
{
	if (event->key() != 16777220)
		return;
	mCorrectPath = PathCorrector::correctPath(mMousePath);
	QString object = this->mKeyBulder->getObject(mMousePath);
	ui->teObject->setText(object);
	mMousePath.clear();
	mCorrectPath.clear();
	this->update();
}

void MouseGestures::mousePressEvent(QMouseEvent * event)
{
	mMousePath.push_back(event->pos());
	this->update();
}

void MouseGestures::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	PaintManager::drawPath(&painter, mMousePath);
	painter.setPen(Qt::red);
	PaintManager::drawPath(&painter, mCorrectPath);
}

void MouseGestures::drawGesture()
{
	int row = ui->twObjectPathTable->currentRow();
	QString pathStr = ui->twObjectPathTable->item(row, pathColumn)->text();
	mPaintManager->drawGesture(pathStr);
}

MouseGestures::~MouseGestures()
{
	delete ui;
}
