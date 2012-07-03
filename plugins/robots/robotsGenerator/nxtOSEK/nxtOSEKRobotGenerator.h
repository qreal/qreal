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
	qrRepo::RepoApi const * const api() const;

	QByteArray portValue1() const;
	QByteArray portValue2() const;
	QByteArray portValue3() const;
	QByteArray portValue4() const;

	qReal::ErrorReporterInterface &errorReporter() const;
	qReal::Id &previousElement();
	QList<QList<SmartLine> > &generatedStringSet();
	QMap<QString, QStack<int> > &elementToStringListNumbers();
	QStack<qReal::Id> &previousLoopElements();

private:
	friend class AbstractElementGenerator;

	friend class ElementGeneratorFactory;

	void addToGeneratedStringSetVariableInit();
	void insertCode(
			QString resultCode,
			QString resultInitCode,
			QString resultTerminateCode,
			QString curInitialNodeNumber);
	void deleteResidualLabels(QString projectName);
	void generateMakeFile(bool toGenerateIsEmpty, QString projectName, QString projectDir);
	void generateFilesForBalancer(QString projectDir);
	QString addTabAndEndOfLine(QList<SmartLine> lineList, QString resultCode);
	QString generateVariableString();

	QString resultOIL;
	QString resultString;
	int curTabNumber;
	QString taskTemplate;

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
