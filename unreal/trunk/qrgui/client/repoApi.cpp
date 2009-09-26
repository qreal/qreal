#include "repoApi.h"

using namespace qReal;
using namespace client;
using namespace details;

RepoApi::RepoApi()
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

void RepoApi::removeElement(Id const &id)
{
	Q_ASSERT(id != ROOT_ID);

	foreach (Id const child, children(id)) {
		removeElement(child);
	}

	foreach (Id const parent, parents(id)) {
		removeChild(parent, id);
	}

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

IdList RepoApi::outcomingLinks(Id const &id) const
{
	return links(id, "from");
}

IdList RepoApi::incomingLinks(Id const &id) const
{
	return links(id, "to");
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

void RepoApi::save() const
{
	mClient.save();
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
