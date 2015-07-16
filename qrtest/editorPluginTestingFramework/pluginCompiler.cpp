#include "pluginCompiler.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>


#include <QtCore/QProcess>
#include <QtCore/QDebug>

#include "qrkernel/settingsManager.h"

#include "qrrepo/repoApi.h"
#include "qrutils/nameNormalizer.h"

using namespace qrRepo;
using namespace qReal;
using namespace editorPluginTestingFramework;
using namespace utils;

void PluginCompiler::compilePlugin(const QString &fileName
		, const QString &directoryToCodeToCompile
		, const QString &pathToQmake
		, const QString &pathToMake
		, const QString &configurationParameter
		)
{
	qDebug() << "STARTING PLUGIN COMPILING";
	QProcess builder;
	QStringList environment = QProcess::systemEnvironment();
	builder.setEnvironment(environment);
	QStringList qmakeArgs;
	qmakeArgs.append("CONFIG+=" + configurationParameter);

	const RepoApi *mRepoApi = new RepoApi(fileName + ".qrs");
	QString pluginName = "";
	const IdList metamodels = mRepoApi->children(Id::rootId());

	for (Id const &key : metamodels) {
		if (mRepoApi->isLogicalElement(key)) {
			const QString &normalizedMetamodelName = NameNormalizer::normalize(mRepoApi->stringProperty(key, "name"), false);
			pluginName = normalizedMetamodelName;
			break;
		}
	}

	if (directoryToCodeToCompile.contains("qrmc"))
	{
		QString fileForQrmc = "./" + pluginName + "/" +  pluginName + ".pro";
		QFile dataQrmc(fileForQrmc);
		dataQrmc.open(QIODevice::ReadOnly);
		QByteArray saveDataQrmc = dataQrmc.readAll();
		dataQrmc.close();
		dataQrmc.open(QIODevice::WriteOnly);
		dataQrmc.write(saveDataQrmc + "DESTDIR = ../plugins/editors/qrtest/qrmc/plugins");
		builder.setWorkingDirectory("./" + pluginName + "/");
		dataQrmc.close();
	} else {
		QString fileForQrxc = directoryToCodeToCompile + "/" + pluginName + ".pro";
		QFile dataQrxc(fileForQrxc);
		dataQrxc.open(QIODevice::ReadOnly);
		QByteArray saveDataQrxc = dataQrxc.readAll();
		dataQrxc.close();
		dataQrxc.open(QIODevice::WriteOnly);
		dataQrxc.write(saveDataQrxc + "DESTDIR = plugins");
		dataQrxc.close();
		builder.setWorkingDirectory(directoryToCodeToCompile);
	}

	qmakeArgs.append(pluginName + ".pro");
	builder.start(pathToQmake, qmakeArgs);

	if (builder.waitForFinished() && builder.exitCode() == 0) {
		builder.start("mingw32-make");

		bool const finished = builder.waitForFinished(100000);
		qDebug()  << "make";

		if (finished && builder.exitCode() == 0) {
			qDebug()  << "make ok";
		}
	}
}
