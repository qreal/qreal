#pragma once

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QVariant>
#include <QtCore/QFile>
#include <QtCore/QDir>

#include "../../qrkernel/ids.h"

namespace qrRepo {
namespace details {

/// Utility class that provides methods to serialize/deserialize various kinds of primitive objects.
class ValuesSerializer
{
public:
	/// Serializes given QVariant to string.
	static QString serializeQVariant(QVariant const &v);

	/// Serializes given QPoint to string.
	static QString serializeQPointF(QPointF const &p);

	/// Serializes given QPolygon to string.
	static QString serializeQPolygon(QPolygon const &p);

	/// Serializes given IdList to a XML subtree.
	/// @param tagName - name of a root of resulting XML subtree.
	/// @param idList - a list to serialize.
	/// @param document - document to which will belong created subtree.
	static QDomElement serializeIdList(QString const &tagName, qReal::IdList const &idList, QDomDocument &document);

	/// Serializes given map from QString to QVariant to a XML subtree. Used to serialize various property maps.
	/// @param tagName - name of a root of resulting XML subtree.
	/// @param map - a map to serialize.
	/// @param document - document to which will belong created subtree.
	static QDomElement serializeNamedVariantsMap(
			QString const &tagName, QMap<QString, QVariant> const &map, QDomDocument &document);

	/// Deserializes IdList from XML subtree.
	/// @param elem - XML subtree which contains a list being deserialized, so this parameter shall be a parent to
	///        a root of a list.
	static qReal::IdList deserializeIdList(QDomElement const &elem, QString const &name);

	/// Loads Id from a string with correct processing of empty strings.
	/// @returns Id(), if a string is empty, or loaded id. Throws exception, if id can not be loaded.
	static qReal::Id deserializeId(QString const &elementStr);

	/// Deserializes QVariant by given type name from string.
	/// @param typeName - value of what type shall be contained in a string.
	/// @param valueStr - string to be deserialized.
	static QVariant deserializeQVariant(QString const &typeName, QString const &valueStr);

	/// Deserializes QPointF from string.
	static QPointF deserializeQPointF(QString const &str);

	/// Deserializes map from QString to QVariant from XML subtree. Used to deserialize property maps.
	/// @param map - a map to put deserialized values to.
	/// @param element - XML DOM subtree to deserialize.
	static void deserializeNamedVariantsMap(QMap<QString, QVariant> &map, QDomElement const &element);

private:
	/// Creating is prohibited, utility class instances can not be created.
	ValuesSerializer();
};

}
}
