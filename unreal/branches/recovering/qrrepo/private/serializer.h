#pragma once

#include "../../qrgui/kernel/roles.h"
#include "classes/logicObject.h"

#include <QtXml/QDomDocument>
#include <QtCore/QVariant>

#include <QFile>

namespace qrRepo {

	namespace details {

		class Serializer {
		public:
			Serializer(QString const& saveDirName, bool failSafeMode);
			void clearWorkingDir() const;
			void setWorkingDir(QString const& workingDir);

			void removeFromDisk(qReal::Id id) const;
			void saveToDisk(QList<LogicObject*> const &objects) const;
			void loadFromDisk(QHash<qReal::Id, LogicObject*> &objectsHash);

			void logRemove(qReal::Id const diagram);
			void log(QString const message, qReal::Id const diagram);
		private:
			void loadFromDisk(QString const &currentPath, QHash<qReal::Id, LogicObject*> &objectsHash);

			QString pathToElement(qReal::Id const &id) const;
			QString createDirectory(qReal::Id const &id) const;

			LogicObject *parseLogicObject(QDomElement const &elem);
			static void clearDir(QString const &path);
			static QVariant parseValue(QString const &typeName, QString const &valueStr);
			static qReal::IdList loadIdList(QDomElement const &elem, QString const &name);
			static bool loadProperties(QDomElement const &elem, LogicObject &object);
			static QPointF parsePointF(QString const &str);

			static QString serializeQVariant(QVariant const &v);
			static QString serializeQPointF(QPointF const &p);
			static QString serializeQPolygon(QPolygon const &p);
			static QDomElement idListToXml(QString const &attributeName, qReal::IdList const &idList, QDomDocument &doc);
			static QDomElement propertiesToXml(LogicObject * const object, QDomDocument &doc);

			QString mWorkingDir;
			bool const mFailSafe;

			QMap<QString, QFile*> files;
		};

	}

}
