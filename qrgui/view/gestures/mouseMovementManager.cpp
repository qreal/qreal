#include "mouseMovementManager.h"
#include "pathCorrector.h"
#include "levenshteinDistance.h"
#include "GeometricForms.h"
#include "mixedgesturesmanager.h"

const QString comma = ", ";
const QString pointDelimeter = " : ";
const QString pathDelimeter = " | ";


MouseMovementManager::MouseMovementManager(QList<qReal::Id> elements, qReal::EditorManager * editorManager,
                                           GesturesPainterInterface *gesturesPaintManager)
{
    mGesturesManager = new MixedGesturesManager();
    mKeyManager = &mKeyStringManager;
    mEditorManager = editorManager;
    mGesturesPaintMan = gesturesPaintManager;
    setElements(elements);
}

void MouseMovementManager::setGesturesPainter(GesturesPainterInterface *gesturesPainter)
{
    mGesturesPaintMan = gesturesPainter;
}

void MouseMovementManager::drawIdealPath()
{
    QString currentElement = mGesturesPaintMan->currentElement();
    foreach (qReal::Id element, mElements)
    {
        if (element.element() == currentElement)
        {
            QString path = mEditorManager->mouseGesture(element);
            //mGesturesPaintMan->draw(stringToPath(path));
        }
    }
}

void MouseMovementManager::printElements()
{
    QList<QString> elements;
    foreach (qReal::Id element, mElements)
    {
        elements.push_back(element.element());
    }
    mGesturesPaintMan->setElements(elements);
}

void MouseMovementManager::clear()
{
    mPath.clear();
}

QLineF MouseMovementManager::newLine()
{
    QLineF line;
    if (mPath.back().size() > 1)
    {
        line.setP1(mPath.back().at(mPath.back().size() - 2));
        line.setP2(mPath.back().back());
    }
    return line;
}

void MouseMovementManager::setElements(const QList<qReal::Id> &elements)
{
    QMap<QString, PathVector> gestures;
    foreach (qReal::Id element, elements)
    {
        QString pathStr = mEditorManager->mouseGesture(element);
        if (!pathStr.isEmpty())
        {
            PathVector path = stringToPath(pathStr);
            gestures.insert(element.toString(), path);
            mElements.push_back(element);
        }
    }
    mGesturesManager->initIdealGestures(gestures);
}

void MouseMovementManager::recountCentre()
{
    if (mPath.empty() || mPath.back().empty())
        return;
    int count = 0;
    foreach (PointVector path, mPath)
        count += path.size();
    mCentre = ((count - 1) * mCentre + mPath.back().back()) / count;
}

void MouseMovementManager::mousePress(QPointF const & pnt)
{
    QList<QPointF> path;
    path.push_back(pnt);
    mPath.push_back(path);
    recountCentre();
}

void MouseMovementManager::mouseMove(QPointF const & pnt)
{
    PointVector path = mPath.back();
    mPath.pop_back();
    path.push_back(pnt);
    mPath.push_back(path);
    recountCentre();
}

QPointF MouseMovementManager::pos()
{
    return mCentre;
}

PathVector MouseMovementManager::stringToPath(QString const &valueStr)
{
    PathVector result;
    QStringList paths = valueStr.split(pathDelimeter, QString::SkipEmptyParts);
    foreach(QString pathStr, paths)
    {
        QStringList points = pathStr.split(pointDelimeter, QString::SkipEmptyParts);
        PointVector path;
        foreach (QString str, points)
        {
            QPointF point = parsePoint(str);
            path.push_back(point);
        }
        result.push_back(path);
    }
    return result;
}

QPoint MouseMovementManager::parsePoint(QString const &str)
{
    bool isInt;
    int x = str.section(comma, 0, 0).toInt(&isInt, 0);
    int y = str.section(comma, 1, 1).toInt(&isInt, 0);
    return QPoint(x, y);
}

qReal::Id MouseMovementManager::getObject()
{
    qDebug() << "try to recognize";
    qReal::Id recognizedObject;
    mGesturesManager->setKey(mPath);
    mPath.clear();
    double minDist = mGesturesManager->getMaxDistance(mElements.at(0).toString());
    foreach (qReal::Id object, mElements)
    {
        minDist = std::min(minDist, mGesturesManager->getMaxDistance(object.toString()));
        double dist = mGesturesManager->getDistance(object.toString());
        qDebug() << object << dist;
        if (dist < minDist)
        {
            minDist = dist;
            recognizedObject = object;
        }
    }
    qDebug() << "found object";
    return recognizedObject;
}

QPointF MouseMovementManager::firstPoint()
{
    if (!mPath.isEmpty() && !mPath.at(0).empty())
        return QPointF(mPath.at(0).at(0));
    return QPointF(0, 0);
}

QPointF MouseMovementManager::lastPoint()
{
    if (!mPath.isEmpty() && !mPath.back().empty())
        return QPointF(mPath.back().back());
    return QPointF(0, 0);
}

bool MouseMovementManager::wasMoving()
{
    return (firstPoint() != lastPoint() || mPath.size() > 1 ||
            (!mPath.isEmpty() && mPath.at(0).size() > 2));
}

bool MouseMovementManager::isEdgeCandidate()
{
    return mPath.count() <= 1;
}
