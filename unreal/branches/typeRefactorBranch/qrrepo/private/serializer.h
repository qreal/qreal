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

                        void removeFromDisk(qReal::NewType type) const;
			void saveToDisk(QList<LogicObject*> const &objects) const;
                        void loadFromDisk(QHash<qReal::NewType, LogicObject*> &objectsHash);

                        void log(QString const message, qReal::NewType const diagram);
		private:
                        void loadFromDisk(QString const &currentPath, QHash<qReal::NewType, LogicObject*> &objectsHash);

                        QString pathToElement(qReal::NewType const &type) const;
                        QString createDirectory(qReal::NewType const &type) const;

			LogicObject *parseLogicObject(QDomElement const &elem);
			static void clearDir(QString const &path);
			static QVariant parseValue(QString const &typeName, QString const &valueStr);
                        static qReal::TypeList loadTypeList(QDomElement const &elem, QString const &name);
			static bool loadProperties(QDomElement const &elem, LogicObject &object);
			static QPointF parsePointF(QString const &str);

			static QString serializeQVariant(QVariant const &v);
			static QString serializeQPointF(QPointF const &p);
			static QString serializeQPolygon(QPolygon const &p);
                        static QDomElement typeListToXml(QString const &attributeName, qReal::TypeList const &typeList, QDomDocument &doc);
			static QDomElement propertiesToXml(LogicObject * const object, QDomDocument &doc);

			QString mWorkingDir;
			bool const mFailSafe;

			QMap<QString, QFile*> files;
		};

	}

}
