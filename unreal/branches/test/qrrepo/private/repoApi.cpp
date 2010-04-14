#include "../repoApi.h"

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

void RepoApi::removeChild(Id const &id, Id const &child)
{
	mClient.removeChild(id, child);
}

void RepoApi::removeChildren(qReal::Id const &id)
{
	foreach (Id const child, children(id))
		removeChild(id, child);
}

void RepoApi::removeElement(Id const &id)
{
	Q_ASSERT(id != ROOT_ID);

	foreach (Id const child, children(id))
		removeElement(child);

// 	removeChild() в клиенте какой-то слишком умный, делает много лишнего, потому пока его убираем

//	foreach (Id const parent, parents(id)) {
//		removeChild(parent, id);
//	}

	if (hasProperty(id, "links")) {
		IdList links = property(id, "links").value<IdList>();
		foreach (Id const link, links) {
			if (hasProperty(link, "from") && property(link, "from").value<Id>() == id)
				setProperty(link, "from", ROOT_ID.toVariant());
			if (hasProperty(link, "to") && property(link, "to").value<Id>() == id)
				setProperty(link, "to", ROOT_ID.toVariant());
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

	// И так далее для всех возможных видов ссылок и для всех их комбинаций...
	// Впрочем, может, этого делать и не надо.
}

void RepoApi::removeLinkEnds(QString const &endName, Id const &id) {
	if (hasProperty(id, endName)) {
		Id target = property(id, endName).value<Id>();
		if (hasProperty(target, "links")) {
			removeFromList(target, "links", id);
		}
	}
}

IdList RepoApi::parents(Id const &id) const
{
	return mClient.parents(id);
}

void RepoApi::addParent(Id const &id, Id const &parent)
{
	mClient.addParent(id, parent);
}

void RepoApi::removeParent(qReal::Id const &id, qReal::Id const &parent)
{
	mClient.removeParent(id, parent);
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

Id RepoApi::from(Id const &id) const
{
	Q_ASSERT(mClient.property(id, "from").canConvert<Id>());
	return mClient.property(id, "from").value<Id>();
}

void RepoApi::setFrom(Id const &id, Id const &from)
{
	Id prev = mClient.property(id, "from").value<Id>();
	removeFromList(prev, "links", id);
	mClient.setProperty(id, "from", from.toVariant());
	addToIdList(from, "links", id);
}

Id RepoApi::to(Id const &id) const
{
	Q_ASSERT(mClient.property(id, "to").canConvert<Id>());
	return mClient.property(id, "to").value<Id>();
}

void RepoApi::setTo(Id const &id, Id const &to)
{
	Id prev = mClient.property(id, "to").value<Id>();
	removeFromList(prev, "links", id);
	mClient.setProperty(id, "to", to.toVariant());
	addToIdList(to, "links", id);
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

void RepoApi::exterminate()
{
	mClient.exterminate();
}

void RepoApi::open(QString const &workingDir)
{
	mClient.open(workingDir);
}

void RepoApi::save() const
{
	mClient.save();
}

void RepoApi::saveTo(QString const &workingDir)
{
	mClient.saveTo(workingDir);
}

void RepoApi::addToIdList(Id const &target, QString const &listName, Id const &data)
{
	if (target == ROOT_ID)
		return;

	IdList list = mClient.property(target, listName).value<IdList>();

	// Значения в списке должны быть уникальны.
	if (list.contains(data))
		return;

	list.append(data);
	mClient.setProperty(target, listName, IdListHelper::toVariant(list));
}

void RepoApi::removeFromList(Id const &target, QString const &listName, Id const &data)
{
	if (target == ROOT_ID)
		return;

	IdList list = mClient.property(target, listName).value<IdList>();
	list.removeAll(data);

	mClient.setProperty(target, listName, IdListHelper::toVariant(list));
}

Id RepoApi::otherEntityFromLink(Id const &linkId, Id const &firstNode) const
{
	Id const fromId = from(linkId);
	if (fromId != firstNode)
		return fromId;
	else
		return to(linkId);
}

IdList RepoApi::elements(Id const &type) const
{
	Q_ASSERT(type.idSize() == 3);  // Применимо только к типам

	IdList result;
	foreach (Id id, mClient.elements()) {
		// Так должно быть
		// if (id.type() == type)
		// Так есть
		if (id.element() == type.element())
			result.append(id);
	}
	return result;
}
