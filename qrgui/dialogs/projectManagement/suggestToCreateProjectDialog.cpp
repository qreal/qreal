#include "suggestToCreateProjectDialog.h"

#include <QtWidgets/QVBoxLayout>

#include "dialogs/projectManagement/suggestToCreateDiagramWidget.h"

using namespace qReal;

SuggestToCreateProjectDialog::SuggestToCreateProjectDialog(const EditorManagerInterface &editorManager
		, QWidget *parent)
	: QDialog(parent)
{
	SuggestToCreateDiagramWidget *suggestWidget = new SuggestToCreateDiagramWidget(editorManager, this);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(suggestWidget);
	setLayout(mainLayout);
	setWindowTitle(tr("Create project"));

	connect(suggestWidget, &ListWidget::userDataSelected, this, &SuggestToCreateProjectDialog::diagramSelected);
	connect(suggestWidget, &ListWidget::userDataSelected, this, &QDialog::accept);
}
