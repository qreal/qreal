#pragma once

#include "../../qrgui/kernel/roles.h"
#include "classes/object.h"
#include "externalClient.h"

#include <QtXml/QDomDocument>
#include <QtCore/QVariant>

#include <QtCore/QFile>
#include <QtCore/QDir>

namespace qrRepo {

	namespace details {

		class Serializer {
		public:
			Serializer(QString const& saveDirName);
			void setWorkingDir(QString const& workingDir);
			void clearWorkingDir();

			void setVersioningClient(SerializerVersioningInterface *client);

			void removeFromDisk(qReal::Id id) const;
			bool saveToDisk(QList<Object*> const &objects);
			void loadFromDisk(QHash<qReal::Id, Object*> &objectsHash);
			QStringList newErrors();
		private:
			void loadFromDisk(QString const &currentPath, QHash<qReal::Id, Object*> &objectsHash);
			void loadModel(QDir const &dir, QHash<qReal::Id, Object*> &objectsHash);

			QString pathToElement(qReal::Id const &id) const;
			QString createDirectory(qReal::Id const &id, qReal::Id const &logicalId);
			bool removeUnsaved(QString const &path);

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
			static QDomElement propertiesToXml(Object * const object, QDomDocument &doc);

			QString mWorkingDir;

			SerializerVersioningInterface *mVersioningClient;
			QSet<QString> mSavedFiles;
			QSet<QString> mSavedDirectories;
			QMap<QString, QFile*> files;

			QStringList mErrors;
		};

	}

}
