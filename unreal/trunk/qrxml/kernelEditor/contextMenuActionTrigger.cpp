#include "contextMenuActionTrigger.h"
#include "methodeditordialog.h"

#include <QtCore/QDebug>

using namespace kernelEditor;

void ContextMenuActionTrigger::contextMenuActionTriggered(QString const &name)
{
	qDebug() << "contextMenuActionTriggered called";

	QString actionName = name;

	int numOfSharps = 0;
	for (int i = 0; i < name.size(); i++) {
		
	}

	if (name == "Add method") {
		MethodEditorDialog dialog;
		dialog.exec();	
	}
}
