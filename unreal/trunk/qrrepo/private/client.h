#pragma once

#include "../../qrgui/kernel/definitions.h"
#include "classes/logicObject.h"
#include "qrRepoGlobal.h"

#include <QVariant>
#include <QHash>
#include <QtXml/QDomDocument>

namespace qrRepo {

	namespace details {

		QString const saveDirName = "./save";

		class Client
		{
		public:
			QRREPO_EXPORT Client();
			QRREPO_EXPORT ~Client();
			qReal::IdList children(const qReal::Id &id) const;
			qReal::IdList parents(const qReal::Id &id) const;
			void addParent(const qReal::Id &id, const qReal::Id &parent);
			void addChild(const qReal::Id &id, const qReal::Id &child);
			void removeParent(const qReal::Id &id, const qReal::Id &parent);
			void removeChild(const qReal::Id &id, const qReal::Id &child);
			void setProperty(const qReal::Id &id, const QString &name, const QVariant &value);
			QVariant property(const qReal::Id &id, const QString &name) const;
			void removeProperty(const qReal::Id &id, const QString &name);
			bool hasProperty(const qReal::Id &id, const QString &name) const;
			void svnUpdate();
			void svnCommit();

			void printDebug() const;

			void exterminate();
			void save() const;

		private:
			void loadFromDisk();
			void saveToDisk() const;
			void init();

			// TODO: Вынести всё, что относится к сериализации, в отдельный класс,
			// как только это переложат в qrgui.
			void loadFromDisk(QString const &currentPath);
			QDomDocument loadXmlDocument(QString const &path);
			void addChildrenToRootObject();

			static LogicObject *parseLogicObject(QDomElement const &elem);
			static QVariant parseValue(QString const &typeName, QString const &valueStr);
			static qReal::IdList loadIdList(QDomElement const &elem, QString const &name);
			static bool loadProperties(QDomElement const &elem, LogicObject &object);
			static QPointF parsePointF(QString const &str);

			static void clearDir(QString const &path);
			static QString serializeQVariant(QVariant const &v);
			static QString serializeQPointF(QPointF const &p);
			static QString serializeQPolygon(QPolygon const &p);
			static QString createDirectory(qReal::Id const &id);
			static QDomElement idListToXml(QString const &attributeName, qReal::IdList const &idList, QDomDocument &doc);
			static QDomElement propertiesToXml(LogicObject * const object, QDomDocument &doc);

			QHash<qReal::Id, LogicObject*> mObjects;
		};

	}

}
