#pragma once

#include "../../qrkernel/roles.h"
#include "classes/object.h"

#include <QtXml/QDomDocument>
#include <QtCore/QVariant>

#include <QtCore/QFile>
#include <QtCore/QDir>

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

	void decompressFile(QString const &fileName);

private:
	void loadFromDisk(QString const &currentPath, QHash<qReal::Id, Object *> &objectsHash);
	void loadModel(QDir const &dir, QHash<qReal::Id, Object *> &objectsHash);

	QString pathToElement(qReal::Id const &id) const;
	QString createDirectory(qReal::Id const &id, qReal::Id const &logicalId) const;

	Object *parseObject(QDomElement const &elem);
	static void clearDir(QString const &path);
	static QVariant parseValue(QString const &typeName, QString const &valueStr);
	static qReal::IdList loadIdList(QDomElement const &elem, QString const &name);
	static qReal::Id loadId(QString const &elementStr);
	static bool loadProperties(QDomElement const &elem, Object &object);

	static QString serializeQVariant(QVariant const &v);
	static QString serializeQPointF(QPointF const &p);
	static QString serializeQPolygon(QPolygon const &p);
	static QString serializeQRect(QRect const &rect);
	static QString serializeQRectF(QRectF const &rect);
	static QString serializeQSize(QSize const &size);
	static QString serializeQSizeF(QSizeF const &size);

	static QPointF parsePointF(QString const &str);
	static QPolygon deserializeQPolygon(QString const &polygon);
	static QRect deserializeQRect(QString const &rect);
	static QRectF deserializeQRectF(QString const &rect);
	static QSize deserializeQSize(QString const &size);
	static QSizeF deserializeQSizeF(QString const &size);

	static QDomElement idListToXml(QString const &attributeName, qReal::IdList const &idList, QDomDocument &doc);
	static QDomElement propertiesToXml(Object const *object, QDomDocument &doc);

	QString mWorkingDir;
	QString mWorkingFile;

	QMap<QString, QFile*> files;
};

}
}
