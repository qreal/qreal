#include "adopter.h"
#include "xmlparser.h"
#include "mousegestures.h"
#include "ui_mousegestures.h"
#include "pathcorrector.h"
#include "validpathcreator.h"
#include "GeometricForms.h"
#include "paintmanager.h"

//todo:: что-то форма чересчур поумнела... надо бы ее тупой сделать

MouseGestures::MouseGestures(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MouseGestures)
{
    ui->setupUi(this);
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(loadFile()));
    connect(ui->twObjectPathTable, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)), this, SLOT(drawGesture()));
    mPaintManager = new PaintManager(ui->gvGesture);
}

void MouseGestures::changePath()
{
    int currentRow = ui->twObjectPathTable->currentRow();
    QString name = ui->twObjectPathTable->item(currentRow, objectColumn)->text();
    QString pathStr = ui->twObjectPathTable->item(currentRow, pathColumn)->text();
    QList<QPoint> path = Adopter::stringToPath(pathStr);
    if (!path.isEmpty())
    {
        mKeyObjectTable.setPath(name, path);
    }
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
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Xml"), ".",
                                                    tr("Xml files (*.xml)"));
    XmlParser xmlParser(fileName);
    EntityVector entities = xmlParser.parseXml();
    addEntities(entities);
    QString text = "";
    foreach (Entity entity, entities)
    {
        int rowCount = ui->twObjectPathTable->rowCount();
        ui->twObjectPathTable->setRowCount(rowCount + 1);
        QTableWidgetItem * item = new QTableWidgetItem(entity.name);
        item->setFlags(Qt::NoItemFlags);
        ui->twObjectPathTable->setItem(rowCount, objectColumn, item);
        text = Adopter::pathToString(mKeyObjectTable.getPath(entity.name));
        item = new QTableWidgetItem(text);
        ui->twObjectPathTable->setItem(rowCount, pathColumn, item);
        text = mKeyObjectTable.getKey(entity.name);
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
        QList<QPoint> path = pathCreator.createPath(entity.lines);
        mKeyObjectTable.add(entity.name, path);
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
    mCorrectPath = PathCorrector::correctPath(mMousePath);
    QString object = mKeyObjectTable.getObject(mCorrectPath);
    ui->teObject->setText(object);
    this->update();
}

void MouseGestures::mousePressEvent(QMouseEvent * event)
{
    mMousePath.clear();
    mCorrectPath.clear();
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
