#pragma once

#include <QtCore/QVariant>
#include <QtCore/QModelIndex>

#include "abstractCommand.h"
#include "../../mainwindow/propertyEditorProxyModel.h"

namespace qReal
{
namespace commands
{

class ChangePropertyCommand : public AbstractCommand
{
public:
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

	PropertyEditorModel *mModel;
	QModelIndex mIndex;
	QVariant mOldValue;
	QVariant mNewValue;
	int mRole;
};

}
}
