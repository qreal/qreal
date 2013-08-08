#pragma once

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QVariant>
#include <QtCore/QFile>
#include <QtCore/QDir>

#include "../../qrkernel/ids.h"

namespace qrRepo {
namespace details {

class ValuesSerializer
{
public:
	static qReal::IdList deserializeIdList(QDomElement const &elem, QString const &name);
	static qReal::Id deserializeId(QString const &elementStr);
	static QVariant deserializeQVariant(QString const &typeName, QString const &valueStr);
	static QPointF deserializeQPointF(QString const &str);
	static void deserializeNamedVariantsMap(QMap<QString, QVariant> &map, QDomElement const &element);

	static QString serializeQVariant(QVariant const &v);
	static QString serializeQPointF(QPointF const &p);
	static QString serializeQPolygon(QPolygon const &p);
	static QDomElement serializeIdList(QString const &attributeName, qReal::IdList const &idList, QDomDocument &doc);
	static QDomElement serializeNamedVariantsMap(
			QString const &attributeName, QMap<QString, QVariant> const &map, QDomDocument &document);
};

}
}
