#include "modelsAssistApi.h"
//#include <QtCore/QUuid>

using namespace qReal;
using namespace models;
using namespace details;
using namespace modelsImplementation;

ModelsAssistApi::ModelsAssistApi(AbstractModel &model, EditorManager const &editorManager)
	: mModel(model), mEditorManager(editorManager)
{
}

EditorManager const &ModelsAssistApi::editorManager() const
{
	return mEditorManager;
}

void ModelsAssistApi::setProperty(qReal::Id const &elem, QVariant const &newValue, int const role)
{
	mModel.setData(mModel.indexById(elem), newValue, role);
}

QVariant ModelsAssistApi::property(qReal::Id const &elem, int const role) const
{
	return mModel.data(mModel.indexById(elem), role);
}

int ModelsAssistApi::roleIndexByName(qReal::Id const &elem, QString const &roleName) const
{
	QStringList properties = editorManager().getPropertyNames(elem.type());
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

bool ModelsAssistApi::dropMimeData(QMimeData const *data, Qt::DropAction action, qReal::Id const &parent)
{
	QModelIndex parentIndex = indexById(parent);
	return mModel.dropMimeData(data, action, mModel.rowCount(parentIndex), mModel.columnCount(parentIndex), parentIndex);
}
