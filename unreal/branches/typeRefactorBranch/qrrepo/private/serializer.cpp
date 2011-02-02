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
	clearDir(mWorkingDir+"/tree");
}

void Serializer::removeFromDisk(NewType type) const
{
        qDebug() << "deleteDiagramDir " << pathToElement(type);
	QDir dir;
        dir.remove(pathToElement(type));
}

void Serializer::setWorkingDir(QString const &workingDir)
{
	mWorkingDir = workingDir + "/save";
}

void Serializer::saveToDisk(QList<LogicObject*> const &objects) const
{
	foreach (LogicObject *object, objects) {
                qDebug() << "SAVED: " << object->type().toString();
                QString filePath = createDirectory(object->type());

		QDomDocument doc;
		QDomElement root = doc.createElement("LogicObject");
		doc.appendChild(root);
                root.setAttribute("type", object->type().toString());

                root.appendChild(typeListToXml("parents", object->parents(), doc));
                root.appendChild(typeListToXml("children", object->children(), doc));
		root.appendChild(propertiesToXml(object, doc));

		OutFile out(filePath);
		doc.save(out(), 2);
	}
}

void Serializer::loadFromDisk(QHash<qReal::NewType, LogicObject*> &objectsHash)
{
	loadFromDisk(mWorkingDir+"/tree", objectsHash);
}

void Serializer::loadFromDisk(QString const &currentPath, QHash<qReal::NewType, LogicObject*> &objectsHash)
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
                                        objectsHash.insert(object->type(), object);
			}
		}
	}
}

void  Serializer::log(QString const message, qReal::NewType const diagram)
{
	QString path = mWorkingDir+"/logs/"+diagram.diagram();
        QString name = diagram.element();
	QDir dir;
	dir.mkpath(path);

	QFile *file;
	if (!files.contains(name)) {
		file = new QFile(path+"/"+name+".log");
		files.insert(name, file);
	} else {
		file = files.value(name);
	}

	if (!file->isOpen())
		file->open(QIODevice::Append | QIODevice::Text);
	QTextStream out(file);
	out << message << "\n";

	//may be, file must be closed or smthng else
}

LogicObject *Serializer::parseLogicObject(QDomElement const &elem)
{
        QString type = elem.attribute("type", "");
        if (type == "")
		return NULL;

        LogicObject object(NewType::loadFromString(type));

        foreach (NewType parent, loadTypeList(elem, "parents"))
		if (!mFailSafe || !object.parents().contains(parent))
			object.addParent(parent);

        foreach (NewType child, loadTypeList(elem, "children"))
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
                        if (property.attribute("type", "") == "qReal::TypeList") {
				QString key = property.tagName();
                                TypeList value = loadTypeList(properties, property.tagName());
                                object.setProperty(key, TypeListHelper::toVariant(value));
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

TypeList Serializer::loadTypeList(QDomElement const &elem, QString const &name)
{
	QDomNodeList list = elem.elementsByTagName(name);
	if (list.count() != 1) {
		qDebug() << "Incorrect element: " + name + " list must appear once";
                return TypeList();
	}

        TypeList result;

	QDomElement elements = list.at(0).toElement();
	QDomElement element = elements.firstChildElement();
	while (!element.isNull()) {
                QString elementStr = element.attribute("type", "");
		if (elementStr == "") {
			qDebug() << "Incorrect Child XML node";
                        return TypeList();
		}
                result.append(NewType::loadFromString(elementStr));
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
        } else if (typeName == "qReal::NewType") {
                return NewType::loadFromString(valueStr).toVariant();
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
                if (v.userType() == QMetaType::type("qReal::NewType"))
                        return v.value<qReal::NewType>().toString();
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

QString Serializer::pathToElement(NewType const &type) const
{
	QString dirName = mWorkingDir;

        QStringList partsList = type.toString().split('/');
	Q_ASSERT(partsList.size() >=1 && partsList.size() <= 5);
	for (int i = 1; i < partsList.size() - 1; ++i) {
		dirName += "/" + partsList[i];
	}

	return dirName + "/" + partsList[partsList.size() - 1];
}

QString Serializer::createDirectory(NewType const &type) const
{
	QString dirName = mWorkingDir+"/tree";
        QStringList partsList = type.toString().split('/');
	Q_ASSERT(partsList.size() >=1 && partsList.size() <= 5);
	for (int i = 1; i < partsList.size() - 1; ++i) {
		dirName += "/" + partsList[i];
	}

	QDir dir;
	dir.rmdir(mWorkingDir);
	dir.mkpath(dirName);

	return dirName + "/" + partsList[partsList.size() - 1];
}

QDomElement Serializer::typeListToXml(QString const &attributeName, TypeList const &typeList, QDomDocument &doc)
{
	QDomElement result = doc.createElement(attributeName);
        foreach (NewType type, typeList) {
		QDomElement element = doc.createElement("object");
                element.setAttribute("type", type.toString());
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
                if (typeName == "qReal::TypeList") {
                        QDomElement list = typeListToXml(i.key(), i.value().value<TypeList>(), doc);
                        list.setAttribute("type", "qReal::TypeList");
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
