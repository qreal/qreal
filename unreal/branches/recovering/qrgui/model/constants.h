#pragma once

#include <QString>

namespace qReal {

	enum action
	{
		actSetData,
		actChangeType,
		actAddElement,
		actRemoveElement
	};

	enum flag
	{
		flgEditors,
		flgDiagrams,
		flgUselessMessages,
	};

	const QString msgInvalid = "Invalid";
	const QString msgSetData = "SetData";
	const QString msgChangeType = "ChangeType";
	const QString msgAddElement = "AddElement";
	const QString msgRemoveElement = "RemoveElement";

	const QString msgScene = "Scene: ";
	const QString msgOperation = "Operation: ";
	const QString msgTarget = "Target: ";
	const QString msgDetails = "Details: ";
	const QString msgPrevValue = "PrevValue: ";
	const QString msgNewValue = "NewValue: ";

	const QString extensionLog = "log";
	const QString extensionPatch = "patch";
}
