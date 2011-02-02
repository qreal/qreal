#pragma once

#include "../qrgui/kernel/roles.h"
#include "../qrgui/kernel/NewType.h"
#include "private/client.h"
#include "private/qrRepoGlobal.h"

#include <QSet>

namespace qrRepo {

	class QRREPO_EXPORT RepoApi {
	public:
		explicit RepoApi(QString const &workingDirectory);
		// Default destructor ok.

                QString name(qReal::NewType const &type) const;
                void setName(qReal::NewType const &type, QString const &name);

                qReal::TypeList children(qReal::NewType const &type) const;
                void addChild(qReal::NewType const &type, qReal::NewType const &child);
                void removeChild(qReal::NewType const &type, qReal::NewType const &child);
                void removeChildren(qReal::NewType const &type);

                void removeElement(qReal::NewType const &type);

                qReal::TypeList parents(qReal::NewType const &type) const;
                void addParent(qReal::NewType const &type, qReal::NewType const &parent);
                void removeParent(qReal::NewType const &type, qReal::NewType const &parent);

                qReal::TypeList outgoingLinks(qReal::NewType const &type) const;
                qReal::TypeList incomingLinks(qReal::NewType const &type) const;
                qReal::TypeList links(qReal::NewType const &type) const;

                qReal::TypeList outgoingConnections(qReal::NewType const &type) const;
                qReal::TypeList incomingConnections(qReal::NewType const &type) const;
                void connect(qReal::NewType const &source, qReal::NewType const &destination);
                void disconnect(qReal::NewType const &source, qReal::NewType const &destination);

                qReal::TypeList outgoingUsages(qReal::NewType const &type) const;
                qReal::TypeList incomingUsages(qReal::NewType const &type) const;
                void addUsage(qReal::NewType const &source, qReal::NewType const &destination);
                void deleteUsage(qReal::NewType const &source, qReal::NewType const &destination);

                QString typeName(qReal::NewType const &type) const;

                QVariant property(qReal::NewType const &type, QString const &propertyName) const;
                QString stringProperty(qReal::NewType const &type, QString const &propertyName) const;
                void setProperty(qReal::NewType const &type, QString const &propertyName, QVariant const &value);
                void removeProperty(qReal::NewType const &type, QString const &propertyName);
                bool hasProperty(qReal::NewType const &type, QString const &propertyName) const;

                qReal::NewType from(qReal::NewType const &type) const;
                void setFrom(qReal::NewType const &type, qReal::NewType const &from);

                qReal::NewType to(qReal::NewType const &type) const;
                void setTo(qReal::NewType const &type, qReal::NewType const &to);

                double fromPort(qReal::NewType const &type) const;
                void setFromPort(qReal::NewType const &type, double fromPort);

                double toPort(qReal::NewType const &type) const;
                void setToPort(qReal::NewType const &type, double toPort);

                qReal::NewType otherEntityFromLink(qReal::NewType const &linkId, qReal::NewType const &firstNode) const;

		void exterminate();

		void saveAll() const;
                void save(qReal::TypeList list) const;
		void saveTo(QString const &workingDir);
                void remove(qReal::TypeList list) const;

		void open(QString const &workingDir);

                void log(QString const message, qReal::NewType const diagram);

		// "Глобальные" методы, позволяющие делать запросы к модели в целом.
		//Returns all elements with .element() == type.element()
                qReal::TypeList elements(qReal::NewType const &type) const;

		//Returns all elements with .element() == type
                qReal::TypeList elementsByType(QString const &type) const;
		int elementsCount() const;

                bool exist(qReal::NewType const &type) const;

                qReal::TypeList getOpenedDiagrams() const;
                qReal::TypeList getChangedDiagrams() const;
		void resetChangedDiagrams();
                void addOpenedDiagram(const qReal::NewType &type);
                void addChangedDiagram(const qReal::NewType &type);
                void resetChangedDiagrams(const qReal::TypeList &list);

	private:
		RepoApi(RepoApi const &other);  // Копировать нельзя.
		RepoApi& operator =(RepoApi const &);  // Присваивать тоже.

                void addToTypeList(qReal::NewType const &target, QString const &listName, qReal::NewType const &data);
                void removeFromList(qReal::NewType const &target, QString const &listName, qReal::NewType const &data);

                qReal::TypeList links(qReal::NewType const &type, QString const &direction) const;
                void removeLinkEnds(QString const &endName, qReal::NewType const &type);

                QSet<qReal::NewType> mDiagramsOpened;
                QSet<qReal::NewType> mDiagramsChanged;

                typedef QPair<qReal::NewType, qReal::TypeList> ChildsOfDiagram;
		QList<ChildsOfDiagram> mDiagramsDeleted;

		details::Client mClient;
	};

}
