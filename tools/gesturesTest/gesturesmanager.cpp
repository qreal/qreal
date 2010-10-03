#include "gesturesmanager.h"
#include "common/levenshteindistance.h"
#include "rectangleAlgorithm/pathcorrector.h"
#include "rectangleAlgorithm/keyBuilder.h"
#include "chaosStarAlgorithm/key8manager.h"

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
		this->mRectIdealKeys.push_back(objKey);

		Key8Manager key8Manager;
		objKey = qMakePair(gesture.first, key8Manager.getKey(mousePath));
		this->mChaosIdealKeys.push_back(objKey);

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
	MouseMovementManager mouseMovementManager;
	QString const key = mouseMovementManager.getKey(path);

	return findNearestIdealObject(key, mRectIdealKeys);
}

QString GesturesManager::chaosRecognize(const QList<QPoint> &path)
{
	Key8Manager manager;
	QString const key = manager.getKey(path);

	return findNearestIdealObject(key, mChaosIdealKeys);
}

QString GesturesManager::findNearestIdealObject(QString const &key, QList<ObjectKey> const &idealKeys)
{
	QString object = "";
	if (key.isEmpty())
		return object;

	float const e = 100;
	float const maxKeyDistance = 100;
	float min = e;
	foreach (ObjectKey item, idealKeys) {
		if (!item.second.isEmpty()) {
			float const distance = static_cast<float>(LevenshteinDistance::getLevenshteinDistance(item.second, key)
					* e / qMin(key.size(), item.second.size()));
			if (distance < min && distance < maxKeyDistance) {
				min = distance;
				object = item.first;
			}
		}
	}
	return object;
}
