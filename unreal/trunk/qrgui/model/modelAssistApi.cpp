#include "modelAssistApi.h"

#include "model.h"

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
