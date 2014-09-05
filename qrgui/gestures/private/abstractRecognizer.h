#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtWidgets/QWidget>
#include <QtGui/QMouseEvent>

#include <qrkernel/ids.h>

#include "geometricForms.h"
#include "pathCorrector.h"

namespace qReal {
namespace gestures {

class GesturesManager
{
public:
	virtual ~GesturesManager() {}
	virtual void initIdealGestures(QMap<QString, PathVector> const &objects) = 0;
	virtual void setKey(const PathVector &path) = 0;
	virtual double getMaxDistance(QString const &object) = 0;
	virtual double getDistance(QString const &object) = 0;
	virtual bool isMultistroke() = 0;
};

template <typename TKey>
class GesturesRecognizer : public GesturesManager
{
public:
	GesturesRecognizer(){}

	virtual ~GesturesRecognizer() {}

	qreal getDistance(QString const &item)
	{
		TKey key = mGestures[item];
		return getDistance(mKey, key);
	}

	void initIdealGestures(QMap<QString, PathVector> const &objects)
	{
		for (QString const &object : objects.keys()) {
			TKey key = getKey(objects[object]);
			mGestures.insert(object, key);
		}
	}

	void setKey(PathVector const &path)
	{
		mKey = getKey(path);
	}

	virtual double getMaxDistance(QString const &object) = 0;

protected:
	TKey mKey;
	virtual double getDistance(TKey const &key1, TKey const &key2) = 0;
	virtual TKey getKey(PathVector const &path) = 0;
	QMap<QString, TKey> mGestures;
	//maybe to do several lists for multistroke gestures
};

}
}
