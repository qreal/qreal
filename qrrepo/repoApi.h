/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include "private/qrRepoGlobal.h"
#include "repoControlInterface.h"
#include "graphicalRepoApi.h"
#include "logicalRepoApi.h"

namespace qrRepo {

namespace details {
class Repository;
}

/// Repository interface. Supports higher level queries, than \see Repository, so is more convenient to work with.
class QRREPO_EXPORT RepoApi : public GraphicalRepoApi, public LogicalRepoApi, public RepoControlInterface
{
public:
	explicit RepoApi(const QString &workingFile, bool ignoreAutosave = false);
	~RepoApi() override;

	/// Replacing property values that contains input value with new value.
	/// @param toReplace Id list that contains ids of elements that properties should be replaced.
	/// @param value Input value that should be contained by any property of each element.
	/// @param newValue String representation of value with what property values should be replaced.
	void replaceProperties(const qReal::IdList &toReplace, const QString value, const QString newValue) override;

	qReal::Id copy(const qReal::Id &src) override;

	QString name(const qReal::Id &id) const override;
	void setName(const qReal::Id &id, const QString &name) override;

	/// Returning IdList of elements that names contains input string.
	/// @param name String that should be contained by names of elements that Id's are in the output list.
	qReal::IdList findElementsByName(const QString &name, bool sensitivity, bool regExpression) const override;

	/// Returning IdList of elements that have input property.
	/// @param name String that should be contained by names of elements that have input property.
	qReal::IdList elementsByProperty(const QString &property, bool sensitivity, bool regExpression) const override;

	/// returning IdList of elements that have input property content
	/// @param name - string that should be contained by names of elements that have input property content
	qReal::IdList elementsByPropertyContent(const QString &propertyContent
			, bool sensitivity, bool regExpression) const override;

	qReal::IdList children(const qReal::Id &id) const override;
	void addChild(const qReal::Id &id, const qReal::Id &child) override;
	void addChild(const qReal::Id &id, const qReal::Id &child, const qReal::Id &logicalId) override;
	void removeChild(const qReal::Id &id, const qReal::Id &child) override;
	void removeChildren(const qReal::Id &id) override;

	void printDebug();

	void removeElement(const qReal::Id &id) override;

	qReal::Id parent(const qReal::Id &id) const override;
	void setParent(const qReal::Id &id, const qReal::Id &parent) override;

	virtual void stackBefore(const qReal::Id &id, const qReal::Id &child, const qReal::Id &sibling) override;

	qReal::IdList outgoingLinks(const qReal::Id &id) const override;
	qReal::IdList incomingLinks(const qReal::Id &id) const override;
	qReal::IdList links(const qReal::Id &id) const override;

	qReal::Id outgoingExplosion(const qReal::Id &id) const override;
	qReal::IdList incomingExplosions(const qReal::Id &id) const override;
	void addExplosion(const qReal::Id &source, const qReal::Id &destination) override;
	void removeExplosion(const qReal::Id &source, const qReal::Id &destination) override;

	qReal::IdList connectedElements(const qReal::Id &id) const;
	qReal::IdList outgoingConnectedElements(const qReal::Id &id) const;
	qReal::IdList incomingConnectedElements(const qReal::Id &id) const;

	QVariant property(const qReal::Id &id, const QString &propertyName) const override;
	QString stringProperty(const qReal::Id &id, const QString &propertyName) const override;
	void setProperty(const qReal::Id &id, const QString &propertyName, const QVariant &value) override;
	void removeProperty(const qReal::Id &id, const QString &propertyName) override;
	void copyProperties(const qReal::Id &dest, const qReal::Id &src) override;
	QMap<QString, QVariant> properties(const qReal::Id &id) const override;
	void setProperties(const qReal::Id &id, QMap<QString, QVariant> const &properties) override;
	bool hasProperty(const qReal::Id &id, const QString &propertyName) const override;
	QMapIterator<QString, QVariant> propertiesIterator(const qReal::Id &id) const override;

	void setBackReference(const qReal::Id &id, const qReal::Id &reference) const override;
	void removeBackReference(const qReal::Id &id, const qReal::Id &reference) const override;

	qReal::IdList temporaryRemovedLinksAt(const qReal::Id &id, const QString &direction) const override;
	void setTemporaryRemovedLinks(const qReal::Id &id, const qReal::IdList &value, const QString &direction) override;
	void removeTemporaryRemovedLinks(const qReal::Id &id) override;

	qReal::Id from(const qReal::Id &id) const override;
	void setFrom(const qReal::Id &id, const qReal::Id &from) override;

	qReal::Id to(const qReal::Id &id) const override;
	void setTo(const qReal::Id &id, const qReal::Id &to) override;

	qreal fromPort(const qReal::Id &id) const override;
	void setFromPort(const qReal::Id &id, qreal fromPort) override;

	qreal toPort(const qReal::Id &id) const override;
	void setToPort(const qReal::Id &id, qreal toPort) override;

	QVariant position(const qReal::Id &id) const override;
	QVariant configuration(const qReal::Id &id) const override;

	void setPosition(const qReal::Id &id, const QVariant &position) override;
	void setConfiguration(const qReal::Id &id, const QVariant &configuration) override;

	qReal::Id otherEntityFromLink(const qReal::Id &linkId, const qReal::Id &firstNode) const override;

	bool isLogicalElement(const qReal::Id &id) const override;
	bool isGraphicalElement(const qReal::Id &id) const override;

	void exterminate() override;

	/// RepoApi's wrapper for Repository.importFromDisk
	/// @param importedFile - file to be imported
	void importFromDisk(const QString &importedFile) override;
	void saveAll() const override;
	void save(const qReal::IdList &list) const override;
	void saveTo(const QString &workingFile) override;
	void saveDiagramsById(QHash<QString, qReal::IdList> const &diagramIds) override;
	void open(const QString &saveFile) override;
	void exportToXml(const QString &targetFile) const override;

	QString workingFile() const override;

	// "Global" methods for querying the whole model.
	// Returns all elements with .element() == type.element()
	qReal::IdList graphicalElements() const override;
	qReal::IdList graphicalElements(const qReal::Id &type) const override;
	qReal::IdList logicalElements(const qReal::Id &type) const override;

	qReal::Id logicalId(const qReal::Id &id) const override;

	/// Returns all elements with .element() == type
	qReal::IdList elementsByType(const QString &type, bool sensitivity = false, bool regExpression = false) const;
	int elementsCount() const;

	bool exist(const qReal::Id &id) const override;

	void createGraphicalPart(const qReal::Id &id, int partIndex) override;

	QList<int> graphicalParts(const qReal::Id &id) const override;

	QVariant graphicalPartProperty(const qReal::Id &id, int partIndex, const QString &propertyName) const override;

	void setGraphicalPartProperty(
			const qReal::Id &id
			, int partIndex
			, const QString &propertyName
			, const QVariant &value
			) override;

	QStringList metaInformationKeys() const override;
	QVariant metaInformation(const QString &key) const override;
	void setMetaInformation(const QString &key, const QVariant &info) override;

private:
	RepoApi(const RepoApi &other);  // Copying is not allowed.
	RepoApi& operator =(const RepoApi &);  // Assigning is not allowed.

	void addToIdList(const qReal::Id &target, const QString &listName, const qReal::Id &data
			, const QString &direction = QString());

	void removeFromList(const qReal::Id &target, const QString &listName, const qReal::Id &data
			, const QString &direction = QString());

	qReal::IdList links(const qReal::Id &id, const QString &direction) const;
	void removeLinkEnds(const QString &endName, const qReal::Id &id);

	QScopedPointer<details::Repository> mRepository;
	bool mIgnoreAutosave;
};

}
