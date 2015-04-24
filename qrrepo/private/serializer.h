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

#pragma once

#include <QtXml/QDomDocument>
#include <QtCore/QVariant>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QList>

#include <qrkernel/roles.h>

#include "qrrepo/workingCopyInspectionInterface.h"
#include "classes/object.h"
#include "valuesSerializer.h"


namespace qrRepo {

namespace details {

/// Class that is responsible for saving repository contents to disk as .qrs file.
class Serializer
{
public:
	Serializer(const QString &saveDirName);

	void clearWorkingDir() const;
	void setWorkingFile(const QString &workingFile);

	void setWorkingCopyInspector(WorkingCopyInspectionInterface *inspector);

	void removeFromDisk(qReal::Id const &id) const;
	void saveToDisk(QList<Object *> const &objects, QHash<QString, QVariant> const &metaInfo) const;
	void loadFromDisk(QHash<qReal::Id, Object *> &objectsHash, QHash<QString, QVariant> &metaInfo);

	void prepareWorkingCopy(const QString &workingCopyPath, QString const &sourceProject = QString());
	void processWorkingCopy(const QString &workingCopyPath, QString const &targetProject = QString());

	void decompressFile(QString const &fileName) const;

private:
	static void clearDir(const QString &path);

	void loadFromDisk(const QString &currentPath, QHash<qReal::Id, Object *> &objectsHash);
	void loadModel(const QDir &dir, QHash<qReal::Id, Object *> &objectsHash);

	void saveMetaInfo(QHash<QString, QVariant> const &metaInfo) const;
	void loadMetaInfo(QHash<QString, QVariant> &metaInfo) const;

	QString pathToElement(const qReal::Id &id) const;
	QString createDirectory(const qReal::Id &id, bool logical) const;


	bool removeUnsaved(QString const &path) const;

	bool prepareSaving() const;

	bool reportAdded() const;
	bool reportRemoved() const;
	bool reportChanged() const;

	QString mWorkingDir;
	QString mWorkingFile;

	WorkingCopyInspectionInterface *mWorkingCopyInspector;
	mutable QSet<QString> mSavedFiles;
	mutable QSet<QString> mSavedDirectories;
	mutable QMap<QString, QFile *> mFiles;

	mutable QList<QString > reportAddedList;
	mutable QList<QString> reportChangedList;
	mutable QList<QString> reportRemovedList;
};

}
}
