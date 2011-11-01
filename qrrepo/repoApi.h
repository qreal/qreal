#pragma once

#include "../qrkernel/roles.h"
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

		qReal::Id copy(qReal::Id const &src);

		QString name(qReal::Id const &id) const;
		void setName(qReal::Id const &id, QString const &name);

		qReal::IdList children(qReal::Id const &id) const;
		virtual void addChild(qReal::Id const &id, qReal::Id const &child);
		virtual void addChild(qReal::Id const &id, qReal::Id const &child, qReal::Id const &logicalId);
		void removeChild(qReal::Id const &id, qReal::Id const &child);
		void removeChildren(qReal::Id const &id);

		void removeElement(qReal::Id const &id);

		qReal::Id parent(qReal::Id const &id) const;
		void setParent(qReal::Id const &id, qReal::Id const &parent);

		virtual void stackBefore(qReal::Id const &id, qReal::Id const &child, qReal::Id const &sibling);

		qReal::IdList outgoingLinks(qReal::Id const &id) const;
		qReal::IdList incomingLinks(qReal::Id const &id) const;
		qReal::IdList links(qReal::Id const &id) const;

		qReal::IdList outgoingConnections(qReal::Id const &id) const;
		qReal::IdList incomingConnections(qReal::Id const &id) const;
		void connect(qReal::Id const &source, qReal::Id const &destination);
		void disconnect(qReal::Id const &source, qReal::Id const &destination);

		qReal::IdList outgoingUsages(qReal::Id const &id) const;
		qReal::IdList incomingUsages(qReal::Id const &id) const;
		void addUsage(qReal::Id const &source, qReal::Id const &destination);
		void deleteUsage(qReal::Id const &source, qReal::Id const &destination);

		qReal::IdList connectedElements(qReal::Id const &id) const;
		qReal::IdList outgoingConnectedElements(qReal::Id const &id) const;
		qReal::IdList incomingConnectedElements(qReal::Id const &id) const;

		QString typeName(qReal::Id const &id) const;

		QVariant property(qReal::Id const &id, QString const &propertyName) const;
		QString stringProperty(qReal::Id const &id, QString const &propertyName) const;
		void setProperty(qReal::Id const &id, QString const &propertyName, QVariant const &value);
		void removeProperty(qReal::Id const &id, QString const &propertyName);
		void copyProperties(const qReal::Id &dest, const qReal::Id &src);
		bool hasProperty(qReal::Id const &id, QString const &propertyName) const;

		qReal::IdList temporaryRemovedLinksAt(qReal::Id const &id, QString const &direction) const;
		void setTemporaryRemovedLinks(qReal::Id const &id, qReal::IdList const &value, QString const &direction);
		void removeTemporaryRemovedLinks(qReal::Id const &id);

		qReal::Id from(qReal::Id const &id) const;
		void setFrom(qReal::Id const &id, qReal::Id const &from);

		qReal::Id to(qReal::Id const &id) const;
		void setTo(qReal::Id const &id, qReal::Id const &to);

		double fromPort(qReal::Id const &id) const;
		void setFromPort(qReal::Id const &id, double fromPort);

		double toPort(qReal::Id const &id) const;
		void setToPort(qReal::Id const &id, double toPort);

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

		bool exist(qReal::Id const &id) const;

	private:
		RepoApi(RepoApi const &other);  // Копировать нельзя.
		RepoApi& operator =(RepoApi const &);  // Присваивать тоже.

		void addToIdList(qReal::Id const &target, QString const &listName, qReal::Id const &data, QString const &direction = QString());
		void removeFromList(qReal::Id const &target, QString const &listName, qReal::Id const &data, QString const &direction = QString());

		qReal::IdList links(qReal::Id const &id, QString const &direction) const;
		void removeLinkEnds(QString const &endName, qReal::Id const &id);

		details::Client mClient;
	};

}
