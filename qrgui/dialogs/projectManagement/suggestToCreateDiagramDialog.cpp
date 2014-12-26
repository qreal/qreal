#include "suggestToCreateDiagramDialog.h"

#include <QtWidgets/QVBoxLayout>

#include "dialogs/projectManagement/suggestToCreateDiagramWidget.h"

using namespace qReal;

SuggestToCreateDiagramDialog::SuggestToCreateDiagramDialog(EditorManagerInterface const &editorManager
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
