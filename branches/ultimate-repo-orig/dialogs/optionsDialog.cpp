#include "optionsDialog.h"
#include "ui_optionsDialog.h"

#include "realrepoinfo.h"

using namespace QReal;

OptionsDialog::OptionsDialog(QVector<bool> const &selected, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.selectAllButton, SIGNAL(clicked()), SLOT(selectAll()));
	connect(ui.unselectAllButton, SIGNAL(clicked()), SLOT(unselectAll()));

	RealRepoInfo info;

	QStringList categories = info.getObjectCategories();
	setUpdatesEnabled(false);
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
	setUpdatesEnabled(true);
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

void OptionsDialog::selectAll()
{
	setUpdatesEnabled(false);
	for (int i = 0; i < ui.editorsListWidget->count(); ++i)
	{
		ui.editorsListWidget->item(i)->setCheckState(Qt::Checked);
	}
	setUpdatesEnabled(true);
}

void OptionsDialog::unselectAll()
{
	setUpdatesEnabled(false);
	for (int i = 0; i < ui.editorsListWidget->count(); ++i)
	{
		ui.editorsListWidget->item(i)->setCheckState(Qt::Unchecked);
	}
	setUpdatesEnabled(true);
}
