#include "changePropertyCommand.h"

using namespace qReal::commands;

ChangePropertyCommand::ChangePropertyCommand(
		PropertyEditorModel * const model
		, QModelIndex const &index
		, QVariant const &oldValue
		, QVariant const &newValue
		, int role)
	: mModel(model)
	, mIndex(index)
	, mOldValue(oldValue)
	, mNewValue(newValue)
	, mRole(role)
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
	return mModel->setData(mIndex, value, mRole);
}
