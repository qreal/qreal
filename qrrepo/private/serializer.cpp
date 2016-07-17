/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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
#include <QtCore/QCoreApplication>
#include <QtCore/QUuid>
#include <QtCore/QFileInfo>

#include <qrkernel/platformInfo.h>
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

const QString unsavedDir = "%1/unsaved/%2";

Serializer::Serializer(const QString &workingFile)
	// Syncroniously running instances of QReal can clear temp dirs of each other.
	// So generating new UUID as temp dir name.
	: mWorkingDir(unsavedDir.arg(PlatformInfo::invariantSettingsPath("pathToTempFolder")
			, QUuid::createUuid().toString()))
	, mWorkingFile(workingFile)
{
	clearWorkingDir();
	QDir dir(PlatformInfo::applicationDirPath());
	if (!dir.cd(mWorkingDir)) {
		QDir().mkdir(mWorkingDir);
	}
}

Serializer::~Serializer()
{
	clearWorkingDir();
}

QString Serializer::workingDirectory() const
{
	return mWorkingDir;
}

void Serializer::clearWorkingDir() const
{
	FileSystemUtils::clearDir(mWorkingDir);
}

void Serializer::removeFromDisk(const Id &id) const
{
	QDir().remove(pathToElement(id));
}

void Serializer::setWorkingFile(const QString &workingFile)
{
	mWorkingFile = workingFile;
}

void Serializer::saveToDisk(QList<Object *> const &objects, QHash<QString, QVariant> const &metaInfo) const
{
	Q_ASSERT_X(!mWorkingFile.isEmpty()
		, "Serializer::saveToDisk(...)"
		, "may be Repository of RepoApi (see Models constructor also) has been initialised with empty filename?");

	foreach (const Object * const object, objects) {
		const QString filePath = createDirectory(object->id(), object->isLogicalObject());

		QDomDocument doc;
		QDomElement root = object->serialize(doc);
		doc.appendChild(root);

		OutFile out(filePath);
		doc.save(out(), 2);
	}

	saveMetaInfo(metaInfo);

	const QFileInfo fileInfo(mWorkingFile);
	const QString fileName = fileInfo.completeBaseName();

	const QDir compressDir(mWorkingDir);
	const QDir dir = fileInfo.absolutePath();

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

	clearWorkingDir();
}

void Serializer::loadFromDisk(QHash<qReal::Id, Object*> &objectsHash, QHash<QString, QVariant> &metaInfo)
{
	clearWorkingDir();
	if (QFileInfo::exists(mWorkingFile)) {
		decompressFile(mWorkingFile);
	}

	loadFromDisk(mWorkingDir, objectsHash);
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

void Serializer::saveMetaInfo(QHash<QString, QVariant> const &metaInfo) const
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
	QString dirName = mWorkingDir + "/tree";
	dirName += logical ? "/logical" : "/graphical";

	const QStringList partsList = id.toString().split('/');
	Q_ASSERT(partsList.size() >= 1 && partsList.size() <= 5);
	for (int i = 1; i < partsList.size() - 1; ++i) {
		dirName += "/" + partsList[i];
	}

	QDir dir;
	dir.rmdir(mWorkingDir);
	dir.mkpath(dirName);

	return dirName + "/" + partsList[partsList.size() - 1];
}

void Serializer::decompressFile(const QString &fileName)
{
	FolderCompressor::decompressFolder(fileName, mWorkingDir);
}
