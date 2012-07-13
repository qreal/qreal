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

		/// replacing property values that contains input value with new value
		/// @param toReplace - id list that contains ids of elements that properties should be replaced
		/// @param value - input value that should be contained by any property of each element
		/// @param newValue - string representation of value with what property values should be replaced
		void replaceProperties(qReal::IdList const &toReplace, QString const value, QString const newValue);

		qReal::Id copy(qReal::Id const &src);

		QString name(qReal::Id const &id) const;
		void setName(qReal::Id const &id, QString const &name);

		/// returning IdList of elements that names contains input string
		/// @param name - string that should be contained by names of elements that Id's are in the output list
		qReal::IdList findElementsByName(QString const &name, bool sensitivity, bool regExpression) const;

		/// returning IdList of elements that have input property
		/// @param name - string that should be contained by names of elements that have input property
		qReal::IdList elementsByProperty(QString const &property, bool sensitivity, bool regExpression) const;

		/// returning IdList of elements that have input property content
		/// @param name - string that should be contained by names of elements that have input property content
		qReal::IdList elementsByPropertyContent(QString const &propertyContent, bool sensitivity, bool regExpression) const;

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
		void setProperty(qReal::Id const &id, QString const &propertyName, QVariant const &value) const;
		void removeProperty(qReal::Id const &id, QString const &propertyName);
		void copyProperties(const qReal::Id &dest, const qReal::Id &src);
		QMap<QString, QVariant> properties(qReal::Id const &id);
		void setProperties(qReal::Id const &id, QMap<QString, QVariant> const &properties);
		bool hasProperty(qReal::Id const &id, QString const &propertyName) const;
		QMapIterator<QString, QVariant> propertiesIterator(qReal::Id const &id) const;

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

		/// RepoApi's wrapper for Client.importFromDisk
		/// @param importedFile - file to be imported
		void importFromDisk(QString const &importedFile);
		void saveAll() const;
		void save(qReal::IdList list) const;
		void saveTo(QString const &workingFile);
		void saveDiagramsById(QHash<QString, qReal::IdList> const &diagramIds);
		void open(QString const &saveFile);

		virtual QString workingFile() const;

		// "Глобальные" методы, позволяющие делать запросы к модели в целом.
		//Returns all elements with .element() == type.element()
		virtual qReal::IdList graphicalElements(qReal::Id const &type) const;
		virtual qReal::IdList logicalElements(qReal::Id const &type) const;

		virtual qReal::Id logicalId(qReal::Id const &id) const;

		/// Returns all elements with .element() == type
		qReal::IdList elementsByType(QString const &type, bool sensitivity = false, bool regExpression = false) const;
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
