#pragma once

#include <QString>

namespace qReal {

	enum action
	{
		actInvalid,

		actSetData,
		actAddElement,
		actRemoveElement,
		actCreateDiagram,
		actDestroyDiagram,

		ptchReplaceElement
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

	const QString cmdReplaceElement = "ReplaceElement";

	const QString msgOperation = "Operation: ";
	const QString msgTarget = "Target: ";
	const QString msgDetails = "Details: ";
	const QString msgPrevValue = "PrevValue: ";
	const QString msgNewValue = "NewValue: ";

	const QString msgAllElements = "<AllElements>";

	const QString extensionLog = "log";
	const QString extensionPatch = "patch";
}
