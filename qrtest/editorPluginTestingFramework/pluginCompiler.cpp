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
		, QString const &directoryToCodeToCompile
		, QString const &pathToQmake
		, QString const &pathToMake
		, QString const &configurationParameter
		)
{
	qDebug() << "STARTING PLUGIN COMPILING";
	QProcess builder;
	builder.setWorkingDirectory(directoryToCodeToCompile);
	//builder.setEnvironment();
	QStringList environment = QProcess::systemEnvironment();
	builder.setEnvironment(environment);
	QStringList qmakeArgs;
	qmakeArgs.append("CONFIG+=" + configurationParameter);
	//QFile *ololo;



	RepoApi *const mRepoApi = new RepoApi(fileName + ".qrs");

	QString pluginName = "";

	IdList const metamodels = mRepoApi->children(Id::rootId());

	foreach (Id const &key, metamodels) {
		if (mRepoApi->isLogicalElement(key)) {
			QString const &normalizedMetamodelName = NameNormalizer::normalize(mRepoApi->stringProperty(key, "name"), false);
			pluginName = normalizedMetamodelName;
			qDebug() << pluginName;
			break;
		}
	}



		QString qwertyuiop = directoryToCodeToCompile + "/" + pluginName + ".pro";
		qDebug() << qwertyuiop;
		//QFile data(directoryToCodeToCompile + pluginName + ".pro");
		QFile data(qwertyuiop);
		data.open(QIODevice::ReadOnly);
		QByteArray a = data.readAll();
		data.close();
		data.open(QIODevice::WriteOnly);

		data.write(a + "DESTDIR = plugins");


		//data.write();
		data.close();

//	QString = ololo.fil;

	qmakeArgs.append(pluginName + ".pro");

	builder.start(pathToQmake, qmakeArgs);
	bool qwerty = builder.waitForFinished();
	int asdf = builder.exitCode();
	QString ololo = builder.readAllStandardOutput();
	qDebug() << ololo;


	if (qwerty && builder.exitCode() == 0) {
		//builder.start(pathToMake);
		builder.start("mingw32-make");

		bool const finished = builder.waitForFinished(100000);
		qDebug()  << "make";
		QString ololo1 = builder.readAllStandardOutput();
		qDebug() << finished;
		qDebug() << ololo1;
		QString ololo2 = builder.readAllStandardError();
		qDebug() << "wwwwwwwwwwwwwwwwwwwww";
		qDebug() << ololo2;


		int tololo = builder.exitCode();

		qDebug() << tololo;


		if (finished && tololo == 0) {
			qDebug()  << "make ok";
		}

	}
}
