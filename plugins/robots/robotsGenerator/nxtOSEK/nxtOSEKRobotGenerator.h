#pragma once

#include <QString>
#include <QPair>
#include <QMap>
#include <QStack>

#include "../../../../qrkernel/ids.h"
#include "../../../../qrrepo/repoApi.h"
#include "../../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

#include "generators/abstractElementGenerator.h"
#include "smartLine.h"

namespace robots {
namespace generator {

/// Class for generate a nxtOSEK code from Robot Language Diagram.
class NxtOSEKRobotGenerator {
public:
	NxtOSEKRobotGenerator(qReal::Id const &diagram, qrRepo::RepoControlInterface &api, qReal::ErrorReporterInterface &errorReporter, QString const &destinationPath = "");
	NxtOSEKRobotGenerator(QString const &pathToRepo, qReal::ErrorReporterInterface &errorReporter, QString const &destinationPath = "");

	~NxtOSEKRobotGenerator();

	/// main method that starts a code generation.
	void generate();
	QList<SmartLine> &variables();
	QList<SmartLine> &initCode();
	QList<SmartLine> &terminateCode();
	qrRepo::RepoApi const * const api() const;

	QByteArray &portValue1();
	QByteArray &portValue2();
	QByteArray &portValue3();
	QByteArray &portValue4();

	qReal::ErrorReporterInterface &errorReporter();
	qReal::Id &previousElement();
	QList<QList<SmartLine> > &generatedStringSet();
	QMap<QString, QStack<int> > &elementToStringListNumbers();
	QStack<qReal::Id> &previousLoopElements();
	qReal::Id previousLoopElementsPop();
	int elementToStringListNumbersPop(QString const &key);
	void setGeneratedStringSet(int key, QList<SmartLine> const &list);
	bool mBalancerIsActivated;

private:
	void createProjectDir(QString const &projectDir);
	void insertCode(
			QString const &resultCode
			, QString const &resultInitCode
			, QString const &resultTerminateCode
			, QString const &curInitialNodeNumber);
	void deleteResidualLabels(QString const &projectName);
	void generateMakeFile(bool const &toGenerateIsEmpty, QString const &projectName, QString const &projectDir);
	void generateFilesForBalancer(QString const &projectDir);
	QString addTabAndEndOfLine(QList<SmartLine> const &lineList, QString resultCode);
	QString generateVariableString();
	void addResultCodeInCFile(int curInitialNodeNumber);
	void outputInCAndOilFile(QString const projectName, QString const projectDir, qReal::IdList toGenerate);
	void initializeFields(QString resultTaskTemplate, qReal::Id curInitialNode);

	/// Loads templates and creates output directory.
	void initializeGeneration(const QString projectDir);

	QString mResultOil;
	QString mResultString;
	int mCurTabNumber;
	QString mTaskTemplate;

	qrRepo::RepoApi *mApi;
	bool mIsNeedToDeleteMApi;
	QString mDestinationPath;

	//! Set of already generated strings united for take a same critical places position (start of loop etc)
	QList< QList<SmartLine> > mGeneratedStringSet;
	QList<SmartLine> mInitCode;
	QList<SmartLine> mTerminateCode;

	QByteArray mPortValue1;
	QByteArray mPortValue2;
	QByteArray mPortValue3;
	QByteArray mPortValue4;

	/// Set of elements that have been already observed, but can create a regular loop (If blocks, Loop etc)
	QStack<qReal::Id> mPreviousLoopElements;
	qReal::Id mPreviousElement;

	/**
	 * Mapped element with lists in mGeneratedStringSet
	 * QString in this case is qReal::Id string presentation.
	 */
	QMap< QString, QStack<int> > mElementToStringListNumbers;

	QList<SmartLine> mVariables;
	int mVariablePlaceInGenStrSet;

	qReal::ErrorReporterInterface &mErrorReporter;

	/// Id of a diagram to generate.
	qReal::Id mDiagram;
};

}
}
