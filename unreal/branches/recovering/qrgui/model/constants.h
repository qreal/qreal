#pragma once

#include <QString>

namespace qReal {

	enum action
	{
		actSetData,
		actAddElement,
		actRemoveElement,
		actCreateDiagram,
		actDestroyDiagram
	};

	enum flag
	{
		flgEditors,
		flgDiagrams,
		flgUselessMessages,
		flgInvalidMessages,
	};

	const QString msgInvalid = "Invalid";
	const QString msgSetData = "SetData";
	const QString msgAddElement = "AddElement";
	const QString msgRemoveElement = "RemoveElement";
	const QString msgCreateDiagram = "CreateDiagram";
	const QString msgDestroyDiagram = "DestroyDiagram";

	const QString msgOperation = "Operation: ";
	const QString msgTarget = "Target: ";
	const QString msgDetails = "Details: ";
	const QString msgPrevValue = "PrevValue: ";
	const QString msgNewValue = "NewValue: ";
}
