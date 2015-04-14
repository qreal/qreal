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

#include <QtCore/QObject>

#include "qrutils/utilsDeclSpec.h"

namespace invocation
{

/// @brief Represents operation execution progress
class QRUTILS_EXPORT Progress : public QObject
{
	Q_OBJECT

public:
	Progress(int minimum = 0, int maximum = 100);

	/// Returns current progress value. The default value is minimum()
	int value() const;
	/// Sets current progress value.
	/// Attempting to change the current value to one outside the
	/// minimum-maximum range has no effect on the current value.
	void setValue(int value);
	/// Returns minimal progress value
	int minimum() const;
	/// Sets minimal progress value
	/// Attempting to change the current minimum to one greater then
	/// the current maximum value has no effect on the current minimum.
	void setMinimum(int minimum);
	/// Returns minimal progress value
	int maximum() const;
	/// Sets maximal progress value
	/// Attempting to change the current maximum to one less then
	/// the current minimum value has no effect on the current maximum.
	void setMaximum(int maximum);

signals:
	/// Emitted when current progress value was changed
	void valueChanged(int newValue);
	/// Emitted when minimum progress value was changed
	void minimumChanged(int newMinimum);
	/// Emitted when maximum progress value was changed
	void maximumChanged(int newMaximum);

private:
	int mValue;
	int mMinimum;
	int mMaximum;
};

}
