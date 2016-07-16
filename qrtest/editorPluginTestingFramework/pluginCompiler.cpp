/* Copyright 2007-2015 QReal Research Group, Yurii Litvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "pluginCompiler.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QtCore/QDebug>

#include <qrkernel/exception/exception.h>
#include <qrrepo/repoApi.h>
#include <qrutils/nameNormalizer.h>

#include "defs.h"

using namespace editorPluginTestingFramework;

QFileInfo PluginCompiler::compilePlugin(const QString &fileWithMetamodel
		, const QString &directoryToCompile
		, const QString &pathToQmake
		, const QString &pathToMake
		, const QString &configurationParameter
		, MetamodelCompiler metamodelCompiler)
{
	qDebug() << "Compiling plugin from" << fileWithMetamodel << "in folder" << directoryToCompile
			<< ", configuration: " << configurationParameter;

	QProcess builder;
	const QStringList environment = QProcess::systemEnvironment();
	builder.setEnvironment(environment);
	QStringList qmakeArgs{"CONFIG+=" + configurationParameter};

	const QString pluginName = getPluginName(fileWithMetamodel);

	const QString proFileName = metamodelCompiler == MetamodelCompiler::qrxc
			? patchProFileForQrxc(pluginName, directoryToCompile)
			: patchProFileForQrmc(pluginName, directoryToCompile);

	builder.setWorkingDirectory(QFileInfo(proFileName).canonicalPath());

	qmakeArgs.append(QFileInfo(proFileName).fileName());

	qDebug() << "Running qmake with the following arguments:" << qmakeArgs << ", from" << builder.workingDirectory();

	builder.start(pathToQmake, qmakeArgs);

	if (builder.waitForFinished()) {
		if (builder.exitCode() == 0) {
			qDebug() << "qmake successful, running make";

			builder.start(pathToMake);

			if (!builder.waitForStarted()) {
				qDebug()  << "make failed to start, used this make command:" << pathToMake;
				throw qReal::Exception("make failed to start");
			}

			if (!builder.waitForFinished(100000)) {
				qDebug()  << "make failed to finish.";
				throw qReal::Exception("make failed to finish");
			}

			if (builder.exitCode() == 0) {
				qDebug()  << "make successful";
			} else {
				qDebug()  << "make failed with exit code" << builder.exitCode();
				throw qReal::Exception("make failed");
			}
		} else {
			qDebug() << "qmake failed with exit code" << builder.exitCode();
			throw qReal::Exception("qmake failed");
		}
	} else {
		qDebug() << "qmake failed to start. Used this qmake command:" << pathToQmake;
		throw qReal::Exception("qmake failed to start");
	}

	if (metamodelCompiler == MetamodelCompiler::qrmc) {
		/// @todo: implement it.
	} else {
		const QDir destdir(directoryToCompile + "/compiledPlugin/");
		for (const auto fileInfo : destdir.entryInfoList(QDir::Files)) {
			if (fileInfo.baseName().contains(pluginName)) {
				qDebug() << stringSeparator;
				return fileInfo;
			}
		}
	}

	throw qReal::Exception("Compilation result not found.");
}

QString PluginCompiler::getPluginName(const QString &fileWithMetamodel)
{
	/// @todo: here we make an assumption that metamodel is available and lies in application directory, which might
	///        not be the case.
	const qrRepo::RepoApi repoApi(fileWithMetamodel + ".qrs");
	QString pluginName = "";
	const qReal::IdList metamodels = repoApi.children(qReal::Id::rootId());

	for (const qReal::Id &key : metamodels) {
		if (repoApi.isLogicalElement(key)) {
			if (pluginName.isEmpty()) {
				pluginName = utils::NameNormalizer::normalize(repoApi.stringProperty(key, "name"), false);
			} else {
				throw qReal::Exception("More than one metamodel in a .qrs file, will not compile it.");
			}
		}
	}

	return pluginName;
}

QString PluginCompiler::patchProFileForQrxc(const QString &pluginName, const QString &directoryToCompile)
{
	qDebug() << "Using QRXC";
	const QString proFileName = directoryToCompile + "/" + pluginName + ".pro";
	QFile proFile(proFileName);
	proFile.open(QIODevice::ReadOnly);
	const QByteArray oldProFileContents = proFile.readAll();
	proFile.close();
	proFile.open(QIODevice::WriteOnly);
	proFile.write(oldProFileContents + "\nDESTDIR = compiledPlugin\n");
	proFile.close();
	return proFileName;
}

QString PluginCompiler::patchProFileForQrmc(const QString &pluginName, const QString &directoryToCompile)
{
	qDebug() << "Using QRMC";
	const QString proFileName = directoryToCompile + "/" + pluginName + "/" +  pluginName + ".pro";
	QFile proFile(proFileName);
	proFile.open(QIODevice::ReadOnly);
	const QByteArray oldProFileContents = proFile.readAll();
	proFile.close();
	proFile.open(QIODevice::WriteOnly);
	/// @todo: fix DESTDIR and debug QRMC.
	proFile.write(oldProFileContents + "\nDESTDIR = compiledPlugin\n");
	proFile.close();
	return proFileName;
}
