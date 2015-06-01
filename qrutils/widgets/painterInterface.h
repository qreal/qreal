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

#include <QtGui/QPainter>

#include "qrutils/utilsDeclSpec.h"

namespace qReal {
namespace ui {

/// An interface for object that implements some painting logic on PaintWidget.
/// @see qReal::ui::PaintWidget
class QRUTILS_EXPORT PainterInterface
{
public:
	virtual ~PainterInterface() {}

	/// Implements the painting process itself.
	virtual void paint(QPainter *painter) = 0;

	/// Clears all drawn stuff.
	virtual void reset() = 0;
};

}
}
