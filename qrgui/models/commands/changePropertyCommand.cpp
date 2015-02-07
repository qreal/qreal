#include "changePropertyCommand.h"

using namespace qReal::commands;

ChangePropertyCommand::ChangePropertyCommand(models::LogicalModelAssistApi * const model
		, const QString &property, const Id &id, const QVariant &newValue)
	: mLogicalModel(model)
	, mId(id)
	, mPropertyName(property)
	, mPropertyEditorModel(nullptr)
	, mOldValue(mLogicalModel->propertyByRoleName(mId, mPropertyName))
	, mNewValue(newValue)
{
}

ChangePropertyCommand::ChangePropertyCommand(
		PropertyEditorModel * const model
		, const QModelIndex &index
		, const QVariant &oldValue
		, const QVariant &newValue
		, int role)
	: mLogicalModel(nullptr)
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

bool ChangePropertyCommand::setProperty(const QVariant &value)
{
	if (mPropertyEditorModel) {
		return mPropertyEditorModel->setData(mPropertyEditorIndex, value, mPropertyEditorRole);
	}
	mLogicalModel->setPropertyByRoleName(mId, value, mPropertyName);
	return true;
}
