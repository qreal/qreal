#pragma once

#include "../../qrgui/kernel/definitions.h"
#include "../../qrgui/kernel/ids.h"
#include "classes/logicObject.h"
#include "qrRepoGlobal.h"
#include "serializer.h"

#include <QHash>

namespace qrRepo {

	namespace details {

		class Client
		{
		public:
			QRREPO_EXPORT Client(QString const &workingDirectory);
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

			qReal::IdList elements() const;

			void svnUpdate();
			void svnCommit();

			void printDebug() const;

			void exterminate();
			void open(QString const &workingDir);

			bool exist(qReal::Id const &id) const;


			void saveAll() const;
			void save(qReal::IdList list) const;
			void remove(qReal::IdList list) const;

			QString getWorkingDir() const;
			void setWorkingDir(QString const &workingDir);

			qReal::IdList idsOfAllChildrenOf(qReal::Id id) const;
			QList<LogicObject*> allChildrenOf(qReal::Id id) const;
		private:
			void init();

			void loadFromDisk();
			void addChildrenToRootObject();

			QHash<qReal::Id, LogicObject*> mObjects;
			Serializer serializer;
		};

	}

}
