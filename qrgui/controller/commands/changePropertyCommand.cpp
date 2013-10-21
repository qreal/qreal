#include "changePropertyCommand.h"

using namespace qReal::commands;

ChangePropertyCommand::ChangePropertyCommand(models::LogicalModelAssistApi * const model
		, QString const &property, Id const &id, QVariant const &newValue)
	: mLogicalModel(model)
	, mId(id)
	, mPropertyName(property)
	, mPropertyEditorModel(NULL)
	, mOldValue(mLogicalModel->propertyByRoleName(mId, mPropertyName))
	, mNewValue(newValue)
{
}

ChangePropertyCommand::ChangePropertyCommand(
		PropertyEditorModel * const model
		, QModelIndex const &index
		, QVariant const &oldValue
		, QVariant const &newValue
		, int role)
	: mLogicalModel(NULL)
	, mPropertyEditorModel(model)
	, mPropertyEditorIndex(index)
	, mPropertyEditorRole(role)
	, mOldValue(oldValue)
	, mNewValue(newValue)
{
}

bool ChangePropertyCommand::execute()
{
	return setProperty(mNewValue);
}

bool ChangePropertyCommand::restoreState()
{
	return setProperty(mOldValue);
}

bool ChangePropertyCommand::setProperty(QVariant const &value)
{
	if (mPropertyEditorModel) {
		return mPropertyEditorModel->setData(mPropertyEditorIndex, value, mPropertyEditorRole);
	}
	mLogicalModel->setPropertyByRoleName(mId, value, mPropertyName);
	return true;
}
