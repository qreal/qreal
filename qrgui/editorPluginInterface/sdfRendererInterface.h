#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QPainter>
#include <QtGui/QIconEngineV2>

class SdfRendererInterface : public QObject
{
public:
	virtual bool load (QString const &filename) = 0;
	virtual void render(QPainter *painter, QRectF const &bounds) = 0;
};

class SdfIconEngineV2Interface : public QIconEngine
{
public:
};
