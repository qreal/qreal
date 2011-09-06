#include "../repoApi.h"

#include <QDebug>

using namespace qrRepo;
using namespace qrRepo::details;
using namespace qReal;

RepoApi::RepoApi(QString const &workingDirectory)
	: mClient(workingDirectory)
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

void RepoApi::addChild(Id const &id, Id const &child)
{
	mClient.addChild(id, child);
}

void RepoApi::addChild(Id const &id, Id const &child, Id const &logicalId)
{
	mClient.addChild(id, child, logicalId);
}

void RepoApi::removeChild(Id const &id, Id const &child)
{
	mClient.removeChild(id, child);
}

void RepoApi::removeChildren(Id const &id)
{
	foreach (Id const child, children(id))
		removeChild(id, child);
}

void RepoApi::removeElement(Id const &id)
{
	Q_ASSERT(id != Id::rootId());

	foreach (Id const child, children(id))
		removeElement(child);

	if (hasProperty(id, "links")) {
		IdList links = property(id, "links").value<IdList>();
		foreach (Id const link, links) {
			if (hasProperty(link, "from") && property(link, "from").value<Id>() == id)
				setProperty(link, "from", Id::rootId().toVariant());
			if (hasProperty(link, "to") && property(link, "to").value<Id>() == id)
				setProperty(link, "to", Id::rootId().toVariant());
		}
	}

	removeLinkEnds("from", id);
	removeLinkEnds("to", id);

	if (hasProperty(id, "outgoingConnections")) {
		IdList connections = property(id, "outgoingConnections").value<IdList>();
		foreach (Id const target, connections)
			disconnect(id, target);
	}

	if (hasProperty(id, "incomingConnections")) {
		IdList connections = property(id, "incomingConnections").value<IdList>();
		foreach (Id const source, connections)
			disconnect(source, id);
	}

	if (hasProperty(id, "outgoingUsages")) {
		IdList connections = property(id, "outgoingUsages").value<IdList>();
		foreach (Id const target, connections)
			deleteUsage(id, target);
	}

	if (hasProperty(id, "incomingUsages")) {
		IdList connections = property(id, "incomingUsages").value<IdList>();
		foreach (Id const source, connections)
			deleteUsage(source, id);
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
		if (mClient.property(link, direction).value<Id>() == id) {
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

qReal::IdList RepoApi::outgoingConnections(qReal::Id const &id) const
{
	return mClient.property(id, "outgoingConnections").value<IdList>();
}

qReal::IdList RepoApi::incomingConnections(qReal::Id const &id) const
{
	return mClient.property(id, "incomingConnections").value<IdList>();
}

void RepoApi::connect(qReal::Id const &source, qReal::Id const &destination)
{
	addToIdList(source, "outgoingConnections", destination);
	addToIdList(destination, "incomingConnections", source);
}

void RepoApi::disconnect(qReal::Id const &source, qReal::Id const &destination)
{
	removeFromList(source, "outgoingConnections", destination);
	removeFromList(destination, "incomingConnections", source);
}

qReal::IdList RepoApi::outgoingUsages(qReal::Id const &id) const
{
	return mClient.property(id, "outgoingUsages").value<IdList>();
}

qReal::IdList RepoApi::incomingUsages(qReal::Id const &id) const
{
	return mClient.property(id, "incomingUsages").value<IdList>();
}

void RepoApi::addUsage(qReal::Id const &source, qReal::Id const &destination)
{
	addToIdList(source, "outgoingUsages", destination);
	addToIdList(destination, "incomingUsages", source);
}

void RepoApi::deleteUsage(qReal::Id const &source, qReal::Id const &destination)
{
	removeFromList(source, "outgoingUsages", destination);
	removeFromList(destination, "incomingUsages", source);
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
		result.append(to(curLink));
	}
	return result;
}

qReal::IdList RepoApi::incomingConnectedElements(qReal::Id const &id) const
{
	qReal::IdList result;
	foreach (qReal::Id curLink, incomingLinks(id)) {
		result.append(from(curLink));
	}
	return result;
}

QString RepoApi::typeName(Id const &id) const
{
	return id.element();
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

bool RepoApi::hasProperty(Id const &id, QString const &propertyName) const
{
	return mClient.hasProperty(id, propertyName);
}

QMapIterator<QString, QVariant> RepoApi::propertiesIterator(const qReal::Id &id) const
{
	return mClient.propertiesIterator(id);
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

void RepoApi::open(QString const &workingDir)
{
	mClient.open(workingDir);
}

bool RepoApi::saveAll()
{
	bool result = mClient.saveAll();
	mErrors.append(mClient.newErrors());
	return result;
}

bool RepoApi::saveTo(QString const &workingDir)
{
	mClient.setWorkingDir(workingDir);
	bool result = mClient.saveAll();
	mErrors.append(mClient.newErrors());
	return result;
}

bool RepoApi::save(qReal::IdList list)
{
	bool result = mClient.save(list);
	mErrors.append(mClient.newErrors());
	return result;
}

bool RepoApi::doCheckout(const QString &from, const QString &to)
{
	bool result = mClient.doCheckout(from, to);
	mErrors.append(mClient.newErrors());
	return result;
}

bool RepoApi::doCommit(const QString &from, const QString &message)
{
	bool result = mClient.doCommit(from, message);
	mErrors.append(mClient.newErrors());
	return result;
}

bool RepoApi::doUpdate(const QString &to)
{
	bool result = mClient.doUpdate(to);
	mErrors.append(mClient.newErrors());
	return result;
}

bool RepoApi::doCleanUp(const QString &workingDir)
{
	bool result = mClient.doCleanUp(workingDir);
	mErrors.append(mClient.newErrors());
	return result;
}

QString RepoApi::info(const QString &workingDir)
{
	QString result = mClient.info(workingDir);
	mErrors.append(mClient.newErrors());
	return result;
}

QString RepoApi::repoUrl(const QString &workingDir)
{
	QString result = mClient.repoUrl(workingDir);
	mErrors.append(mClient.newErrors());
	return result;
}

int RepoApi::currentRevision(const QString &workingDir)
{
	int result = mClient.currentRevision(workingDir);
	mErrors.append(mClient.newErrors());
	return result;
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

IdList RepoApi::elementsByType(QString const &type) const
{
	IdList result;
	foreach (Id id, mClient.elements()) {
		if (id.element() == type)
			result.append(id);
	}
	return result;
}

int RepoApi::elementsCount() const
{
	return mClient.elements().size();
}

bool RepoApi::exist(Id const &id) const
{
	return mClient.exist(id);
}

QStringList RepoApi::newErrors()
{
	QStringList result(mErrors);
	mErrors.clear();
	return result;
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
