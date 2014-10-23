#include "serializer.h"

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QPointF>
#include <QtGui/QPolygon>

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>
#include <qrutils/outFile.h>
#include <qrutils/xmlUtils.h>
#include <qrutils/fileSystemUtils.h>

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
	, mWorkingCopyInspector(nullptr)
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

void Serializer::setWorkingCopyInspector(WorkingCopyInspectionInterface *inspector)
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
	Q_ASSERT_X(!mWorkingFile.isEmpty()
			, "Serializer::saveToDisk(...)"
			, "may be Client of RepoApi (see Models constructor also) has been initialised with empty filename?");

	prepareSaving();

	for (Object const * const object : objects) {
		QString const filePath = createDirectory(object->id(), object->isLogicalObject());

		QDomDocument doc;
		QDomElement root = object->serialize(doc);
		doc.appendChild(root);

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

	QString const filePath = fileInfo.absolutePath() + "/" + fileName + ".qrs";
	FolderCompressor::compressFolder(compressDir.absolutePath(), filePath);

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

void Serializer::prepareWorkingCopy(const QString &targetFolder, QString const &sourceProject)
{
	if (!QDir(targetFolder).removeRecursively()){
		clearDir(targetFolder);
	}
	QString const workingFile = sourceProject.isEmpty() ? mWorkingFile : sourceProject;
	if (QFileInfo(workingFile).exists()) {
		FolderCompressor::decompressFolder(workingFile, targetFolder);
	} else {
		QDir qDir;
		qDir.mkdir(targetFolder);
	}
}

void Serializer::processWorkingCopy(const QString &workingCopyPath, QString const &targetProject)
{
	QString const targetProjectPath = targetProject.isEmpty() ? mWorkingFile : targetProject;
		if (QDir(workingCopyPath).exists()) {
		FolderCompressor::compressFolder(workingCopyPath, targetProjectPath);
	}
	if (!QDir(workingCopyPath).removeRecursively()){
		clearDir(workingCopyPath);
		QDir(workingCopyPath).rmdir(workingCopyPath);
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

void Serializer::clearDir(QString const &path)
{
	if (path.isEmpty()) {
		return;
	}

	QDir dir(path);
	if (!dir.exists()) {
		return;
	}

	foreach (QFileInfo const &fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot)) {
		if (fileInfo.isDir()) {
			clearDir(fileInfo.filePath());
			dir.rmdir(fileInfo.fileName());
		} else {
			if(!QFile::setPermissions(fileInfo.fileName(), QFile::ReadOther | QFile::WriteOther)){
				FileSystemUtils::resetAttributes(fileInfo.filePath());
			}
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

QString Serializer::createDirectory(Id const &id, bool logical)
{
	mSavedDirectories << QFileInfo(mWorkingDir).filePath();
	QString dirName = mWorkingDir + "/tree";
	mSavedDirectories << QFileInfo(dirName).filePath();
	dirName += logical ? "/logical" : "/graphical";
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
		if (fileInfo.fileName().startsWith('.')) {
			continue;
		}
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

void Serializer::decompressFile(QString const &fileName)
{
	FolderCompressor::decompressFolder(fileName, mWorkingDir);
}
