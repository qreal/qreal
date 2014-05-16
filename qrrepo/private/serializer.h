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
	Serializer(QString const &saveDirName);

	void clearWorkingDir() const;
	void setWorkingFile(QString const &workingFile);

	void removeFromDisk(qReal::Id const &id) const;
	void saveToDisk(QList<Object *> const &objects, QHash<QString, QVariant> const &metaInfo) const;
	void loadFromDisk(QHash<qReal::Id, Object *> &objectsHash, QHash<QString, QVariant> &metaInfo);

	void decompressFile(QString const &fileName);

private:
	static void clearDir(QString const &path);

	void loadFromDisk(QString const &currentPath, QHash<qReal::Id, Object *> &objectsHash);
	void loadModel(QDir const &dir, QHash<qReal::Id, Object *> &objectsHash);

	void saveMetaInfo(QHash<QString, QVariant> const &metaInfo) const;
	void loadMetaInfo(QHash<QString, QVariant> &metaInfo) const;

	QString pathToElement(qReal::Id const &id) const;
	QString createDirectory(qReal::Id const &id, bool logical) const;

	QString mWorkingDir;
	QString mWorkingFile;
};

}
}
