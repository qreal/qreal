#pragma once

#include <QDialog>

#include "ui_optionsDialog.h"

namespace QReal
{
	class OptionsDialog: public QDialog
	{
		Q_OBJECT
	public:
		explicit OptionsDialog(
			QVector<bool> const &selected,
			QWidget *parent = 0
		);
		~OptionsDialog() {}
		QVector<bool> getSelectedEditors() const;
	private:
		Ui::OptionsDialogUi ui;
	private slots:
		void selectAll();
		void unselectAll();
	};
}
