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
#include <QtCore/QStack>

#include <qrkernel/ids.h>

namespace generationRules {
namespace generator {

/// Class that provides information about current scope.
class CurrentScope
{
public:
	CurrentScope();

	/// Methods for work with stack of current "this" elements.
	/// Changes current "this" id (when callGeneratorFor is called, "this" changes).
	/// @param newCurrentVariableId - new "this" id.
	void changeCurrentId(const qReal::Id &newCurrentVariableId);

	/// Returns current variable ("this").
	qReal::Id currentId() const;

	/// Removes last added id from stack of ids.
	void removeLastCurrentId();

	/// Changes current generator name.
	void changeCurrentGeneratorName(const QString &newGeneratorName);

	/// Returns current generator name.
	QString currentGeneratorName() const;

private:
	QStack<qReal::Id> mStackOfThis;
	QString mCurrentGeneratorName;
};

}
}
