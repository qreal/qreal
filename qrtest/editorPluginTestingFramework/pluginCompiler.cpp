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
	QStringList environment = QProcess::systemEnvironment();
	builder.setEnvironment(environment);
	QStringList qmakeArgs;
	qmakeArgs.append("CONFIG+=" + configurationParameter);

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


	if (directoryToCodeToCompile.contains("qrmc"))
	{
		QString file1 = "./" + pluginName + "/" +  pluginName + ".pro";
		QFile data1(file1);
		data1.open(QIODevice::ReadOnly);
		QByteArray b = data1.readAll();
		data1.close();
		data1.open(QIODevice::WriteOnly);

		data1.write(b + "DESTDIR = ../plugins/editors/qrtest/qrmc/plugins");

		builder.setWorkingDirectory("./" + pluginName + "/");
		data1.close();

	} else {


	QString file = directoryToCodeToCompile + "/" + pluginName + ".pro";
	qDebug() << file;
	QFile data(file);
	//data.open(QIODevice::Append);
	data.open(QIODevice::ReadOnly);

	QByteArray a = data.readAll();
	data.close();
	data.open(QIODevice::WriteOnly);

	data.write(a + "DESTDIR = plugins");


		//data.write();
	data.close();
	builder.setWorkingDirectory(directoryToCodeToCompile);

	}

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
