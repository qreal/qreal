#include "repository.h"

#include <QtCore/QDebug>

#include "../../qrkernel/exception/exception.h"
#include "singleXmlSerializer.h"

using namespace qReal;
using namespace qrRepo;
using namespace qrRepo::details;

Repository::Repository(QString const &workingFile)
		: mWorkingFile(workingFile)
		, mSerializer(workingFile)
{
	init();
	loadFromDisk();
}


void Repository::init()
{
	mObjects.insert(Id::rootId(), new LogicalObject(Id::rootId()));
	mObjects[Id::rootId()]->setProperty("name", Id::rootId().toString());
}

Repository::~Repository()
{
	mSerializer.clearWorkingDir();

	foreach (Id id, mObjects.keys()) {
		delete mObjects[id];
	}
}

IdList Repository::findElementsByName(QString const &name, bool sensitivity, bool regExpression) const
{
	Qt::CaseSensitivity const caseSensitivity = sensitivity ? Qt::CaseSensitive : Qt::CaseInsensitive;

	QRegExp const regExp(name, caseSensitivity);
	IdList result;

	if (regExpression){
		foreach (Object * const element, mObjects.values()) {
			if (element->property("name").toString().contains(regExp)
					&& !isLogicalId(mObjects.key(element))) {
				result.append(mObjects.key(element));
			}
		}
	} else {
		foreach (Object * const element, mObjects.values()) {
			if (element->property("name").toString().contains(name, caseSensitivity)
					&& !isLogicalId(mObjects.key(element))) {
				result.append(mObjects.key(element));
			}
		}
	}

	return result;
}

qReal::IdList Repository::elementsByProperty(QString const &property, bool sensitivity
		, bool regExpression) const
{
	IdList result;

	foreach (Object *element, mObjects.values()) {
		if ((element->hasProperty(property, sensitivity, regExpression))
				&& (!isLogicalId(mObjects.key(element)))) {
			result.append(mObjects.key(element));
		}
	}

	return result;
}

qReal::IdList Repository::elementsByPropertyContent(QString const &propertyValue, bool sensitivity
		, bool regExpression) const
{
	Qt::CaseSensitivity const caseSensitivity = sensitivity ? Qt::CaseSensitive : Qt::CaseInsensitive;

	QRegExp const regExp(propertyValue, caseSensitivity);
	IdList result;

	foreach (Object * const element, mObjects.values()) {
		QMapIterator<QString, QVariant> iterator = element->propertiesIterator();
		if (regExpression) {
			while (iterator.hasNext()) {
				if (iterator.next().value().toString().contains(regExp)) {
					result.append(mObjects.key(element));
					break;
				}
			}
		} else {
			while (iterator.hasNext()) {
				if (iterator.next().value().toString().contains(propertyValue, caseSensitivity)) {
					result.append(mObjects.key(element));
					break;
				}
			}
		}
	}

	return result;
}

void Repository::replaceProperties(qReal::IdList const &toReplace, QString const value, QString const newValue)
{
	foreach (qReal::Id const &currentId, toReplace) {
		mObjects[currentId]->replaceProperties(value, newValue);
	}
}

IdList Repository::children(Id const &id) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->children();
	} else {
		throw Exception("Repository: Requesting children of nonexistent object " + id.toString());
	}
}

Id Repository::parent(Id const &id) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->parent();
	} else {
		throw Exception("Repository: Requesting parents of nonexistent object " + id.toString());
	}
}

Id Repository::cloneObject(qReal::Id const &id)
{
	Object const * const result = mObjects[id]->clone(mObjects);
	return result->id();
}

void Repository::setParent(Id const &id, Id const &parent)
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(parent)) {
			mObjects[id]->setParent(parent);
			if (!mObjects[parent]->children().contains(id))
				mObjects[parent]->addChild(id);
		} else {
			throw Exception("Repository: Adding nonexistent parent " + parent.toString() + " to  object " + id.toString());
		}
	} else {
		throw Exception("Repository: Adding parent " + parent.toString() + " to nonexistent object " + id.toString());
	}
}

void Repository::addChild(const Id &id, const Id &child)
{
	addChild(id, child, Id());
}

void Repository::addChild(const Id &id, const Id &child, Id const &logicalId)
{
	if (mObjects.contains(id)) {
		if (!mObjects[id]->children().contains(child))
			mObjects[id]->addChild(child);

		if (mObjects.contains(child)) { // should we move element?
			mObjects[child]->setParent(id);
		} else {
			Object * const object = logicalId.isNull()
					? static_cast<Object *>(new LogicalObject(child))
					: static_cast<Object *>(new GraphicalObject(child, id, logicalId))
					;

			object->setParent(id);

			mObjects.insert(child, object);
		}
	} else {
		throw Exception("Repository: Adding child " + child.toString() + " to nonexistent object " + id.toString());
	}
}

void Repository::stackBefore(qReal::Id const &id, qReal::Id const &child, qReal::Id const &sibling) {
	if(!mObjects.contains(id)) {
		throw Exception("Repository: Moving child " + child.toString() + " of nonexistent object " + id.toString());
	}

	if(!mObjects.contains(child)) {
		throw Exception("Repository: Moving nonexistent child " + child.toString());
	}

	if(!mObjects.contains(sibling)) {
		throw Exception("Repository: Stacking before nonexistent child " + sibling.toString());
	}

	mObjects[id]->stackBefore(child, sibling);
}

void Repository::removeParent(const Id &id)
{
	if (mObjects.contains(id)) {
		Id const parent = mObjects[id]->parent();
		if (mObjects.contains(parent)) {
			mObjects[id]->setParent(Id());
			mObjects[parent]->removeChild(id);
		} else {
			throw Exception("Repository: Removing nonexistent parent " + parent.toString() + " from object " + id.toString());
		}
	} else {
		throw Exception("Repository: Removing parent from nonexistent object " + id.toString());
	}
}

void Repository::removeChild(const Id &id, const Id &child)
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(child)) {
			mObjects[id]->removeChild(child);
		} else {
			throw Exception("Repository: removing nonexistent child " + child.toString() + " from object " + id.toString());
		}
	} else {
		throw Exception("Repository: removing child " + child.toString() + " from nonexistent object " + id.toString());
	}
}

void Repository::setProperty(const Id &id, QString const &name, const QVariant &value ) const
{
	if (mObjects.contains(id)) {
		// see Object::property() for details
//		Q_ASSERT(mObjects[id]->hasProperty(name)
//				 ? mObjects[id]->property(name).userType() == value.userType()
//				 : true);
		mObjects[id]->setProperty(name, value);
	} else {
		throw Exception("Repository: Setting property of nonexistent object " + id.toString());
	}
}

void Repository::copyProperties(const Id &dest, const Id &src)
{
	mObjects[dest]->copyPropertiesFrom(*mObjects[src]);
}

QMap<QString, QVariant> Repository::properties(Id const &id)
{
	return mObjects[id]->properties();
}

void Repository::setProperties(Id const &id, QMap<QString, QVariant> const &properties)
{
	mObjects[id]->setProperties(properties);
}

QVariant Repository::property( const Id &id, QString const &name ) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->property(name);
	} else {
		throw Exception("Repository: Requesting property of nonexistent object " + id.toString());
	}
}

void Repository::removeProperty( const Id &id, QString const &name )
{
	if (mObjects.contains(id)) {
		return mObjects[id]->removeProperty(name);
	} else {
		throw Exception("Repository: Removing property of nonexistent object " + id.toString());
	}
}

bool Repository::hasProperty(const Id &id, QString const &name, bool sensitivity, bool regExpression) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->hasProperty(name, sensitivity, regExpression);
	} else {
		throw Exception("Repository: Checking the existence of a property '" + name + "' of nonexistent object " + id.toString());
	}
}

void Repository::setBackReference(Id const &id, Id const &reference) const
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(reference)) {
			mObjects[id]->setBackReference(reference);
		} else {
			throw Exception("Repository: setting nonexistent back reference " + reference.toString()
							+ " to object " + id.toString());
		}
	} else {
		throw Exception("Repository: setting back reference of nonexistent object " + id.toString());
	}
}

void Repository::removeBackReference(Id const &id, Id const &reference) const
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(reference)) {
			mObjects[id]->removeBackReference(reference);
		} else {
			throw Exception("Repository: removing nonexistent back reference " + reference.toString()
							+ " of object " + id.toString());
		}
	} else {
		throw Exception("Repository: removing back reference of nonexistent object " + id.toString());
	}
}

void Repository::setTemporaryRemovedLinks(Id const &id, QString const &direction, qReal::IdList const &linkIdList)
{
	if (mObjects.contains(id)) {
		mObjects[id]->setTemporaryRemovedLinks(direction, linkIdList);
	} else {
		throw Exception("Repository: Setting temporaryRemovedLinks of nonexistent object " + id.toString());
	}
}

IdList Repository::temporaryRemovedLinksAt(Id const &id, QString const &direction) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->temporaryRemovedLinksAt(direction);
	} else {
		throw Exception("Repository: Requesting temporaryRemovedLinks of nonexistent object " + id.toString());
	}
}

IdList Repository::temporaryRemovedLinks(Id const &id) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->temporaryRemovedLinks();
	} else {
		throw Exception("Repository: Requesting temporaryRemovedLinks of nonexistent object " + id.toString());
	}
}

void Repository::removeTemporaryRemovedLinks(Id const &id)
{
	if (mObjects.contains(id)) {
		return mObjects[id]->removeTemporaryRemovedLinks();
	} else {
		throw Exception("Repository: Removing temporaryRemovedLinks of nonexistent object " + id.toString());
	}
}

void Repository::loadFromDisk()
{
	mSerializer.loadFromDisk(mObjects);
	addChildrenToRootObject();
}

void Repository::importFromDisk(QString const &importedFile)
{
	mSerializer.setWorkingFile(importedFile);
	loadFromDisk();
	mSerializer.setWorkingFile(mWorkingFile);
}

void Repository::addChildrenToRootObject()
{
	foreach (Object *object, mObjects.values()) {
		if (object->parent() == Id::rootId()) {
			if (!mObjects[Id::rootId()]->children().contains(object->id()))
				mObjects[Id::rootId()]->addChild(object->id());
		}
	}
}

IdList Repository::idsOfAllChildrenOf(Id id) const
{
	IdList result;
	result.clear();
	result.append(id);
	IdList list = mObjects[id]->children();
	foreach(Id const &childId, list)
		result.append(idsOfAllChildrenOf(childId));
	return result;
}

QList<Object*> Repository::allChildrenOf(Id id) const
{
	QList<Object*> result;
	result.append(mObjects[id]);
	foreach(Id const &childId, mObjects[id]->children())
		result.append(allChildrenOf(childId));
	return result;
}

QList<Object*> Repository::allChildrenOfWithLogicalId(Id id) const
{
	QList<Object*> result;
	result.append(mObjects[id]);

	// along with each ID we also add its logical ID.

	foreach(Id const &childId, mObjects[id]->children())
		result << allChildrenOf(childId)
				<< allChildrenOf(logicalId(childId));
	return result;
}

bool Repository::exist(const Id &id) const
{
	return (mObjects[id] != NULL);
}

void Repository::saveAll() const
{
	mSerializer.saveToDisk(mObjects.values());
}

void Repository::save(IdList const &list) const
{
	QList<Object*> toSave;
	foreach(Id const &id, list)
		toSave.append(allChildrenOf(id));

	mSerializer.saveToDisk(toSave);
}

void Repository::saveWithLogicalId(qReal::IdList const &list) const
{
	QList<Object*> toSave;
	foreach(Id const &id, list)
		toSave.append(allChildrenOfWithLogicalId(id));

	mSerializer.saveToDisk(toSave);
}

void Repository::saveDiagramsById(QHash<QString, IdList> const &diagramIds)
{
	QString const currentWorkingFile = mWorkingFile;
	foreach (QString const &savePath, diagramIds.keys()) {
		qReal::IdList diagrams = diagramIds[savePath];
		setWorkingFile(savePath);
		qReal::IdList elementsToSave;
		foreach (qReal::Id const &id, diagrams) {
			elementsToSave += idsOfAllChildrenOf(id);
			// id is a graphical ID for this diagram
			// we have to add logical diagram ID
			// to this list manually
			elementsToSave += logicalId(id);
		}
		saveWithLogicalId(elementsToSave);
	}
	setWorkingFile(currentWorkingFile);
}

void Repository::remove(IdList list) const
{
	foreach(Id const &id, list) {
		qDebug() << id.toString();
		mSerializer.removeFromDisk(id);
	}
}

void Repository::remove(const qReal::Id &id)
{
	if (mObjects.contains(id)) {
		delete mObjects[id];
		mObjects.remove(id);
	} else {
		throw Exception("Repository: Trying to remove nonexistent object " + id.toString());
	}
}

void Repository::setWorkingFile(QString const &workingFile)
{
	mSerializer.setWorkingFile(workingFile);
	mWorkingFile = workingFile;
}

void Repository::exportToXml(QString const &targetFile) const
{
	SingleXmlSerializer::exportToXml(targetFile, mObjects);
}

QString Repository::workingFile() const
{
	return mWorkingFile;
}

void Repository::printDebug() const
{
	qDebug() << mObjects.size() << " objects in repository";
	foreach (Object *object, mObjects.values()) {
		qDebug() << object->id().toString();
		qDebug() << "Children:";
		foreach (Id id, object->children())
			qDebug() << id.toString();
		qDebug() << "Parent:";
		qDebug() << object->parent().toString();
		qDebug() << "============";
	}
}

void Repository::exterminate()
{
	printDebug();
	mObjects.clear();
	//serializer.clearWorkingDir();
	mSerializer.saveToDisk(mObjects.values());
	init();
	printDebug();
}

void Repository::open(QString const &saveFile)
{
	mObjects.clear();
	init();
	mSerializer.setWorkingFile(saveFile);
	loadFromDisk();
}

qReal::IdList Repository::elements() const
{
	return mObjects.keys();
}

bool Repository::isLogicalId(qReal::Id const &elem) const
{
	return mObjects[elem]->isLogicalObject();
}

qReal::Id Repository::logicalId(qReal::Id const &elem) const
{
	GraphicalObject const * const graphicalObject = dynamic_cast<GraphicalObject *>(mObjects[elem]);
	if (!graphicalObject) {
		throw Exception("Trying to get logical id from non-graphical object");
	}

	return graphicalObject->logicalId();
}

QMapIterator<QString, QVariant> Repository::propertiesIterator(qReal::Id const &id) const
{
	return mObjects[id]->propertiesIterator();
}

void Repository::createGraphicalPart(qReal::Id const &id, int partIndex)
{
	GraphicalObject * const graphicalObject = dynamic_cast<GraphicalObject *>(mObjects[id]);
	if (!graphicalObject) {
		throw Exception("Trying to create graphical part for non-graphical object");
	}

	graphicalObject->createGraphicalPart(partIndex);
}

QList<int> Repository::graphicalParts(qReal::Id const &id) const
{
	GraphicalObject * const graphicalObject = dynamic_cast<GraphicalObject *>(mObjects[id]);
	if (!graphicalObject) {
		return QList<int>();
	}

	return graphicalObject->graphicalParts();
}

QVariant Repository::graphicalPartProperty(qReal::Id const &id, int partIndex, QString const &propertyName) const
{
	GraphicalObject * const graphicalObject = dynamic_cast<GraphicalObject *>(mObjects[id]);
	if (!graphicalObject) {
		throw Exception("Trying to obtain graphical part property for non-graphical item");
	}

	return graphicalObject->graphicalPartProperty(partIndex, propertyName);
}

void Repository::setGraphicalPartProperty(
		qReal::Id const &id
		, int partIndex
		, QString const &propertyName
		, QVariant const &value
		)
{
	GraphicalObject * const graphicalObject = dynamic_cast<GraphicalObject *>(mObjects[id]);
	if (!graphicalObject) {
		throw Exception("Trying to obtain graphical part property for non-graphical item");
	}

	graphicalObject->setGraphicalPartProperty(partIndex, propertyName, value);
}
