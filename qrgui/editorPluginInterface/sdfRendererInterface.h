#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QPainter>
#include <QtGui/QIconEngineV2>
#include <QtXml/QDomDocument>

class SdfRendererInterface : public QObject
{
public:
	virtual bool load (const QString &filename) = 0;
	virtual bool load(QDomDocument &document) = 0;
	virtual void render(QPainter *painter, const QRectF &bounds) = 0;
};

class SdfIconEngineV2Interface : public QIconEngineV2
{
public:
};
