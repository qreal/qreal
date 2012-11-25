#pragma once

#include <QDialog>
#include "../pluginManager/interpreterEditorManager.h"

namespace Ui {
	class PropertiesDialog;
}

namespace qReal {

class PropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	PropertiesDialog(QWidget *parent = 0);
	~PropertiesDialog();
	void init(EditorManagerInterface* interperterEditorManager, Id const &id);
private slots:
	void closeDialog();
	void deleteProperty();
	void addProperty();
	void changeProperty();
private:
	void change(QString const &text);
	Ui::PropertiesDialog *mUi;
	EditorManagerInterface *mInterperterEditorManager;
	Id mId;
};
}
