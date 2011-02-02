#include "../repoApi.h"

#include <QDebug>

using namespace qrRepo;
using namespace qrRepo::details;
using namespace qReal;

RepoApi::RepoApi(QString const &workingDirectory)
	: mClient(workingDirectory)
{
}

QString RepoApi::name(NewType const &type) const
{
        Q_ASSERT(mClient.property(type, "name").canConvert<QString>());
        return mClient.property(type, "name").toString();
}

void RepoApi::setName(NewType const &type, QString const &name)
{
        mClient.setProperty(type, "name", name);
        addChangedDiagram(type.diagramId());

}

TypeList RepoApi::children(NewType const &type) const
{
        return mClient.children(type);
}

void RepoApi::addChild(NewType const &type, NewType const &child)
{
        mClient.addChild(type, child);
        addChangedDiagram(type.diagramId());
}

void RepoApi::removeChild(NewType const &type, NewType const &child)
{
        mClient.removeChild(type, child);
        addChangedDiagram(type.diagramId());
}

void RepoApi::removeChildren(NewType const &type)
{
        foreach (NewType const child, children(type))
                removeChild(type, child);
        addChangedDiagram(type.diagramId());
}

void RepoApi::removeElement(NewType const &type)
{
        Q_ASSERT(type != ROOT_ID);

        foreach (NewType const child, children(type))
		removeElement(child);

// 	removeChild() в клиенте какой-то слишком умный, делает много лишнего, потому пока его убираем

//	foreach (NewType const parent, parents(id)) {
//		removeChild(parent, id);
//	}

        if (hasProperty(type, "links")) {
                TypeList links = property(type, "links").value<TypeList>();
                foreach (NewType const link, links) {
                        if (hasProperty(link, "from") && property(link, "from").value<NewType>() == type)
				setProperty(link, "from", ROOT_ID.toVariant());
                        if (hasProperty(link, "to") && property(link, "to").value<NewType>() == type)
				setProperty(link, "to", ROOT_ID.toVariant());
		}
	}

        removeLinkEnds("from", type);
        removeLinkEnds("to", type);

        if (hasProperty(type, "outgoingConnections")) {
                TypeList connections = property(type, "outgoingConnections").value<TypeList>();
                foreach (NewType const target, connections)
                        disconnect(type, target);
	}

        if (hasProperty(type, "incomingConnections")) {
                TypeList connections = property(type, "incomingConnections").value<TypeList>();
                foreach (NewType const source, connections)
                        disconnect(source, type);
	}

        if (hasProperty(type, "outgoingUsages")) {
                TypeList connections = property(type, "outgoingUsages").value<TypeList>();
                foreach (NewType const target, connections)
                        deleteUsage(type, target);
	}

        if (hasProperty(type, "incomingUsages")) {
                TypeList connections = property(type, "incomingUsages").value<TypeList>();
                foreach (NewType const source, connections)
                        deleteUsage(source, type);
	}

	// И так далее для всех возможных видов ссылок и для всех их комбинаций...
	// Впрочем, может, этого делать и не надо.

        addChangedDiagram(type.diagramId());
}

void RepoApi::removeLinkEnds(QString const &endName, NewType const &type) {
        if (hasProperty(type, endName)) {
                NewType target = property(type, endName).value<NewType>();
		if (hasProperty(target, "links")) {
                        removeFromList(target, "links", type);
		}
	}
        addChangedDiagram(type.diagramId());
}

TypeList RepoApi::parents(NewType const &type) const
{
        return mClient.parents(type);
}

void RepoApi::addParent(NewType const &type, NewType const &parent)
{
        mClient.addParent(type, parent);
        addChangedDiagram(type.diagramId());
}

void RepoApi::removeParent(NewType const &type, qReal::NewType const &parent)
{
        mClient.removeParent(type, parent);
        addChangedDiagram(type.diagramId());
}

TypeList RepoApi::links(NewType const &type, QString const &direction) const
{
        TypeList links = mClient.property(type, "links").value<TypeList>();
        TypeList result;
        foreach (NewType const link, links) {
                if (mClient.property(link, direction).value<NewType>() == type) {
			result.append(link);
		}
	}
	return result;
}

TypeList RepoApi::outgoingLinks(NewType const &type) const
{
        return links(type, "from");
}

TypeList RepoApi::incomingLinks(NewType const &type) const
{
        return links(type, "to");
}

TypeList RepoApi::links(NewType const &type) const
{
        return incomingLinks(type) << outgoingLinks(type);
}

qReal::TypeList RepoApi::outgoingConnections(qReal::NewType const &type) const
{
        return mClient.property(type, "outgoingConnections").value<TypeList>();
}

qReal::TypeList RepoApi::incomingConnections(qReal::NewType const &type) const
{
        return mClient.property(type, "incomingConnections").value<TypeList>();
}

void RepoApi::connect(qReal::NewType const &source, qReal::NewType const &destination)
{
        addToTypeList(source, "outgoingConnections", destination);
        addToTypeList(destination, "incomingConnections", source);
	addChangedDiagram(source.diagramId());
}

void RepoApi::disconnect(qReal::NewType const &source, qReal::NewType const &destination)
{
	removeFromList(source, "outgoingConnections", destination);
	removeFromList(destination, "incomingConnections", source);
	addChangedDiagram(source.diagramId());
}

qReal::TypeList RepoApi::outgoingUsages(qReal::NewType const &type) const
{
        return mClient.property(type, "outgoingUsages").value<TypeList>();
}

qReal::TypeList RepoApi::incomingUsages(qReal::NewType const &type) const
{
        return mClient.property(type, "incomingUsages").value<TypeList>();
}

void RepoApi::addUsage(qReal::NewType const &source, qReal::NewType const &destination)
{
        addToTypeList(source, "outgoingUsages", destination);
        addToTypeList(destination, "incomingUsages", source);
	addChangedDiagram(source.diagramId());
}

void RepoApi::deleteUsage(qReal::NewType const &source, qReal::NewType const &destination)
{
	removeFromList(source, "outgoingUsages", destination);
	removeFromList(destination, "incomingUsages", source);
	addChangedDiagram(source.diagramId());
}

QString RepoApi::typeName(NewType const &type) const
{
        return type.element();
}

QVariant RepoApi::property(NewType const &type, QString const &propertyName) const
{
        return mClient.property(type, propertyName);
}

QString RepoApi::stringProperty(NewType const &type, QString const &propertyName) const
{
        Q_ASSERT(mClient.property(type, propertyName).canConvert<QString>());
        return mClient.property(type, propertyName).toString();
}

void RepoApi::setProperty(NewType const &type, QString const &propertyName, QVariant const &value)
{
        if (mClient.hasProperty(type, propertyName) &&
                (mClient.property(type, propertyName) != value))
                addChangedDiagram(type.diagramId());
        mClient.setProperty(type, propertyName, value);
}

void RepoApi::removeProperty(NewType const &type, QString const &propertyName)
{
        mClient.removeProperty(type, propertyName);
        addChangedDiagram(type.diagramId());
}

bool RepoApi::hasProperty(NewType const &type, QString const &propertyName) const
{
        return mClient.hasProperty(type, propertyName);
}

NewType RepoApi::from(NewType const &type) const
{
        Q_ASSERT(mClient.property(type, "from").canConvert<NewType>());
        return mClient.property(type, "from").value<NewType>();
}

void RepoApi::setFrom(NewType const &type, NewType const &from)
{
        NewType prev = mClient.property(type, "from").value<NewType>();
        removeFromList(prev, "links", type);
        mClient.setProperty(type, "from", from.toVariant());
        addToTypeList(from, "links", type);
        addChangedDiagram(type.diagramId());
}

NewType RepoApi::to(NewType const &type) const
{
        Q_ASSERT(mClient.property(type, "to").canConvert<NewType>());
        return mClient.property(type, "to").value<NewType>();
}

void RepoApi::setTo(NewType const &type, NewType const &to)
{
        NewType prev = mClient.property(type, "to").value<NewType>();
        removeFromList(prev, "links", type);
        mClient.setProperty(type, "to", to.toVariant());
        addToTypeList(to, "links", type);
        addChangedDiagram(type.diagramId());
}

double RepoApi::fromPort(NewType const &type) const
{
        Q_ASSERT(mClient.property(type, "fromPort").canConvert<double>());
        return mClient.property(type, "fromPort").value<double>();
}

void RepoApi::setFromPort(NewType const &type, double fromPort)
{
        mClient.setProperty(type, "fromPort", fromPort);
        addChangedDiagram(type.diagramId());
}

double RepoApi::toPort(NewType const &type) const
{
        Q_ASSERT(mClient.property(type, "toPort").canConvert<double>());
        return mClient.property(type, "toPort").value<double>();
}

void RepoApi::setToPort(NewType const &type, double toPort)
{
        mClient.setProperty(type, "toPort", toPort);
        addChangedDiagram(type.diagramId());
}

void RepoApi::exterminate()
{
	mClient.exterminate();
}

void RepoApi::open(QString const &workingDir)
{
	mClient.open(workingDir);
}

void RepoApi::log(QString message, qReal::NewType diagram)
{
	mClient.log(message, diagram);
}

void RepoApi::saveAll() const
{
	mClient.saveAll();
}

void RepoApi::saveTo(QString const &workingDir)
{
	mClient.setWorkingDir(workingDir);
	mClient.saveAll();
}

void RepoApi::save(qReal::TypeList list) const
{
        qDebug() << "RepoApi::save(TypeList), list.size() > 0 == " << (list.size()>0);
	mClient.save(list);
}

void RepoApi::remove(qReal::TypeList list) const
{
        qDebug() << "RepoApi::remove(TypeList), list.size() > 0 == " << (list.size()>0);

	foreach(ChildsOfDiagram pair, mDiagramsDeleted)
                foreach(NewType diagram, list) {
			if (pair.first == diagram)
				mClient.remove(pair.second);
		}
}



void RepoApi::addToTypeList(NewType const &target, QString const &listName, NewType const &data)
{
	if (target == ROOT_ID)
		return;

        TypeList list = mClient.property(target, listName).value<TypeList>();

	// Значения в списке должны быть уникальны.
	if (list.contains(data))
		return;

	list.append(data);
        mClient.setProperty(target, listName, TypeListHelper::toVariant(list));

	addChangedDiagram(target.diagramId());
}

void RepoApi::removeFromList(NewType const &target, QString const &listName, NewType const &data)
{
	if (target == ROOT_ID)
		return;

        TypeList list = mClient.property(target, listName).value<TypeList>();
	list.removeAll(data);

        mClient.setProperty(target, listName, TypeListHelper::toVariant(list));
	addChangedDiagram(target.diagramId());
}

NewType RepoApi::otherEntityFromLink(NewType const &linkId, NewType const &firstNode) const
{
        NewType const fromId = from(linkId);
        if (fromId != firstNode)
		return fromId;
	else
		return to(linkId);
}

TypeList RepoApi::elements(NewType const &type) const
{
        Q_ASSERT(type.typeSize() == 3);  // Применимо только к типам

        TypeList result;
        foreach (NewType type, mClient.elements()) {
		 // Так должно быть
		// if (id.type() == type)
		// Так есть
                if (type.element() == type.element())
                        result.append(type);
	}
	return result;
}

TypeList RepoApi::elementsByType(QString const &typeString) const
{
        TypeList result;
        foreach (NewType type, mClient.elements()) {
                if (type.element() == typeString)
                        result.append(type);
	}
	return result;
}

int RepoApi::elementsCount() const
{
	return mClient.elements().size();
}

bool RepoApi::exist(NewType const &type) const
{
        return mClient.exist(type);
}

TypeList RepoApi::getOpenedDiagrams() const
{
	return mDiagramsOpened.values();
}

TypeList RepoApi::getChangedDiagrams() const
{
	return mDiagramsChanged.values();
}

void RepoApi::resetChangedDiagrams()
{
	mDiagramsChanged.clear();
}

void RepoApi::addOpenedDiagram(const NewType &type)
{
        mDiagramsOpened.insert(type);
}

void RepoApi::addChangedDiagram(const NewType &type)
{
        mDiagramsChanged.insert(type);
/**	if (!exist(type)) {
                QPair<Id,TypeList> pair = qMakePair(id,mClient.typesOfAllChildrenOf(id));
		if (!mDiagramsDeleted.contains(pair))
			this->mDiagramsDeleted.append(pair);
	}**/
}

void RepoApi::resetChangedDiagrams(const TypeList &list)
{
        foreach(NewType type, list)
                mDiagramsChanged.remove(type);
}
