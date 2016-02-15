/* Copyright 2007-2016 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	LabelPropertiesDialog(const Id &id, qrRepo::LogicalRepoApi &logicalRepoApi
			, models::Exploser &exploser, QWidget *parent = 0);
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


