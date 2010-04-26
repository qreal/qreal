#pragma once

#include "../../qrgui/kernel/definitions.h"
#include "classes/logicObject.h"

#include <QtXml/QDomDocument>
#include <QtCore/QVariant>

namespace qrRepo {

	namespace details {

		class Serializer {
		public:
			Serializer(QString const& saveDirName, bool failSafeMode);
			void setWorkingDir(QString const& workingDir);
			void loadFromDisk(QHash<qReal::Id, LogicObject*> &objectsHash);
			void saveToDisk(QList<LogicObject*> const &objects) const;

		private:
			void loadFromDisk(QString const &currentPath, QHash<qReal::Id, LogicObject*> &objectsHash);

			QString createDirectory(qReal::Id const &id) const;

			LogicObject *parseLogicObject(QDomElement const &elem);
			static QVariant parseValue(QString const &typeName, QString const &valueStr);
			static qReal::IdList loadIdList(QDomElement const &elem, QString const &name);
			static bool loadProperties(QDomElement const &elem, LogicObject &object);
			static QPointF parsePointF(QString const &str);

			static void clearDir(QString const &path);
			static QString serializeQVariant(QVariant const &v);
			static QString serializeQPointF(QPointF const &p);
			static QString serializeQPolygon(QPolygon const &p);
			static QDomElement idListToXml(QString const &attributeName, qReal::IdList const &idList, QDomDocument &doc);
			static QDomElement propertiesToXml(LogicObject * const object, QDomDocument &doc);

			QString mWorkingDir;
			bool const mFailSafe;
		};

	}

}
