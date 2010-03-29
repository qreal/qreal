#include "adopter.h"
#include "xmlparser.h"
#include "mousegestures.h"
#include "ui_mousegestures.h"
#include "mousemovementmanager.h"
#include "key8manager.h"
#include "pathcorrector.h"
#include "xmlparser.h"
#include "validpathcreator.h"
#include "GeometricForms.h"
#include "levenshteindistance.h"

//todo:: что-то форма чересчур поумнела... надо бы ее тупой сделать

MouseGestures::MouseGestures(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MouseGestures)
{
    ui->setupUi(this);
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(loadFile()));
}

void MouseGestures::changePath()
{
    int currentRow = ui->twObjectPathTable->currentRow();
    QString name = ui->twObjectPathTable->item(currentRow, objectColumn)->text();
    QString pathStr = ui->twObjectPathTable->item(currentRow, pathColumn)->text();
    QList<QPoint> path = Adopter().stringToPath(pathStr);
    if (!path.isEmpty())
    {
        mKeyObjectTable.setPath(name, path);
    }
    else
    {
        path = mKeyObjectTable.getPath(name);
        pathStr = Adopter().pathToString(path);
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
    QString text;
    int rowCount;
    foreach (Entity entity, entities)
    {
        rowCount = ui->twObjectPathTable->rowCount();
        ui->twObjectPathTable->setRowCount(rowCount + 1);
        QTableWidgetItem * item = new QTableWidgetItem(entity.name);
        item->setFlags(Qt::NoItemFlags);
        ui->twObjectPathTable->setItem(rowCount, objectColumn, item);
        text = Adopter().pathToString(mKeyObjectTable.getPath(entity.name));
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
    PathCorrector pathCorrector(mMousePath);
    mCorrectPath = pathCorrector.correctPath();
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
    drawPath(&painter, mMousePath);
    painter.setPen(Qt::red);
    drawPath(&painter, mCorrectPath);
}

void MouseGestures::drawPath(QPainter * painter, QList<QPoint> const & path)
{
    QPoint previousPoint(-1, -1);
    if (path.isEmpty())
    {
        return;
    }
    foreach (QPoint currentPoint, path)
    {
        if (previousPoint.x() >= 0 && previousPoint.y() >= 0)
        {
            painter->drawLine(previousPoint, currentPoint);
        }
        else
        {
            painter->drawPoint(currentPoint);
        }
        previousPoint = currentPoint;
    }
}

MouseGestures::~MouseGestures()
{
    delete ui;
}
