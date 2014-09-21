#pragma once

#include <QtCore/QVariant>
#include <QtCore/QModelIndex>

#include "controller/commands/abstractCommand.h"
#include "mainWindow/propertyEditorProxyModel.h"
#include "models/logicalModelAssistApi.h"

namespace qReal {
namespace commands {

/// Sets given property to a given value
class ChangePropertyCommand : public AbstractCommand
{
public:
	/// Use this overload to modify properties via models api
	ChangePropertyCommand(models::LogicalModelAssistApi * const model
			, QString const &property, Id const &id, QVariant const &newValue);

	/// Constructs new change property command instance modifying
	/// properties via property editor proxy model
	ChangePropertyCommand(
		PropertyEditorModel * const model /* Doesn`t take ownership */
		, QModelIndex const &index
		, QVariant const &oldValue
		, QVariant const &newValue
		, int role = Qt::EditRole);

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	bool setProperty(QVariant const &value);

	models::LogicalModelAssistApi *mLogicalModel;
	Id const mId;
	QString mPropertyName;

	PropertyEditorModel *mPropertyEditorModel;
	QModelIndex mPropertyEditorIndex;
	int mPropertyEditorRole;

	QVariant mOldValue;
	QVariant mNewValue;
};

}
}
