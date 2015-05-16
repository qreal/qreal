#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/node.h"
#include "variablesTable.h"
#include "currentScope.h"

namespace generationRules {
namespace generator {

/// Common data for all generators.
class GeneratorConfigurer
{
public:
	/// Constructor.
	/// @param logicalModelInterface - data about model.
	/// @param editorManagerInterface - data about metamodel.
	/// @param variablesTable - table of variables.
	/// @param editorId - current editor id.
	/// @param diagramID - current diagram id.
	GeneratorConfigurer(qReal::LogicalModelAssistInterface *logicalModelInterface
			, qReal::EditorManagerInterface *editorManagerInterface
			, VariablesTable &variablesTable
			, CurrentScope &currentScope
			, const qReal::Id &editorId
			, const qReal::Id &diagramId
			, const QString &pathToGeneratedCode)
		: mLogicalModelAssistInterface(logicalModelInterface)
		, mEditorManagerInterface(editorManagerInterface)
		, mVariablesTable(variablesTable)
		, mCurrentScope(currentScope)
		, mEditorId(editorId)
		, mDiagramId(diagramId)
		, mPathToGeneratedCode(pathToGeneratedCode)
	{}

	/// Returns logical model interface (data about model).
	qReal::LogicalModelAssistInterface *logicalModelInterface() const
	{
		return mLogicalModelAssistInterface;
	}

	/// Returns data about metamodel.
	qReal::EditorManagerInterface *editorManagerInterface() const
	{
		return mEditorManagerInterface;
	}

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

	/// Returns current editor id.
	const qReal::Id &editorId() const
	{
		return mEditorId;
	}

	/// Returns current diagram id.
	const qReal::Id &diagramId() const
	{
		return mDiagramId;
	}

	/// Returns path to generated code.
	const QString &pathToGeneratedCode() const
	{
		return mPathToGeneratedCode;
	}

private:
	qReal::LogicalModelAssistInterface *mLogicalModelAssistInterface;  // doesn't have ownership
	VariablesTable &mVariablesTable;
	qReal::EditorManagerInterface *mEditorManagerInterface;  // doesn't have ownership
	CurrentScope &mCurrentScope;
	const qReal::Id &mEditorId;
	const qReal::Id &mDiagramId;
	const QString &mPathToGeneratedCode;
};

}
}
