#include "serializer.h"

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QPointF>
#include <QtGui/QPolygon>

#include "../../qrkernel/settingsManager.h"
#include "../../qrkernel/exception/exception.h"
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

void Serializer::removeFromDisk(Id const &id) const
{
	QDir dir;
	dir.remove(pathToElement(id));
}

void Serializer::setWorkingFile(QString const &workingFile)
{
	mWorkingFile = workingFile;
}

void Serializer::saveToDisk(QList<Object *> const &objects, QHash<QString, QVariant> const &metaInfo) const
{
	Q_ASSERT_X(!mWorkingFile.isEmpty()
		, "Serializer::saveToDisk(...)"
		, "may be Repository of RepoApi (see Models constructor also) has been initialised with empty filename?");

	foreach (Object const * const object, objects) {
		QString const filePath = createDirectory(object->id(), object->isLogicalObject());

		QDomDocument doc;
		QDomElement root = object->serialize(doc);
		doc.appendChild(root);

		OutFile out(filePath);
		doc.save(out(), 2);
	}

	saveMetaInfo(metaInfo);

	QFileInfo fileInfo(mWorkingFile);
	QString fileName = fileInfo.baseName();

	QDir compressDir(SettingsManager::value("temp").toString());
	QDir dir = fileInfo.absolutePath();

	QFile previousSave(dir.absolutePath() + "/" + fileName +".qrs");
	if (previousSave.exists()) {
		previousSave.remove();
	}

	QString const filePath = fileInfo.absolutePath() + "/" + fileName + ".qrs";
	FolderCompressor::compressFolder(compressDir.absolutePath(), filePath);

	// Hiding autosaved files
	if (fileName.contains("~")) {
		FileSystemUtils::makeHidden(filePath);
	}

	clearDir(mWorkingDir);
}

void Serializer::loadFromDisk(QHash<qReal::Id, Object*> &objectsHash, QHash<QString, QVariant> &metaInfo)
{
	clearWorkingDir();
	if (!mWorkingFile.isEmpty()) {
		decompressFile(mWorkingFile);
	}

	loadFromDisk(SettingsManager::value("temp").toString(), objectsHash);
	loadMetaInfo(metaInfo);
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
	foreach (QFileInfo const &fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		QString const path = fileInfo.filePath();
		if (fileInfo.isDir()) {
			loadModel(path, objectsHash);
		} else if (fileInfo.isFile()) {
			QDomDocument doc = xmlUtils::loadDocument(path);
			QDomElement const element = doc.documentElement();

			// To ensure backwards compatibility. Replace this by separate tag names when save updating mechanism
			// will be implemented.
			Object * const object = element.hasAttribute("logicalId") && element.attribute("logicalId") != "qrm:/"
					? dynamic_cast<Object *>(new GraphicalObject(element))
					: dynamic_cast<Object *>(new LogicalObject(element))
					;

			objectsHash.insert(object->id(), object);
		}
	}
}

void Serializer::saveMetaInfo(QHash<QString, QVariant> const &metaInfo) const
{
	QDomDocument document;
	QDomElement root = document.createElement("metaInformation");
	document.appendChild(root);
	for (QString const &key : metaInfo.keys()) {
		QDomElement element = document.createElement("info");
		element.setAttribute("key", key);
		element.setAttribute("type", metaInfo[key].typeName());
		element.setAttribute("value", ValuesSerializer::serializeQVariant(metaInfo[key]));
		root.appendChild(element);
	}

	QString const filePath = mWorkingDir + "/metaInfo.xml";
	OutFile out(filePath);
	out() << document.toString(4);
}

void Serializer::loadMetaInfo(QHash<QString, QVariant> &metaInfo) const
{
	metaInfo.clear();

	QString const filePath = mWorkingDir + "/metaInfo.xml";
	if (!QFile::exists(filePath)) {
		return;
	}

	QDomDocument const document = xmlUtils::loadDocument(filePath);
	for (QDomElement child = document.documentElement().firstChildElement("info")
			; !child.isNull()
			; child = child.nextSiblingElement("info"))
	{
		metaInfo[child.attribute("key")] = ValuesSerializer::deserializeQVariant(
				child.attribute("type"), child.attribute("value"));
	}
}

void Serializer::clearDir(QString const &path)
{
	if (path.isEmpty()) {
		return;
	}

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

void Serializer::decompressFile(QString const &fileName)
{
	FolderCompressor::decompressFolder(fileName, mWorkingDir);
}
