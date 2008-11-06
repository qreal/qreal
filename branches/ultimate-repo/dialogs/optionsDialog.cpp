#include "optionsDialog.h"
#include "ui_optionsDialog.h"

#include "realrepoinfo.h"

using namespace QReal;

OptionsDialog::OptionsDialog(QVector<bool> const &selected, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	RealRepoInfo info;

	QStringList categories = info.getObjectCategories();
	for (int i = 0; i < categories.size(); ++i)
	{
		QString str = categories[i];
		QListWidgetItem *item = new QListWidgetItem(str, ui.editorsListWidget);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		if (selected[i])
			item->setCheckState(Qt::Checked);
		else
			item->setCheckState(Qt::Unchecked);
	}
}

QVector<bool> OptionsDialog::getSelectedEditors() const
{
	QVector<bool> result(ui.editorsListWidget->count());
	for(int i = 0; i < ui.editorsListWidget->count(); ++i)
	{
		QListWidgetItem *item = ui.editorsListWidget->item(i);
		result[i] = item->checkState() == Qt::Checked;
	}
	return result;
}
