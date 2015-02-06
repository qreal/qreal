#include "modelsAssistApi.h"

#include "models/details/modelsImplementation/abstractModel.h"

using namespace qReal;
using namespace models;
using namespace models::details;
using namespace modelsImplementation;

ModelsAssistApi::ModelsAssistApi(AbstractModel &model, const EditorManagerInterface &editorManagerInterface)
	: mModel(model), mEditorManagerInterface(editorManagerInterface)
{
}

const EditorManagerInterface &ModelsAssistApi::editorManagerInterface() const
{
	return mEditorManagerInterface;
}

Id ModelsAssistApi::createElement(const Id &parent, const Id &id, const Id &logicalId
		, bool isFromLogicalModel, const QString &name, const QPointF &position)
{
	Q_ASSERT(parent.idSize() == 4);
	Id newId = id;
	Id realLogicalId = logicalId;
	if (isFromLogicalModel) {
		realLogicalId = id;
		newId = Id(id.editor(), id.diagram(), id.element(), QUuid::createUuid().toString());
	}

	mModel.addElementToModel(parent, newId, realLogicalId, name, position);
	return newId;
}

void ModelsAssistApi::setProperty(const Id &elem, const QVariant &newValue, const int role)
{
	mModel.setData(indexById(elem), newValue, role);
}

void ModelsAssistApi::stackBefore(const Id &element, const Id &sibling)
{
	mModel.stackBefore(indexById(element), indexById(sibling));
}

QVariant ModelsAssistApi::property(const Id &elem, const int role) const
{
	return mModel.data(indexById(elem), role);
}

int ModelsAssistApi::roleIndexByName(const Id &elem, const QString &roleName) const
{
	const QStringList properties = editorManagerInterface().propertyNames(elem.type());
	return properties.indexOf(roleName) + roles::customPropertiesBeginRole;
}

QModelIndex ModelsAssistApi::indexById(const Id &id) const
{
	return mModel.indexById(id);
}

Id ModelsAssistApi::idByIndex(QModelIndex const &index) const
{
	return mModel.idByIndex(index);
}

bool ModelsAssistApi::hasRootDiagrams() const
{
	 return mModel.rowCount(QModelIndex()) != 0;
}

int ModelsAssistApi::childrenOfRootDiagram() const
{
	return mModel.rowCount(rootIndex());
}

int ModelsAssistApi::childrenOfDiagram(const Id &parent) const
{
	return mModel.rowCount(indexById(parent));
}

QPersistentModelIndex ModelsAssistApi::rootIndex() const
{
	return mModel.rootIndex();
}

Id ModelsAssistApi::rootId() const
{
	return idByIndex(mModel.rootIndex());
}

void ModelsAssistApi::setTo(const Id &elem, const Id &newValue)
{
	setProperty(elem, newValue.toVariant(), roles::toRole);
}

Id ModelsAssistApi::to(const Id &elem) const
{
	return property(elem, roles::toRole).value<Id>();
}

void ModelsAssistApi::setFrom(const Id &elem, const Id &newValue)
{
	setProperty(elem, newValue.toVariant(), roles::fromRole);
}

Id ModelsAssistApi::from(const Id &elem) const
{
	return property(elem, roles::fromRole).value<Id>();
}
