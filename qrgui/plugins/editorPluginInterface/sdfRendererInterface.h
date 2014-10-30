#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QPainter>
#include <QtGui/QIconEngineV2>
#include <QtXml/QDomDocument>

#include "plugins/editorPluginInterface/elementRepoInterface.h"

namespace qReal {

class SdfRendererInterface : public QObject
{
public:
	virtual bool load(QString const &filename) = 0;
	virtual bool load(QDomDocument const &document) = 0;
	virtual void render(QPainter *painter, QRectF const &bounds, bool isIcon = false) = 0;
	virtual void setElementRepo(ElementRepoInterface *elementRepo) = 0;
};

// TODO: ???
class SdfIconEngineV2Interface : public QIconEngine
{
public:
};

}
