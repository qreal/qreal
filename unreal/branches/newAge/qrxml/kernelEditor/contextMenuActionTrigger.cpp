#include "contextMenuActionTrigger.h"
#include "methodeditordialog.h"

#include <QtCore/QDebug>
#include <QUuid>
#include <QVariant>
#include "../../qrgui/kernel/ids.h"

using namespace kernelEditor;
using namespace qReal;

void ContextMenuActionTrigger::contextMenuActionTriggered(QString const &name)
{
	QString actionName = name.section("###", 0, 0);
	QString uuid = name.section("###", 1, 1);

	if (actionName == "Add method") {
		MethodEditorDialog dialog;
		int dialogResult = dialog.exec();	
		
		if (dialogResult) {
			qDebug() << "ACCEPTED!!!";

			Id id = Id::loadFromString(uuid);
			if (id.element() == "MethodsContainer") {
				Id newMethodId = mApi->createElement(id, Id("Kernel_metamodel", "Kernel",
							"Method"));
				mApi->setProperty(newMethodId, Qt::DisplayRole, QVariant(dialog.generateMethodString()));	
			} else if (id.element() == "Class") {
			}
		}
		else {
			qDebug() << "not accepted...";
		}
	}
}
