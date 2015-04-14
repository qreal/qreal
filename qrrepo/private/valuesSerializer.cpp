/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "valuesSerializer.h"

#include <qrkernel/exception/exception.h>

#include <QtCore/QPointF>
#include <QtGui/QPolygon>

using namespace qReal;
using namespace qrRepo::details;

IdList ValuesSerializer::deserializeIdList(const QDomElement &elem, const QString &name)
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

Id ValuesSerializer::deserializeId(const QString &elementStr)
{
	return elementStr.isEmpty() ? Id() : Id::loadFromString(elementStr);
}

QVariant ValuesSerializer::deserializeQVariant(const QString &typeName, const QString &valueStr)
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
		const QStringList points = valueStr.split(" : ", QString::SkipEmptyParts);
		QPolygon polygonResult;
		QPolygonF polygonFResult;
		foreach (const QString &str, points) {
			const QPointF point = deserializeQPointF(str);
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

QPointF ValuesSerializer::deserializeQPointF(const QString &str)
{
	qreal x = str.section(", ", 0, 0).toDouble();
	qreal y = str.section(", ", 1, 1).toDouble();
	return QPointF(x, y);
}

QString ValuesSerializer::serializeQVariant(const QVariant &v)
{
	switch (v.type()) {
	case QVariant::Int:
		return QString::number(v.toInt());
	case QVariant::UInt:
		return QString::number(v.toUInt());
	case QVariant::Double:
		return QString::number(v.toDouble());
	case QVariant::Bool:
		return QString("%1").arg(v.toBool() ? "true" : "false");
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

QString ValuesSerializer::serializeQPointF(const QPointF &p)
{
	return QString::number(p.x()) + ", " + QString::number(p.y());
}

QString ValuesSerializer::serializeQPolygonF(const QPolygonF &p)
{
	QString result("");
	foreach (const QPointF &point, p) {
		result += serializeQPointF(point) + " : ";
	}

	return result;
}

QDomElement ValuesSerializer::serializeIdList(const QString &tagName, const IdList &idList, QDomDocument &document)
{
	QDomElement result = document.createElement(tagName);
	foreach (Id id, idList) {
		QDomElement element = document.createElement("object");
		element.setAttribute("id", id.toString());
		result.appendChild(element);
	}

	return result;
}

QDomElement ValuesSerializer::serializeNamedVariantsMap(const QString &tagName, QMap<QString, QVariant> const &map
		, QDomDocument &document)
{
	QDomElement result = document.createElement(tagName);

	for (QMap<QString, QVariant>::const_iterator i = map.constBegin(); i != map.constEnd(); ++i) {
		const QString typeName = i.value().typeName();
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

void ValuesSerializer::deserializeNamedVariantsMap(QMap<QString, QVariant> &map, const QDomElement &element)
{
	for (QDomElement property = element.firstChildElement();
			!property.isNull();
			property = property.nextSiblingElement())
	{
		if (property.hasAttribute("type")) {
			if (property.attribute("type") == "qReal::IdList") {
				const QString key = property.tagName();
				const IdList value = ValuesSerializer::deserializeIdList(element, property.tagName());
				map.insert(key, IdListHelper::toVariant(value));
			} else {
				throw Exception("Unknown list type");
			}
		} else {
			const QString type = property.tagName();
			const QString key = property.attribute("key");
			if (key.isEmpty()) {
				throw Exception("Missing property name");
			}

			const QString valueStr = property.attribute("value", "");
			const QVariant value = ValuesSerializer::deserializeQVariant(type, valueStr);
			map.insert(key, value);
		}
	}
}
