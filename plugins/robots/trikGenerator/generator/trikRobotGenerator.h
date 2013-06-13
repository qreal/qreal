#pragma once

#include <QtCore/QString>
#include <QtCore/QPair>
#include <QtCore/QMap>
#include <QtCore/QStack>

#include "../../../../qrkernel/ids.h"
#include "../../../../qrrepo/repoApi.h"
#include "../../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../../robotsInterpreter/sensorConstants.h"

#include "smartLine.h"
#include "generators/variables.h"

namespace robots {
namespace trikGenerator {

/// Class for generation of a QtScript code for TRIK from Robot Language Diagram.
class TrikRobotGenerator
{
public:
	TrikRobotGenerator(
			qReal::Id const &diagram
			, qrRepo::RepoControlInterface &api
			, qReal::ErrorReporterInterface &errorReporter
			);

	~TrikRobotGenerator();

	/// main method that starts a code generation.
	void generate(QString const &programName);

	Variables &variables();

	qrRepo::RepoApi const *api() const;

	qReal::interpreters::robots::sensorType::SensorTypeEnum portValue(int port) const;

	qReal::ErrorReporterInterface &errorReporter();
	qReal::Id &previousElement();
	QList<QList<SmartLine> > &generatedStringSet();
	QMap<QString, QStack<int> > &elementToStringListNumbers();
	QStack<qReal::Id> &previousLoopElements();
	qReal::Id previousLoopElementsPop();
	int elementToStringListNumbersPop(QString const &key);
	void setGeneratedStringSet(int key, QList<SmartLine> const &list);

	/// Returns string property treated as expression and casts it to int if nessesary
	QString intExpression(qReal::Id const &id, QString const &propertyName) const;

private:
	void insertCode(QString const &resultCode);

	void deleteResidualLabels();
	QString addTabAndEndOfLine(QList<SmartLine> const &lineList, QString resultCode);
	void addResultCode();
	void outputQtsFile(QString const fileName);
	void initializeFields(qReal::Id curInitialNode);

	/// Loads templates.
	void initializeGeneration();

	QString mResultString;
	int mCurTabNumber;

	qrRepo::RepoApi *mApi;  // Does not have ownership.

	/// Set of already generated strings united for take a same critical places position (start of loop etc)
	QList< QList<SmartLine> > mGeneratedStringSet;

	/// Set of elements that have been already observed, but can create a regular loop (If blocks, Loop etc)
	QStack<qReal::Id> mPreviousLoopElements;

	qReal::Id mPreviousElement;

	/// Mapped element with lists in mGeneratedStringSet
	/// QString in this case is qReal::Id string presentation.
	QMap<QString, QStack<int> > mElementToStringListNumbers;

	Variables mVariables;

	qReal::ErrorReporterInterface &mErrorReporter;

	/// Id of a diagram to generate.
	qReal::Id mDiagram;
};

}
}
