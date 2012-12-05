#include <QtCore/QString>
#include "../../qrgui/editorPluginInterface/editorInterface.h"

using namespace qReal;
class MethodsTester
{
public:
	MethodsTester(EditorInterface* qrmcGeneratedPlugin, EditorInterface* qrxcGeneratedPlugin);

	void testMethods();

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

	class DiagramPaletteGroupListStringGenerator;
	class DiagramPaletteGroupDescriptionStringGenerator;

	void testMethod(StringGenerator const &stringGenerator);

	EditorInterface* mQrmcGeneratedPlugin;
	EditorInterface* mQrxcGeneratedPlugin;
};

