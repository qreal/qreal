#include "mousemovementmanager.h"
#include "pathcorrector.h"
#include "levenshteindistance.h"

const QString comma = ", ";
const QString pointDelimeter = " : ";

MouseMovementManager::MouseMovementManager(QList<qReal::Id> elements, qReal::EditorManager * editorManager)
{
    mKeyManager = &mKeyStringManager;
    mEditorManager = editorManager;
    setElements(elements);
}

void MouseMovementManager::clear()
{
    mPath.clear();
}

void MouseMovementManager::createMap()
{
    mGestures.clear();
    foreach (qReal::Id element, mElements)
    {
        QString pathStr = mEditorManager->mouseGesture(element);
        if (!pathStr.isEmpty())
        {
            QList<QPointF> path = stringToPath(pathStr);
            QString key = mKeyManager->getKey(path);
            mGestures.insert(key, element);
        }
    }
}

void MouseMovementManager::setElements(const QList<qReal::Id> &elements)
{
    mElements = elements;
    createMap();
}

void MouseMovementManager::addPoint(const QPointF &point)
{
    mPath.push_back(point);
}

QList<QPointF> MouseMovementManager::stringToPath(QString const &valueStr)
{
    QStringList points = valueStr.split(pointDelimeter, QString::SkipEmptyParts);
    QList<QPointF> result;
    foreach (QString str, points)
    {
        QPointF point = parsePoint(str);
        result.push_back(point);
    }
    result = PathCorrector::getMousePath(result);
    return result;
}

QPointF MouseMovementManager::parsePoint(QString const &str)
{
    bool isInt;
    int x = str.section(comma, 0, 0).toInt(&isInt, 0);
    int y = str.section(comma, 1, 1).toInt(&isInt, 0);
    return QPointF(x, y);
}

qReal::Id MouseMovementManager::getObject()
{
    const float e = 100;
    const float maxKeyDistance = 100;
    float min = e;
    float distance;
    qReal::Id id;
    mPath = PathCorrector::correctPath(mPath);
    QString key = mKeyManager->getKey(mPath);
    if (key.isEmpty())
        return id;
    foreach (QString idealKey, mGestures.keys())
    {
        distance = (float)(LevenshteinDistance::getLevenshteinDistance(idealKey, key) * e
                           / std::min(key.size(), idealKey.size()));
        if (distance < min && distance < maxKeyDistance)
        {
            min = distance;
            id = mGestures[idealKey];
        }
    }
    return id;
}
