#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QPainter>
#include <QtGui/QIconEngineV2>
#include <QtXml/QDomDocument>

#include "qrgui/plugins/editorPluginInterface/elementRepoInterface.h"

namespace qReal {

class SdfRendererInterface : public QObject
{
public:
	virtual bool load(const QString &filename) = 0;
	virtual bool load(const QDomDocument &document) = 0;
	virtual void render(QPainter *painter, const QRectF &bounds, bool isIcon = false) = 0;
	virtual void setElementRepo(ElementRepoInterface *elementRepo) = 0;
};

// TODO: ???
class SdfIconEngineV2Interface : public QIconEngine
{
public:
};

}
