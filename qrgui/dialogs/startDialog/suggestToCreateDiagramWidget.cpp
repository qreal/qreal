#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtCore/QDebug>
#include "suggestToCreateDiagramWidget.h"
#include "../../../qrkernel/ids.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;

SuggestToCreateDiagramWidget::SuggestToCreateDiagramWidget(MainWindow *mainWindow, QDialog *parent)
	: QWidget(parent)
	, mMainWindow(mainWindow)
	, mDiagramsListWidget(new QListWidget(this))
{
	foreach(Id editor, mMainWindow->manager()->editors()) {
		foreach(Id diagram, mMainWindow->manager()->diagrams(
					Id::loadFromString("qrm:/" + editor.editor()))) {
			QString const diagramName = mMainWindow->mEditorManager.editorInterface(
					editor.editor())->diagramName(diagram.diagram());
			QString const diagramNodeName = mMainWindow->mEditorManager.editorInterface(
					editor.editor())->diagramNodeName(diagram.diagram());
			if (diagramNodeName.isEmpty()) {
				continue;
			}
			mMainWindow->mDiagramsList.append(
					"qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName);
			mDiagramsListWidget->addItem(diagramName);
		}
	}
	mDiagramsListWidget->setCurrentRow(0);

	QPushButton *okButton = new QPushButton(tr("&OK"), this);
	QObject::connect(mDiagramsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), parent, SIGNAL(close()));
	QObject::connect(okButton, SIGNAL(clicked()), mMainWindow, SLOT(setDiagramCreateFlag()));
	QObject::connect(okButton, SIGNAL(clicked()), parent, SLOT(close()));

	QVBoxLayout *vLayout = new QVBoxLayout;
	vLayout->addWidget(mDiagramsListWidget);
	vLayout->addWidget(okButton);

	setLayout(vLayout);
}
