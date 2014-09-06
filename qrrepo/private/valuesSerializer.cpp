#include "valuesSerializer.h"

#include "../../qrkernel/exception/exception.h"

#include <QtCore/QPointF>
#include <QtGui/QPolygon>

using namespace qReal;
using namespace qrRepo::details;

IdList ValuesSerializer::deserializeIdList(QDomElement const &elem, QString const &name)
{
	QDomNodeList list = elem.elementsByTagName(name);
	if (list.count() != 1) {
		qDebug() << "Incorrect element: " + name + " list must appear once";
		return IdList();
	}

	IdList result;

	QDomElement elements = list.at(0).toElement();

	for (QDomElement element = elements.firstChildElement();
			!element.isNull();
			element = element.nextSiblingElement())
	{
		QString elementStr = element.attribute("id", "");
		if (elementStr == "") {
			qDebug() << "Incorrect Child XML node";
			return IdList();
		}

		result.append(Id::loadFromString(elementStr));
	}
	return result;
}

Id ValuesSerializer::deserializeId(QString const &elementStr)
{
	return elementStr.isEmpty() ? Id() : Id::loadFromString(elementStr);
}

QVariant ValuesSerializer::deserializeQVariant(QString const &typeName, QString const &valueStr)
{
	if (typeName.toLower() == "int") {
		return QVariant(valueStr.toInt());
	} else if (typeName.toLower() == "uint") {
		return QVariant(valueStr.toUInt());
	} else if (typeName.toLower() == "double") {
		return QVariant(valueStr.toDouble());
	} else if (typeName.toLower() == "bool") {
		return QVariant(valueStr.toLower() == "true");
	} else if (typeName == "QString") {
		return QVariant(valueStr);
	} else if (typeName == "QStringList") {
		return QVariant(valueStr.split(',', QString::SkipEmptyParts));
	} else if (typeName.toLower() == "char") {
		return QVariant(valueStr[0]);
	} else if (typeName == "QPointF") {
		return QVariant(deserializeQPointF(valueStr));
	} else if (typeName == "QPolygon" || typeName == "QPolygonF") {
		QStringList const points = valueStr.split(" : ", QString::SkipEmptyParts);
		QPolygon polygonResult;
		QPolygonF polygonFResult;
		foreach (QString const &str, points) {
			QPointF const point = deserializeQPointF(str);
			polygonFResult << point;
			polygonResult << point.toPoint();
		}

		return typeName == "QPolygon" ? QVariant(polygonResult) : QVariant(polygonFResult);
	} else if (typeName == "qReal::Id") {
		return Id::loadFromString(valueStr).toVariant();
	} else {
		Q_ASSERT(!"Unknown property type");
		return QVariant();
	}
}

QPointF ValuesSerializer::deserializeQPointF(QString const &str)
{
	qreal x = str.section(", ", 0, 0).toDouble();
	qreal y = str.section(", ", 1, 1).toDouble();
	return QPointF(x, y);
}

QString ValuesSerializer::serializeQVariant(QVariant const &v)
{
	switch (v.type()) {
	case QVariant::Int:
		return QString::number(v.toInt());
	case QVariant::UInt:
		return QString::number(v.toUInt());
	case QVariant::Double:
		return QString::number(v.toDouble());
	case QVariant::Bool:
		return QString("%1").arg(v.toBool());
	case QVariant::String:
		return v.toString();
	case QVariant::StringList:
		return v.toStringList().join(',');
	case QVariant::Char:
		return v.toChar();
	case QVariant::PointF:
		return serializeQPointF(v.toPointF());
	case QVariant::Polygon:
		return serializeQPolygonF(v.value<QPolygon>());
	case QVariant::PolygonF:
		return serializeQPolygonF(v.value<QPolygonF>());
	case QVariant::UserType:
		if (v.userType() == QMetaType::type("qReal::Id")) {
			return v.value<qReal::Id>().toString();
		}
		// something bad
	default:
		qDebug() << v;
		Q_ASSERT(!"Unsupported QVariant type.");
		return "";
	}
}

QString ValuesSerializer::serializeQPointF(QPointF const &p)
{
	return QString::number(p.x()) + ", " + QString::number(p.y());
}

QString ValuesSerializer::serializeQPolygonF(QPolygonF const &p)
{
	QString result("");
	foreach (QPointF const &point, p) {
		result += serializeQPointF(point) + " : ";
	}

	return result;
}

QDomElement ValuesSerializer::serializeIdList(QString const &tagName, IdList const &idList, QDomDocument &document)
{
	QDomElement result = document.createElement(tagName);
	foreach (Id id, idList) {
		QDomElement element = document.createElement("object");
		element.setAttribute("id", id.toString());
		result.appendChild(element);
	}

	return result;
}

QDomElement ValuesSerializer::serializeNamedVariantsMap(QString const &tagName, QMap<QString, QVariant> const &map, QDomDocument &document)
{
	QDomElement result = document.createElement(tagName);

	for (QMap<QString, QVariant>::const_iterator i = map.constBegin(); i != map.constEnd(); ++i) {
		QString const typeName = i.value().typeName();
		if (typeName == "qReal::IdList") {
			QDomElement list = ValuesSerializer::serializeIdList(i.key(), i.value().value<IdList>(), document);
			list.setAttribute("type", "qReal::IdList");
			result.appendChild(list);
		} else {
			QDomElement property = document.createElement(i.value().typeName());
			property.setAttribute("key", i.key());
			QString value = ValuesSerializer::serializeQVariant(i.value());
			property.setAttribute("value", value);
			result.appendChild(property);
		}
	}

	return result;
}

void ValuesSerializer::deserializeNamedVariantsMap(QMap<QString, QVariant> &map, QDomElement const &element)
{
	for (QDomElement property = element.firstChildElement();
			!property.isNull();
			property = property.nextSiblingElement())
	{
		if (property.hasAttribute("type")) {
			if (property.attribute("type") == "qReal::IdList") {
				QString const key = property.tagName();
				IdList const value = ValuesSerializer::deserializeIdList(element, property.tagName());
				map.insert(key, IdListHelper::toVariant(value));
			} else {
				throw Exception("Unknown list type");
			}
		} else {
			QString const type = property.tagName();
			QString const key = property.attribute("key");
			if (key.isEmpty()) {
				throw Exception("Missing property name");
			}

			QString const valueStr = property.attribute("value", "");
			QVariant const value = ValuesSerializer::deserializeQVariant(type, valueStr);
			map.insert(key, value);
		}
	}
}
