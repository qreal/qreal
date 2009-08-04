#include "client.h"
#include "../kernel/exception/exception.h"

#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QPointF>
#include <QtGui/QPolygon>

using namespace qReal;

using namespace client;
Client::Client()
{
	init();
	loadFromDisk();
}

void Client::init()
{
	mObjects.insert(ROOT_ID,new LogicObject(ROOT_ID));
	mObjects[ROOT_ID]->setProperty("Name",ROOT_ID.toString());
}

Client::~Client()
{
	delete mObjects[ROOT_ID];
	mObjects.remove(ROOT_ID);
	saveToDisk();
}

IdTypeList Client::children(const IdType &id) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->children();
	} else {
		throw Exception("Client: Requesting children of nonexistent object " + id.toString());
	}
}

IdTypeList Client::parents( const IdType &id ) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->parents();
	} else {
		throw Exception("Client: Requesting parents of nonexistent object " + id.toString());
	}
}

void Client::addParent( const IdType &id, const IdType &parent )
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(parent)) {
			mObjects[id]->addParent(parent);
			mObjects[parent]->addChild(id);
		} else {
			throw Exception("Client: Adding nonexistent parent " + parent.toString() + " to  object " + id.toString());
		}
	} else {
		throw Exception("Client: Adding parent " + parent.toString() + " to nonexistent object " + id.toString());
	}
}

void Client::addChild(const IdType &id, const IdType &child)
{
	if (id == ROOT_ID) {
		mObjects.insert(child,new LogicObject(child,ROOT_ID));
	} else {
		if (mObjects.contains(id)) {
			mObjects[id]->addChild(child);
			if (mObjects.contains(child)) {
				mObjects[child]->addParent(id);
			} else {
				mObjects.insert(child,new LogicObject(child,id));
			}
		} else {
			throw Exception("Client: Adding child " + child.toString() + " to nonexistent object " + id.toString());
		}
	}
}

void Client::removeParent(const IdType &id, const IdType &parent)
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(parent)) {
			mObjects[id]->removeParent(parent);
			mObjects[parent]->removeChild(id);
		} else {
			throw Exception("Client: Removing nonexistent parent " + parent.toString() + " from object " + id.toString());
		}
	} else {
		throw Exception("Client: Removing parent " + parent.toString() + " from nonexistent object " + id.toString());
	}
}

void Client::removeChild(const IdType &id, const IdType &child)
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(child)) {
			mObjects[id]->removeChild(child);
			if (mObjects[child]->parents().size()!=1) {
				mObjects[child]->removeParent(id);
			} else {
				if (mObjects[child]->parents().first()==id) {
					delete mObjects[child];
					mObjects.remove(child);
				} else {
					throw Exception("Client: removing child " + child.toString() + " from object " + id.toString() + ", which is not his parent");
				}
			}
		} else {
			throw Exception("Client: removing nonexistent child " + child.toString() + " from object " + id.toString());
		}
	} else {
		throw Exception("Client: removing child " + child.toString() + " from nonexistent object " + id.toString());
	}
}

void Client::setProperty( const IdType &id, const PropertyName &name, const QVariant &value )
{
	if (mObjects.contains(id)) {
		mObjects[id]->setProperty(name,value);
	} else {
		throw Exception("Client: Setting property of nonexistent object " + id.toString());
	}
}

QVariant Client::property( const IdType &id, const PropertyName &name ) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->property(name);
	} else {
		throw Exception("Client: Requesting property of nonexistent object " + id.toString());
	}
}

void Client::removeProperty( const IdType &id, const PropertyName &name )
{
	if (mObjects.contains(id)) {
		return mObjects[id]->removeProperty(name);
	} else {
		throw Exception("Client: Removing property of nonexistent object " + id.toString());
	}
}

void Client::loadFromDisk()
{
	loadFromDisk(saveDirName);
	addChildrenToRootObject();
}

void Client::addChildrenToRootObject()
{
	foreach (LogicObject *object, mObjects.values()) {
		if (object->parents().contains(ROOT_ID)) {
			mObjects[ROOT_ID]->addChild(object->id());
		}
	}
}

void Client::loadFromDisk(QString const &currentPath)
{
	QDir dir(currentPath);
	if (dir.exists()) {
		foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
			QString path = fileInfo.filePath();
			if (fileInfo.isDir())
				loadFromDisk(path);
			else if (fileInfo.isFile()) {
				QDomDocument doc = loadXmlDocument(path);
				LogicObject *object = parseLogicObject(doc.documentElement());
				if (object != NULL)
					mObjects.insert(object->id(), object);
			}
		}
	}
}

QDomDocument Client::loadXmlDocument(QString const &path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "cannot open file" << path;
		return QDomDocument();
	}

	QDomDocument doc;
	QString error = "";
	int errorLine = 0;
	int errorCol = 0;

	if (!doc.setContent(&file, false, &error, &errorLine, &errorCol))
	{
		qDebug() << "parse error in " << path
				<< " at (" << errorLine << "," << errorCol
				<< "): " << error;
		file.close();
		return QDomDocument();
	}
	file.close();
	return doc;
}

LogicObject *Client::parseLogicObject(QDomElement const &elem)
{
	QString id = elem.attribute("id", "");
	if (id == "")
		return NULL;

	LogicObject object(Id::loadFromString(id));

	foreach (IdType parent, loadIdList(elem, "parents"))
		object.addParent(parent);

	foreach (IdType child, loadIdList(elem, "children"))
		object.addChild(child);

	if (!loadProperties(elem, object))
		return NULL;

	return new LogicObject(object);
}

bool Client::loadProperties(QDomElement const &elem, LogicObject &object)
{
	QDomNodeList propertiesList = elem.elementsByTagName("properties");
	if (propertiesList.count() != 1) {
		qDebug() << "Incorrect element: children list must appear once";
		return false;
	}

	QDomElement properties = propertiesList.at(0).toElement();
	QDomElement property = properties.firstChildElement();
	while (!property.isNull()) {
		QString type = property.tagName();
		QString key = property.attribute("key", "");
		if (key == "")
			return false;

		QString valueStr = property.attribute("value", "");
		QVariant value = parseValue(type, valueStr);
		object.setProperty(key, value);
		property = property.nextSiblingElement();
	}
	return true;
}

IdTypeList Client::loadIdList(QDomElement const &elem, QString const &name)
{
	QDomNodeList list = elem.elementsByTagName(name);
	if (list.count() != 1) {
		qDebug() << "Incorrect element: " + name + " list must appear once";
		return IdTypeList();
	}

	IdTypeList result;

	QDomElement elements = list.at(0).toElement();
	QDomElement element = elements.firstChildElement();
	while (!element.isNull()) {
		QString elementStr = element.attribute("id", "");
		if (elementStr == "") {
			qDebug() << "Incorrect Child XML node";
			return IdTypeList();
		}
		result.append(Id::loadFromString(elementStr));
		element = element.nextSiblingElement();
	}
	return result;
}

QVariant Client::parseValue(QString const &typeName, QString const &valueStr)
{
	if (typeName == "Int") {
		return QVariant(valueStr.toInt());
	} else if (typeName == "UInt") {
		return QVariant(valueStr.toUInt());
	} else if (typeName == "Double") {
		return QVariant(valueStr.toDouble());
	} else if (typeName == "Bool") {
		return QVariant(valueStr.toUpper() == "TRUE");
	} else if (typeName == "QString") {
		return QVariant(valueStr);
	} else if (typeName == "Char") {
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
	} else {
		Q_ASSERT(!"Unknown property type");
		return QVariant();
	}
}

QPointF Client::parsePointF(QString const &str)
{
	double x = str.section(", ", 0, 0).toDouble();
	double y = str.section(", ", 1, 1).toDouble();
	return QPointF(x, y);
}

void Client::saveToDisk()
{
	clearDir(saveDirName);
	foreach (LogicObject *object, mObjects.values()) {
		QString filePath = createDirectory(object->id());

		QDomDocument doc;
		QDomElement root = doc.createElement("LogicObject");
		doc.appendChild(root);
		root.setAttribute("id", object->id().toString());

		root.appendChild(idListToXml("parents", object->parents(), doc));
		root.appendChild(idListToXml("children", object->children(), doc));
		root.appendChild(propertiesToXml(object, doc));

		QFile file(filePath);
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream stream(&file);
		doc.save(stream, 2);
		file.close();
	}
}

QString Client::createDirectory(Id const &id)
{
	QString dirName = saveDirName;
	QStringList partsList = id.toString().split('/');
	Q_ASSERT(partsList.size() >=1 && partsList.size() <= 5);
	for (int i = 1; i < partsList.size() - 1; ++i) {
		dirName += "/" + partsList[i];
	}

	QDir dir;
	dir.rmdir(saveDirName);
	dir.mkpath(dirName);

	return dirName + "/" + partsList[partsList.size() - 1];
}

QDomElement Client::idListToXml(QString const &attributeName, IdTypeList const &idList, QDomDocument &doc)
{
	QDomElement result = doc.createElement(attributeName);
	foreach (IdType id, idList) {
		QDomElement element = doc.createElement("object");
		element.setAttribute("id", id.toString());
		result.appendChild(element);
	}
	return result;
}

QDomElement Client::propertiesToXml(LogicObject * const object, QDomDocument &doc)
{
	QDomElement result = doc.createElement("properties");
	QMapIterator<PropertyName, QVariant> i = object->propertiesIterator();
	while (i.hasNext()) {
		i.next();
		QDomElement property = doc.createElement(i.value().typeName());
		property.setAttribute("key", i.key());
		QString value = serializeQVariant(i.value());
		property.setAttribute("value", value);
		result.appendChild(property);
	}
	return result;
}

void Client::clearDir(QString const &path)
{
	QDir dir(path);
	if (dir.exists()) {
		foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
			if (fileInfo.isDir()) {
				dir.rmdir(fileInfo.filePath());
				clearDir(fileInfo.filePath());
			}
			else
				dir.remove(fileInfo.fileName());
		}
	}
}

QString Client::serializeQVariant(QVariant const &v)
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
	default:
		qDebug() << v;
		Q_ASSERT(!"Unsupported QVariant type.");
		return "";
	}
}

QString Client::serializeQPointF(QPointF const &p)
{
	return QString("%1").arg(p.x()) + ", " + QString("%1").arg(p.y());
}

QString Client::serializeQPolygon(QPolygon const &p)
{
	QString result("");
	foreach (QPoint point, p) {
		result += serializeQPointF(point) + " : ";
	}
	return result;
}

void Client::printDebug() const
{
	qDebug() << mObjects.size() << " objects in repository";
	foreach (LogicObject *object, mObjects.values()) {
		qDebug() << object->id().toString();
		qDebug() << "Children:";
		foreach (IdType id, object->children())
			qDebug() << id.toString();
		qDebug() << "Parents:";
		foreach (IdType id, object->parents())
			qDebug() << id.toString();
		qDebug() << "============";
	}
}

void Client::exterminate()
{
	printDebug();
	mObjects.clear();
	init();
	saveToDisk();
	printDebug();
}
