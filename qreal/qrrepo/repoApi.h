#pragma once

#include "../qrgui/kernel/roles.h"
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
		bool hasProperty(qReal::Id const &id, QString const &propertyName) const;
		QMapIterator<QString, QVariant> propertiesIterator(const qReal::Id &id) const;

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

		bool saveAll();
		bool save(qReal::IdList list);
		bool saveTo(QString const &workingDir);

		void open(QString const &workingDir);

		bool doCheckout(const QString &from, const QString &to);
		bool doCommit(const QString &from, const QString &message);
		bool doUpdate(const QString &to);
		bool doCleanUp(const QString &workingDir);
		QString svnInfo(const QString &workingDir);
		QString repoUrl(const QString &workingDir);
		int currentRevision(const QString &workingDir);

		// "����������" ������, ����������� ������ ������� � ������ � �����.
		//Returns all elements with .element() == type.element()
		virtual qReal::IdList graphicalElements(qReal::Id const &type) const;
		virtual qReal::IdList logicalElements(qReal::Id const &type) const;

		virtual qReal::Id logicalId(qReal::Id const &id) const;

		//Returns all elements with .element() == type
		qReal::IdList elementsByType(QString const &type) const;
		int elementsCount() const;

		bool exist(qReal::Id const &id) const;

		QStringList newErrors();

	private:
		RepoApi(RepoApi const &other);  // ���������� ������.
		RepoApi& operator =(RepoApi const &);  // ����������� ����.

		void addToIdList(qReal::Id const &target, QString const &listName, qReal::Id const &data, QString const &direction = QString());
		void removeFromList(qReal::Id const &target, QString const &listName, qReal::Id const &data, QString const &direction = QString());

		qReal::IdList links(qReal::Id const &id, QString const &direction) const;
		void removeLinkEnds(QString const &endName, qReal::Id const &id);

		details::Client mClient;
		QStringList mErrors;
	};

}
