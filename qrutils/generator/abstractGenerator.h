/* Copyright 2007-2015 QReal Research Group
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

#pragma once

#include <QtCore/QDir>

#include <qrutils/utilsDeclSpec.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace utils {
/// File name for generation utility templates. Searched in templates folder.
const QString utilsFileName = "utils.template";

/// Separator used in utility templates file to separate templates.
const QString utilsSeparator = "==========";

/// Base class for all generators, contains common methods and infrastructure to
/// generate one file.
class QRUTILS_EXPORT AbstractGenerator
{
public:
	/** Constructor.
	  @param templateDirPath Path to a directory with generation templates.
	  @param outputDirPath Path to a directory where file shall be generated.
	  @param logicalModel Logical model reference.
	  @param errorReporter Object to return errors to.
	  */
	AbstractGenerator(const QString &templateDirPath
			, const QString &outputDirPath
			, const qReal::LogicalModelAssistInterface &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

	virtual ~AbstractGenerator();

	/// Abstract method that starts generation.
	virtual void generate() = 0;

protected:
	/// Loads a template from file and puts the result into loadedTemplate, returns true if successful.
	bool loadTemplateFromFile(const QString &templateFileName, QString &loadedTemplate);

	/// Loads utility templates from utilsFileName, returns true if successful.
	bool loadUtilsTemplates();

	/// Saves the result of generation into output directory.
	void saveOutputFile(const QString &fileName, const QString &content);

	/// Generates code for C# property.
	QString generatePropertiesCode(const qReal::Id &element);

	/// Returns default value for a given C# type.
	static QString getDefaultValue(const QString &type);

	/// Logical repository API, the only thing that generator needs to know about model.
	const qrRepo::LogicalRepoApi &mApi;

	/// Error reporter.
	qReal::ErrorReporterInterface &mErrorReporter;

	/// A map with utility templates, maps template name to template.
	QMap<QString, QString> mTemplateUtils;

private:
	/// Loads utility templates from a file whose name in utilsFileName.
	bool loadUtilsFromFile();

	/// Alternate way to store utility templates, one template in one file in a separate dir.
	/// This method loads them and puts to mTemplateUtils.
	bool loadUtilsFromDir();

	/// Directory to place output files to.
	const QString mOutputDirPath;

	/// Returns a directory by given path or QDir() if there is no such path.
	QDir getDir(const QString &path);

	/// Path to a directory with templates.
	const QString mTemplateDirPath;
};
}
