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
