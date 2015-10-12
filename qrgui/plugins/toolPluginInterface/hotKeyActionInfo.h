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
#include <QtWidgets/QAction>

namespace qReal {

/// Value object, provides info about hotkeys that can be added by tool plugin to a list of
/// global hotkeys and initiate action from a plugin.
class HotKeyActionInfo
{
public:
	HotKeyActionInfo(const QString &id, const QString &label, QAction * const action)
		: mAction(action)
		, mId(id)
		, mLabel(label)
	{
	}

	QAction *action() const
	{
		return mAction;
	}

	QString id() const
	{
		return mId;
	}

	QString label() const
	{
		return mLabel;
	}

private:
	/// Does not have ownership.
	QAction * mAction;
	QString mId;
	QString mLabel;
};

}

