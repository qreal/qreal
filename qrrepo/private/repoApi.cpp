#include "../repoApi.h"

#include <QtCore/QDebug>

using namespace qrRepo;
using namespace qrRepo::details;
using namespace qReal;

RepoApi::RepoApi(QString const &workingDirectory, bool ignoreAutosave)
		: mClient(workingDirectory)
		, mIgnoreAutosave(ignoreAutosave)
{
}

QString RepoApi::name(Id const &id) const
{
	Q_ASSERT(mClient.property(id, "name").canConvert<QString>());
	return mClient.property(id, "name").toString();
}

void RepoApi::setName(Id const &id, QString const &name)
{
	mClient.setProperty(id, "name", name);
}

IdList RepoApi::children(Id const &id) const
{
	return mClient.children(id);
}

void RepoApi::printDebug()
{
	mClient.printDebug();
}

IdList RepoApi::findElementsByName(QString const &name, bool sensitivity, bool regExpression) const
{
	return mClient.findElementsByName(name, sensitivity, regExpression);
}

qReal::IdList RepoApi::elementsByPropertyContent(QString const &propertyContent, bool sensitivity,
		bool regExpression) const
{
	return mClient.elementsByPropertyContent(propertyContent, sensitivity, regExpression);
}

void RepoApi::replaceProperties(qReal::IdList const &toReplace, QString const value, QString const newValue)
{
	mClient.replaceProperties(toReplace, value, newValue);
}

void RepoApi::addChild(Id const &id, Id const &child)
{
	mClient.addChild(id, child);
}

void RepoApi::addChild(Id const &id, Id const &child, Id const &logicalId)
{
	mClient.addChild(id, child, logicalId);
}

void RepoApi::stackBefore(Id const &id, Id const &child, Id const &sibling)
{
	mClient.stackBefore(id, child, sibling);
}

Id RepoApi::copy(qReal::Id const &src)
{
	return mClient.cloneObject(src);
}

void RepoApi::removeChild(Id const &id, Id const &child)
{
	mClient.removeChild(id, child);
}

void RepoApi::removeChildren(Id const &id)
{
	foreach (Id const &child, children(id)) {
		removeChild(id, child);
	}
}

void RepoApi::removeElement(Id const &id)
{
	Q_ASSERT(id != Id::rootId());

	foreach (Id const &child, children(id)) {
		removeElement(child);
	}

	if (hasProperty(id, "links")) {
		IdList const links = property(id, "links").value<IdList>();
		foreach (Id const &link, links) {
			if (mClient.exist(link)) {
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
		IdList const explosions = property(id, "outgoingExplosions").value<IdList>();
		foreach (Id const &target, explosions) {
			removeExplosion(id, target);
		}
	}

	if (hasProperty(id, "incomingExplosions")) {
		IdList const explosions = property(id, "incomingExplosions").value<IdList>();
		foreach (Id const &source, explosions) {
			if (exist(source)) {
				removeExplosion(source, id);
			}
		}
	}

	mClient.remove(id);
}

void RepoApi::removeLinkEnds(QString const &endName, Id const &id) {
	if (hasProperty(id, endName)) {
		Id target = property(id, endName).value<Id>();
		if (hasProperty(target, "links")) {
			removeFromList(target, "links", id, endName);
		}
	}
}

Id RepoApi::parent(Id const &id) const
{
	return mClient.parent(id);
}

void RepoApi::setParent(Id const &id, Id const &parent)
{
	Id const oldParent = mClient.parent(id);
	mClient.removeChild(oldParent, id);
	mClient.setParent(id, parent);
}

IdList RepoApi::links(Id const &id, QString const &direction) const
{
	IdList links = mClient.property(id, "links").value<IdList>();
	IdList result;
	foreach (Id const link, links) {
		if (mClient.exist(link) && mClient.property(link, direction).value<Id>() == id) {
			result.append(link);
		}
	}
	return result;
}

IdList RepoApi::outgoingLinks(Id const &id) const
{
	return links(id, "from");
}

IdList RepoApi::incomingLinks(Id const &id) const
{
	return links(id, "to");
}

IdList RepoApi::links(Id const &id) const
{
	return incomingLinks(id) << outgoingLinks(id);
}

qReal::Id RepoApi::outgoingExplosion(qReal::Id const &id) const
{
	return mClient.property(id, "outgoingExplosion").value<Id>();
}

qReal::IdList RepoApi::incomingExplosions(qReal::Id const &id) const
{
	return mClient.property(id, "incomingExplosions").value<IdList>();
}

void RepoApi::addExplosion(qReal::Id const &source, qReal::Id const &destination)
{
	Id const oldTarget = outgoingExplosion(source);
	if (oldTarget == destination) {
		return;
	}
	if (oldTarget != Id()) {
		removeExplosion(source, oldTarget);
	}
	mClient.setProperty(source, "outgoingExplosion", destination.toVariant());
	addToIdList(destination, "incomingExplosions", source);
}

void RepoApi::removeExplosion(qReal::Id const &source, qReal::Id const &destination)
{
	mClient.setProperty(source, "outgoingExplosion", Id().toVariant());
	removeFromList(destination, "incomingExplosions", source);
}

qReal::IdList RepoApi::connectedElements(qReal::Id const &id) const
{
	qReal::IdList result = outgoingConnectedElements(id);
	result.append(incomingConnectedElements(id));
	return result;
}

qReal::IdList RepoApi::outgoingConnectedElements(qReal::Id const &id) const
{
	qReal::IdList result;
	foreach (qReal::Id curLink, outgoingLinks(id)) {
		qReal::Id toElem = to(curLink);

		result.append(toElem);
	}
	return result;
}

qReal::IdList RepoApi::incomingConnectedElements(qReal::Id const &id) const
{
	qReal::IdList result;
	foreach (qReal::Id curLink, incomingLinks(id)) {
		qReal::Id fromElem = from(curLink);

		result.append(fromElem);
	}
	return result;
}

QVariant RepoApi::property(Id const &id, QString const &propertyName) const
{
	return mClient.property(id, propertyName);
}

QString RepoApi::stringProperty(Id const &id, QString const &propertyName) const
{
	Q_ASSERT(mClient.property(id, propertyName).canConvert<QString>());
	return mClient.property(id, propertyName).toString();
}

void RepoApi::setProperty(Id const &id, QString const &propertyName, QVariant const &value)
{
	mClient.setProperty(id, propertyName, value);
}

void RepoApi::removeProperty(Id const &id, QString const &propertyName)
{
	mClient.removeProperty(id, propertyName);
}

void RepoApi::copyProperties(const Id &dest, const Id &src)
{
	mClient.copyProperties(dest, src);
}

QMap<QString, QVariant> RepoApi::properties(Id const &id)
{
	return mClient.properties(id);
}

void RepoApi::setProperties(Id const &id, QMap<QString, QVariant> const &properties)
{
	mClient.setProperties(id, properties);
}

bool RepoApi::hasProperty(Id const &id, QString const &propertyName) const
{
	return mClient.hasProperty(id, propertyName);
}

void RepoApi::setBackReference(Id const &id, Id const &reference) const
{
	mClient.setBackReference(id, reference);
}

void RepoApi::removeBackReference(Id const &id, Id const &reference) const
{
	mClient.removeBackReference(id, reference);
}

Id RepoApi::from(Id const &id) const
{
	Q_ASSERT(mClient.property(id, "from").canConvert<Id>());
	return mClient.property(id, "from").value<Id>();
}

void RepoApi::setFrom(Id const &id, Id const &from)
{
	if (hasProperty(id, "from")) {
		Id prev = mClient.property(id, "from").value<Id>();
		removeFromList(prev, "links", id, "from");
	}
	mClient.setProperty(id, "from", from.toVariant());
	addToIdList(from, "links", id, "from");
}

Id RepoApi::to(Id const &id) const
{
	Q_ASSERT(mClient.property(id, "to").canConvert<Id>());
	return mClient.property(id, "to").value<Id>();
}

void RepoApi::setTo(Id const &id, Id const &to)
{
	if (hasProperty(id, "to")) {
		Id prev = mClient.property(id, "to").value<Id>();
		removeFromList(prev, "links", id, "to");
	}
	mClient.setProperty(id, "to", to.toVariant());
	addToIdList(to, "links", id, "to");
}

double RepoApi::fromPort(Id const &id) const
{
	Q_ASSERT(mClient.property(id, "fromPort").canConvert<double>());
	return mClient.property(id, "fromPort").value<double>();
}

void RepoApi::setFromPort(Id const &id, double fromPort)
{
	mClient.setProperty(id, "fromPort", fromPort);
}

double RepoApi::toPort(Id const &id) const
{
	Q_ASSERT(mClient.property(id, "toPort").canConvert<double>());
	return mClient.property(id, "toPort").value<double>();
}

void RepoApi::setToPort(Id const &id, double toPort)
{
	mClient.setProperty(id, "toPort", toPort);
}

QVariant RepoApi::position(Id const &id) const
{
	return mClient.property(id, "position");
}

QVariant RepoApi::configuration(Id const &id) const
{
	return mClient.property(id, "configuration");
}

void RepoApi::setPosition(Id const &id, QVariant const &position)
{
	mClient.setProperty(id, "position", position);
}

void RepoApi::setConfiguration(Id const &id, QVariant const &configuration)
{
	mClient.setProperty(id, "configuration", configuration);
}

bool RepoApi::isLogicalElement(qReal::Id const &id) const
{
	return mClient.isLogicalId(id);
}

bool RepoApi::isGraphicalElement(qReal::Id const &id) const
{
	return !mClient.isLogicalId(id);
}

qReal::Id RepoApi::logicalId(qReal::Id const &id) const
{
	return mClient.logicalId(id);
}

void RepoApi::exterminate()
{
	mClient.exterminate();
}

void RepoApi::open(QString const &saveFile)
{
	mClient.open(saveFile);
}

void RepoApi::saveAll() const
{
	mClient.saveAll();
}

void RepoApi::saveTo(QString const &workingFile)
{
	if (!mIgnoreAutosave) {
		mClient.setWorkingFile(workingFile);
		mClient.saveAll();
	}
}

void RepoApi::saveDiagramsById(QHash<QString, IdList> const &diagramIds)
{
	mClient.saveDiagramsById(diagramIds);
}

void RepoApi::importFromDisk(QString const &importedFile)
{
	mClient.importFromDisk(importedFile);
}

void RepoApi::save(qReal::IdList list) const
{
	mClient.save(list);
}

QString RepoApi::workingFile() const
{
	return mClient.workingFile();
}

void RepoApi::addToIdList(Id const &target, QString const &listName, Id const &data, QString const &direction)
{
	if (target == Id::rootId())
		return;

	IdList list = mClient.property(target, listName).value<IdList>();

	// Значения в списке должны быть уникальны.
	if (list.contains(data))
		return;

	list.append(data);
	mClient.setProperty(target, listName, IdListHelper::toVariant(list));

	if (listName == "links") {
		IdList temporaryRemovedList = mClient.temporaryRemovedLinksAt(target, direction);
		temporaryRemovedList.removeAll(data);
		mClient.setTemporaryRemovedLinks(target, direction, temporaryRemovedList);
	}
}

void RepoApi::removeFromList(Id const &target, QString const &listName, Id const &data, QString const &direction)
{
	if (target == Id::rootId())
		return;

	IdList list = mClient.property(target, listName).value<IdList>();
	IdList temporaryRemovedList = mClient.temporaryRemovedLinksAt(target, direction);
	if(listName == "links" && list.contains(data)) {
		temporaryRemovedList.append(data);
	}
	list.removeAll(data);

	mClient.setProperty(target, listName, IdListHelper::toVariant(list));
	mClient.setTemporaryRemovedLinks(target, direction, temporaryRemovedList);
}

Id RepoApi::otherEntityFromLink(Id const &linkId, Id const &firstNode) const
{
	Id const fromId = from(linkId);
	if (fromId != firstNode)
		return fromId;
	else
		return to(linkId);
}

IdList RepoApi::logicalElements(Id const &type) const
{
	Q_ASSERT(type.idSize() == 3);

	IdList result;
	foreach (Id id, mClient.elements()) {
		if (id.element() == type.element() && mClient.isLogicalId(id))
			result.append(id);
	}
	return result;
}

IdList RepoApi::graphicalElements(Id const &type) const
{
	Q_ASSERT(type.idSize() == 3);

	IdList result;
	foreach (Id id, mClient.elements()) {
		if (id.element() == type.element() && !mClient.isLogicalId(id))
			result.append(id);
	}
	return result;
}

IdList RepoApi::elementsByType(QString const &type, bool sensitivity, bool regExpression) const
{
	Qt::CaseSensitivity const caseSensitivity = sensitivity ? Qt::CaseSensitive : Qt::CaseInsensitive;

	QRegExp *regExp = new QRegExp(type,caseSensitivity);

	IdList result;

	if (regExpression) {
		foreach (Id const &id, mClient.elements()) {
			if (id.element().contains(*regExp)) {
				result.append(id);
			}
		}
	} else {
		foreach (Id const &id, mClient.elements()) {
			if (id.element().contains(type, caseSensitivity)) {
				result.append(id);
			}
		}
	}
	return result;
}

qReal::IdList RepoApi::elementsByProperty(QString const &property, bool sensitivity, bool regExpression) const
{
	return mClient.elementsByProperty(property, sensitivity, regExpression);
}

int RepoApi::elementsCount() const
{
	return mClient.elements().size();
}

bool RepoApi::exist(Id const &id) const
{
	return mClient.exist(id);
}

IdList RepoApi::temporaryRemovedLinksAt(Id const &id, QString const &direction) const
{
	return mClient.temporaryRemovedLinksAt(id, direction);
}

void RepoApi::setTemporaryRemovedLinks(Id const &id, IdList const &value, QString const &direction)
{
	mClient.setTemporaryRemovedLinks(id, direction, value);
}

void RepoApi::removeTemporaryRemovedLinks(Id const &id)
{
	mClient.removeTemporaryRemovedLinks(id);
}

QMapIterator<QString, QVariant> RepoApi::propertiesIterator(qReal::Id const &id) const
{
	return mClient.propertiesIterator(id);
}
