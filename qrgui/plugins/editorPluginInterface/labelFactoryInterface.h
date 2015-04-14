/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QString>

#include "qrgui/plugins/editorPluginInterface/labelInterface.h"

namespace qReal {

/// Interface that allows to create new text labels (\see LabelInterface) in editor plugins.
class LabelFactoryInterface
{
public:
	virtual ~LabelFactoryInterface() {}

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
