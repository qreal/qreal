#pragma once

#include "../qrgui/kernel/roles.h"
#include "../qrgui/kernel/NewType.h"
#include "private/client.h"
#include "private/qrRepoGlobal.h"
#include "repoControlInterface.h"
#include "commonRepoApi.h"
#include "graphicalRepoApi.h"
#include "logicalRepoApi.h"

#include <QtCore/QSet>

namespace qrRepo {

	class QRREPO_EXPORT RepoApi : public GraphicalRepoApi, public LogicalRepoApi, public RepoControlInterface
	{
	public:
		explicit RepoApi(QString const &workingDirectory);
		// Default destructor ok.

                QString name(qReal::Id const &type) const;
                void setName(qReal::Id const &id, QString const &name);

		qReal::IdList children(qReal::Id const &id) const;
		virtual void addChild(qReal::Id const &id, qReal::Id const &child);
		virtual void addChild(qReal::Id const &id, qReal::Id const &child, qReal::Id const &logicalId);
		void removeChild(qReal::Id const &id, qReal::Id const &child);
		void removeChildren(qReal::Id const &id);

                void removeElement(qReal::Id const &type);

		qReal::Id parent(qReal::Id const &id) const;
		void setParent(qReal::Id const &id, qReal::Id const &parent);

                qReal::IdList outgoingLinks(qReal::Id const &type) const;
                qReal::IdList incomingLinks(qReal::Id const &type) const;
                qReal::IdList links(qReal::Id const &id) const;

                qReal::IdList outgoingConnections(qReal::Id const &type) const;
                qReal::IdList incomingConnections(qReal::Id const &type) const;
                void connect(qReal::Id const &source, qReal::Id const &destination);
                void disconnect(qReal::Id const &source, qReal::Id const &destination);

                qReal::IdList outgoingUsages(qReal::Id const &type) const;
                qReal::IdList incomingUsages(qReal::Id const &type) const;
                void addUsage(qReal::Id const &source, qReal::Id const &destination);
                void deleteUsage(qReal::Id const &source, qReal::Id const &destination);

                QVariant property(qReal::Id const &id, QString const &propertyName) const;
                QString stringProperty(qReal::Id const &type, QString const &propertyName) const;
                void setProperty(qReal::Id const &type, QString const &propertyName, QVariant const &value);
                void removeProperty(qReal::Id const &type, QString const &propertyName);
                bool hasProperty(qReal::Id const &type, QString const &propertyName) const;

                qReal::Id from(qReal::Id const &type) const;
                void setFrom(qReal::Id const &type, qReal::Id const &from);

		qReal::IdList temporaryRemovedLinksAt(qReal::Id const &id, QString const &direction) const;
		void setTemporaryRemovedLinks(qReal::Id const &id, qReal::IdList const &value, QString const &direction);
		void removeTemporaryRemovedLinks(qReal::Id const &id);

		qReal::Id from(qReal::Id const &id) const;
		void setFrom(qReal::Id const &id, qReal::Id const &from);

                double fromPort(qReal::Id const &type) const;
                void setFromPort(qReal::Id const &type, double fromPort);

                double toPort(qReal::Id const &type) const;
                void setToPort(qReal::Id const &type, double toPort);

                qReal::Id otherEntityFromLink(qReal::Id const &linkId, qReal::Id const &firstNode) const;

		virtual QVariant position(qReal::Id const &id) const;
		virtual QVariant configuration(qReal::Id const &id) const;

		virtual void setPosition(qReal::Id const &id, QVariant const &position);
		virtual void setConfiguration(qReal::Id const &id, QVariant const &configuration);

		qReal::Id otherEntityFromLink(qReal::Id const &linkId, qReal::Id const &firstNode) const;

		virtual bool isLogicalElement(qReal::Id const &id) const;
		virtual bool isGraphicalElement(qReal::Id const &id) const;

		void exterminate();

		void saveAll() const;
                void save(qReal::IdList list) const;
		void saveTo(QString const &workingDir);

		void open(QString const &workingDir);

		// "Глобальные" методы, позволяющие делать запросы к модели в целом.
		//Returns all elements with .element() == type.element()
		virtual qReal::IdList graphicalElements(qReal::Id const &type) const;
		virtual qReal::IdList logicalElements(qReal::Id const &type) const;

		virtual qReal::Id logicalId(qReal::Id const &id) const;

		//Returns all elements with .element() == type
                qReal::IdList elementsByType(QString const &type) const;
		int elementsCount() const;

                bool exist(qReal::Id const &type) const;

	private:
		RepoApi(RepoApi const &other);  // Копировать нельзя.
		RepoApi& operator =(RepoApi const &);  // Присваивать тоже.

		void addToIdList(qReal::Id const &target, QString const &listName, qReal::Id const &data, QString const &direction = QString());
		void removeFromList(qReal::Id const &target, QString const &listName, qReal::Id const &data, QString const &direction = QString());

                qReal::IdList links(qReal::Id const &id, QString const &direction) const;
                void removeLinkEnds(QString const &endName, qReal::Id const &type);

		details::Client mClient;
	};

}
