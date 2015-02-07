#pragma once

#include <QtXml/QDomDocument>
#include <QtCore/QVariant>
#include <QtCore/QFile>
#include <QtCore/QDir>

#include "../../qrkernel/roles.h"
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

	void removeFromDisk(const qReal::Id &id) const;
	void saveToDisk(QList<Object *> const &objects, QHash<QString, QVariant> const &metaInfo) const;
	void loadFromDisk(QHash<qReal::Id, Object *> &objectsHash, QHash<QString, QVariant> &metaInfo);

	void decompressFile(const QString &fileName);

private:
	static void clearDir(const QString &path);

	void loadFromDisk(const QString &currentPath, QHash<qReal::Id, Object *> &objectsHash);
	void loadModel(const QDir &dir, QHash<qReal::Id, Object *> &objectsHash);

	void saveMetaInfo(QHash<QString, QVariant> const &metaInfo) const;
	void loadMetaInfo(QHash<QString, QVariant> &metaInfo) const;

	QString pathToElement(const qReal::Id &id) const;
	QString createDirectory(const qReal::Id &id, bool logical) const;

	QString mWorkingDir;
	QString mWorkingFile;
};

}
}
