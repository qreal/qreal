#pragma once

#include <QtCore/QVariant>
#include <QtCore/QModelIndex>

#include "controller/commands/abstractCommand.h"
#include "models/propertyEditorModel.h"
#include "models/logicalModelAssistApi.h"

namespace qReal {
namespace commands {

/// Sets given property to a given value
class QRGUI_MODELS_EXPORT ChangePropertyCommand : public AbstractCommand
{
public:
	/// Use this overload to modify properties via models api
	ChangePropertyCommand(models::LogicalModelAssistApi * const model
			, const QString &property, const Id &id, const QVariant &newValue);

	/// Constructs new change property command instance modifying
	/// properties via property editor proxy model
	ChangePropertyCommand(
		PropertyEditorModel * const model /* Doesn`t take ownership */
		, const QModelIndex &index
		, const QVariant &oldValue
		, const QVariant &newValue
		, int role = Qt::EditRole);

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	bool setProperty(const QVariant &value);

	models::LogicalModelAssistApi *mLogicalModel;
	const Id mId;
	QString mPropertyName;

	PropertyEditorModel *mPropertyEditorModel;
	QModelIndex mPropertyEditorIndex;
	int mPropertyEditorRole;

	QVariant mOldValue;
	QVariant mNewValue;
};

}
}
