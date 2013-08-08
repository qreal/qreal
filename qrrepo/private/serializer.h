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

class Serializer
{
public:
	Serializer(QString const &saveDirName);
	void clearWorkingDir() const;
	void setWorkingFile(QString const &workingDir);

	void removeFromDisk(qReal::Id id) const;
	void saveToDisk(QList<Object *> const &objects) const;
	void loadFromDisk(QHash<qReal::Id, Object *> &objectsHash);

//	void exportToXml(QString const &targetFile, QHash<qReal::Id, Object*> const &objects) const;

	void decompressFile(QString const &fileName);

private:
	void loadFromDisk(QString const &currentPath, QHash<qReal::Id, Object *> &objectsHash);
	void loadModel(QDir const &dir, QHash<qReal::Id, Object *> &objectsHash);

	QString pathToElement(qReal::Id const &id) const;
	QString createDirectory(qReal::Id const &id, bool logical) const;

	Object *parseObject(QDomElement const &elem);
	static void clearDir(QString const &path);
	static bool loadProperties(QDomElement const &elem, Object &object);

	QString mWorkingDir;
	QString mWorkingFile;

	QMap<QString, QFile*> files;
};

}
}
