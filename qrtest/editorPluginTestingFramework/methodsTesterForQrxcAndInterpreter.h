#pragma once

#include "qrgui/plugins/pluginManager/editorManagerInterface.h"
#include "qrgui/plugins/pluginManager/interpreterEditorManager.h"
#include "qrgui/plugins/pluginManager/editorManager.h"

#include "methodsTester.h"
#include "defs.h"
#include "abstractStringGenerator.h"

namespace editorPluginTestingFramework {

class MethodsTesterForQrxcAndInterpreter : public MethodsTester
{
public:

	/// gets qrxc generated plugin and interpreter generated plugin from main class
	MethodsTesterForQrxcAndInterpreter(
			qReal::EditorManager* qrxcGeneratedPlugin
			, qReal::InterpreterEditorManager* interpreterGeneratedPlugin
			);

	/// returns results of testing to main class
	QList<QPair<QString, QPair<QString, QString> > > generatedResult();

	/// clones generator, initializes it with qrxcGeneratedPlugin and returns it to methodsTester
	AbstractStringGenerator * initGeneratorWithFirstInterface(AbstractStringGenerator const &generator) const;

	/// clones generator, initializes it with interpreterGeneratedPlugin and returns it to methodsTester
	AbstractStringGenerator * initGeneratorWithSecondInterface(AbstractStringGenerator const &generator) const;

private:

	class StringGenerator;

	class StringGeneratorForEditors;
	class StringGeneratorForDiagrams;
	class StringGeneratorForElements;
	class StringGeneratorForProperties;

	class EditorsListGenerator;
	class DiagramsListGenerator;
	class ElementsListGeneratorWithIdParameter;
	class ElementsListGeneratorWithQStringParameters;

	class MouseGesturesListGenerator;
	class FriendlyNameListGenerator;
	class DescriptionListGenerator;
	class PropertyDescriptionListGenerator;
	class PropertyDisplayedNameListGenerator;

	class ContainedTypesListGenerator;
	class ExplosionsListGenerator;
	class EnumValuesListGenerator;
	class TypeNameListGenerator;
	class AllChildrenTypesOfListGenerator;

	class IsEditorListGenerator;
	class IsDiagramListGenerator;
	class IsElementListGenerator;

	class PropertyNamesListGenerator;
	class PortTypesListGenerator;
	class DefaultPropertyValuesListGenerator;
	class PropertiesWithDefaultValuesListGenerator;

	class HasElementListGenerator;
	class FindElementByTypeListGenerator;
	class IsGraphicalElementListGenerator;

	class IsNodeOrEdgeListGenerator;
	class DiagramNameListGenerator;
	class DiagramNodeNameListGenerator;
	class IsParentPropertyListGenerator;
	class ChildrenListGenerator;
	class ShapeListGenerator;

	/// forms results of testing for each method, puts them into mGeneratedList
	void testMethods();

	qReal::EditorManager* mQrxcGeneratedPlugin;
	qReal::InterpreterEditorManager* mInterpreterGeneratedPlugin;

	QList<QPair<QString, QPair<QString, QString> > > mGeneratedList;
};

}
