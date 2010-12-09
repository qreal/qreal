#pragma once

#include <QString>

namespace qReal {

	enum action
	{
		actSetData,
		actAddElement,
		actRemoveElement,
		actReplaceElement
	};

	enum flag
	{
		flgEditors,
		flgDiagrams,
		flgUselessMessages,
	};

	const QString msgInvalid = "Invalid";
	const QString msgSetData = "SetData";
	const QString msgAddElement = "AddElement";
	const QString msgRemoveElement = "RemoveElement";
	const QString msgReplaceElement = "ReplaceElement";


	const QString msgScene = "Scene: ";
	const QString msgOperation = "Operation: ";
	const QString msgTarget = "Target: ";
	const QString msgDetails = "Details: ";
	const QString msgPrevValue = "PrevValue: ";
	const QString msgNewValue = "NewValue: ";

	const QString msgAllElements = "<AllElements>";

	const QString extensionLog = "log";
	const QString extensionPatch = "patch";
}
