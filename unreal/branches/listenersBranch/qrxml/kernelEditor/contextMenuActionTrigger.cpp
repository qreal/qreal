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
		QString parameters, QString displayName)
{
	QStringList listOfPropertyNames = mApi->editorManager().getPropertyNames(methodElemId);

	qDebug() << listOfPropertyNames;

	mApi->setProperty(methodElemId, listOfPropertyNames.indexOf("methodName") + roles::customPropertiesBeginRole, QVariant(name));
	mApi->setProperty(methodElemId, listOfPropertyNames.indexOf("methodReturnType") + roles::customPropertiesBeginRole, QVariant(returnType));
	mApi->setProperty(methodElemId, listOfPropertyNames.indexOf("methodParameters") + roles::customPropertiesBeginRole, QVariant(parameters));
	mApi->setProperty(methodElemId, Qt::DisplayRole, QVariant(displayName));
}

void ContextMenuActionTrigger::writeFieldPropertiesToModel(qReal::Id fieldElemId, QString name, QString type,
		QString displayName)
{
	QStringList listOfPropertyNames = mApi->editorManager().getPropertyNames(fieldElemId);
	mApi->setProperty(fieldElemId, listOfPropertyNames.indexOf("fieldName") + roles::customPropertiesBeginRole, QVariant(name));
	mApi->setProperty(fieldElemId, listOfPropertyNames.indexOf("fieldType") + roles::customPropertiesBeginRole, QVariant(type));
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
			qDebug() << "ACCEPTED!!!";

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
					dialog.methodParameteresInString(), dialog.generateMethodString());
		}
	} else if (actionName == "Add field") {
		FieldEditorDialog dialog;
		int dialogResult = dialog.exec();	
		
		if (dialogResult) {
			qDebug() << "ACCEPTED!!!";

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
			writeFieldPropertiesToModel(newFieldId, dialog.getFieldName(), dialog.getFieldType(),
					dialog.generateFieldString());
		}
	}
}
