#include "modelAssistApi.h"

#include <QtCore/QUuid>

#include "model.h"
#include "../kernel/exception/exception.h"

using namespace qReal;
using namespace model;

ModelAssistApi::ModelAssistApi(Model &model, EditorManager const &editorManager)
	: mModel(model), mEditorManager(editorManager)
{
}

EditorManager const &ModelAssistApi::editorManager() const
{
	return mEditorManager;
}

void ModelAssistApi::connect(qReal::Id const &source, qReal::Id const &destination)
{
	mModel.mutableApi().connect(source, destination);
}

void ModelAssistApi::disconnect(qReal::Id const &source, qReal::Id const &destination)
{
	mModel.mutableApi().disconnect(source, destination);
}

void ModelAssistApi::addUsage(qReal::Id const &source, qReal::Id const &destination)
{
	mModel.mutableApi().addUsage(source, destination);
}

void ModelAssistApi::deleteUsage(qReal::Id const &source, qReal::Id const &destination)
{
	mModel.mutableApi().deleteUsage(source, destination);
}

Id ModelAssistApi::createElement(qReal::Id const &parent, qReal::Id const &type)
{
	Q_ASSERT(type.idSize() == 3);
	Q_ASSERT(parent.idSize() == 4);

	Id const newElementId(type, QUuid::createUuid().toString());
	QString const elementFriendlyName = editorManager().friendlyName(type);
	if (!mModel.addElementToModel(parent, newElementId, "(anonymous " + elementFriendlyName + ")", QPointF(0, 0)))
		throw Exception("Incorrect automatic element creation");
	return newElementId;
}

Id ModelAssistApi::createConnectedElement(Id const &source, Id const &elementType)
{
	Id element = createElement(ROOT_ID, elementType);
	QString sourceName = mModel.data(mModel.indexById(source), Qt::DisplayRole).toString();
	QString typeName = editorManager().friendlyName(elementType);
	mModel.setData(mModel.indexById(element), sourceName + " " + typeName, Qt::DisplayRole);
	return element;
}

void ModelAssistApi::createConnected(Id const &sourceElement, Id const &elementType)
{
	Id element = createConnectedElement(sourceElement, elementType);
	connect(sourceElement, element);
}

void ModelAssistApi::createUsed(Id const &sourceElement, Id const &elementType)
{
	Id element = createConnectedElement(sourceElement, elementType);
	addUsage(sourceElement, element);
}

IdList ModelAssistApi::diagramsFromList(IdList const &list)
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

IdList ModelAssistApi::diagramsAbleToBeConnectedTo(Id const &element) const
{
	return diagramsFromList(editorManager().getConnectedTypes(element.type()));
}

IdList ModelAssistApi::diagramsAbleToBeUsedIn(Id const &element) const
{
	return diagramsFromList(editorManager().getUsedTypes(element.type()));
}
