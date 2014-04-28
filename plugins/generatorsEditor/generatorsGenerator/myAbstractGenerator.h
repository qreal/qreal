#pragma once

#include "../../../qrutils/generator/abstractGenerator.h"

namespace generators {
namespace generator {

class MyAbstractGenerator : public utils::AbstractGenerator
{
public:
	/** Constructor.
	  @param templateDirPath Path to a directory with generation template.
	  @param outputDirPath Path to a directory.
	  @param logicalModel Logical model reference.
	  @param errorReporter Object to return errors to.
	  */
	MyAbstractGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, QString const &pathToQReal
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			, QString const &metamodelLanguageName
			, QString const &languageName
			, QString const &toGeneratePropertyName
			, QString const &programNamePropertyName
			, QString const &generatorMetamodelName
			);

	virtual ~MyAbstractGenerator();

	/// Starts generation.
	void generate() = 0;

	QString modelFullName();
	QString metamodelName();
	QString languageName();
	QString generatorModelName();//i.e. pluginName
	QString normalizerMetamodelName();
	QString normalizerLanguageName();
	QString normalizerGeneratorModelName();//i.e. normalizerPluginName

protected :

	void generateTemplate(qReal::Id const &element);
	void saveTemplateUtils();

	QString mPathToQReal;
	QString const &mMetamodelName;//имя метамодели языков, для которых напсана эта модель генератора, по которой генерируется код
	QString const &mLanguageName; //имя конкретного языка метамодели, для которого напсана эта модель генератора
	QString const &mToGeneratePropertyName;//имя свойства главного узла языка, в котором указывается путь до папки генерации программы
	QString const &mProgramNamePropertyName; //имя свойства главного узла языка, в котором указывается имя генерируемой программы
	QString const &mGeneratorName;//настоящее имя модели генератора, по которой генерируется код

	QMap<QString, QString> mMarksCode;
	QString const mTemplateDirName;
};

}
}

