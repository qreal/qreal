#pragma once

#include <QDialog>
#include <QString>
#include <QLineEdit>

namespace Ui {
	class MethodEditorDialog;
}

class MethodEditorDialog : public QDialog {
	Q_OBJECT
	public:
		MethodEditorDialog(QWidget *parent = 0);
		~MethodEditorDialog();
		QString generateMethodString();

	protected:
		void changeEvent(QEvent *e);

	private slots:
		void createParameter();

	private:
		Ui::MethodEditorDialog *ui;

		struct ParameterEditor {
			ParameterEditor(QLineEdit *nameEdit, QLineEdit *typeEdit):
					nameEdit(nameEdit), typeEdit(typeEdit)
			{}

			QLineEdit *nameEdit;
			QLineEdit *typeEdit;
		};
		QList<ParameterEditor> parameterList;
};
