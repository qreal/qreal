#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/node.h"
#include "variablesTable.h"

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
			, const qReal::Id &editorId
			, const qReal::Id &diagramId)
		: mLogicalModelAssistInterface(logicalModelInterface)
		, mEditorManagerInterface(editorManagerInterface)
		, mVariablesTable(variablesTable)
		, mEditorId(editorId)
		, mDiagramId(diagramId)
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

private:
	qReal::LogicalModelAssistInterface *mLogicalModelAssistInterface;  // doesn't have ownership
	VariablesTable &mVariablesTable;
	qReal::EditorManagerInterface *mEditorManagerInterface;  // doesn't have ownership
	const qReal::Id &mEditorId;
	const qReal::Id &mDiagramId;
};

}
}
