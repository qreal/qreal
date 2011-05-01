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

Serializer::Serializer(QString const& saveDirName, ExternalClient client)
	: mWorkingDir(saveDirName + "/save"), mExternalClient(client)

{
}

void Serializer::removeFromDisk(Id id) const
{
	QDir dir;
	dir.remove(pathToElement(id));
}

void Serializer::setWorkingDir(QString const &workingDir)
{
	mWorkingDir = workingDir + "/save";
}

void Serializer::clearWorkingDir()
{
	clearDir(mWorkingDir+"/tree");
}

bool Serializer::saveToDisk(QList<Object*> const &objects)
{
	bool result = true;
	mSavedDirectories.clear();
	mSavedFiles.clear();
	foreach (Object *object, objects) {
		QString filePath = createDirectory(object->id(), object->logicalId());

		QDomDocument doc;
		QDomElement root = doc.createElement("object");
		doc.appendChild(root);
		root.setAttribute("id", object->id().toString());
		if (object->logicalId() != Id())
			root.setAttribute("logicalId", object->logicalId().toString());

		root.setAttribute("parent", object->parent().toString());

		root.appendChild(idListToXml("children", object->children(), doc));
		root.appendChild(propertiesToXml(object, doc));

		QFileInfo fileInfo(filePath);
		bool fileExists = fileInfo.exists();
		if (fileExists)
		{
			QDir().remove(filePath);
		}
		OutFile out(filePath);
		doc.save(out(), 2);
		mSavedFiles << fileInfo.filePath();
	}
	if (!removeUnsaved(mWorkingDir))
		result = false;
	if (!mExternalClient.doAdd(mWorkingDir))
		result = false;
	mErrors.append(mExternalClient.newErrors());
	return result;
}

void Serializer::loadFromDisk(QHash<qReal::Id, Object*> &objectsHash)
{
	loadFromDisk(mWorkingDir + "/tree", objectsHash);
}

QStringList Serializer::newErrors()
{
	QStringList result(mErrors);
	mErrors.clear();
	return result;
}

void Serializer::loadFromDisk(QString const &currentPath, QHash<qReal::Id, Object*> &objectsHash)
{
	QDir dir(currentPath);
	if (dir.exists()) {
		dir.cd("logical");
		loadModel(dir, objectsHash);
		dir.cdUp();
		dir.cd("graphical");
		loadModel(dir, objectsHash);
	}
}

void Serializer::loadModel(QDir const &dir, QHash<qReal::Id, Object*> &objectsHash)
{
	foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		QString const path = fileInfo.filePath();
		if (fileInfo.isDir())
			loadModel(path, objectsHash);
		else if (fileInfo.isFile()) {
			QDomDocument doc = xmlUtils::loadDocument(path);
			Object *object = parseObject(doc.documentElement());
			Q_ASSERT(object);  // All objects in a repository shall be loadable.
			if (object != NULL)
				objectsHash.insert(object->id(), object);
		}
	}
}

Object *Serializer::parseObject(QDomElement const &elem)
{
	QString const id = elem.attribute("id", "");
	if (id == "")
		return NULL;

	QString const logicalIdString = elem.attribute("logicalId", "");
	Id const logicalId = loadId(logicalIdString);

	Object object(Id::loadFromString(id), Id(), logicalId);

	QString const parentIdString = elem.attribute("parent", "");
	Id const parent = loadId(parentIdString);
	if (object.parent() != parent)
		object.setParent(parent);

	foreach (Id child, loadIdList(elem, "children"))
		if (!object.children().contains(child))
			object.addChild(child);

	if (!loadProperties(elem, object))
		return NULL;

	return new Object(object);
}

bool Serializer::loadProperties(QDomElement const &elem, Object &object)
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
			// ����� ��� ������. ������� ���������, ���� ������������� ��
			// �������� �����/���������.
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

Id Serializer::loadId(QString const &elementStr)
{
	return elementStr.isEmpty() ? Id() : Id::loadFromString(elementStr);
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
		return QString::number(v.toInt());
	case QVariant::UInt:
		return QString::number(v.toUInt());
	case QVariant::Double:
		return QString::number(v.toDouble());
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
		// ���� ���, ��� � default � ��� ��������.
	default:
		qDebug() << v;
		Q_ASSERT(!"Unsupported QVariant type.");
		return "";
	}
}

QString Serializer::serializeQPointF(QPointF const &p)
{
	return QString::number(p.x()) + ", " + QString::number(p.y());
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

QString Serializer::createDirectory(Id const &id, Id const &logicalId)
{
	mSavedDirectories << QFileInfo(mWorkingDir).filePath();
	QString dirName = mWorkingDir + "/tree";
	mSavedDirectories << QFileInfo(dirName).filePath();
	if (logicalId == Id()) {
		dirName += "/logical";
	} else {
		dirName += "/graphical";
	}
	mSavedDirectories << QFileInfo(dirName).filePath();
	QStringList const partsList = id.toString().split('/');
	Q_ASSERT(partsList.size() >= 1 && partsList.size() <= 5);
	for (int i = 1; i < partsList.size() - 1; ++i) {
		dirName += "/" + partsList[i];
		QDir dir(dirName);
		if (!dir.exists())
		{
			dir.mkpath(dirName);
		}
		mSavedDirectories << QFileInfo(dirName).filePath();
	}

	return dirName + "/" + partsList[partsList.size() - 1];
}

bool Serializer::removeUnsaved(const QString &path)
{
	bool result = true;
	QDir dir(path);
	if (dir.exists())
	{
		foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
		{
			if (fileInfo.isDir())
			{
				if (mSavedDirectories.contains(fileInfo.filePath()))
				{
					if (!removeUnsaved(fileInfo.filePath()))
						result = false;
				}
				else
				{
					if (!mExternalClient.doRemove(fileInfo.filePath()))
						result = false;
				}
			}
			else
			{
				if (!mSavedFiles.contains(fileInfo.filePath()))
				{
					if (!mExternalClient.doRemove(fileInfo.filePath()))
						result = false;
				}
			}
		}
	}
	return result;
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

QDomElement Serializer::propertiesToXml(Object* const object, QDomDocument &doc)
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
