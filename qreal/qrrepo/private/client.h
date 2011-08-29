#pragma once

#include "../../qrgui/kernel/definitions.h"
#include "../../qrgui/kernel/ids.h"
#include "classes/object.h"
#include "qrRepoGlobal.h"
#include "serializer.h"
#include "externalClient.h"

#include <QHash>

namespace qrRepo {

	namespace details {

		class Client
		{
		public:
			QRREPO_EXPORT Client(QString const &workingDirectory);
			QRREPO_EXPORT ~Client();
			qReal::IdList children(const qReal::Id &id) const;
			qReal::Id parent(const qReal::Id &id) const;
			void setParent(const qReal::Id &id, const qReal::Id &parent);
			void addChild(const qReal::Id &id, const qReal::Id &child);
			void addChild(const qReal::Id &id, const qReal::Id &child, qReal::Id const &logicalId);
			void removeParent(const qReal::Id &id);
			void removeChild(const qReal::Id &id, const qReal::Id &child);
			void setProperty(const qReal::Id &id, const QString &name, const QVariant &value);
			QVariant property(const qReal::Id &id, const QString &name) const;
			void removeProperty(const qReal::Id &id, const QString &name);
			bool hasProperty(const qReal::Id &id, const QString &name) const;
			QMapIterator<QString, QVariant> propertiesIterator(const qReal::Id &id) const;
			void remove(const qReal::Id &id);
			void setTemporaryRemovedLinks(qReal::Id const &id, QString const &direction, qReal::IdList const &linkIdList);
			qReal::IdList temporaryRemovedLinksAt(qReal::Id const &id, QString const &direction) const;
			qReal::IdList temporaryRemovedLinks(qReal::Id const &id) const;
			void removeTemporaryRemovedLinks(qReal::Id const &id);

			qReal::IdList elements() const;
			bool isLogicalId(qReal::Id const &elem) const;
			qReal::Id logicalId(qReal::Id const &elem) const;

			bool svnCheckout(QString const &from, QString const &to);
			bool svnUpdate(QString const &to);
			bool svnCommit(QString const &from, QString const &message);
			bool svnCleanUp(QString const &workingDir);
			QString svnInfo(QString const &workingDir);
			QString repoUrl(const QString &workingDir);
			int currentRevision(QString const &workingDir);

			void printDebug() const;

			bool exterminate();
			void open(QString const &workingDir);

			bool exist(qReal::Id const &id) const;

			bool saveAll();
			bool save(qReal::IdList list);
			void remove(qReal::IdList list) const;
			void setWorkingDir(QString const &workingDir);

			QStringList newErrors();

		private:
			void init();

			void loadFromDisk();
			void addChildrenToRootObject();

			qReal::IdList idsOfAllChildrenOf(qReal::Id id) const;
			QList<Object*> allChildrenOf(qReal::Id id) const;

			QHash<qReal::Id, Object*> mObjects;
			ExternalClient mExternalClient;
			Serializer serializer;
			QStringList mErrors;
		};

	}

}
