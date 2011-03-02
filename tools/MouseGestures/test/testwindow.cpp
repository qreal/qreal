#include "testwindow.h"
#include "ui_testwindow.h"
#include "multistrokeRecognizers/multistrokeGesturesManagers.h"
#include "xmlparser.h"
#include "adopter.h"
#include "NeuralNetwork/neuralNetwork.h"
#include <QFileDialog>

const QString pathToTestFile = "usersGestures.xml";
const QString pathToTrainingFile = "NeuralNetwork/learnGestures/learn_gestures1.xml";
const QString levXYSortAlgorithm = "levenshtein distance square and sorting by XY coordinates";
const QString levDistHullAlgorithm = "levenshtein distance and hull sorting";
const QString sortXYAlgorithm = "one size keys and sorting by XY";
const QString hullSortAlgorithm = "one size keys and hull sorting";
const QString squaresCurvesAlgorithm = "distance between nearest squares";
const QString trainingGesturesManagerAlgorithm = "training algorithm";

TestWindow::TestWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    ui->cbAlgorithm->addItem(squaresCurvesAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(trainingGesturesManagerAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(levXYSortAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(levDistHullAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(sortXYAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(hullSortAlgorithm, QVariant());
    connect(ui->bTest, SIGNAL(clicked()), this, SLOT(test()));
    ui->pbTested->setValue(0);
}

void TestWindow::test()
{
    ui->pbTested->setValue(0);
    GesturesManager * gesturesManager = getGesturesManager();
    QMap<QString, UsersGestures> usersGestures = XmlParser::parseXml(pathToTestFile);
    QList<Entity> entities;
    int objectsNum = usersGestures.keys().size();
    foreach (QString key, usersGestures.keys())
    {
        PathVector path;
        path.push_back(usersGestures[key].first);
        entities.push_back(QPair<QString, PathVector>(key, path));
    }
    AbstractRecognizer * recognizer = new AbstractRecognizer(gesturesManager, entities);
    int all = 0;
    int recognized = 0;
    int notRecognized = 0;
    int checkedObjects = 0;
    foreach (QString object, usersGestures.keys())
    {
        foreach (QString pathStr, usersGestures[object].second)
        {
            all ++;
            QList<QPoint> path = Parser::stringToPath(pathStr);
            if (object == recognizer->recognizeObject(path))
                recognized ++;
            else
                notRecognized ++;
        }
        checkedObjects ++;
        ui->pbTested->setValue(100 * checkedObjects / objectsNum);
    }
    ui->teAll->setText(QString::number(all));
    ui->teRecognized->setText(QString::number(recognized));
    ui->teNotRecognized->setText(QString::number(notRecognized));
}

GesturesManager * TestWindow::getGesturesManager()
{
    //TODO:: add new thread
    QString name = ui->cbAlgorithm->currentText();
    if (name ==  levXYSortAlgorithm)
        return new LevenshteinXYSortGesturesManager();
    else if (name == levDistHullAlgorithm)
        return new LevenshteinHullGesturesManager();
    else if (name == sortXYAlgorithm)
        return new OneSizeXYSortGesturesManager();
    else if (name == squaresCurvesAlgorithm)
        return new SquaresCurveGesturesManager();
    else if (name == trainingGesturesManagerAlgorithm)
    {
        TrainingGesturesManager<GridClassifier> * trainingGesturesManager = new TrainingGesturesManager<GridClassifier>();
        QMap<QString, UsersGestures> gestures = XmlParser::parseXml(pathToTrainingFile);
        foreach (QString object, gestures.keys())
        {
            UsersGestures paths = gestures[object];
            foreach (QString path, paths.second)
            {
                qDebug() << "new gesture";
                trainingGesturesManager->learn(object, Parser::stringToPath(path));
            }
        }
        qDebug() << "learnt";
        return trainingGesturesManager;
    }
    else return new OneSizeHullGesturesManager();
}


TestWindow::~TestWindow()
{
    delete ui;
}
