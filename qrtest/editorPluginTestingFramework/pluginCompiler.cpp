#include "pluginCompiler.h"

#include <QtCore/QProcess>
#include <QtCore/QDebug>

#include "qrkernel/settingsManager.h"

using namespace qReal;
using namespace editorPluginTestingFramework;

void PluginCompiler::compilePlugin(
		QString const &directoryToCodeToCompile
		, QString const &pathToQmake
		, QString const &pathToMake
		, QString const &configurationParameter
		)
{
	qDebug() << "STARTING PLUGIN COMPILING";
	QProcess builder;
	builder.setWorkingDirectory(directoryToCodeToCompile);

	QStringList qmakeArgs;
	qmakeArgs.append("CONFIG+=" + configurationParameter);

	builder.start(pathToQmake, qmakeArgs);
	if (builder.waitForFinished() && builder.exitCode() == 0) {
		builder.start(pathToMake);

		bool const finished = builder.waitForFinished(100000);
		qDebug()  << "make";
		if (finished && builder.exitCode() == 0) {
			qDebug()  << "make ok";
		}
	}
}
