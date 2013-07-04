#pragma once

#include "../../qrgui/pluginManager/editorManagerInterface.h"
#include "../../qrgui/pluginManager/interpreterEditorManager.h"
#include "../../qrgui/pluginManager/editorManager.h"

namespace editorPluginTestingFramework {

class EditorManagerMethodsTester
{
public:
	EditorManagerMethodsTester(qReal::EditorManager* qrxcGeneratedPlugin
			, qReal::InterpreterEditorManager* interpreterGeneratedPlugin);

	void testMethods();

	QList<QPair<QString, QPair<QString, QString> > > generatedResult();

private:
	class StringGenerator;

	class StringGeneratorForEditors;
	class StringGeneratorForDiagrams;
	class StringGeneratorForElements;
	class StringGeneratorForProperties;
	class StringGeneratorForGroups;

	class DiagramsListGenerator;
	class ElementsListGenerator;
	class FriendlyNameListGenerator;
	class DescriptionListGenerator;

	void testMethod(StringGenerator const &stringGenerator);

	qReal::EditorManager* mQrxcGeneratedPlugin;
	qReal::InterpreterEditorManager* mInterpreterGeneratedPlugin;

	QList<QPair<QString, QPair<QString, QString> > > mGeneratedList;
};

}
