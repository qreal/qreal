/* Copyright 2015-2016 Kogutich Denis
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
#include <QtWidgets/QScrollArea>

#include <qrkernel/ids.h>
#include <qrgui/models/exploser.h>
#include <qrrepo/repoApi.h>
#include "qrgui/dialogs/dialogsDeclSpec.h"
#include "shapePropertyWidget.h"

namespace Ui {
class DynamicPropertiesDialog;
}

namespace qReal {
namespace gui {

class QRGUI_DIALOGS_EXPORT DynamicPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	DynamicPropertiesDialog(const Id &id, qrRepo::LogicalRepoApi &logicalRepoApi
			, models::Exploser &exploser, QWidget *parent = 0);
	~DynamicPropertiesDialog();

private slots:
	void addLabelButtonClicked();
	void saveButtonClicked();
	void deleteButtonClicked();
	void typeChanged(const QString &newType);

private:
	///Calls from constructor to init labels
	void init();
	bool canSave();
	void addLabel(const QString &name, const QString &type, const QString &value);

	Ui::DynamicPropertiesDialog *mUi;
	ShapePropertyWidget *mShapeWidget;
	QScrollArea *mScrollArea;
	qrRepo::LogicalRepoApi &mLogicalRepoApi;
	models::Exploser &mExploser;
	const Id mId;

};

}
}


