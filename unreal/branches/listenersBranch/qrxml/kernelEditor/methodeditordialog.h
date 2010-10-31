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

		QString methodName();
		QString methodReturnType();
		QMap<QString, QString> methodParameters(); //<param, paramType>
		QString methodParameteresInString();// param1%%param1Type##param2%%param2Type##...

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
