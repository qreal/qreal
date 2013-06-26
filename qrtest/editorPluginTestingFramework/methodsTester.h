#pragma once

#include <QtCore/QString>

#include "../../qrgui/editorPluginInterface/editorInterface.h"

namespace editorPluginTestingFramework {

class MethodsTester
{

public:
	MethodsTester(qReal::EditorInterface * const qrmcGeneratedPlugin, qReal::EditorInterface * const qrxcGeneratedPlugin);

	void testMethods();

	QList<QPair<QString, QString> > generateOutputList();

private:
	class StringGenerator;

	class StringGeneratorForDiagrams;
	class StringGeneratorForElements;
	class StringGeneratorForProperties;
	class StringGeneratorForGroups;

	class PropertiesWithDefaultValuesStringGenerator;
	class TypesContainedByStringGenerator;
	class ConnectedTypesStringGenerator;
	class UsedTypesStringGenerator;
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

	void testMethod(StringGenerator const &stringGenerator);

	void generateOutputForOneMethod(StringGenerator const &stringGenerator);

	static bool containsOnly(QString const &string, QChar const &symbol);

	qReal::EditorInterface* mQrmcGeneratedPlugin;
	qReal::EditorInterface* mQrxcGeneratedPlugin;

	QList<QPair<QString, QString> > mGeneratedOutputList;
};

}

