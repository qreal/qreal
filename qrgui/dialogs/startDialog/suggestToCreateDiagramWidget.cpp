#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtCore/QDebug>
#include "suggestToCreateDiagramWidget.h"
#include "../../../qrkernel/ids.h"
#include "../mainwindow/mainWindow.h"

qReal::SuggestToCreateDiagramWidget::SuggestToCreateDiagramWidget(MainWindow *mainWindow, QDialog *parent)
	: QWidget(parent)
	, mMainWindow(mainWindow)
	, mDiagramsListWidget(new QListWidget(this))
{
	int i = 0;
	foreach(Id editor, mMainWindow->manager()->editors()) {
		foreach(Id diagram, mMainWindow->manager()->diagrams(Id::loadFromString("qrm:/" + editor.editor()))) {
			QString const diagramName = mMainWindow->mEditorManager.editorInterface(editor.editor())->diagramName(diagram.diagram());
			QString const diagramNodeName = mMainWindow->mEditorManager.editorInterface(editor.editor())->diagramNodeName(diagram.diagram());
			if (diagramNodeName.isEmpty()) {
				continue;
			}
			mMainWindow->mDiagramsList.append("qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName);
			mDiagramsListWidget->addItem(diagramName);
			i++;
		}
	}
//	mDiagramsListWidget->setCurrentRow(0);

	QPushButton *cancelButton = new QPushButton(tr("&Cancel"), this);
	QPushButton *okButton = new QPushButton(tr("&OK"), this);

	QObject::connect(mDiagramsListWidget, SIGNAL(currentRowChanged(int)), mMainWindow, SLOT(diagramInCreateListSelected(int)));
	QObject::connect(mDiagramsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), mMainWindow, SLOT(setDiagramCreateFlag()));
	QObject::connect(mDiagramsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), parent, SIGNAL(close()));

	QObject::connect(this, SIGNAL(destroyed()), mMainWindow, SLOT(diagramInCreateListDeselect()));
	QObject::connect(cancelButton, SIGNAL(clicked()), parent, SLOT(close()));

	QObject::connect(okButton, SIGNAL(clicked()), mMainWindow, SLOT(setDiagramCreateFlag()));
	QObject::connect(okButton, SIGNAL(clicked()), parent, SLOT(close()));

	QVBoxLayout *vLayout = new QVBoxLayout;

//	QLabel label(tr("Choose new diagram"));
//	vLayout.addWidget(&label);
	vLayout->addWidget(mDiagramsListWidget);

	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addWidget(okButton);
	hLayout->addWidget(cancelButton);

	vLayout->addLayout(hLayout);
	setLayout(vLayout);

	mMainWindow->mDiagramCreateFlag = false;
}
