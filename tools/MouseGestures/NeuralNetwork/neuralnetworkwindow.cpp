#include "neuralnetworkwindow.h"
#include "ui_neuralnetworkwindow.h"
#include "QList"
#include "QFileDialog"
#include "adopter.h"
#include "xmlparser.h"
#include <QDebug>

//typedef QPair<QList<QPoint>, QList<QString> > UsersGestures;

NeuralNetworkWindow::NeuralNetworkWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::NeuralNetworkWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->bLearn, SIGNAL(pressed()), this, SLOT(learn()));
    QObject::connect(ui->bTest, SIGNAL(pressed()), this, SLOT(test()));
    ui->dspRadius->setValue(15.0);
    connect(ui->dspRadius, SIGNAL(valueChanged(double)), this, SLOT(setRad()));
    ui->pbLearn->setValue(0);
    ui->pbTest->setValue(0);
    mPaintManager = new PaintManager(ui->gVGestures);
}

NeuralNetworkWindow::~NeuralNetworkWindow()
{
    delete ui;
}

void NeuralNetworkWindow::clear()
{
    mNeuralNetwork.clear();
}

void NeuralNetworkWindow::learn()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open learn examples"),
                                                "learnGestures",
                                                tr("Xml files (*.xml)"));
    ui->pbLearn->setValue(0);
    ui->lvObjects->clear();
    QMap<QString, UsersGestures> gestures = XmlParser::parseXml(file);
    foreach (QString object, gestures.keys())
    {
        qDebug() << object;
        UsersGestures paths = gestures[object];
        mGestures.insert(object, Adopter::pathToString(paths.first));
        ui->lvObjects->addItem(object);
        mNeuralNetwork.learn(object, Adopter::getMousePath(paths.first));
        foreach (QString path, paths.second)
        {
            mNeuralNetwork.learn(object, Adopter::stringToPath(path));
        }
    }
    ui->pbLearn->setValue(100);
    connect(ui->lvObjects, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(paint()));
    qDebug() << "learnt";
}

void NeuralNetworkWindow::test()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open learn examples"),
                                                "testGestures",
                                                tr("Xml files (*.xml)"));
    QMap<QString, UsersGestures> gestures = XmlParser::parseXml(file);
    int recognizedGestures = 0;
    int allGestures = 0;
    ui->pbTest->setValue(0);
    int num = 0;
    foreach (QString object, gestures.keys())
    {
        UsersGestures paths = gestures[object];
        //qDebug() << object;
        foreach (QString path, paths.second)
        {
           // qDebug() << path;
            allGestures ++;
            //qDebug() << allGestures;
            QList<QPoint> mousePath = Adopter::stringToPath(path);
            //			mNeuralNetwork->recognize(mousePath);
            //			qDebug() << "returned";
            //QString recognizedObject(mNeuralNetwork->recognize(mousePath));
            ///qDebug() << mousePath.size() << " size" << object;

            if (mNeuralNetwork.recognize(mousePath) == object)
                recognizedGestures ++;
//            else
//                qDebug() << "not recognized";
//            qDebug() << "path worked" << allGestures <<paths.second.size();
        }
        num ++;
        ui->pbTest->setValue(10 * num);
    }
    ui->pbTest->setValue(100);
    ui->tbAll->setText(QString::number(allGestures));
    ui->tbRecognized->setText(QString::number(recognizedGestures));
    qDebug() << allGestures << recognizedGestures;
}

void NeuralNetworkWindow::setRad()
{
    double minRad = ui->dspRadius->value();
    mNeuralNetwork.setMinRad(minRad);
}

void NeuralNetworkWindow::paint()
{
    QString currentObject = ui->lvObjects->currentItem()->text();
    QString path = mGestures[currentObject];
    mPaintManager->drawGesture(path);
}
