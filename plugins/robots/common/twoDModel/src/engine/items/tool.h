/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
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

#include <QtWidgets/QAction>

namespace twoDModel {
namespace items {

/// @brief Represents an item on 2D model palette.
class Tool
{
public:
	Tool(const QIcon &icon, const QString &displayedName);
	~Tool();

	/// Returns an icon that will be drawn on 2D model palette.
	QIcon icon() const;

	/// Returns name of this tool that will be shown in a tooltip of item on 2D model palette.
	QString displayedName() const;

	/// Returns an action
	QAction &action() const;

private:
	const QIcon mIcon;
	const QString mDisplayedName;
	QAction mAction;
};

}
}
