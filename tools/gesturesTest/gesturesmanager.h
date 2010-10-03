#pragma once
#include "qtAlgorithm/mousegesturerecognizer.h"
#include <QPair>
#include <QPoint>

typedef QPair<QString, QList<QPoint> > GestureObject;
typedef QPair<QString, QString> ObjectKey;

class GesturesManager
{
public:
	GesturesManager();
	void setIdealGestres(QList<GestureObject> const & gestures);
	QString rectRecognize(QList<QPoint> const & path);
	QString qtRecognize(QList<QPoint> const & path);
	QString chaosRecognize(const QList<QPoint> &path);
	QList<QPoint> getPath(const QString & name);

private:
	QList<ObjectKey> mRectIdealKeys;
	QList<ObjectKey> mChaosIdealKeys;
	QList<GestureObject> mGestures;
	MouseGestureRecognizer *mouseGestureRecognizer;

	static QString findNearestIdealObject(QString const &key, QList<ObjectKey> const &idealKeys);
};
