/* Copyright 2007-2017 QReal Research Group
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

#include <QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidgetItem>

namespace Ui {
	class umlCheckerForm;
}

namespace qReal {

class UmlCheckerTmplWindow: public QDialog
{
	Q_OBJECT

public:
	explicit UmlCheckerTmplWindow(QWidget *parent = 0);

	~UmlCheckerTmplWindow();

	void openTemplatesForBlocks(const QString &fileName);
	void setBlockName(const QString &blockName);
	QPair<QString, QStringList> getElementForBlock();

signals:
	void checkClicked();
	void blocksButtonClicked();
	void edgesButtonClicked();


private slots:
	void blocksButtonActivate();
	void edgesButtonActivate();
	void openPicture(QListWidgetItem *item);
	void addToCollection();

private:
	QString mBlockName = "";
	QStringList mFiles;
	Ui::umlCheckerForm *mUi;
};
}
