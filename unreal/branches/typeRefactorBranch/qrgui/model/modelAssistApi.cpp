#include "model.h"
#include "modelAssistApi.h"
#include "../kernel/exception/exception.h"

#include <QtCore/QUuid>
#include <QtCore/QDebug>

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
        //Q_ASSERT(type.typeSize() == 3);
        //Q_ASSERT(parent.typeSize() == 4);

        Id const newElementId(QUuid::createUuid().toString());
        QString const elementFriendlyName = editorManager().friendlyName(mModel.api().type(type));
	if (!mModel.addElementToModel(parent, newElementId, "(" + elementFriendlyName + ")", QPointF(0, 0)))
		throw Exception("Incorrect automatic element creation");
	return newElementId;
}

Id ModelAssistApi::createConnectedElement(Id const &source, Id const &elementId)
{
        Id element = createElement(ROOT_ID, elementId);
        QString sourceName = mModel.data(mModel.indexById(source), Qt::DisplayRole).toString();
        QString typeName = editorManager().friendlyName(mModel.api().type(elementId));
        mModel.setData(mModel.indexById(element), sourceName + " " + typeName, Qt::DisplayRole);
	return element;
}

void ModelAssistApi::createConnected(Id const &sourceElement, Id const &elementId)
{
        Id element = createConnectedElement(sourceElement, elementId);
	connect(sourceElement, element);
}

void ModelAssistApi::createUsed(Id const &sourceElement, Id const &elementId)
{
        Id element = createConnectedElement(sourceElement, elementId);
	addUsage(sourceElement, element);
}

TypeList ModelAssistApi::diagramsFromList(TypeList const &list)
{
	// TODO: diagrams are kinda special, so we need the editor to be able to
	// tell us whether this particular element is a diagram or not
        TypeList result;
        foreach (TypeList type, list) {
                if (type.element().split("_").back().contains("Diagram", Qt::CaseInsensitive)) {
                        if (!result.contains(type))
                                result.append(type);
		}
	}
	return result;
}

TypeList ModelAssistApi::diagramsAbleToBeConnectedTo(Id const &element) const
{
        return diagramsFromList(editorManager().getConnectedTypes(mModel.api(element)));
}

TypeList ModelAssistApi::diagramsAbleToBeUsedIn(Id const &element) const
{
        return diagramsFromList(editorManager().getUsedTypes(mModel.api(element)));
}

void ModelAssistApi::setProperty(qReal::Id const &elem, int const role, QVariant const &newValue)
{

        mModel.setData(mModel.indexById(elem), newValue, role);
}

QVariant ModelAssistApi::getProperty(qReal::Id const &elem, int const role) const
{
        return mModel.data(mModel.indexById(elem), role);
}

IdList ModelAssistApi::children(qReal::Id const &element) const
{
	return mModel.api().children(element);
}
