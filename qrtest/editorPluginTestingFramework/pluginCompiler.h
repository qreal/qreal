#include <QtCore/QString>
#include "../../qrkernel/settingsManager.h"

class PluginCompiler
{
public:
	PluginCompiler();

	void compilePlugin(QString const directoryToCodeToCompile);
	// DESTDIR(куда генерить dll) должен быть параметром в прошнике, а в editorsCommon нужно проверять,
	//если он не установлен, использовать DESTDIR по умолчанию
};

