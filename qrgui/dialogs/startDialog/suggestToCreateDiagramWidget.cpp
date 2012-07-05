#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtCore/QDebug>
#include "suggestToCreateDiagramWidget.h"
#include "../../../qrkernel/ids.h"

SuggestToCreateDiagramWidget::SuggestToCreateDiagramWidget(QWidget *parent)
	: QWidget(parent)
{

	QVBoxLayout vLayout;
	QHBoxLayout hLayout;
	setLayout(&vLayout);
	setMinimumSize(320, 240);
	setMaximumSize(320, 240);

	QListWidget diagramsListWidget(this);
	int i = 0;
//	foreach(Id editor, manager()->editors()) {
//		foreach(Id diagram, manager()->diagrams(Id::loadFromString("qrm:/" + editor.editor()))) {
//			QString const diagramName = mEditorManager.editorInterface(editor.editor())->diagramName(diagram.diagram());
//			QString const diagramNodeName = mEditorManager.editorInterface(editor.editor())->diagramNodeName(diagram.diagram());
//			if (diagramNodeName.isEmpty()) {
//				continue;
//			}
//			mDiagramsList.append("qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName);
//			diagramsListWidget.addItem(diagramName);
//			diagramsListWidget.addItem(diagramName);
//			i++;
//		}
//	}

	QPushButton cancelButton;
	cancelButton.setText(tr("&Cancel"));
	QPushButton okButton;
	okButton.setText(tr("&OK"));

//	QObject::connect(&diagramsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(diagramInCreateListSelected(int)));
//	QObject::connect(&diagramsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(setDiagramCreateFlag()));
//	QObject::connect(&diagramsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), &widget, SLOT(close()));

//	QObject::connect(&widget, SIGNAL(destroyed()), this, SLOT(diagramInCreateListDeselect()));
//	QObject::connect(&cancelButton, SIGNAL(clicked()), &widget, SLOT(close()));

//	QObject::connect(&okButton, SIGNAL(clicked()), this, SLOT(setDiagramCreateFlag()));
//	QObject::connect(&okButton, SIGNAL(clicked()), &widget, SLOT(close()));

	diagramsListWidget.setCurrentRow(0);
//	mDiagramCreateFlag = false;

	QLabel label(tr("Choose new diagram"));
	vLayout.addWidget(&label);
	vLayout.addWidget(&diagramsListWidget);

	hLayout.addWidget(&okButton);
	hLayout.addWidget(&cancelButton);

	vLayout.addLayout(&hLayout);
}
