#include <QtCore/QString>
#include "../../qrgui/editorPluginInterface/editorInterface.h"

using namespace qReal;
class MethodsTester
{
public:
	MethodsTester(EditorInterface* qrmcGeneratedPlugin, EditorInterface* qrxcGeneratedPlugin);

	void testMethods();

private:
	class ListGenerator;

	class EditorNameListGenerator;
	class DiagramsListGenerator;
	class ElementsListGenerator;
	class PropertiesWithDefaultValuesListGenerator;
	class TypesContainedByListGenerator;
	class ConnectedTypesListGenerator;
	class UsedTypesListGenerator;

	void testMethod(ListGenerator const &listGenerator);

	EditorInterface* mQrmcGeneratedPlugin;
	EditorInterface* mQrxcGeneratedPlugin;
};

