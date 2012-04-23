#include "../repoApi.h"
#include "../../qrkernel/exception/exception.h"


#include <QDebug>

using namespace qrRepo;
using namespace qrRepo::details;
using namespace qReal;
RepoApi::RepoApi(QString const &workingDirectory)
{
    /*Considering first in list as main save-file
    others will be appended to latter indexes.

    Methods without comment - "multirepos" will work in legacy mode (only with main save-file)
    */

    mClients = new QList<details::Client*>();
    Client *defaultClient = new Client(workingDirectory);
    mClients->append(defaultClient);
}


/// Searches for client corresponding to passes id
/// @return is link for client, representig save file inclusing requested id
details::Client* RepoApi::getRelevantClient(const qReal::Id &id) const
{
        for (int i = 0; i < mClients->count(); ++i){
        if(mClients->at(i)->exist(id))
            return mClients->at(i);
    }
    Client *client = mClients->at(0);
    return client;
    throw Exception("RepoApi: Requesting nonexistent object " + id.toString());
}

/// @return client created at initialization
/// it is considered as a default one
details::Client* RepoApi::getDefaultClient() const
{
    int defaultPosition = 0;
    return mClients->at(defaultPosition);
}

QString RepoApi::name(Id const &id) const
{
    Client *client = this->getRelevantClient(id);
    Q_ASSERT(client->property(id, "name").canConvert<QString>());
    return client->property(id, "name").toString();
}

void RepoApi::setName(Id const &id, QString const &name)
{
    Client *client = getRelevantClient(id);
    client->setProperty(id, "name", name);
}

IdList RepoApi::children(Id const &id) const
{
    Client *client = getRelevantClient(id);
    return client->children(id);
}


void RepoApi::addChild(Id const &id, Id const &child)
{
    Client *client = getRelevantClient(id);
    client->addChild(id, child);
}


void RepoApi::addChild(Id const &id, Id const &child, Id const &logicalId)
{
    Client *client = getRelevantClient(id);
    client->addChild(id, child, logicalId);
}


void RepoApi::stackBefore(Id const &id, Id const &child, Id const &sibling)
{
    Client *client = getRelevantClient(id);
    client->stackBefore(id, child, sibling);
}


Id RepoApi::copy(qReal::Id const &src)
{
    Client *client = getRelevantClient(src);
    return client->cloneObject(src);
}


void RepoApi::removeChild(Id const &id, Id const &child)
{
    Client *client = getRelevantClient(id);
    client->removeChild(id, child);
}


void RepoApi::removeChildren(Id const &id)
{
	foreach (Id const child, children(id))
		removeChild(id, child);
}


void RepoApi::removeElement(Id const &id)
{
	Q_ASSERT(id != Id::rootId());

    Client *client = getRelevantClient(id);

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

    client->remove(id);
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
    Client *client = getRelevantClient(id);
    return client->parent(id);
}


void RepoApi::setParent(Id const &id, Id const &parent)
{
    Client *client = getRelevantClient(id);
    Id const oldParent = client->parent(id);
    client->removeChild(oldParent, id);
    client->setParent(id, parent);
}


IdList RepoApi::links(Id const &id, QString const &direction) const
{
    Client *client = getRelevantClient(id);
    IdList links = client->property(id, "links").value<IdList>();
	IdList result;
	foreach (Id const link, links) {
        if (client->property(link, direction).value<Id>() == id) {
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
    Client *client = getRelevantClient(id);
    return client->property(id, "outgoingConnections").value<IdList>();
}


qReal::IdList RepoApi::incomingConnections(qReal::Id const &id) const
{
    Client *client = getRelevantClient(id);
    return client->property(id, "incomingConnections").value<IdList>();
}


/// Creating connection link between entities
/// @param source is entity where connection starts
/// @param destination is entity where connection ends
/// Be aware, problems may occur if entities are from different clients
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
    Client *client = getRelevantClient(id);
    return client->property(id, "outgoingUsages").value<IdList>();
}


qReal::IdList RepoApi::incomingUsages(qReal::Id const &id) const
{
    Client *client = getRelevantClient(id);
    return client->property(id, "incomingUsages").value<IdList>();
}


/// Creating usage link between entities
/// param source is entity where usage link starts
/// param destination is entity where usage link ends
/// Be aware, problems may occur if entities are from different clients
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
		qReal::Id toElem = to(curLink);
		//if (toElem == Id::rootId())
		//	continue;

		result.append(toElem);
	}
	return result;
}


qReal::IdList RepoApi::incomingConnectedElements(qReal::Id const &id) const
{
	qReal::IdList result;
	foreach (qReal::Id curLink, incomingLinks(id)) {
		qReal::Id fromElem = from(curLink);
		//if (fromElem == Id::rootId())
		//	continue;

		result.append(fromElem);
	}
	return result;
}

QString RepoApi::typeName(Id const &id) const
{
	return id.element();
}


QVariant RepoApi::property(Id const &id, QString const &propertyName) const
{
    Client *client = getRelevantClient(id);
    return client->property(id, propertyName);
}


QString RepoApi::stringProperty(Id const &id, QString const &propertyName) const
{
    Client *client = getRelevantClient(id);
    Q_ASSERT(client->property(id, propertyName).canConvert<QString>());
    return client->property(id, propertyName).toString();
}


void RepoApi::setProperty(Id const &id, QString const &propertyName, QVariant const &value)
{
    Client *client = getRelevantClient(id);
    client->setProperty(id, propertyName, value);
}


void RepoApi::removeProperty(Id const &id, QString const &propertyName)
{
    Client *client = getRelevantClient(id);
    client->removeProperty(id, propertyName);
}


void RepoApi::copyProperties(const Id &dest, const Id &src)
{
    Client *client = getRelevantClient(src);
    client->copyProperties(dest, src);
}


bool RepoApi::hasProperty(Id const &id, QString const &propertyName) const
{
    Client *client = getRelevantClient(id);
    return client->hasProperty(id, propertyName);
}


Id RepoApi::from(Id const &id) const
{
    Client *client = getRelevantClient(id);
    Q_ASSERT(client->property(id, "from").canConvert<Id>());
    return client->property(id, "from").value<Id>();
}


void RepoApi::setFrom(Id const &id, Id const &from)
{
    Client *client = getRelevantClient(id);
	if (hasProperty(id, "from")) {
        Id prev = client->property(id, "from").value<Id>();
		removeFromList(prev, "links", id, "from");
	}
    client->setProperty(id, "from", from.toVariant());
	addToIdList(from, "links", id, "from");
}


Id RepoApi::to(Id const &id) const
{
    Client *client = getRelevantClient(id);
    Q_ASSERT(client->property(id, "to").canConvert<Id>());
    return client->property(id, "to").value<Id>();
}


void RepoApi::setTo(Id const &id, Id const &to)
{
    Client *client = getRelevantClient(id);
	if (hasProperty(id, "to")) {
        Id prev = client->property(id, "to").value<Id>();
		removeFromList(prev, "links", id, "to");
	}
    client->setProperty(id, "to", to.toVariant());
	addToIdList(to, "links", id, "to");
}


double RepoApi::fromPort(Id const &id) const
{
    Client *client = getRelevantClient(id);
    Q_ASSERT(client->property(id, "fromPort").canConvert<double>());
    return client->property(id, "fromPort").value<double>();
}


void RepoApi::setFromPort(Id const &id, double fromPort)
{
    Client *client = getRelevantClient(id);
    client->setProperty(id, "fromPort", fromPort);
}


double RepoApi::toPort(Id const &id) const
{
    Client *client = getRelevantClient(id);
    Q_ASSERT(client->property(id, "toPort").canConvert<double>());
    return client->property(id, "toPort").value<double>();
}


void RepoApi::setToPort(Id const &id, double toPort)
{
    Client *client = getRelevantClient(id);
    client->setProperty(id, "toPort", toPort);
}


QVariant RepoApi::position(Id const &id) const
{
    Client *client = getRelevantClient(id);
    return client->property(id, "position");
}


QVariant RepoApi::configuration(Id const &id) const
{
    Client *client = getRelevantClient(id);
    return client->property(id, "configuration");
}


void RepoApi::setPosition(Id const &id, QVariant const &position)
{
    Client *client = getRelevantClient(id);
    client->setProperty(id, "position", position);
}


void RepoApi::setConfiguration(Id const &id, QVariant const &configuration)
{
    Client *client = getRelevantClient(id);
    client->setProperty(id, "configuration", configuration);
}


bool RepoApi::isLogicalElement(qReal::Id const &id) const
{
    Client *client = getRelevantClient(id);
    return client->isLogicalId(id);
}


bool RepoApi::isGraphicalElement(qReal::Id const &id) const
{
    Client *client = getRelevantClient(id);
    return !client->isLogicalId(id);
}


qReal::Id RepoApi::logicalId(qReal::Id const &id) const
{
    Client *client = getRelevantClient(id);
    return client->logicalId(id);
}


//investigated
void RepoApi::exterminate()
{
    for (int i = 0; i < mClients->count(); i++){
        mClients->at(i)->exterminate();
    }
}


/// Open new project, disregard all previosly opened
/// usage scenario is staring work with completely new project
void RepoApi::open(QString const &saveFile)
{
    mClients->clear();
    Client *openedSaveFile = new Client(saveFile);
    mClients->append(openedSaveFile);
}

/// Opens additional save file
/// original scenario is loading save file consistiong of library files
/// changes to this files aren't intendent
void RepoApi::loadSaveFile(QString const &saveFile)
{
    Client *saveFileToAppend = new Client(saveFile);
    mClients->append(saveFileToAppend);
}

//Default
/// Saving default project in default location
void RepoApi::saveAll() const
{
    Client *client = getDefaultClient();

    client->saveAll();
}

//Default
/// Saving current project in specified location
void RepoApi::saveTo(QString const &workingFile)
{
    Client *client = getDefaultClient();
    client->setWorkingFile(workingFile);

    client->saveAll();
}

//Default
/// Importing specified file into default project
/// usage scenario is pasting saved file into currently opened project
void RepoApi::importFromDisk(QString const &importedFile)
{
    Client *client = getDefaultClient();

    client->importFromDisk(importedFile);
}

//Default
/// Save selected elements with all of their children into path of current project
void RepoApi::save(qReal::IdList list) const
{
    Client *client = getDefaultClient();

    client->save(list);
}

//Default
/// Get savefile location of current project
QString RepoApi::workingFile() const
{
    Client *client = getDefaultClient();

    return client->workingFile();
}


void RepoApi::addToIdList(Id const &target, QString const &listName, Id const &data, QString const &direction)
{
    Client *client = getRelevantClient(target);

	if (target == Id::rootId())
		return;

    IdList list = client->property(target, listName).value<IdList>();

	// Ð â€”Ð Ð…Ð Â°Ð¡â€¡Ð ÂµÐ Ð…Ð Ñ‘Ð¡Ð Ð Ð† Ð¡ÐƒÐ Ñ—Ð Ñ‘Ð¡ÐƒÐ Ñ”Ð Âµ Ð Ò‘Ð Ñ•Ð Â»Ð Â¶Ð Ð…Ð¡â€¹ Ð Â±Ð¡â€¹Ð¡â€šÐ¡ÐŠ Ð¡Ñ“Ð Ð…Ð Ñ‘Ð Ñ”Ð Â°Ð Â»Ð¡ÐŠÐ Ð…Ð¡â€¹.
	if (list.contains(data))
		return;

	list.append(data);
    client->setProperty(target, listName, IdListHelper::toVariant(list));

	if (listName == "links") {
        IdList temporaryRemovedList = client->temporaryRemovedLinksAt(target, direction);
		temporaryRemovedList.removeAll(data);
        client->setTemporaryRemovedLinks(target, direction, temporaryRemovedList);
	}
}


void RepoApi::removeFromList(Id const &target, QString const &listName, Id const &data, QString const &direction)
{
    Client *client = getRelevantClient(target);

	if (target == Id::rootId())
		return;

    IdList list = client->property(target, listName).value<IdList>();
    IdList temporaryRemovedList = client->temporaryRemovedLinksAt(target, direction);
	if(listName == "links" && list.contains(data)) {
		temporaryRemovedList.append(data);
	}
	list.removeAll(data);

    client->setProperty(target, listName, IdListHelper::toVariant(list));
    client->setTemporaryRemovedLinks(target, direction, temporaryRemovedList);
}


Id RepoApi::otherEntityFromLink(Id const &linkId, Id const &firstNode) const
{
	Id const fromId = from(linkId);
	if (fromId != firstNode)
		return fromId;
	else
		return to(linkId);
}

//Default
// Used in EditorViewScene::createAddConnectionMenu
// thus doesn't alloved to see clients other than default
IdList RepoApi::logicalElements(Id const &type) const
{
	Q_ASSERT(type.idSize() == 3);
    Client *client = getDefaultClient();

	IdList result;
    foreach (Id id, client->elements()) {
        if (id.element() == type.element() && client->isLogicalId(id))
			result.append(id);
	}
	return result;
}

//Default
IdList RepoApi::graphicalElements(Id const &type) const
{
	Q_ASSERT(type.idSize() == 3);
    Client *client = getDefaultClient();

	IdList result;
    foreach (Id id, client->elements()) {
        if (id.element() == type.element() && !client->isLogicalId(id))
			result.append(id);
	}
	return result;
}

//Default
IdList RepoApi::elementsByType(QString const &type) const
{
    Client *client = getDefaultClient();
	IdList result;
    foreach (Id id, client->elements()) {
		if (id.element() == type)
			result.append(id);
	}
	return result;
}

//Default
int RepoApi::elementsCount() const
{
    Client *client = getDefaultClient();
    return client->elements().size();
}

//Default
bool RepoApi::exist(Id const &id) const
{
    Client *client = getDefaultClient();
    return client->exist(id);
}

//Default
IdList RepoApi::temporaryRemovedLinksAt(Id const &id, QString const &direction) const
{
    Client *client = getDefaultClient();
    return client->temporaryRemovedLinksAt(id, direction);
}

//Default
void RepoApi::setTemporaryRemovedLinks(Id const &id, IdList const &value, QString const &direction)
{
    Client *client = getDefaultClient();
    client->setTemporaryRemovedLinks(id, direction, value);
}

//Default
void RepoApi::removeTemporaryRemovedLinks(Id const &id)
{
    Client *client = getDefaultClient();
    client->removeTemporaryRemovedLinks(id);
}
