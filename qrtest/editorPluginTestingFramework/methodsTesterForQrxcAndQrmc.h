#pragma once

#include <QtCore/QString>

#include "qrgui/plugins/editorPluginInterface/editorInterface.h"
#include "qrgui/plugins/pluginManager/interpreterEditorManager.h"
#include "abstractStringGenerator.h"
#include "methodsTester.h"

namespace editorPluginTestingFramework {

class MethodsTesterForQrxcAndQrmc : public MethodsTester
{
public:

	/// gets editorInterfaces from mainClass and initializes mQrmcGeneratedPlugin and mQrxcGeneratedPlugin with them
	MethodsTesterForQrxcAndQrmc(
			qReal::EditorInterface * const qrmcGeneratedPlugin
			, qReal::EditorInterface * const qrxcGeneratedPlugin
			);

	/// returns list of generated output to main class
	QList<QPair<QString, QPair<QString, QString> > > generatedOutput();

	/// clones generator, initializes it with qrxcGeneratedPlugin and returns it to methodsTester
	AbstractStringGenerator * initGeneratorWithFirstInterface(AbstractStringGenerator const &generator) const;

	/// clones generator, initializes it with qrmcGeneratedPlugin and returns it to methodsTester
	AbstractStringGenerator * initGeneratorWithSecondInterface(AbstractStringGenerator const &generator) const;

private:
	class StringGenerator;

	class StringGeneratorForDiagrams;
	class StringGeneratorForElements;
	class StringGeneratorForProperties;
	class StringGeneratorForGroups;

	class PropertiesWithDefaultValuesStringGenerator;
	class TypesContainedByStringGenerator;
	class GetPossibleEdgesStringGenerator;
	class IsNodeOrEdgeStringGenerator;

	class GetPropertyNamesStringGenerator;
	class GetReferencePropertiesStringGenerator;
	class GetParentsOfStringGenerator;

	class GetPropertyTypesStringGenerator;
	class GetPropertyDefaultValueStringGenerator;

	class DiagramNameStringGenerator;
	class DiagramNodeNameStringGenerator;

	class ElementNameStringGenerator;
	class ElementMouseGestureStringGenerator;
	class ElementDescriptionStringGenerator;

	class PropertyDescriptionStringGenerator;
	class PropertyDisplayedNameStringGenerator;

	class IsParentOfStringGenerator;

	class DiagramPaletteGroupListStringGenerator;
	class DiagramPaletteGroupDescriptionStringGenerator;

	class DiagramsStringGenerator;
	class ElementsStringGenerator;
	class ExplosionsStringGenerator;
	class PortTypesStringGenerator;
	class EnumValueStringGenerator;

	/// fills mGeneratedList with results of testing
	void testMethods();

	/// finds out if string contains only of given symbol (for example, "aaa" contains only of symbol 'a')
	static bool containsOnly(QString const &string, QChar const &symbol);

	qReal::EditorInterface* mQrmcGeneratedPlugin;
	qReal::EditorInterface* mQrxcGeneratedPlugin;

	qReal::InterpreterEditorManager* mInterpreterGeneratedPlugin;

	QList<QPair<QString, QPair<QString, QString> > > mGeneratedList;
};

}

