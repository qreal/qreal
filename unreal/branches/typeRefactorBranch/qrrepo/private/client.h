#pragma once

#include "../../qrgui/kernel/definitions.h"
#include "../../qrgui/kernel/NewType.h"
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
                        qReal::TypeList children(const qReal::NewType &type) const;
                        qReal::TypeList parents(const qReal::NewType &type) const;
                        void addParent(const qReal::NewType &type, const qReal::NewType &parent);
                        void addChild(const qReal::NewType &type, const qReal::NewType &child);
                        void removeParent(const qReal::NewType &type, const qReal::NewType &parent);
                        void removeChild(const qReal::NewType &type, const qReal::NewType &child);
                        void setProperty(const qReal::NewType &type, const QString &name, const QVariant &value);
                        QVariant property(const qReal::NewType &type, const QString &name) const;
                        void removeProperty(const qReal::NewType &type, const QString &name);
                        bool hasProperty(const qReal::NewType &type, const QString &name) const;

                        qReal::TypeList elements() const;

			void svnUpdate();
			void svnCommit();

			void printDebug() const;

			void exterminate();
			void open(QString const &workingDir);

                        bool exist(qReal::NewType const &type) const;


			void saveAll() const;
                        void save(qReal::TypeList list) const;
                        void remove(qReal::TypeList list) const;
			void setWorkingDir(QString const &workingDir);
                        void log(QString const message, const qReal::NewType diagram);

                        qReal::TypeList typesOfAllChildrenOf(qReal::NewType type) const;
                        QList<LogicObject*> allChildrenOf(qReal::NewType type) const;
		private:
			void init();

			void loadFromDisk();
			void addChildrenToRootObject();

                        QHash<qReal::NewType, LogicObject*> mObjects;
			Serializer serializer;
		};

	}

}
