#include "adopter.h"
#include "serializer.h"
#include "mousegestures.h"
#include "ui_mousegestures.h"
#include "pathcorrector.h"
#include "validpathcreator.h"
#include "GeometricForms.h"
#include "paintmanager.h"
#include "multistrokeRecognizers/multistrokeGesturesManagers.h"
#include "test/xmlparser.h"


static const QString xmlDir = "../../unreal/trunk/qrxml";
static const QString idealGesturesFile =
        "NeuralNetwork/learnGestures/ideal_gestures.xml";
static const QString generatedGesturesFile = "NeuralNetwork/learnGestures/generated_gestures.xml";
static const QString testGesturesFile = "usersGestures.xml";

MouseGestures::MouseGestures(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MouseGestures)
{
    ui->setupUi(this);
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(loadFile()));
    connect(ui->twObjectPathTable, SIGNAL
            (currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)), this, SLOT(drawGesture()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionTest, SIGNAL(triggered()), this, SLOT(chooseTestAlgorithm()));
    connect(ui->actionGenerate_gestures, SIGNAL(triggered()), this, SLOT(generateGestures()));
    connect(ui->actionSave_test_gestures, SIGNAL(triggered()), this, SLOT(saveTestGestures()));
    connect(ui->actionOpen_test_gestures, SIGNAL(triggered()), this, SLOT(openTestGestures()));
    mPaintManager = new PaintManager(ui->gvGesture);
    mGesturesManager = new LevenshteinHullGesturesManager();
    mRecognizer = new AbstractRecognizer(mGesturesManager);
}

void MouseGestures::openTestGestures()
{
    QMap<QString, UsersGestures> usersGestures = XmlParser::parseXml(testGesturesFile);
    mRecognizer = new AbstractRecognizer(mGesturesManager, usersGestures);
    showTable();
}

void MouseGestures::saveTestGestures()
{
    XmlParser::saveMore(this->mRecognizer->getAllGestures(), testGesturesFile);
}

void MouseGestures::generateGestures()
{
    //TODO:: change idealGestures
    QMap<QString, UsersGestures> usersGestures = XmlParser::parseXml(idealGesturesFile);
    QMap<QString, UsersGestures> generatedGestures;
    foreach (QString object, usersGestures.keys())
    {
        const int maxAngle = 45;
        const int angleDiff = 10;
        PathVector idealGesture = usersGestures[object].first;
        QList<QString> newGestures;
        qDebug() << object;
        for (int i = - maxAngle; i <= maxAngle; i += angleDiff)
        {
            int maxSecondAngle = maxAngle - abs(i);
            for (int j = - maxSecondAngle; j <= maxSecondAngle; j += angleDiff)
            {
                PathVector gesture = PathCorrector::distortGesture(idealGesture, i, j);
                //TODO:: change for incoherent gestures
                newGestures.push_back(Parser::pathToString(gesture));
            }
        }
        UsersGestures generatedGesture(usersGestures[object].first, newGestures);
        generatedGestures.insert(object, generatedGesture);
    }
    XmlParser::save(generatedGestures, generatedGesturesFile);
}

void MouseGestures::chooseTestAlgorithm()
{
    mTestWindow = new TestWindow(this);
    mTestWindow->show();
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
    //    QString pathStr = ui->twObjectPathTable->currentItem()->text();
    //    QList<QPoint> path = Parser::stringToPath(pathStr);
    //    path = PathCorrector::rotate(path);
    //    ui->twObjectPathTable->currentItem()->setText(Parser::pathToString(path));
    //    //TODO:: add changing path for single-stroke gestures
}

void MouseGestures::increasePath(double koef)
{
    //    QString pathStr = ui->twObjectPathTable->currentItem()->text();
    //    QList<QPoint> path = Parser::stringToPath(pathStr);
    //    path = PathCorrector::increase(path, koef);
    //    ui->twObjectPathTable->currentItem()->setText(Parser::pathToString(path));
    //    //TODO:: add changing path for single-stroke gestures
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
    Objects objects;
    Serializer serializer(mFileName);
    serializer.serialize(objects);
}

void MouseGestures::changePath()
{
    //	int currentRow = ui->twObjectPathTable->currentRow();
    //	QString name = ui->twObjectPathTable->item(currentRow, objectColumn)->text();
    //	QString pathStr = ui->twObjectPathTable->item(currentRow, pathColumn)->text();
    //	QList<QPoint> path = Parser::stringToPath(pathStr);
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
    //mGesturesManager = new LevenshteinHullGesturesManager();
    mRecognizer = new AbstractRecognizer(mGesturesManager, entities);
    //	addEntities(entities);
    showTable();
    //	this->mKeyBulder = new MultistrokeGesturesManager(entities);
}

void MouseGestures::showTable()
{
    this->disconnect(ui->twObjectPathTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(changePath()));
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
    if (event->key() != Qt::Key_Return)
        return;
    QString object = mRecognizer->recognizeObject();
    if (ui->twObjectPathTable->rowCount() != 0)
    {
        QTableWidgetItem * currentItem = ui->twObjectPathTable->currentItem();
        if (currentItem != NULL)
            mRecognizer->saveGesture(currentItem->text());
    }
    //showObjectsMenu(object);
    //mCorrectPath = PathCorrector::correctPath(mMousePath);
    ui->teObject->setText(object);
    this->update();
}

void MouseGestures::showObjectsMenu(QString const & object)
{
    QMenu * menu = new QMenu(this);
    QList<QString> objects = mRecognizer->getObjects();
    objects.removeAll(object);
    objects.push_front(object);
    foreach (QString object, objects)
    {
        QAction * action = new QAction(object, this);
        menu->addAction(action);
    }
    connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(addTestGesture(QAction *)));
    menu->exec();
}

void MouseGestures::addTestGesture(QAction * action)
{
    mRecognizer->saveGesture(action->text());
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
