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
	void cancel();

private:
	Ui::PropertiesDialog *mUi;
};
}
