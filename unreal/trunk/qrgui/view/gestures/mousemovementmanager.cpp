#include "mousemovementmanager.h"
#include "pathcorrector.h"
#include "levenshteindistance.h"

const QString comma = ", ";
const QString pointDelimeter = " : ";

MouseMovementManager::MouseMovementManager(qReal::Id diagram, qReal::EditorManager editorManager)
{
    mKeyManager = &mKeyStringManager;
    mDiagram = &diagram;
    mEditorManager = &editorManager;
}

void MouseMovementManager::setDiagram(qReal::Id *diagram)
{
    mDiagram = diagram;
}

void MouseMovementManager::addPoint(const QPoint &point)
{
    mPath.push_back(point);
}

QList<QPoint> MouseMovementManager::stringToPath(QString const &valueStr)
{
    QStringList points = valueStr.split(pointDelimeter, QString::SkipEmptyParts);
    QList<QPoint> result;
    foreach (QString str, points)
    {
        QPoint point = parsePoint(str);
        result.push_back(point);
    }
    result = PathCorrector::getMousePath(result);
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
    const float e = 100;
    const float maxKeyDistance = 100;
    float min = e;
    float distance;
    qReal::Id id;
    //bool found = false;
    mPath = PathCorrector::correctPath(mPath);
    QString key = mKeyManager->getKey(mPath);
    if (key.isEmpty())
        return id;
    foreach (qReal::Id element, mEditorManager->elements(*mDiagram))
    {
        QString pathStr = mEditorManager->mouseGesture(element);
        if (!pathStr.isEmpty())
        {
            QString idealKey = mKeyManager->getKey(stringToPath(pathStr));
            distance = (float)(LevenshteinDistance::getLevenshteinDistance(idealKey, key) * e
                                / std::min(key.size(), idealKey.size()));
            if (distance < min && distance < maxKeyDistance)
            {
                min = distance;
                id = element;
                //found = true;
            }
        }
    }
    return id;
}
