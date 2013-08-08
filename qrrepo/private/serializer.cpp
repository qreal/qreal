#include "serializer.h"

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QPointF>
#include <QtGui/QPolygon>

#include "../../qrkernel/settingsManager.h"
#include "../../qrutils/outFile.h"
#include "../../qrutils/xmlUtils.h"
#include "../../qrutils/fileSystemUtils.h"

#include "folderCompressor.h"
#include "classes/logicalObject.h"
#include "classes/graphicalObject.h"

using namespace qrRepo;
using namespace details;
using namespace utils;
using namespace qReal;

Serializer::Serializer(QString const& saveDirName)
	: mWorkingDir(SettingsManager::value("temp").toString())
	, mWorkingFile(saveDirName)
{
	clearWorkingDir();
}

void Serializer::clearWorkingDir() const
{
	clearDir(mWorkingDir);
}

void Serializer::removeFromDisk(Id id) const
{
	QDir dir;
	dir.remove(pathToElement(id));
}

void Serializer::setWorkingFile(QString const &workingFile)
{
	mWorkingFile = workingFile;
}

void Serializer::saveToDisk(QList<Object*> const &objects) const
{
	Q_ASSERT_X(!mWorkingFile.isEmpty()
		, "Serializer::saveToDisk(...)"
		, "may be Client of RepoApi (see Models constructor also) has been initialised with empty filename?");

	foreach (Object *object, objects) {

		QString filePath = createDirectory(object->id(), object->isLogicalObject());

		QDomDocument doc;
		QDomElement root = doc.createElement("object");
		doc.appendChild(root);
		root.setAttribute("id", object->id().toString());

		GraphicalObject const * const graphicalObject = dynamic_cast<GraphicalObject const *>(object);
		if (graphicalObject) {
			root.setAttribute("logicalId", graphicalObject->logicalId().toString());
		}

		root.setAttribute("parent", object->parent().toString());
		root.appendChild(idListToXml("children", object->children(), doc));
		root.appendChild(createPropertiesXmlElement(object, doc));

		OutFile out(filePath);
		doc.save(out(), 2);
	}

	QFileInfo fileInfo(mWorkingFile);
	QString fileName = fileInfo.baseName();

	QDir compressDir(SettingsManager::value("temp").toString());
	QDir dir = fileInfo.absolutePath();

	QFile previousSave(dir.absolutePath() + "/" + fileName +".qrs");
	if (previousSave.exists()) {
		previousSave.remove();
	}

	QString const filePath = fileInfo.absolutePath() + "/" + fileName + ".qrs";
	FolderCompressor().compressFolder(compressDir.absolutePath(), filePath);

	// Hiding autosaved files
	if (fileName.contains("~")) {
		FileSystemUtils::makeHidden(filePath);
	}

	clearDir(mWorkingDir);
}

void Serializer::loadFromDisk(QHash<qReal::Id, Object*> &objectsHash)
{
	clearWorkingDir();
	if (!mWorkingFile.isEmpty()) {
		decompressFile(mWorkingFile);
	}
	loadFromDisk(SettingsManager::value("temp").toString(), objectsHash);
}

void Serializer::loadFromDisk(QString const &currentPath, QHash<qReal::Id, Object*> &objectsHash)
{
	QDir dir(currentPath + "/tree");
	if (dir.cd("logical")) {
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
			if (object != NULL) {
				objectsHash.insert(object->id(), object);
			}
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

	Object * const object = logicalId == Id()
			? static_cast<Object *>(new LogicalObject(Id::loadFromString(id), Id()))
			: static_cast<Object *>(new GraphicalObject(Id::loadFromString(id), Id(), logicalId))
			;

	QString const parentIdString = elem.attribute("parent", "");
	Id const parent = loadId(parentIdString);
	if (object->parent() != parent) {
		object->setParent(parent);
	}

	foreach (Id child, loadIdList(elem, "children")) {
		if (!object->children().contains(child)) {
			object->addChild(child);
		}
	}

	if (!loadProperties(elem, *object)) {
		return NULL;
	}

	return object;
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
	} else if (typeName == "QStringList") {
		return QVariant(valueStr.split(',', QString::SkipEmptyParts));
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
	if (!dir.exists()) {
		return;
	}
	foreach (QFileInfo const &fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		if (fileInfo.isDir()) {
			clearDir(fileInfo.filePath());
			dir.rmdir(fileInfo.fileName());
		} else {
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
	case QVariant::StringList:
		return v.toStringList().join(',');
	case QVariant::Char:
		return v.toChar();
	case QVariant::PointF:
		return serializeQPointF(v.toPointF());
	case QVariant::Polygon:
		return serializeQPolygon(v.value<QPolygon>());
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

QString Serializer::createDirectory(Id const &id, bool logical) const
{
	QString dirName = mWorkingDir + "/tree";
	dirName += logical ? "/logical" : "/graphical";

	QStringList const partsList = id.toString().split('/');
	Q_ASSERT(partsList.size() >= 1 && partsList.size() <= 5);
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

void Serializer::getSingleElementProperties(QDomElement &result, Object const *object, QDomDocument &doc)
{
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
}

QDomElement Serializer::createPropertiesXmlElement(Object const *object, QDomDocument &doc, Object const *logicalObject)
{
	QDomElement result = doc.createElement("properties");
	getSingleElementProperties(result, object, doc);

	if (logicalObject) {
		getSingleElementProperties(result, logicalObject, doc);
	}

	return result;
}

void Serializer::decompressFile(QString const &fileName)
{
	FolderCompressor().decompressFolder(fileName, mWorkingDir);
}

void Serializer::exportToXml(QString const &targetFile, QHash<qReal::Id, Object*> const &objects) const
{
	Q_ASSERT_X(!targetFile.isEmpty(), "XmlSerializer::exportTo(...)", "target filename is empty");

	QDomDocument doc;
	QDomElement root = doc.createElement("project");
	doc.appendChild(root);

	foreach (Id const &id, objects[Id::rootId()]->children()) {

		// skip logical elements of diagrams
		if (objects[id]->isLogicalObject()) {
			continue;
		}

		exportDiagram(id, doc, root, objects);
	}

	OutFile out(targetFile);
	doc.save(out(), 4);
}

void Serializer::exportDiagram(Id const &diagramId, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects)
{
	QDomElement diagram = doc.createElement("diagram");

	GraphicalObject const * const graphicalObject = dynamic_cast<GraphicalObject const *>(objects[diagramId]);
	if (graphicalObject) {
		diagram.setAttribute("logical_id", graphicalObject->logicalId().toString());
	}

	diagram.setAttribute("graphical_id", diagramId.toString());
	diagram.setAttribute("name", objects[diagramId]->properties()["name"].toString());
	exportProperties(diagramId, doc, diagram, objects);

	QDomElement elements = doc.createElement("elements");

	foreach (Id const &id, objects[diagramId]->children()) {
		exportElement(id, doc, elements, objects);
	}

	diagram.appendChild(elements);

	root.appendChild(diagram);
}

void Serializer::exportElement(Id const &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects)
{
	QDomElement element = doc.createElement("element");
	element.setAttribute("name", objects[id]->properties()["name"].toString());
	element.setAttribute("graphical_id", id.toString());

	GraphicalObject const * const graphicalObject = dynamic_cast<GraphicalObject const *>(objects[id]);
	if (graphicalObject) {
		element.setAttribute("logical_id", graphicalObject->logicalId().toString());
	}

	exportProperties(id, doc, element, objects);
	exportChildren(id, doc, element, objects);

	root.appendChild(element);
}

void Serializer::exportChildren(Id const &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects)
{
	Object *object = objects[id];
	int size = object->children().size();
	if (size == 0) {
		return;
	}

	QDomElement children = doc.createElement("children");
	children.setAttribute("count", size);

	foreach (Id const &id, object->children()) {
		exportElement(id, doc, children, objects);
	}

	root.appendChild(children);
}

void Serializer::exportProperties(Id const&id, QDomDocument &doc, QDomElement &root, QHash<Id, Object *> const &objects)
{
	QDomElement props = doc.createElement("properties");

	GraphicalObject const * const graphicalObject = dynamic_cast<GraphicalObject const *>(objects[id]);
	LogicalObject const * const logicalObject
			= dynamic_cast<LogicalObject const *>(objects[graphicalObject->logicalId()]);

	QMap<QString, QVariant> properties;

	QMapIterator<QString, QVariant> i = logicalObject->propertiesIterator();
	while (i.hasNext()) {
		i.next();
		properties[i.key()] = i.value();

	}

	i = graphicalObject->propertiesIterator();
	while (i.hasNext()) {
		i.next();
		properties[i.key()] = i.value();
	}

	foreach (QString const &key, properties.keys()) {
		QDomElement prop = doc.createElement("property");

		QString typeName = properties[key].typeName();
		QVariant value = properties[key];
		if (typeName == "qReal::IdList" && (value.value<IdList>().size() != 0)) {
			QDomElement list = idListToXml("list", value.value<IdList>(), doc);
			prop.appendChild(list);
		} else if (typeName == "qReal::Id"){
			prop.setAttribute("value", value.value<Id>().toString());
		} else if (value.toString().isEmpty()) {
			continue;
		} else {
			prop.setAttribute("value", properties[key].toString());
		}

		prop.setAttribute("name", key);

		props.appendChild(prop);
	}

	root.appendChild(props);
}


