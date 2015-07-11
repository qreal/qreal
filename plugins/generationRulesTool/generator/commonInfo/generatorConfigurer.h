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
	/// @param editorId - current editor id.
	/// @param diagramID - current diagram id.
	/// @param pathToGeneratedCode - path to folder with generated code.
	GeneratorConfigurer(qReal::LogicalModelAssistInterface *logicalModelInterface
			, qReal::EditorManagerInterface *editorManagerInterface
			, const qReal::Id &editorId
			, const qReal::Id &diagramId
			, const QString &pathToGeneratedCode)
		: mLogicalModelAssistInterface(logicalModelInterface)
		, mEditorManagerInterface(editorManagerInterface)
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
	qReal::EditorManagerInterface *mEditorManagerInterface;  // doesn't have ownership
	const qReal::Id &mEditorId;
	const qReal::Id &mDiagramId;
	const QString &mPathToGeneratedCode;
};

}
}
