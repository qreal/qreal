#pragma once

#include <QtCore/QString>

#include "labelInterface.h"

/// Interface that allows to create new text labels (\see LabelInterface) in editor plugins.
class LabelFactoryInterface
{
public:
	/// Creates static label with given fixed text, coordinates and rotation.
	/// @param x X coordinate of a label in coordinate system of a shape.
	/// @param y Y coordinate of a label in coordinate system of a shape.
	/// @param text Static textual contents of a label, not connected to repository and can not be edited.
	/// @param rotation Sets rotation of a label.
	virtual LabelInterface *createLabel(qreal x, qreal y, QString const &text, qreal rotation) = 0;

	/// Creates dynamic label with given binding string, coordinates and rotation.
	/// @param x X coordinate of a label in coordinate system of a shape.
	/// @param y Y coordinate of a label in coordinate system of a shape.
	/// @param binding Binding string. Can contain name of a property from which to take actual label value,
	///        or a string that can contain properties mixed with static text using ## as separator.
	///        For example, "Power: ##power##%, mode: ##engineMode##".
	/// @param readOnly If true, user will not be allowed to modify label's value, if false, user can edit
	///        contents of a label, and modify underlying value in repository.
	/// @param rotation Sets rotation of a label.
	virtual LabelInterface *createLabel(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation) = 0;
};
