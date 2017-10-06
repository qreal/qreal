/* Copyright 2007-2015 QReal Research Group
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

#include "repoApi.h"
#include "private/repository.h"

using namespace qrRepo;
using namespace qrRepo::details;
using namespace qReal;

RepoApi::RepoApi(const QString &workingFile, bool ignoreAutosave)
	: mRepository(new Repository(workingFile))
	, mIgnoreAutosave(ignoreAutosave)
{
}

RepoApi::~RepoApi()
{
}

QString RepoApi::name(const Id &id) const
{
	if (!mRepository->property(id, "name").canConvert<QString>()) {
		Q_ASSERT(mRepository->property(id, "name").canConvert<QString>());
	}

	return mRepository->property(id, "name").toString();
}

void RepoApi::setName(const Id &id, const QString &name)
{
	mRepository->setProperty(id, "name", name);
}

IdList RepoApi::children(const Id &id) const
{
	return mRepository->children(id);
}

void RepoApi::printDebug()
{
	mRepository->printDebug();
}

IdList RepoApi::findElementsByName(const QString &name, bool sensitivity, bool regExpression) const
{
	return mRepository->findElementsByName(name, sensitivity, regExpression);
}

qReal::IdList RepoApi::elementsByPropertyContent(const QString &propertyContent, bool sensitivity
		, bool regExpression) const
{
	return mRepository->elementsByPropertyContent(propertyContent, sensitivity, regExpression);
}

void RepoApi::replaceProperties(const qReal::IdList &toReplace, const QString value, const QString newValue)
{
	mRepository->replaceProperties(toReplace, value, newValue);
}

void RepoApi::addChild(const Id &id, const Id &child)
{
	mRepository->addChild(id, child);
}

void RepoApi::addChild(const Id &id, const Id &child, const Id &logicalId)
{
	mRepository->addChild(id, child, logicalId);
}

void RepoApi::stackBefore(const Id &id, const Id &child, const Id &sibling)
{
	mRepository->stackBefore(id, child, sibling);
}

Id RepoApi::copy(const qReal::Id &src)
{
	return mRepository->cloneObject(src);
}

void RepoApi::removeChild(const Id &id, const Id &child)
{
	mRepository->removeChild(id, child);
}

void RepoApi::removeChildren(const Id &id)
{
	foreach (const Id &child, children(id)) {
		removeChild(id, child);
	}
}

void RepoApi::removeElement(const Id &id)
{
	Q_ASSERT(id != Id::rootId());

	foreach (const Id &child, children(id)) {
		removeElement(child);
	}

	if (hasProperty(id, "links")) {
		const IdList links = property(id, "links").value<IdList>();
		foreach (const Id &link, links) {
			if (mRepository->exist(link)) {
				if (hasProperty(link, "from") && property(link, "from").value<Id>() == id) {
					setProperty(link, "from", Id::rootId().toVariant());
				}
				if (hasProperty(link, "to") && property(link, "to").value<Id>() == id) {
					setProperty(link, "to", Id::rootId().toVariant());
				}
			}
		}
	}

	removeLinkEnds("from", id);
	removeLinkEnds("to", id);

	if (hasProperty(id, "outgoingExplosions")) {
		const IdList explosions = property(id, "outgoingExplosions").value<IdList>();
		foreach (const Id &target, explosions) {
			removeExplosion(id, target);
		}
	}

	if (hasProperty(id, "incomingExplosions")) {
		const IdList explosions = property(id, "incomingExplosions").value<IdList>();
		foreach (const Id &source, explosions) {
			if (exist(source)) {
				removeExplosion(source, id);
			}
		}
	}

	mRepository->remove(id);
}

void RepoApi::removeLinkEnds(const QString &endName, const Id &id) {
	if (hasProperty(id, endName)) {
		Id target = property(id, endName).value<Id>();
		if (hasProperty(target, "links")) {
			removeFromList(target, "links", id, endName);
		}
	}
}

Id RepoApi::parent(const Id &id) const
{
	return mRepository->parent(id);
}

void RepoApi::setParent(const Id &id, const Id &parent)
{
	const Id oldParent = mRepository->parent(id);
	mRepository->removeChild(oldParent, id);
	mRepository->setParent(id, parent);
}

IdList RepoApi::links(const Id &id, const QString &direction) const
{
	const IdList links = mRepository->property(id, "links").value<IdList>();
	IdList result;
	for (const Id &link : links) {
		if (mRepository->exist(link) && mRepository->property(link, direction).value<Id>() == id) {
			result.append(link);
		}
	}

	return result;
}

IdList RepoApi::outgoingLinks(const Id &id) const
{
	return links(id, "from");
}

IdList RepoApi::incomingLinks(const Id &id) const
{
	return links(id, "to");
}

IdList RepoApi::links(const Id &id) const
{
	return incomingLinks(id) << outgoingLinks(id);
}

qReal::Id RepoApi::outgoingExplosion(const qReal::Id &id) const
{
	return mRepository->property(id, "outgoingExplosion").value<Id>();
}

qReal::IdList RepoApi::incomingExplosions(const qReal::Id &id) const
{
	return mRepository->property(id, "incomingExplosions").value<IdList>();
}

void RepoApi::addExplosion(const qReal::Id &source, const qReal::Id &destination)
{
	const Id oldTarget = outgoingExplosion(source);
	if (oldTarget == destination) {
		return;
	}
	if (oldTarget != Id()) {
		removeExplosion(source, oldTarget);
	}
	mRepository->setProperty(source, "outgoingExplosion", destination.toVariant());
	addToIdList(destination, "incomingExplosions", source);
}

void RepoApi::removeExplosion(const qReal::Id &source, const qReal::Id &destination)
{
	mRepository->setProperty(source, "outgoingExplosion", Id().toVariant());
	removeFromList(destination, "incomingExplosions", source);
}

qReal::IdList RepoApi::connectedElements(const qReal::Id &id) const
{
	qReal::IdList result = outgoingConnectedElements(id);
	result.append(incomingConnectedElements(id));
	return result;
}

qReal::IdList RepoApi::outgoingConnectedElements(const qReal::Id &id) const
{
	qReal::IdList result;
	foreach (qReal::Id curLink, outgoingLinks(id)) {
		qReal::Id toElem = to(curLink);

		result.append(toElem);
	}
	return result;
}

qReal::IdList RepoApi::incomingConnectedElements(const qReal::Id &id) const
{
	qReal::IdList result;
	foreach (qReal::Id curLink, incomingLinks(id)) {
		qReal::Id fromElem = from(curLink);

		result.append(fromElem);
	}
	return result;
}

QVariant RepoApi::property(const Id &id, const QString &propertyName) const
{
	return mRepository->property(id, propertyName);
}

QString RepoApi::stringProperty(const Id &id, const QString &propertyName) const
{
	// TODO: restore q_assert
	//Q_ASSERT(mRepository.property(id, propertyName).canConvert<QString>());
	return mRepository->property(id, propertyName).toString();
}

void RepoApi::setProperty(const Id &id, const QString &propertyName, const QVariant &value)
{
	mRepository->setProperty(id, propertyName, value);
}

void RepoApi::removeProperty(const Id &id, const QString &propertyName)
{
	mRepository->removeProperty(id, propertyName);
}

void RepoApi::copyProperties(const Id &dest, const Id &src)
{
	mRepository->copyProperties(dest, src);
}

QMap<QString, QVariant> RepoApi::properties(const Id &id) const
{
	return mRepository->properties(id);
}

void RepoApi::setProperties(const Id &id, QMap<QString, QVariant> const &properties)
{
	mRepository->setProperties(id, properties);
}

bool RepoApi::hasProperty(const Id &id, const QString &propertyName) const
{
	return mRepository->hasProperty(id, propertyName);
}

void RepoApi::setBackReference(const Id &id, const Id &reference) const
{
	mRepository->setBackReference(id, reference);
}

void RepoApi::removeBackReference(const Id &id, const Id &reference) const
{
	mRepository->removeBackReference(id, reference);
}

Id RepoApi::from(const Id &id) const
{
	Q_ASSERT(mRepository->property(id, "from").canConvert<Id>());
	return mRepository->property(id, "from").value<Id>();
}

void RepoApi::setFrom(const Id &id, const Id &from)
{
	if (hasProperty(id, "from")) {
		Id prev = mRepository->property(id, "from").value<Id>();
		removeFromList(prev, "links", id, "from");
	}
	mRepository->setProperty(id, "from", from.toVariant());
	addToIdList(from, "links", id, "from");
}

Id RepoApi::to(const Id &id) const
{
	Q_ASSERT(mRepository->property(id, "to").canConvert<Id>());
	return mRepository->property(id, "to").value<Id>();
}

void RepoApi::setTo(const Id &id, const Id &to)
{
	if (hasProperty(id, "to")) {
		Id prev = mRepository->property(id, "to").value<Id>();
		removeFromList(prev, "links", id, "to");
	}
	mRepository->setProperty(id, "to", to.toVariant());
	addToIdList(to, "links", id, "to");
}

qreal RepoApi::fromPort(const Id &id) const
{
	Q_ASSERT(mRepository->property(id, "fromPort").canConvert<qreal>());
	return mRepository->property(id, "fromPort").value<qreal>();
}

void RepoApi::setFromPort(const Id &id, qreal fromPort)
{
	mRepository->setProperty(id, "fromPort", fromPort);
}

qreal RepoApi::toPort(const Id &id) const
{
	Q_ASSERT(mRepository->property(id, "toPort").canConvert<qreal>());
	return mRepository->property(id, "toPort").value<qreal>();
}

void RepoApi::setToPort(const Id &id, qreal toPort)
{
	mRepository->setProperty(id, "toPort", toPort);
}

QVariant RepoApi::position(const Id &id) const
{
	return mRepository->property(id, "position");
}

QVariant RepoApi::configuration(const Id &id) const
{
	return mRepository->property(id, "configuration");
}

void RepoApi::setPosition(const Id &id, const QVariant &position)
{
	mRepository->setProperty(id, "position", position);
}

void RepoApi::setConfiguration(const Id &id, const QVariant &configuration)
{
	mRepository->setProperty(id, "configuration", configuration);
}

bool RepoApi::isLogicalElement(const qReal::Id &id) const
{
	return mRepository->isLogicalId(id);
}

bool RepoApi::isGraphicalElement(const qReal::Id &id) const
{
	return !mRepository->isLogicalId(id);
}

qReal::Id RepoApi::logicalId(const qReal::Id &id) const
{
	return mRepository->logicalId(id);
}

void RepoApi::exterminate()
{
	mRepository->exterminate();
}

void RepoApi::open(const QString &saveFile)
{
	mRepository->open(saveFile);
}

void RepoApi::exportToXml(const QString &targetFile) const
{
	mRepository->exportToXml(targetFile);
}

void RepoApi::saveAll() const
{
	mRepository->saveAll();
}

void RepoApi::saveTo(const QString &workingFile)
{
	if (!mIgnoreAutosave) {
		mRepository->setWorkingFile(workingFile);
		if (!workingFile.isEmpty()) {
			mRepository->saveAll();
		}
	}
}

void RepoApi::saveDiagramsById(QHash<QString, IdList> const &diagramIds)
{
	mRepository->saveDiagramsById(diagramIds);
}

void RepoApi::importFromDisk(const QString &importedFile)
{
	mRepository->importFromDisk(importedFile);
}

void RepoApi::save(const qReal::IdList &list) const
{
	mRepository->save(list);
}

QString RepoApi::workingFile() const
{
	return mRepository->workingFile();
}

IdList RepoApi::graphicalElements() const
{
	IdList result;
	foreach (const Id &id, mRepository->elements()) {
		if (!mRepository->isLogicalId(id))
			result.append(id);
	}
	return result;
}

void RepoApi::addToIdList(const Id &target, const QString &listName, const Id &data, const QString &direction)
{
	if (target == Id::rootId())
		return;

	IdList list = mRepository->property(target, listName).value<IdList>();

	// Values in the list must be unique.
	if (list.contains(data))
		return;

	list.append(data);
	mRepository->setProperty(target, listName, IdListHelper::toVariant(list));

	if (listName == "links") {
		IdList temporaryRemovedList = mRepository->temporaryRemovedLinksAt(target, direction);
		temporaryRemovedList.removeAll(data);
		mRepository->setTemporaryRemovedLinks(target, direction, temporaryRemovedList);
	}
}

void RepoApi::removeFromList(const Id &target, const QString &listName, const Id &data, const QString &direction)
{
	if (target == Id::rootId())
		return;

	IdList list = mRepository->property(target, listName).value<IdList>();
	IdList temporaryRemovedList = mRepository->temporaryRemovedLinksAt(target, direction);
	if(listName == "links" && list.contains(data)) {
		temporaryRemovedList.append(data);
	}
	list.removeAll(data);

	mRepository->setProperty(target, listName, IdListHelper::toVariant(list));
	mRepository->setTemporaryRemovedLinks(target, direction, temporaryRemovedList);
}

Id RepoApi::otherEntityFromLink(const Id &linkId, const Id &firstNode) const
{
	const Id fromId = from(linkId);
	if (fromId != firstNode)
		return fromId;
	else
		return to(linkId);
}

IdList RepoApi::logicalElements(const Id &type) const
{
	Q_ASSERT(type.idSize() == 3);

	IdList result;
	foreach (Id id, mRepository->elements()) {
		if (id.element() == type.element() && mRepository->isLogicalId(id))
			result.append(id);
	}
	return result;
}

IdList RepoApi::graphicalElements(const Id &type) const
{
	Q_ASSERT(type.idSize() == 3);

	IdList result;
	foreach (Id id, mRepository->elements()) {
		if (id.element() == type.element() && !mRepository->isLogicalId(id))
			result.append(id);
	}
	return result;
}

IdList RepoApi::elementsByType(const QString &type, bool sensitivity, bool regExpression) const
{
	const Qt::CaseSensitivity caseSensitivity = sensitivity ? Qt::CaseSensitive : Qt::CaseInsensitive;

	QRegExp *regExp = new QRegExp(type,caseSensitivity);

	IdList result;

	if (regExpression) {
		foreach (const Id &id, mRepository->elements()) {
			if (id.element().contains(*regExp)) {
				result.append(id);
			}
		}
	} else {
		foreach (const Id &id, mRepository->elements()) {
			if (id.element().contains(type, caseSensitivity)) {
				result.append(id);
			}
		}
	}
	return result;
}

qReal::IdList RepoApi::elementsByProperty(const QString &property, bool sensitivity, bool regExpression) const
{
	return mRepository->elementsByProperty(property, sensitivity, regExpression);
}

int RepoApi::elementsCount() const
{
	return mRepository->elements().size();
}

bool RepoApi::exist(const Id &id) const
{
	return mRepository->exist(id);
}

IdList RepoApi::temporaryRemovedLinksAt(const Id &id, const QString &direction) const
{
	return mRepository->temporaryRemovedLinksAt(id, direction);
}

void RepoApi::setTemporaryRemovedLinks(const Id &id, const IdList &value, const QString &direction)
{
	mRepository->setTemporaryRemovedLinks(id, direction, value);
}

void RepoApi::removeTemporaryRemovedLinks(const Id &id)
{
	mRepository->removeTemporaryRemovedLinks(id);
}

QMapIterator<QString, QVariant> RepoApi::propertiesIterator(const qReal::Id &id) const
{
	return mRepository->propertiesIterator(id);
}

void RepoApi::createGraphicalPart(const qReal::Id &id, int partIndex)
{
	mRepository->createGraphicalPart(id, partIndex);
}

QList<int> RepoApi::graphicalParts(const qReal::Id &id) const
{
	return mRepository->graphicalParts(id);
}

QVariant RepoApi::graphicalPartProperty(const qReal::Id &id, int partIndex, const QString &propertyName) const
{
	return mRepository->graphicalPartProperty(id, partIndex, propertyName);
}

void RepoApi::setGraphicalPartProperty(
		const qReal::Id &id
		, int partIndex
		, const QString &propertyName
		, const QVariant &value
		)
{
	mRepository->setGraphicalPartProperty(id, partIndex, propertyName, value);
}

QStringList RepoApi::metaInformationKeys() const
{
	return mRepository->metaInformationKeys();
}

QVariant RepoApi::metaInformation(const QString &key) const
{
	return mRepository->metaInformation(key);
}

void RepoApi::setMetaInformation(const QString &key, const QVariant &info)
{
	mRepository->setMetaInformation(key, info);
}
