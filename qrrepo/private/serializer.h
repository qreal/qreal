#pragma once

#include <QtXml/QDomDocument>
#include <QtCore/QVariant>
#include <QtCore/QFile>
#include <QtCore/QDir>

#include "../../qrkernel/roles.h"
#include "classes/object.h"

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

	void exportToXml(QString const &targetFile, QHash<qReal::Id, Object*> const &objects) const;

	void decompressFile(QString const &fileName);

private:
	void loadFromDisk(QString const &currentPath, QHash<qReal::Id, Object *> &objectsHash);
	void loadModel(QDir const &dir, QHash<qReal::Id, Object *> &objectsHash);

	QString pathToElement(qReal::Id const &id) const;
	QString createDirectory(qReal::Id const &id, bool logical) const;

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
	static QDomElement createPropertiesXmlElement(Object const *object, QDomDocument &doc, Object const *logicalObject = NULL);
	static void getSingleElementProperties(QDomElement &result, Object const *object, QDomDocument &doc);

	static void exportDiagram(qReal::Id const &diagramId, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects);
	static void exportElement(qReal::Id const &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects);
	static void exportChildren(qReal::Id const &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects);
	static void exportProperties(qReal::Id const &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects);


	QString mWorkingDir;
	QString mWorkingFile;

	QMap<QString, QFile*> files;
};

}
}
