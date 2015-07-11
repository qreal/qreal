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
