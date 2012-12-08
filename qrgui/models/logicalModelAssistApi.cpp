#include "logicalModelAssistApi.h"
#include <QtCore/QUuid>

using namespace qReal;
using namespace models;
using namespace models::details;

LogicalModelAssistApi::LogicalModelAssistApi(LogicalModel &logicalModel, EditorManager const &editorManager)
	: mModelsAssistApi(logicalModel, editorManager), mLogicalModel(logicalModel)
{
}

EditorManager const &LogicalModelAssistApi::editorManager() const
{
	return mModelsAssistApi.editorManager();
}

qrRepo::LogicalRepoApi const &LogicalModelAssistApi::logicalRepoApi() const
{
	return mLogicalModel.api();
}

qrRepo::LogicalRepoApi &LogicalModelAssistApi::mutableLogicalRepoApi()
{
	return mLogicalModel.mutableApi();
}

Id LogicalModelAssistApi::createElement(Id const &parent, Id const &type)
{
	Q_ASSERT(type.idSize() == 3);
	Q_ASSERT(parent.idSize() == 4);

	Id const newElementId(type, QUuid::createUuid().toString());
	QString const elementFriendlyName = mModelsAssistApi.editorManager().friendlyName(type);
	mLogicalModel.addElementToModel(parent, newElementId, Id(), "(" + elementFriendlyName + ")", QPointF(0, 0));
	return newElementId;
}

Id LogicalModelAssistApi::createElement(Id const &parent, Id const &id, bool isFromLogicalModel, QString const &name, QPointF const &position)
{
	return mModelsAssistApi.createElement(parent, id, isFromLogicalModel, name, position);
}

void LogicalModelAssistApi::stackBefore(const Id &element, const Id &sibling)
{
	mModelsAssistApi.stackBefore(element, sibling);
}

IdList LogicalModelAssistApi::children(Id const &element) const
{
	return mLogicalModel.api().children(element);
}

void LogicalModelAssistApi::changeParent(Id const &element, Id const &parent, QPointF const &position)
{
	Q_UNUSED(position);
	mLogicalModel.changeParent(mLogicalModel.indexById(element), mLogicalModel.indexById(parent), QPointF());
}

void LogicalModelAssistApi::connect(Id const &source, Id const &destination)
{
	mLogicalModel.mutableApi().connect(source, destination);
}

void LogicalModelAssistApi::disconnect(Id const &source, Id const &destination)
{
	mLogicalModel.mutableApi().disconnect(source, destination);
}

void LogicalModelAssistApi::addUsage(Id const &source, Id const &destination)
{
	mLogicalModel.mutableApi().addUsage(source, destination);
}

void LogicalModelAssistApi::deleteUsage(Id const &source, Id const &destination)
{
	mLogicalModel.mutableApi().deleteUsage(source, destination);
}

Id LogicalModelAssistApi::createConnectedElement(Id const &source, Id const &elementType)
{
	Id element = createElement(Id::rootId(), elementType);
	QString sourceName = mLogicalModel.data(mLogicalModel.indexById(source), Qt::DisplayRole).toString();
	QString typeName = editorManager().friendlyName(elementType);
	mLogicalModel.setData(mLogicalModel.indexById(element), sourceName + " " + typeName, Qt::DisplayRole);
	return element;
}

void LogicalModelAssistApi::createConnected(Id const &sourceElement, Id const &elementType)
{
	Id element = createConnectedElement(sourceElement, elementType);
	connect(sourceElement, element);
}

void LogicalModelAssistApi::createUsed(Id const &sourceElement, Id const &elementType)
{
	Id element = createConnectedElement(sourceElement, elementType);
	addUsage(sourceElement, element);
}

IdList LogicalModelAssistApi::diagramsFromList(IdList const &list) const
{
	// TODO: diagrams are kinda special, so we need the editor to be able to
	// tell us whether this particular element is a diagram or not
	IdList result;
	foreach (Id type, list) {
		if (type.element().split("_").back().contains("Diagram", Qt::CaseInsensitive)) {
			if (!result.contains(type))
				result.append(type);
		}
	}
	return result;
}

IdList LogicalModelAssistApi::diagramsAbleToBeConnectedTo(Id const &element) const
{
	return diagramsFromList(editorManager().getConnectedTypes(element.type()));
}

IdList LogicalModelAssistApi::diagramsAbleToBeUsedIn(Id const &element) const
{
	return diagramsFromList(editorManager().getUsedTypes(element.type()));
}

void LogicalModelAssistApi::setPropertyByRoleName(Id const &elem, QVariant const &newValue, QString const &roleName)
{
	int roleIndex = mModelsAssistApi.roleIndexByName(elem, roleName);
	if (roleIndex < roles::customPropertiesBeginRole)
		return;
	mModelsAssistApi.setProperty(elem, newValue, roleIndex);
}

QVariant LogicalModelAssistApi::propertyByRoleName(Id const &elem, QString const &roleName) const
{
	int roleIndex = mModelsAssistApi.roleIndexByName(elem, roleName);
	if (roleIndex < roles::customPropertiesBeginRole)
		return "";
	return mModelsAssistApi.property(elem, roleIndex);
}

bool LogicalModelAssistApi::isLogicalId(Id const &id) const
{
	return mModelsAssistApi.indexById(id) != QModelIndex();
}

void LogicalModelAssistApi::setTo(Id const &elem, Id const &newValue)
{
	mModelsAssistApi.setTo(elem, newValue);
}

Id LogicalModelAssistApi::to(Id const &elem) const
{
	return mModelsAssistApi.to(elem);
}

void LogicalModelAssistApi::setFrom(Id const &elem, Id const &newValue)
{
	mModelsAssistApi.setFrom(elem, newValue);
}

Id LogicalModelAssistApi::from(Id const &elem) const
{
	return mModelsAssistApi.from(elem);
}

QModelIndex LogicalModelAssistApi::indexById(Id const &id) const
{
	return mModelsAssistApi.indexById(id);
}

Id LogicalModelAssistApi::idByIndex(QModelIndex const &index) const
{
	return mModelsAssistApi.idByIndex(index);
}

QPersistentModelIndex LogicalModelAssistApi::rootIndex() const
{
	return mModelsAssistApi.rootIndex();
}

Id LogicalModelAssistApi::rootId() const
{
	return mModelsAssistApi.rootId();
}

bool LogicalModelAssistApi::hasRootDiagrams() const
{
	return mModelsAssistApi.hasRootDiagrams();
}

int LogicalModelAssistApi::childrenOfRootDiagram() const
{
	return mModelsAssistApi.childrenOfRootDiagram();
}

int LogicalModelAssistApi::childrenOfDiagram(const Id &parent) const
{
	return mModelsAssistApi.childrenOfDiagram(parent);
}

void LogicalModelAssistApi::removeReferencesTo(Id const &id) const
{
	IdList backReferences = mLogicalModel.api().property(id, "backReferences").value<IdList>();

	foreach (Id const &reference, backReferences) {
		mLogicalModel.api().removeBackReference(id, reference);
		removeReference(reference, id);
	}
}

void LogicalModelAssistApi::removeReferencesFrom(Id const &id) const
{
	QStringList referenceProperties = editorManager().getReferenceProperties(id.type());

	foreach (QString const &property, referenceProperties) {
		QString propertyString = mLogicalModel.api().property(id, property).toString();
		if (!propertyString.isEmpty()) {
			Id propertyValue = Id::loadFromString(propertyString);
			mLogicalModel.api().removeBackReference(propertyValue, id);
		}
	}
}

void LogicalModelAssistApi::removeReference(Id const &id, Id const &reference) const
{
	QStringList referenceProperties = editorManager().getReferenceProperties(id.type());

	foreach (QString const &propertyName, referenceProperties) {
		QString stringData = mLogicalModel.api().property(id, propertyName).toString();
		if (stringData == reference.toString()) {
			mLogicalModel.api().setProperty(id, propertyName, "");
		}
	}
}
