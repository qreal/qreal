#pragma once

#include <QtCore/QSet>

#include <qrkernel/roles.h>
#include "private/repository.h"
#include "private/qrRepoGlobal.h"
#include "repoControlInterface.h"
#include "commonRepoApi.h"
#include "graphicalRepoApi.h"
#include "logicalRepoApi.h"

namespace qrRepo {

/// Repository interface. Supports higher level queries, than \see Repository, so is more convenient to work with.
class QRREPO_EXPORT RepoApi : public GraphicalRepoApi, public LogicalRepoApi, public RepoControlInterface
{
public:
	explicit RepoApi(QString const &workingDirectory, bool ignoreAutosave = false);
	// Default destructor ok.

	/// Replacing property values that contains input value with new value.
	/// @param toReplace Id list that contains ids of elements that properties should be replaced.
	/// @param value Input value that should be contained by any property of each element.
	/// @param newValue String representation of value with what property values should be replaced.
	void replaceProperties(qReal::IdList const &toReplace, QString const value, QString const newValue) override;

	qReal::Id copy(qReal::Id const &src) override;

	QString name(qReal::Id const &id) const override;
	void setName(qReal::Id const &id, QString const &name) override;

	/// Returning IdList of elements that names contains input string.
	/// @param name String that should be contained by names of elements that Id's are in the output list.
	qReal::IdList findElementsByName(QString const &name, bool sensitivity, bool regExpression) const override;

	/// Returning IdList of elements that have input property.
	/// @param name String that should be contained by names of elements that have input property.
	qReal::IdList elementsByProperty(QString const &property, bool sensitivity, bool regExpression) const override;

	/// returning IdList of elements that have input property content
	/// @param name - string that should be contained by names of elements that have input property content
	qReal::IdList elementsByPropertyContent(QString const &propertyContent
			, bool sensitivity, bool regExpression) const override;

	qReal::IdList children(qReal::Id const &id) const override;
	void addChild(qReal::Id const &id, qReal::Id const &child) override;
	void addChild(qReal::Id const &id, qReal::Id const &child, qReal::Id const &logicalId) override;
	void removeChild(qReal::Id const &id, qReal::Id const &child) override;
	void removeChildren(qReal::Id const &id) override;

	void printDebug();

	void removeElement(qReal::Id const &id) override;

	qReal::Id parent(qReal::Id const &id) const override;
	void setParent(qReal::Id const &id, qReal::Id const &parent) override;

	virtual void stackBefore(qReal::Id const &id, qReal::Id const &child, qReal::Id const &sibling) override;

	qReal::IdList outgoingLinks(qReal::Id const &id) const override;
	qReal::IdList incomingLinks(qReal::Id const &id) const override;
	qReal::IdList links(qReal::Id const &id) const override;

	qReal::Id outgoingExplosion(qReal::Id const &id) const override;
	qReal::IdList incomingExplosions(qReal::Id const &id) const override;
	void addExplosion(qReal::Id const &source, qReal::Id const &destination) override;
	void removeExplosion(qReal::Id const &source, qReal::Id const &destination) override;

	qReal::IdList connectedElements(qReal::Id const &id) const;
	qReal::IdList outgoingConnectedElements(qReal::Id const &id) const;
	qReal::IdList incomingConnectedElements(qReal::Id const &id) const;

	QVariant property(qReal::Id const &id, QString const &propertyName) const override;
	QString stringProperty(qReal::Id const &id, QString const &propertyName) const override;
	void setProperty(qReal::Id const &id, QString const &propertyName, QVariant const &value) override;
	void removeProperty(qReal::Id const &id, QString const &propertyName) override;
	void copyProperties(const qReal::Id &dest, const qReal::Id &src) override;
	QMap<QString, QVariant> properties(qReal::Id const &id) override;
	void setProperties(qReal::Id const &id, QMap<QString, QVariant> const &properties) override;
	bool hasProperty(qReal::Id const &id, QString const &propertyName) const override;
	QMapIterator<QString, QVariant> propertiesIterator(qReal::Id const &id) const override;

	void setBackReference(qReal::Id const &id, qReal::Id const &reference) const override;
	void removeBackReference(qReal::Id const &id, qReal::Id const &reference) const override;

	qReal::IdList temporaryRemovedLinksAt(qReal::Id const &id, QString const &direction) const override;
	void setTemporaryRemovedLinks(qReal::Id const &id, qReal::IdList const &value, QString const &direction) override;
	void removeTemporaryRemovedLinks(qReal::Id const &id) override;

	qReal::Id from(qReal::Id const &id) const override;
	void setFrom(qReal::Id const &id, qReal::Id const &from) override;

	qReal::Id to(qReal::Id const &id) const override;
	void setTo(qReal::Id const &id, qReal::Id const &to) override;

	double fromPort(qReal::Id const &id) const override;
	void setFromPort(qReal::Id const &id, double fromPort) override;

	double toPort(qReal::Id const &id) const override;
	void setToPort(qReal::Id const &id, double toPort) override;

	QVariant position(qReal::Id const &id) const override;
	QVariant configuration(qReal::Id const &id) const override;

	void setPosition(qReal::Id const &id, QVariant const &position) override;
	void setConfiguration(qReal::Id const &id, QVariant const &configuration) override;

	qReal::Id otherEntityFromLink(qReal::Id const &linkId, qReal::Id const &firstNode) const override;

	bool isLogicalElement(qReal::Id const &id) const override;
	bool isGraphicalElement(qReal::Id const &id) const override;

	void exterminate() override;

	/// RepoApi's wrapper for Repository.importFromDisk
	/// @param importedFile - file to be imported
	void importFromDisk(QString const &importedFile) override;
	void saveAll() const override;
	void save(qReal::IdList const &list) const override;
	void saveTo(QString const &workingFile) override;
	void saveDiagramsById(QHash<QString, qReal::IdList> const &diagramIds) override;
	void open(QString const &saveFile) override;
	void exportToXml(QString const &targetFile) const override;

	QString workingFile() const override;

	// "Global" methods for querying the whole model.
	// Returns all elements with .element() == type.element()
	qReal::IdList graphicalElements() const override;
	qReal::IdList graphicalElements(qReal::Id const &type) const override;
	qReal::IdList logicalElements(qReal::Id const &type) const override;

	qReal::Id logicalId(qReal::Id const &id) const override;

	/// Returns all elements with .element() == type
	qReal::IdList elementsByType(QString const &type, bool sensitivity = false, bool regExpression = false) const;
	int elementsCount() const;

	bool exist(qReal::Id const &id) const override;

	void createGraphicalPart(qReal::Id const &id, int partIndex) override;

	QList<int> graphicalParts(qReal::Id const &id) const override;

	QVariant graphicalPartProperty(qReal::Id const &id, int partIndex, QString const &propertyName) const override;

	void setGraphicalPartProperty(
			qReal::Id const &id
			, int partIndex
			, QString const &propertyName
			, QVariant const &value
			) override;

	QString metaInformation(QString const &key) const override;
	void setMetaInformation(QString const &key, QString const &info) override;

private:
	RepoApi(RepoApi const &other);  // Copying is not allowed.
	RepoApi& operator =(RepoApi const &);  // Assigning is not allowed.

	void addToIdList(qReal::Id const &target, QString const &listName, qReal::Id const &data, QString const &direction = QString());
	void removeFromList(qReal::Id const &target, QString const &listName, qReal::Id const &data, QString const &direction = QString());

	qReal::IdList links(qReal::Id const &id, QString const &direction) const;
	void removeLinkEnds(QString const &endName, qReal::Id const &id);

	details::Repository mRepository;
	bool mIgnoreAutosave;
};

}
