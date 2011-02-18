#include "../../qrgui/editorManager/editorManager.h"

#include "contextMenuActionTrigger.h"
#include "methodeditordialog.h"
#include "fieldeditordialog.h"

#include <QtCore/QDebug>
#include <QUuid>
#include <QVariant>
#include "../../qrgui/kernel/ids.h"

using namespace kernelEditor;
using namespace qReal;

void ContextMenuActionTrigger::writeMethodPropertiesToModel(qReal::Id methodElemId, QString name, QString returnType, 
		QString parameters, QString visibility, QString displayName)
{
	QStringList listOfPropertyNames = mApi->editorManager().getPropertyNames(methodElemId);

	mApi->setProperty(methodElemId, listOfPropertyNames.indexOf("methodName") + roles::customPropertiesBeginRole, QVariant(name));
	mApi->setProperty(methodElemId, listOfPropertyNames.indexOf("methodReturnType") + roles::customPropertiesBeginRole, QVariant(returnType));
	mApi->setProperty(methodElemId, listOfPropertyNames.indexOf("methodParameters") + roles::customPropertiesBeginRole, QVariant(parameters));
	mApi->setProperty(methodElemId, listOfPropertyNames.indexOf("methodVisibility") + roles::customPropertiesBeginRole, QVariant(visibility));
	mApi->setProperty(methodElemId, Qt::DisplayRole, QVariant(displayName));

	//qDebug() << "SASAAS" << mApi->children(methodElemId).size();
}

void ContextMenuActionTrigger::writeFieldPropertiesToModel(qReal::Id fieldElemId, QString name, QString type,
		QString visibility, QString displayName)
{
	QStringList listOfPropertyNames = mApi->editorManager().getPropertyNames(fieldElemId);
	mApi->setProperty(fieldElemId, listOfPropertyNames.indexOf("fieldName") + roles::customPropertiesBeginRole, QVariant(name));
	mApi->setProperty(fieldElemId, listOfPropertyNames.indexOf("fieldType") + roles::customPropertiesBeginRole, QVariant(type));
	mApi->setProperty(fieldElemId, listOfPropertyNames.indexOf("fieldVisibility") + roles::customPropertiesBeginRole, QVariant(visibility));
	mApi->setProperty(fieldElemId, Qt::DisplayRole, QVariant(displayName));	
}

void ContextMenuActionTrigger::contextMenuActionTriggered(QString const &name)
{
	QString actionName = name.section("###", 0, 0);
	QString uuid = name.section("###", 1, 1);

	if (actionName == "Add method") {
		MethodEditorDialog dialog;
		int dialogResult = dialog.exec();	
		
		if (dialogResult) {
			Id id = Id::loadFromString(uuid);
			Id newMethodId;
			if (id.element() == "MethodsContainer") {
				newMethodId = mApi->createElement(id, Id("Kernel_metamodel", "Kernel",
							"Method"));
			} else if (id.element() == "Class") {
				foreach(Id curChildId, mApi->children(id)) {
					if (curChildId.element() == "MethodsContainer") {
						newMethodId = mApi->createElement(curChildId, Id("Kernel_metamodel", "Kernel",
							"Method"));
						break;
					}
				}
			}
			writeMethodPropertiesToModel(newMethodId, dialog.methodName(), dialog.methodReturnType(), 
					dialog.methodParameteresInString(), dialog.methodVisibility(),
					dialog.generateMethodString());
		}
	} else if (actionName == "Add field") {
		FieldEditorDialog dialog;
		int dialogResult = dialog.exec();	
		
		if (dialogResult) {
			Id id = Id::loadFromString(uuid);
			Id newFieldId;
			if (id.element() == "FieldsContainer") {
				newFieldId = mApi->createElement(id, Id("Kernel_metamodel", "Kernel",
							"Field"));
			} else if (id.element() == "Class") {
				foreach(Id curChildId, mApi->children(id)) {
					if (curChildId.element() == "FieldsContainer") {
						newFieldId = mApi->createElement(curChildId, Id("Kernel_metamodel", "Kernel",
							"Field"));
						break;
					}
				}
			}
			writeFieldPropertiesToModel(newFieldId, dialog.fieldName(), dialog.fieldType(),
					dialog.fieldVisibility(), dialog.generateFieldString());
		}
	}
}
