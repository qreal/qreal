#pragma once

#include "../../qrgui/pluginManager/editorManagerInterface.h"
#include "../../qrgui/pluginManager/interpreterEditorManager.h"
#include "../../qrgui/pluginManager/editorManager.h"
#include "../../qrgui/editorPluginInterface/editorInterface.h"

#include "unifiedStringGenerator.h"

namespace editorPluginTestingFramework {

class MethodsTester
{

protected:

	virtual UnifiedStringGenerator * initGeneratorWithFirstInterface(UnifiedStringGenerator const &generator) const = 0;
	virtual UnifiedStringGenerator * initGeneratorWithSecondInterface(UnifiedStringGenerator const &generator) const = 0;

	/// tests given method if it exists in list of methods to be tested
	QPair<QString, QPair<QString, QString> > testMethodIfExistsInList(
			UnifiedStringGenerator const &stringGenerator
			, QString const &method
			);

	/// fills list of methods to test with methods from given .txt-file
	void fillMethodsToTestList(QString const &fileName);

private:

	/// generated method name and results for two interfaces, returns triplet of strings
	QPair<QString, QPair<QString, QString> > generateOutputForOneMethod(UnifiedStringGenerator const &stringGenerator);

	/// list of methods to be tested, is filled from files
	QStringList mMethodsToTest;
};

}
