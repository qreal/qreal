#pragma once

#include "../qrgui/kernel/roles.h"
#include "private/client.h"
#include "private/qrRepoGlobal.h"
#include "repoControlInterface.h"
//#include "commonRepoApi.h"
#include "graphicalRepoApi.h"
#include "logicalRepoApi.h"

#include <QSet>

namespace qrRepo {

	class QRREPO_EXPORT RepoApi : public GraphicalRepoApi, public LogicalRepoApi, public RepoControlInterface
	{
	public:
		explicit RepoApi(QString const &workingDirectory);
		// Default destructor ok.

		QString name(qReal::Id const &id) const;
		void setName(qReal::Id const &id, QString const &name);

		qReal::IdList children(qReal::Id const &id) const;
		void addChild(qReal::Id const &id, qReal::Id const &child);
		virtual void addChild(qReal::Id const &id, qReal::Id const &logicalId, qReal::Id const &child);
		void removeChild(qReal::Id const &id, qReal::Id const &child);
		void removeChildren(qReal::Id const &id);

		void removeElement(qReal::Id const &id);

		qReal::IdList parents(qReal::Id const &id) const;
		void addParent(qReal::Id const &id, qReal::Id const &parent);
		void removeParent(qReal::Id const &id, qReal::Id const &parent);

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

		QString typeName(qReal::Id const &id) const;

		QVariant property(qReal::Id const &id, QString const &propertyName) const;
		QString stringProperty(qReal::Id const &id, QString const &propertyName) const;
		void setProperty(qReal::Id const &id, QString const &propertyName, QVariant const &value);
		void removeProperty(qReal::Id const &id, QString const &propertyName);
		bool hasProperty(qReal::Id const &id, QString const &propertyName) const;

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

		virtual void setPosition(qReal::Id const &id, QVariant const &position) const;
		virtual void setConfiguration(qReal::Id const &id, QVariant const &configuration) const;

		qReal::Id otherEntityFromLink(qReal::Id const &linkId, qReal::Id const &firstNode) const;

		void exterminate();

		void saveAll() const;
		void save(qReal::IdList list) const;
		void saveTo(QString const &workingDir);
		void remove(qReal::IdList list) const;

		void open(QString const &workingDir);

		void log(QString const message, qReal::Id const diagram);

		// "Глобальные" методы, позволяющие делать запросы к модели в целом.
		//Returns all elements with .element() == type.element()
		qReal::IdList elements(qReal::Id const &type) const;

		//Returns all elements with .element() == type
		qReal::IdList elementsByType(QString const &type) const;
		int elementsCount() const;

		bool exist(qReal::Id const &id) const;

		qReal::IdList getOpenedDiagrams() const;
		qReal::IdList getChangedDiagrams() const;
		void resetChangedDiagrams();
		void addOpenedDiagram(const qReal::Id &id);
		void addChangedDiagram(const qReal::Id &id);
		void resetChangedDiagrams(const qReal::IdList &list);

	private:
		RepoApi(RepoApi const &other);  // Копировать нельзя.
		RepoApi& operator =(RepoApi const &);  // Присваивать тоже.

		void addToIdList(qReal::Id const &target, QString const &listName, qReal::Id const &data);
		void removeFromList(qReal::Id const &target, QString const &listName, qReal::Id const &data);

		qReal::IdList links(qReal::Id const &id, QString const &direction) const;
		void removeLinkEnds(QString const &endName, qReal::Id const &id);

		QSet<qReal::Id> mDiagramsOpened;
		QSet<qReal::Id> mDiagramsChanged;

		typedef QPair<qReal::Id, qReal::IdList> ChildsOfDiagram;
		QList<ChildsOfDiagram> mDiagramsDeleted;

		details::Client mClient;
	};

}
