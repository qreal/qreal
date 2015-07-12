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

#include "variablesTable.h"
#include "currentScope.h"

namespace generationRules {
namespace generator {

/// Information about current scope.
class ScopeInfo
{
public:
	/// Constructor.
	/// @param variablesTable - table of variables.
	/// @param currentScope - information about current scope.
	ScopeInfo(VariablesTable &variablesTable
			, CurrentScope &currentScope)
		: mVariablesTable(variablesTable)
		, mCurrentScope(currentScope)
		, mExcludedText(QString())
	{}

	/// Returns table of variables.
	VariablesTable &variablesTable() const
	{
		return mVariablesTable;
	}

	/// Returns information about current scope (generator name, "this").
	CurrentScope &currentScope() const
	{
		return mCurrentScope;
	}

	/// Returns true if we have to exclude text.
	const QString &excludedText() const
	{
		return mExcludedText;
	}

	/// Updates exclude text flag.
	void setExcludedText(const QString &excludedText)
	{
		mExcludedText = excludedText;
	}

private:
	VariablesTable &mVariablesTable;
	CurrentScope &mCurrentScope;
	QString mExcludedText;
};

}
}
