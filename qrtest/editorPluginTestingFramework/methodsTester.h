#include <QtCore/QString>
#include "../../qrgui/editorPluginInterface/editorInterface.h"

using namespace qReal;
class MethodsTester
{
public:
	MethodsTester(EditorInterface* qrmcGeneratedPlugin, EditorInterface* qrxcGeneratedPlugin);

	// хотим метод, который берет название метода у EditorInterface, генерит 2 листа и сравнивает
	// и метод, который выводит что-то...

	void testMethods();

private:
	class ListGenerator;

	class EditorNameListGenerator;
	class DiagramsListGenerator;

	void testMethod(ListGenerator const &listGenerator);
	EditorInterface* mQrmcGeneratedPlugin;
	EditorInterface* mQrxcGeneratedPlugin;
};

