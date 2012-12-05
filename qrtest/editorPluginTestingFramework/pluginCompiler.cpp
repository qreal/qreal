#include "pluginCompiler.h"

#include <QtCore/QProcess>
#include <QtCore/QDebug>

#include "../../qrkernel/settingsManager.h"

using namespace qReal;
using namespace editorPluginTestingFramework;

void PluginCompiler::compilePlugin(QString const &directoryToCodeToCompile)
{
	QProcess builder;
	builder.setWorkingDirectory(directoryToCodeToCompile);
	builder.start(SettingsManager::value("pathToQmake").toString());
	qDebug()  << "qmake";
	if ((builder.waitForFinished()) && (builder.exitCode() == 0)) {

		builder.start(SettingsManager::value("pathToMake").toString());

		bool finished = builder.waitForFinished(100000);
		qDebug()  << "make";
		if (finished && (builder.exitCode() == 0)) {
			qDebug()  << "make ok";
		}
	}
}
