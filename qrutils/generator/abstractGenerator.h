#pragma once

#include <QtCore/QDir>

#include "../utilsDeclSpec.h"
#include "../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace utils {
/// File name for generation utility templates. Searched in templates folder.
QString const utilsFileName = "utils.template";

/// Separator used in utility templates file to separate templates.
QString const utilsSeparator = "==========";

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
	AbstractGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

	virtual ~AbstractGenerator();

	/// Abstract method that starts generation.
	virtual void generate() = 0;

protected:
	/// Loads a template from file and puts the result into loadedTemplate, returns true if successful.
	bool loadTemplateFromFile(QString const &templateFileName, QString &loadedTemplate);

	/// Loads utility templates from utilsFileName, returns true if successful.
	bool loadUtilsTemplates();

	/// Saves the result of generation into output directory.
	void saveOutputFile(QString const &fileName, QString const &content);

	/// Generates code for C# property.
	QString generatePropertiesCode(qReal::Id const &element);

	/// Returns default value for a given C# type.
	static QString getDefaultValue(QString const &type);

	/// Logical repository API, the only thing that generator needs to know about model.
	qrRepo::LogicalRepoApi const &mApi;

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
	QString const mOutputDirPath;

	/// Returns a directory by given path or QDir() if there is no such path.
	QDir getDir(QString const &path);

	/// Path to a directory with templates.
	QString const mTemplateDirPath;
};
}
