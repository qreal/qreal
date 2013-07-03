#pragma once

#include <QtXml/QDomDocument>
#include <QtCore/QVariant>
#include <QtCore/QFile>
#include <QtCore/QDir>

#include "../../qrkernel/roles.h"
#include "../workingCopyInspectionInterface.h"
#include "classes/object.h"

namespace qrRepo {

namespace details {

class Serializer
{
public:
	Serializer(QString const& saveDirName);
	void clearWorkingDir();
	void setWorkingFile(QString const& workingDir);

	void setWorkingCopyInspector(WorkingCopyInspectionInterface *inspector);

	void removeFromDisk(qReal::Id id) const;
	void saveToDisk(QList<Object*> const &objects);
	void loadFromDisk(QHash<qReal::Id, Object*> &objectsHash);

	void prepareWorkingCopy(const QString &workingCopyPath, QString const &sourceProject = QString());
	void processWorkingCopy(const QString &workingCopyPath, QString const &targetProject = QString());

	void decompressFile(QString const &fileName);

private:
	void loadFromDisk(QString const &currentPath, QHash<qReal::Id, Object*> &objectsHash);
	void loadModel(QDir const &dir, QHash<qReal::Id, Object*> &objectsHash);

	QString pathToElement(qReal::Id const &id) const;
	QString createDirectory(qReal::Id const &id, qReal::Id const &logicalId);

	bool addSaved();
	bool removeUnsaved(QString const &path);

	void prepareSaving();

	bool reportAdded(QString const &fileName);
	bool reportRemoved(QString const &fileName);
	bool reportChanged(QString const &fileName);

	Object *parseObject(QDomElement const &elem);
	static void clearDir(QString const &path);
	static QVariant parseValue(QString const &typeName, QString const &valueStr);
	static qReal::IdList loadIdList(QDomElement const &elem, QString const &name);
	static qReal::Id loadId(QString const &elementStr);
	static bool loadProperties(QDomElement const &elem, Object &object);
	static QPointF parsePointF(QString const &str);

	static QString serializeQVariant(QVariant const &v);
	static QString serializeQPointF(QPointF const &p);
	static QString serializeQPolygon(QPolygon const &p);
	static QDomElement idListToXml(QString const &attributeName, qReal::IdList const &idList, QDomDocument &doc);
	static QDomElement propertiesToXml(Object const *object, QDomDocument &doc);


	QString mWorkingDir;
	QString mWorkingFile;

	WorkingCopyInspectionInterface *mWorkingCopyInspector;
	QSet<QString> mSavedFiles;
	QSet<QString> mSavedDirectories;
	QMap<QString, QFile*> mFiles;
};

}
}
