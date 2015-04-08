/* Copyright 2007-2015 QReal Research Group
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

#include "suggestToCreateDiagramDialog.h"

#include <QtWidgets/QVBoxLayout>

#include "dialogs/projectManagement/suggestToCreateDiagramWidget.h"

using namespace qReal;

SuggestToCreateDiagramDialog::SuggestToCreateDiagramDialog(const EditorManagerInterface &editorManager
		, QWidget *parent, bool isClosable)
	: ManagedClosableDialog(parent, isClosable)
{
	SuggestToCreateDiagramWidget *suggestWidget = new SuggestToCreateDiagramWidget(editorManager, this);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(suggestWidget);
	setLayout(mainLayout);
	setWindowTitle(tr("Create diagram"));

	connect(suggestWidget, &ListWidget::userDataSelected, this, &SuggestToCreateDiagramDialog::diagramSelected);
	connect(suggestWidget, &ListWidget::userDataSelected, this, &ManagedClosableDialog::forceClose);
}
