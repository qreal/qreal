#include "gesturesmanager.h"
#include "rectangleAlgorithm/levenshteindistance.h"
#include "rectangleAlgorithm/pathcorrector.h"
#include "rectangleAlgorithm/keyBuilder.h"

GesturesManager::GesturesManager()
{
	mouseGestureRecognizer = new MouseGestureRecognizer();
}

QList<QPoint> GesturesManager::getPath(const QString &name)
{
	foreach (GestureObject gesture, mGestures)
	{
		if (gesture.first == name)
		{
			return gesture.second;
		}
	}
	return QList<QPoint>();
}

void GesturesManager::setIdealGestres(const QList<GestureObject> &gestures)
{
	mGestures = gestures;
	foreach (GestureObject gesture, gestures)
	{
		QList<QPoint> path = gesture.second;
		QList<QPoint> mousePath = PathCorrector::getMousePath(path);
		MouseMovementManager mouseMovementManager;
		QPair<QString, QString> objKey(
				gesture.first, mouseMovementManager.getKey(mousePath));
		this->mIdealKeys.push_back(objKey);
		mouseGestureRecognizer->addGestureDefinition(gesture.first, path);
	}
}

QString GesturesManager::qtRecognize(const QList<QPoint> &path)
{
	return mouseGestureRecognizer->recognizeGesture(path);
}

//todo::перенести распознавание в другой класс
QString GesturesManager::rectRecognize(const QList<QPoint> &path)
{
	const float e = 100;
	const float maxKeyDistance = 100;
	float min = e;
	float distance;
	MouseMovementManager mouseMovementManager;
	QString key = mouseMovementManager.getKey(path);
	QString object = "";
	if (key.isEmpty())
		return object;
	foreach (ObjectKey item, this->mIdealKeys)
	{
		if (!item.second.isEmpty())
		{
			distance = (float)(LevenshteinDistance::getLevenshteinDistance(item.second, key)
							   * e / std::min(key.size(), item.second.size()));
			if (distance < min && distance < maxKeyDistance)
			{
				min = distance;
				object = item.first;
			}
		}
	}
	return object;
}
