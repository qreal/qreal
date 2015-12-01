#pragma once

#include <QtWidgets/QDialog>

#include <qrkernel/ids.h>

#include <qrgui/models/exploser.h>
#include <qrrepo/repoApi.h>
#include "qrgui/dialogs/dialogsDeclSpec.h"

namespace Ui {
class LabelPropertiesDialog;
}

namespace qReal {
namespace gui {

class QRGUI_DIALOGS_EXPORT LabelPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	LabelPropertiesDialog(const Id &id, qrRepo::LogicalRepoApi &logicalRepoApi, models::Exploser &exploser, QWidget *parent = 0);
	~LabelPropertiesDialog();

private slots:
	void addLabelButtonClicked();
	void saveButtonClicked();
	void deleteButtonClicked();
	void typeChanged(const QString &newType);

private:
	///Calls from constructor to init labels
	void init();
	/// Tells if we can save labels, return false for example if (name, type) not filled
	bool canSave();
	void addLabel(const QString &name, const QString &type, const QString &value);

	Ui::LabelPropertiesDialog *mUi;
	qrRepo::LogicalRepoApi &mLogicalRepoApi;
	models::Exploser &mExploser;
	const Id mId;
};

}
}


