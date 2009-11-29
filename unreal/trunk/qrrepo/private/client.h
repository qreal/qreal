#pragma once

#include "../../qrgui/kernel/definitions.h"
#include "classes/logicObject.h"

#include <QVariant>
#include <QHash>
#include <QtXml/QDomDocument>

namespace qReal {

	namespace client {

		namespace details {

			const QString saveDirName = "./save";

			class Client
			{
			public:
				Client();
				~Client();
				IdList children(const Id &id) const;
				IdList parents(const Id &id) const;
				void addParent(const Id &id, const Id &parent);
				void addChild(const Id &id, const Id &child);
				void removeParent(const Id &id, const Id &parent);
				void removeChild(const Id &id, const Id &child);
				void setProperty(const Id &id, const QString &name, const QVariant &value);
				QVariant property(const Id &id, const QString &name) const;
				void removeProperty(const Id &id, const QString &name);
				bool hasProperty(const Id &id, const QString &name) const;
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
				static IdList loadIdList(QDomElement const &elem, QString const &name);
				static bool loadProperties(QDomElement const &elem, LogicObject &object);
				static QPointF parsePointF(QString const &str);

				static void clearDir(QString const &path);
				static QString serializeQVariant(QVariant const &v);
				static QString serializeQPointF(QPointF const &p);
				static QString serializeQPolygon(QPolygon const &p);
				static QString createDirectory(Id const &id);
				static QDomElement idListToXml(QString const &attributeName, IdList const &idList, QDomDocument &doc);
				static QDomElement propertiesToXml(LogicObject * const object, QDomDocument &doc);

				QHash<Id, LogicObject*> mObjects;
			};

		}

	}

}
