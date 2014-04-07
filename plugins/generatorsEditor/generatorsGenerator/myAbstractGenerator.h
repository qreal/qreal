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
			, QString const &generatorMetamodelName
			);

	virtual ~MyAbstractGenerator();

	/// Starts generation.
	void generate() = 0;

	QString generatorModelFullName();
	QString generatorModelName();
	QString generatorGeneratorModelName();//i.e. pluginName
	QString generatorNormalizerModelName(); //i.e. fileBase
	QString generatorNormalizerGeneratorModelName();//i.e. normalizerPluginName
	QString generatorModelId();//i.e. pluginId

private :

	QString mPathToQReal;
	QString const &mMetamodelName;//имя метамодели языков, для которых напсана эта модель генератора, по которой генерируется код
	QString const &mGeneratorName;//настоящее имя модели генератора, по которой генерируется код

};

}
}

