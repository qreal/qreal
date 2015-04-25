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

#include "serializer.h"

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QPointF>
#include <QtCore/QCoreApplication>
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

const QString unsavedDir = "unsaved";

Serializer::Serializer(const QString& saveDirName)
	: mWorkingDir(QCoreApplication::applicationDirPath() + "/" + unsavedDir)
	, mWorkingFile(saveDirName)
	, mWorkingCopyInspector(nullptr)
{
	clearWorkingDir();
	/// @todo: throw away this legacy piece of sh.t
	SettingsManager::setValue("temp", mWorkingDir);
	QDir dir(QCoreApplication::applicationDirPath());
	if (!dir.cd(mWorkingDir)) {
		QDir().mkdir(mWorkingDir);
	}
}

void Serializer::clearWorkingDir() const
{
	clearDir(mWorkingDir);
}

void Serializer::removeFromDisk(const Id &id) const
{
	QDir dir;
	dir.remove(pathToElement(id));
}

void Serializer::setWorkingFile(const QString &workingFile, bool isNewSave)
{
	mWorkingFile = workingFile;
	if (isNewSave) {
		QDir dir;
		dir.remove(QFileInfo(mWorkingFile).absoluteFilePath());
	}
}

void Serializer::setWorkingCopyInspector(WorkingCopyInspectionInterface *inspector)
{
	mWorkingCopyInspector = inspector;
}

bool Serializer::prepareSaving() const
{
	mSavedDirectories.clear();
	mSavedFiles.clear();
	reportAddedList.clear();
	reportChangedList.clear();
	reportRemovedList.clear();

	clearDir(mWorkingDir);
	if (QFileInfo(mWorkingFile).exists()) {
		decompressFile(mWorkingFile);
		return false;
	}
	return true;
}

bool Serializer::reportAdded() const
{
	if (mWorkingCopyInspector) {
		return mWorkingCopyInspector->onFileAdded(reportAddedList, mWorkingDir);
	}
	return true;
}

bool Serializer::reportRemoved() const
{
	if (mWorkingCopyInspector) {
		return mWorkingCopyInspector->onFileRemoved(reportRemovedList, mWorkingDir);
	}
	return true;
}

bool Serializer::reportChanged() const
{
	if (mWorkingCopyInspector) {
		return mWorkingCopyInspector->onFileChanged(reportChangedList, mWorkingDir);
	}
	return true;
}


void Serializer::saveToDisk(QList<Object *> const &objects, QHash<QString, QVariant> const &metaInfo) const
{
	Q_ASSERT_X(!mWorkingFile.isEmpty()
			, "Serializer::saveToDisk(...)"
			, "may be Client of RepoApi (see Models constructor also) has been initialised with empty filename?");

	bool isFirstTimeSave = prepareSaving();

	foreach (const Object * const object, objects) {
		const QString filePath = createDirectory(object->id(), object->isLogicalObject());

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
			reportAddedList.push_front(filePath);
		} else {
			reportChangedList.push_back(filePath);
		}
	}

	saveMetaInfo(metaInfo, isFirstTimeSave);
	removeUnsaved(mWorkingDir);

	QFileInfo fileInfo(mWorkingFile);
	QString fileName = fileInfo.baseName();

	QDir compressDir(SettingsManager::value("temp").toString());
	QDir dir = fileInfo.absolutePath();

	QFile previousSave(dir.absolutePath() + "/" + fileName +".qrs");
	if (previousSave.exists()) {
		previousSave.remove();
	}

	const QString filePath = fileInfo.absolutePath() + "/" + fileName + ".qrs";
	FolderCompressor::compressFolder(compressDir.absolutePath(), filePath);

	// Hiding autosaved files
	if (fileName.contains("~")) {
		FileSystemUtils::makeHidden(filePath);
	}

	clearDir(mWorkingDir);
	reportAdded();
	reportChanged();
	reportRemoved();
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

void Serializer::loadFromDisk(const QString &currentPath, QHash<qReal::Id, Object*> &objectsHash)
{
	QDir dir(currentPath + "/tree");
	if (dir.cd("logical")) {
		loadModel(dir, objectsHash);
		dir.cdUp();
		dir.cd("graphical");
		loadModel(dir, objectsHash);
	}
}

void Serializer::prepareWorkingCopy(const QString &targetFolder, const QString &sourceProject)
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

void Serializer::processWorkingCopy(const QString &workingCopyPath, const QString &targetProject)
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

void Serializer::loadModel(const QDir &dir, QHash<qReal::Id, Object*> &objectsHash)
{
	foreach (const QFileInfo &fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		const QString path = fileInfo.filePath();
		if (fileInfo.isDir()) {
			loadModel(path, objectsHash);
		} else if (fileInfo.isFile()) {
			QDomDocument doc = xmlUtils::loadDocument(path);
			const QDomElement element = doc.documentElement();

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

void Serializer::saveMetaInfo(QHash<QString, QVariant> const &metaInfo, bool isFirstTimeSave) const
{
	QDomDocument document;
	QDomElement root = document.createElement("metaInformation");
	document.appendChild(root);
	for (const QString &key : metaInfo.keys()) {
		QDomElement element = document.createElement("info");
		element.setAttribute("key", key);
		element.setAttribute("type", metaInfo[key].typeName());
		element.setAttribute("value", ValuesSerializer::serializeQVariant(metaInfo[key]));
		root.appendChild(element);
	}

	const QString filePath = mWorkingDir + "/metaInfo.xml";
	OutFile out(filePath);
	out() << document.toString(4);

	if (isFirstTimeSave) {
		reportAddedList.push_back(filePath);
	} else {
		reportChangedList.push_back(filePath);
	}
	mSavedFiles << filePath;
}

void Serializer::loadMetaInfo(QHash<QString, QVariant> &metaInfo) const
{
	metaInfo.clear();

	const QString filePath = mWorkingDir + "/metaInfo.xml";
	if (!QFile::exists(filePath)) {
		return;
	}

	const QDomDocument document = xmlUtils::loadDocument(filePath);
	for (QDomElement child = document.documentElement().firstChildElement("info")
			; !child.isNull()
			; child = child.nextSiblingElement("info"))
	{
		metaInfo[child.attribute("key")] = ValuesSerializer::deserializeQVariant(
				child.attribute("type"), child.attribute("value"));
	}
}

void Serializer::clearDir(const QString &path)
{
	if (path.isEmpty()) {
		return;
	}

	QDir dir(path);
	if (!dir.exists()) {
		return;
	}

	foreach (const QFileInfo &fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot)) {
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

QString Serializer::pathToElement(const Id &id) const
{
	QString dirName = mWorkingDir;

	QStringList partsList = id.toString().split('/');
	Q_ASSERT(partsList.size() >=1 && partsList.size() <= 5);
	for (int i = 1; i < partsList.size() - 1; ++i) {
		dirName += "/" + partsList[i];
	}

	return dirName + "/" + partsList[partsList.size() - 1];
}


QString Serializer::createDirectory(const Id &id, bool logical) const
{
	mSavedDirectories << QFileInfo(mWorkingDir).filePath();
	QString dirName = mWorkingDir + "/tree";
	mSavedDirectories << QFileInfo(dirName).filePath();
	dirName += logical ? "/logical" : "/graphical";
	mSavedDirectories << QFileInfo(dirName).filePath();

	const QStringList partsList = id.toString().split('/');
	Q_ASSERT(partsList.size() >= 1 && partsList.size() <= 5);
	for (int i = 1; i < partsList.size() - 1; ++i) {
		dirName += "/" + partsList[i];
		QDir dir(dirName);
		if (!dir.exists()) {
			dir.mkpath(dirName);
		}
		mSavedDirectories << QFileInfo(dirName).filePath();
	}

	QDir dir;
	dir.rmdir(mWorkingDir);
	dir.mkpath(dirName);
	return dirName + "/" + partsList[partsList.size() - 1];
}

bool Serializer::removeUnsaved(const QString &path) const
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
			bool invocationResult = true;
			if (mSavedDirectories.contains(fileInfo.filePath())) {
				invocationResult = removeUnsaved(fileInfo.filePath());
			} else {
				reportRemovedList.push_back(fileInfo.filePath());
				clearDir(fileInfo.absoluteFilePath());
				dir.rmdir(fileInfo.baseName());
			}
			result = result & invocationResult;
		} else {
			if (!mSavedFiles.contains(fileInfo.filePath())) {
				reportRemovedList.push_back(fileInfo.filePath());
			}
		}
	}
	return result;
}

void Serializer::decompressFile(const QString &fileName) const
{
	FolderCompressor::decompressFolder(fileName, mWorkingDir);
}
