#pragma once

#include <QtCore/QString>
#include "qrkernel/settingsManager.h"

namespace editorPluginTestingFramework {

class PluginCompiler
{
public:

	/// compiles code that is generated in qrmcLauncher and qrxcLauncher
	void compilePlugin(const QString &fileName,
		const QString &directoryToCodeToCompile
		, const QString &pathToQmake
		, const QString &pathToMake
		, const QString &configurationParameter
		);
};

}

