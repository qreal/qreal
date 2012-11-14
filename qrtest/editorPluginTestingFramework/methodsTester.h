#include <QtCore/QString>
#include "../../qrgui/editorPluginInterface/editorInterface.h"

using namespace qReal;
class MethodsTester
{
public:
	MethodsTester(EditorInterface* qrmcGeneratedPlugin, EditorInterface* qrxcGeneratedPlugin);

	void testMethods();

	/*
	virtual QString editorName() const = 0;
	virtual QString diagramName(QString const &diagram) const = 0;
	virtual QStringList diagrams() const = 0;
	*/

private:
	void testEditorName();
	void testDiagrams();
	void testElements();

	EditorInterface* mQrmcGeneratedPlugin;
	EditorInterface* mQrxcGeneratedPlugin;
};

