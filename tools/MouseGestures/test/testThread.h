#pragma once
#include "abstractRecognizer.h"
#include "xmlparser.h"
#include <QThread>
#include "adopter.h"
#include <QtGui/QApplication>
#include <QDebug>

const QString pathToTestFile = "usersGestures.xml";

class TestThread : public QThread
{
    Q_OBJECT

private:
    GesturesManager * mGesturesManager;

signals:
    void tested(int, int, int);

public:
    TestThread(GesturesManager * gesturesManager, QObject * parent) :
            QThread(parent)
    {
        mGesturesManager = gesturesManager;
    }

    void run()
    {
        QMap<QString, UsersGestures> usersGestures = XmlParser::parseXml(pathToTestFile);
        QList<Entity> entities;
        int objectsNum = usersGestures.keys().size();
        foreach (QString key, usersGestures.keys())
        {
            PathVector path = usersGestures[key].first;
            entities.push_back(QPair<QString, PathVector>(key, path));
        }
        AbstractRecognizer * recognizer = new AbstractRecognizer(mGesturesManager, entities);
        int allGestures = 0;
        int recognizedGestures = 0;
        int checkedObjects = 0;
        foreach (QString object, usersGestures.keys())
        {

            foreach (QString pathStr, usersGestures[object].second)
            {
                QApplication::processEvents();
                allGestures ++;
                QList<QList<QPoint> > path = Parser::stringToPath(pathStr);
                QString recognizedObject = recognizer->recognizeObject(path);
                if (object == recognizedObject)
                    recognizedGestures ++;
                else
                    qDebug() << object << "recognized as" << recognizedObject;
            }
            checkedObjects ++;
            emit tested(100 * checkedObjects / objectsNum, allGestures,
                        recognizedGestures);
        }
    }
};
