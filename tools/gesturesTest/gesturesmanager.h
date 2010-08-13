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
	QList<QPoint> getPath(const QString & name);

private:
	QList<ObjectKey> mIdealKeys;
	QList<GestureObject> mGestures;
	MouseGestureRecognizer * mouseGestureRecognizer;
};
