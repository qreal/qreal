#include "modelsAssistApi.h"

#include "models/details/modelsImplementation/abstractModel.h"

using namespace qReal;
using namespace models;
using namespace models::details;
using namespace modelsImplementation;

ModelsAssistApi::ModelsAssistApi(AbstractModel &model, EditorManagerInterface const &editorManagerInterface)
	: mModel(model), mEditorManagerInterface(editorManagerInterface)
{
}

EditorManagerInterface const &ModelsAssistApi::editorManagerInterface() const
{
	return mEditorManagerInterface;
}

Id ModelsAssistApi::createElement(Id const &parent, Id const &id, Id const &logicalId
		, bool isFromLogicalModel, QString const &name, QPointF const &position)
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

void ModelsAssistApi::setProperty(Id const &elem, QVariant const &newValue, int const role)
{
	mModel.setData(indexById(elem), newValue, role);
	emit propertyChangedInModelApi(elem);
}

void ModelsAssistApi::stackBefore(Id const &element, Id const &sibling)
{
	mModel.stackBefore(indexById(element), indexById(sibling));
}

QVariant ModelsAssistApi::property(Id const &elem, int const role) const
{
	return mModel.data(indexById(elem), role);
}

int ModelsAssistApi::roleIndexByName(Id const &elem, QString const &roleName) const
{
	QStringList const properties = editorManagerInterface().propertyNames(elem.type());
	return properties.indexOf(roleName) + roles::customPropertiesBeginRole;
}

QModelIndex ModelsAssistApi::indexById(Id const &id) const
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

void ModelsAssistApi::setTo(Id const &elem, Id const &newValue)
{
	setProperty(elem, newValue.toVariant(), roles::toRole);
}

Id ModelsAssistApi::to(Id const &elem) const
{
	return property(elem, roles::toRole).value<Id>();
}

void ModelsAssistApi::setFrom(Id const &elem, Id const &newValue)
{
	setProperty(elem, newValue.toVariant(), roles::fromRole);
}

Id ModelsAssistApi::from(Id const &elem) const
{
	return property(elem, roles::fromRole).value<Id>();
}
