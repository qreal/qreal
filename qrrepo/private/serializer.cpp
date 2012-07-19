#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QPointF>
#include <QtGui/QPolygon>

#include "serializer.h"
#include "folderCompressor.h"
#include "../../qrkernel/settingsManager.h"
#include "../../qrutils/outFile.h"
#include "../../qrutils/xmlUtils.h"

using namespace qrRepo;
using namespace details;
using namespace utils;
using namespace qReal;

Serializer::Serializer(QString const& saveDirName)
	: mWorkingDir(SettingsManager::value("temp").toString())
	, mWorkingFile(saveDirName)
	, mWorkingCopyInspector(NULL)
{
	clearWorkingDir();
}

void Serializer::clearWorkingDir()
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

void Serializer::setWorkingCopyInspector(versioning::WorkingCopyInspectionInterface *inspector)
{
	mWorkingCopyInspector = inspector;
}

void Serializer::prepareSaving()
{
	mSavedDirectories.clear();
	mSavedFiles.clear();

	clearDir(mWorkingDir);
	if (QFileInfo(mWorkingFile).exists()) {
		decompressFile(mWorkingFile);
	}
}

bool Serializer::reportAdded(const QString &fileName)
{
	if (mWorkingCopyInspector) {
		return mWorkingCopyInspector->onFileAdded(fileName, mWorkingDir);
	}
	return true;
}

bool Serializer::reportRemoved(const QString &fileName)
{
	if (mWorkingCopyInspector) {
		return mWorkingCopyInspector->onFileRemoved(fileName, mWorkingDir);
	}
	return true;
}

bool Serializer::reportChanged(const QString &fileName)
{
	if (mWorkingCopyInspector) {
		return mWorkingCopyInspector->onFileChanged(fileName, mWorkingDir);
	}
	return true;
}

void Serializer::saveToDisk(QList<Object*> const &objects)
{
	prepareSaving();

	foreach (Object *object, objects) {
		QString filePath = createDirectory(object->id(), object->logicalId());

		QDomDocument doc;
		QDomElement root = doc.createElement("object");
		doc.appendChild(root);
		root.setAttribute("id", object->id().toString());
		if (object->logicalId() != Id()) {
			root.setAttribute("logicalId", object->logicalId().toString());
		}

		root.setAttribute("parent", object->parent().toString());

		root.appendChild(idListToXml("children", object->children(), doc));
		root.appendChild(propertiesToXml(object, doc));

		QFileInfo fileInfo(filePath);
		bool fileExists = fileInfo.exists();
		if (fileExists) {
			QDir().remove(filePath);
		}
		OutFile out(filePath);
		doc.save(out(), 2);
		mSavedFiles << filePath;
		if (!fileExists) {
			reportAdded(filePath);
		} else {
			reportChanged(filePath);
		}
	}

	removeUnsaved(mWorkingDir);

	QFileInfo fileInfo(mWorkingFile);
	QString fileName = fileInfo.baseName();

	QDir compressDir(SettingsManager::value("temp").toString());
	QDir dir = fileInfo.absolutePath();

	QFile previousSave(dir.absolutePath() + "/" + fileName +".qrs");
	if (previousSave.exists()) {
		previousSave.remove();
	}

	FolderCompressor().compressFolder(compressDir.absolutePath(), fileInfo.absolutePath() + "/" + fileName + ".qrs");

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

void Serializer::prepareWorkingCopy(const QString &workingCopyPath)
{
	clearDir(workingCopyPath);
	if (QFileInfo(mWorkingFile).exists()) {
		FolderCompressor().decompressFolder(mWorkingFile, workingCopyPath);
	}
}

void Serializer::processWorkingCopy(const QString &workingCopyPath, QString const &targetProject)
{
	QString const targetProjectPath = targetProject.isEmpty() ? mWorkingFile : targetProject;
	if (QDir(workingCopyPath).exists()) {
		FolderCompressor().compressFolder(workingCopyPath, targetProjectPath);
	}
	clearDir(workingCopyPath);
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
		foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot)) {
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
		// Если нет, идём в default и там ругаемся.
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
		if (!dir.exists()) {
			dir.mkpath(dirName);
		}
		mSavedDirectories << QFileInfo(dirName).filePath();
	}

	return dirName + "/" + partsList[partsList.size() - 1];
}

bool Serializer::removeUnsaved(const QString &path)
{
	QDir dir(path);
	if (!dir.exists()) {
		return true;
	}
	bool result = true;
	foreach (QFileInfo const &fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		if (fileInfo.isDir()) {
			bool const invocationResult =
					mSavedDirectories.contains(fileInfo.filePath())
						? removeUnsaved(fileInfo.filePath())
						: reportRemoved(fileInfo.filePath());
			result = result & invocationResult;
		} else {
			result = mSavedFiles.contains(fileInfo.filePath())
					? result
					: (reportRemoved(fileInfo.filePath()) && result);
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

void Serializer::decompressFile(QString fileName)
{
	FolderCompressor().decompressFolder(fileName, mWorkingDir);
}
