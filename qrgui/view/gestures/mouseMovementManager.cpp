#include "mouseMovementManager.h"
#include "pathCorrector.h"
#include "levenshteinDistance.h"

const QString comma = ", ";
const QString pointDelimeter = " : ";

MouseMovementManager::MouseMovementManager(QList<qReal::Id> elements, qReal::EditorManager * editorManager,
										   GesturesPainterInterface *gesturesPaintManager)
{
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
	foreach (qReal::Id element, mGestures.values())
	{
		if (element.element() == currentElement)
		{
			QString path = mEditorManager->mouseGesture(element);
			mGesturesPaintMan->draw(stringToPath(path));
		}
	}
}

void MouseMovementManager::printElements()
{
	QList<QString> elements;
	foreach (qReal::Id element, mGestures.values())
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
	if (mPath.size() > 1)
	{
		line.setP1(mPath[mPath.size() - 2]);
		line.setP2(mPath.back());
	}
	return line;
}

void MouseMovementManager::createMap()
{
	mGestures.clear();
	foreach (qReal::Id element, mElements)
	{
		QString pathStr = mEditorManager->mouseGesture(element);
		if (!pathStr.isEmpty())
		{
			QList<QPoint> path = stringToPath(pathStr);
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
	mCentre = ((mPath.count() * mCentre + point) / (mPath.count() + 1));
	mPath.push_back(QPoint((int)point.x(), (int)point.y()));
}

QPointF MouseMovementManager::pos()
{
	return mCentre;
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

QPointF MouseMovementManager::firstPoint()
{
	if (!mPath.isEmpty())
		return QPointF(mPath[0]);
	return QPointF(0, 0);
}

QPointF MouseMovementManager::lastPoint()
{
	if (!mPath.isEmpty())
		return QPointF(mPath.back());
	return QPointF(0, 0);
}

bool MouseMovementManager::wasMoving()
{
	return (firstPoint() != lastPoint() || mPath.size() > 2);
}
