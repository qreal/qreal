#include "serializer.h"

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QPointF>
#include <QtGui/QPolygon>

#include "../../utils/outFile.h"
#include "../../utils/xmlUtils.h"

using namespace qrRepo;
using namespace details;
using namespace utils;
using namespace qReal;

Serializer::Serializer(QString const& saveDirName, bool failSafeMode)
	: mWorkingDir(saveDirName + "/save"), mFailSafe(failSafeMode)
{
}

void Serializer::clearWorkingDir() const
{
	clearDir(mWorkingDir);
}

void Serializer::removeFromDisk(Id id) const
{
	qDebug() << "deleteDiagramDir " << pathToElement(id);
	QDir dir;
	dir.remove(pathToElement(id));
}

void Serializer::setWorkingDir(QString const &workingDir)
{
	mWorkingDir = workingDir + "/save";
}

void Serializer::saveToDisk(QList<LogicObject*> const &objects) const
{
	foreach (LogicObject *object, objects) {
		qDebug() << "SAVED: " << object->id().toString();
		QString filePath = createDirectory(object->id());

		QDomDocument doc;
		QDomElement root = doc.createElement("LogicObject");
		doc.appendChild(root);
		root.setAttribute("id", object->id().toString());

		root.appendChild(idListToXml("parents", object->parents(), doc));
		root.appendChild(idListToXml("children", object->children(), doc));
		root.appendChild(propertiesToXml(object, doc));

		OutFile out(filePath);
		doc.save(out(), 2);
	}
}

void Serializer::loadFromDisk(QHash<qReal::Id, LogicObject*> &objectsHash)
{
	loadFromDisk(mWorkingDir, objectsHash);
}

void Serializer::loadFromDisk(QString const &currentPath, QHash<qReal::Id, LogicObject*> &objectsHash)
{
	QDir dir(currentPath);
	if (dir.exists()) {
		foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
			QString path = fileInfo.filePath();
			if (fileInfo.isDir())
				loadFromDisk(path, objectsHash);
			else if (fileInfo.isFile()) {
				QDomDocument doc = xmlUtils::loadDocument(path);
				LogicObject *object = parseLogicObject(doc.documentElement());
				Q_ASSERT(object);  // Пока требуем, что все объекты в репозитории загружаемы.
				if (object != NULL)
					objectsHash.insert(object->id(), object);
			}
		}
	}
}

LogicObject *Serializer::parseLogicObject(QDomElement const &elem)
{
	QString id = elem.attribute("id", "");
	if (id == "")
		return NULL;

	LogicObject object(Id::loadFromString(id));

	foreach (Id parent, loadIdList(elem, "parents"))
		if (!mFailSafe || !object.parents().contains(parent))
			object.addParent(parent);

	foreach (Id child, loadIdList(elem, "children"))
		if (!mFailSafe || !object.children().contains(child))
			object.addChild(child);

	if (!loadProperties(elem, object))
		return NULL;

	return new LogicObject(object);
}

bool Serializer::loadProperties(QDomElement const &elem, LogicObject &object)
{
	QDomNodeList propertiesList = elem.elementsByTagName("properties");
	if (propertiesList.count() != 1) {
		qDebug() << "Incorrect element: children list must appear once";
		return false;
	}

	QDomElement properties = propertiesList.at(0).toElement();
	QDomElement property = properties.firstChildElement();
	while (!property.isNull()) {
		if (property.hasAttribute("type")) {
			// Тогда это список. Немного кривовато, зато унифицировано со
			// списками детей/родителей.
			if (property.attribute("type", "") == "qReal::IdList") {
				QString key = property.tagName();
				IdList value = loadIdList(properties, property.tagName());
				object.setProperty(key, IdListHelper::toVariant(value));
			} else {
				Q_ASSERT(!"Unknown list type");
			}
		} else {
			QString type = property.tagName();
			QString key = property.attribute("key", "");
			if (key == "")
				return false;

			QString valueStr = property.attribute("value", "");
			QVariant value = parseValue(type, valueStr);
			object.setProperty(key, value);
		}
		property = property.nextSiblingElement();
	}
	return true;
}

IdList Serializer::loadIdList(QDomElement const &elem, QString const &name)
{
	QDomNodeList list = elem.elementsByTagName(name);
	if (list.count() != 1) {
		qDebug() << "Incorrect element: " + name + " list must appear once";
		return IdList();
	}

	IdList result;

	QDomElement elements = list.at(0).toElement();
	QDomElement element = elements.firstChildElement();
	while (!element.isNull()) {
		QString elementStr = element.attribute("id", "");
		if (elementStr == "") {
			qDebug() << "Incorrect Child XML node";
			return IdList();
		}
		result.append(Id::loadFromString(elementStr));
		element = element.nextSiblingElement();
	}
	return result;
}

QVariant Serializer::parseValue(QString const &typeName, QString const &valueStr)
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
	} else if (typeName.toLower() == "char") {
		return QVariant(valueStr[0]);
	} else if (typeName == "QPointF") {
		return QVariant(parsePointF(valueStr));
	} else if (typeName == "QPolygon") {
		QStringList const points = valueStr.split(" : ", QString::SkipEmptyParts);
		QPolygon result;
		foreach (QString str, points) {
			QPointF point = parsePointF(str);
			result << point.toPoint();
		}
		return QVariant(result);
	} else if (typeName == "qReal::Id") {
		return Id::loadFromString(valueStr).toVariant();
	} else {
		Q_ASSERT(!"Unknown property type");
		return QVariant();
	}
}

QPointF Serializer::parsePointF(QString const &str)
{
	double x = str.section(", ", 0, 0).toDouble();
	double y = str.section(", ", 1, 1).toDouble();
	return QPointF(x, y);
}

void Serializer::clearDir(QString const &path)
{
	QDir dir(path);
	if (dir.exists()) {
		foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
			if (fileInfo.isDir()) {
				clearDir(fileInfo.filePath());
				dir.rmdir(fileInfo.fileName());
			}
			else
				dir.remove(fileInfo.fileName());
		}
	}
}

QString Serializer::serializeQVariant(QVariant const &v)
{
	switch (v.type()) {
	case QVariant::Int:
		return QString("%1").arg(v.toInt());
	case QVariant::UInt:
		return QString("%1").arg(v.toUInt());
	case QVariant::Double:
		return QString("%1").arg(v.toDouble());
	case QVariant::Bool:
		return QString("%1").arg(v.toBool());
	case QVariant::String:
		return v.toString();
	case QVariant::Char:
		return v.toChar();
	case QVariant::PointF:
		return serializeQPointF(v.toPointF());
	case QVariant::Polygon:
		return serializeQPolygon(v.value<QPolygon>());
	case QVariant::UserType:
		if (v.userType() == QMetaType::type("qReal::Id"))
			return v.value<qReal::Id>().toString();
		// Если нет, идём в default и там ругаемся.
	default:
		qDebug() << v;
		Q_ASSERT(!"Unsupported QVariant type.");
		return "";
	}
}

QString Serializer::serializeQPointF(QPointF const &p)
{
	return QString("%1").arg(p.x()) + ", " + QString("%1").arg(p.y());
}

QString Serializer::serializeQPolygon(QPolygon const &p)
{
	QString result("");
	foreach (QPoint point, p) {
		result += serializeQPointF(point) + " : ";
	}
	return result;
}

QString Serializer::pathToElement(Id const &id) const
{
	QString dirName = mWorkingDir;

	QStringList partsList = id.toString().split('/');
	Q_ASSERT(partsList.size() >=1 && partsList.size() <= 5);
	for (int i = 1; i < partsList.size() - 1; ++i) {
		dirName += "/" + partsList[i];
	}

	return dirName + "/" + partsList[partsList.size() - 1];
}

QString Serializer::createDirectory(Id const &id) const
{
	QString dirName = mWorkingDir;
	QStringList partsList = id.toString().split('/');
	Q_ASSERT(partsList.size() >=1 && partsList.size() <= 5);
	for (int i = 1; i < partsList.size() - 1; ++i) {
		dirName += "/" + partsList[i];
	}

	QDir dir;
	dir.rmdir(mWorkingDir);
	dir.mkpath(dirName);

	return dirName + "/" + partsList[partsList.size() - 1];
}

QDomElement Serializer::idListToXml(QString const &attributeName, IdList const &idList, QDomDocument &doc)
{
	QDomElement result = doc.createElement(attributeName);
	foreach (Id id, idList) {
		QDomElement element = doc.createElement("object");
		element.setAttribute("id", id.toString());
		result.appendChild(element);
	}
	return result;
}

QDomElement Serializer::propertiesToXml(LogicObject* const object, QDomDocument &doc)
{
	QDomElement result = doc.createElement("properties");
	QMapIterator<QString, QVariant> i = object->propertiesIterator();
	while (i.hasNext()) {
		i.next();
		QString typeName = i.value().typeName();
		if (typeName == "qReal::IdList") {
			QDomElement list = idListToXml(i.key(), i.value().value<IdList>(), doc);
			list.setAttribute("type", "qReal::IdList");
			result.appendChild(list);
		} else {
			QDomElement property = doc.createElement(i.value().typeName());
			property.setAttribute("key", i.key());
			QString value = serializeQVariant(i.value());
			property.setAttribute("value", value);
			result.appendChild(property);
		}
	}
	return result;
}
