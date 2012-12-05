#pragma once

#include <QtCore/QString>
#include "../../qrkernel/settingsManager.h"

namespace editorPluginTestingFramework {

class PluginCompiler
{
public:
	void compilePlugin(QString const &directoryToCodeToCompile);
};

}

