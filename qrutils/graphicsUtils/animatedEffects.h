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

#include "qrutils/utilsDeclSpec.h"

class QGraphicsItem;
class QWidget;

namespace graphicsUtils {

/// Highlights the given item animating its opacity.
class QRUTILS_EXPORT AnimatedEffects
{
public:
	/// Highlights the given item animating its opacity.
	static void highlight(QGraphicsItem * const item);

	/// The given widget disappear with opacity animation for duration.
	static void disappear(QWidget *widget, int duration);
};

}
