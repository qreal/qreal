#pragma once

#include <QtCore/QString>
#include <QtCore/QPair>

#include "../../../../qrkernel/ids.h"
#include "../../../../qrrepo/repoApi.h"
#include "../../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../../robotsInterpreter/sensorConstants.h"

#include "generators/abstractElementGenerator.h"
#include "smartLine.h"

#include "generators/variables.h"
#include "generators/imageGenerator.h"
#include "generators/subprogramsGenerator.h"
#include "generators/controlFlowGenerator.h"

namespace robots {
namespace generator {

/// Class for generate a nxtOSEK code from Robot Language Diagram.
class NxtOSEKRobotGenerator
{
public:
	NxtOSEKRobotGenerator(qReal::Id const &diagram, qrRepo::RepoControlInterface &api, qReal::ErrorReporterInterface &errorReporter, QString const &destinationPath = "");
	NxtOSEKRobotGenerator(QString const &pathToRepo, qReal::ErrorReporterInterface &errorReporter, QString const &destinationPath = "");

	~NxtOSEKRobotGenerator();

	/// main method that starts a code generation.
	void generate();

	Variables &variables();
	ImageGenerator &imageGenerator();
	SubprogramsGenerator &subprogramsGenerator();

	/// Returns control flow generator that currently perfroms generation process
	ControlFlowGenerator *currentGenerator();

	/// Must be called each time when another control flow generator starts work
	void beforeSubprogramGeneration(ControlFlowGenerator * const generator);

	/// Returns false if currently some subprogram`s code is being generated and
	/// true otherwise (primary generation of main task code)
	bool areWeGeneratingMainTask() const;

	QList<SmartLine_old> &initCode();
	QList<SmartLine_old> &terminateCode();
	QList<SmartLine_old> &isrHooksCode();

	qrRepo::RepoApi const *api() const;
	qReal::ErrorReporterInterface &errorReporter();

	qReal::interpreters::robots::enums::sensorType::SensorTypeEnum portValue(int port) const;
	qReal::interpreters::robots::enums::sensorType::SensorTypeEnum portValue1() const;
	qReal::interpreters::robots::enums::sensorType::SensorTypeEnum portValue2() const;
	qReal::interpreters::robots::enums::sensorType::SensorTypeEnum portValue3() const;
	qReal::interpreters::robots::enums::sensorType::SensorTypeEnum portValue4() const;

	/// Returns string property treated as expression and casts it to int if nessesary
	QString intExpression(qReal::Id const &id, QString const &propertyName) const;

	// TODO: remove it or move into separate generator (like with images)
	void activateBalancer();

private:
	void createProjectDir(QString const &projectDir);
	void insertCode(
			QString const &resultCode
			, QString const &resultInitCode
			, QString const &resultTerminateCode
			, QString const &resultIsrHooksCode
			, QString const &subprogramsCode
			, QString const &curInitialNodeNumber);
	void deleteResidualLabels(QString const &projectName);
	void generateMakeFile(QString const &projectName, QString const &projectDir);
	void generateFilesForBalancer(QString const &projectDir);
	QString toString(QList<SmartLine_old> const &lineList);
	void addResultCodeInCFile(int curInitialNodeNumber);
	void outputInCAndOilFile(QString const &projectName, QString const &projectDir);
	void initializeFields(QString const &resultTaskTemplate);

	/// Loads templates and creates output directory.
	void initializeGeneration(QString const &projectDir);

	QString mResultOil;
	QString mResultString;
	int mCurTabNumber;
	QString mTaskTemplate;

	qrRepo::RepoApi *mApi;
	bool mIsNeedToDeleteMApi;
	QString mDestinationPath;

	QList<SmartLine_old> mInitCode;
	QList<SmartLine_old> mTerminateCode;
	QList<SmartLine_old> mIsrHooksCode;

	Variables mVariables;
	int mVariablePlaceInGenStrSet;

	qReal::ErrorReporterInterface &mErrorReporter;

	/// Id of a diagram to generate.
	qReal::Id mDiagram;

	ImageGenerator mImageGenerator;
	SubprogramsGenerator mSubprogramsGenerator;
	ControlFlowGenerator mMainControlFlowGenerator;
	ControlFlowGenerator *mCurrentGenerator; // Doesn`t take ownership

	bool mBalancerIsActivated;
};

}
}
