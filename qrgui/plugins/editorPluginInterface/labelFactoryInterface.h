#pragma once

#include <QtCore/QString>

#include "qrgui/plugins/editorPluginInterface/labelInterface.h"

namespace qReal {

/// Interface that allows to create new text labels (\see LabelInterface) in editor plugins.
class LabelFactoryInterface
{
public:
	/// Creates static label with given fixed text, coordinates and rotation.
	/// @param index - index of a label in parent element, uniquely identifies it.
	/// @param x - x coordinate of a label in coordinate system of a shape.
	/// @param y - y coordinate of a label in coordinate system of a shape.
	/// @param text - static textual contents of a label, not connected to repository and can not be edited.
	/// @param rotation - sets rotation of a label.
	virtual LabelInterface *createLabel(int index, qreal x, qreal y, const QString &text, qreal rotation) = 0;

	/// Creates dynamic label with given binding string, coordinates and rotation.
	/// @param index - index of a label in parent element, uniquely identifies it.
	/// @param x - x coordinate of a label in coordinate system of a shape.
	/// @param y - y coordinate of a label in coordinate system of a shape.
	/// @param binding - binding string. Can contain name of a property from which to take actual label value,
	///        or a string that can contain properties mixed with static text using ## as separator.
	///        For example, "Power: ##power##%, mode: ##engineMode##".
	/// @param readOnly - if true, user will not be allowed to modify label's value, if false, user can edit
	///        contents of a label, and modify underlying value in repository.
	/// @param rotation - sets rotation of a label.
	virtual LabelInterface *createLabel(int index, qreal x, qreal y, const QString &binding, bool readOnly
			, qreal rotation) = 0;
};

}
