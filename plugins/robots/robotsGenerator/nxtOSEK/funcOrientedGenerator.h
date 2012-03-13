#pragma once

#include "nxtOSEKgenerator.h"

#include <QString>
#include <QList>
#include <QMap>

#include "../../../qrkernel/ids.h"
#include "../../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../../../qrrepo/repoApi.h"

#include "smartLine.h"

namespace robots {
namespace generator {

/*! Class for generate a nxtOSEK code from Robot Language Diagram.
 * Creates code that contains separate functions for each diagram node.
 */
class FuncOrientedGenerator: public NxtOSEKgenerator {
	friend class AbstractElementGenerator;
	
	friend class SimpleElementGenerator;
	
	friend class BeepGenerator;
	friend class EnginesBackwardGenerator;
	friend class EnginesForwardGenerator;
	friend class EnginesStopGenerator;
	friend class FinalNodeGenerator;
	friend class InitialBlockGenerator;
	friend class InitialNodeGenerator;
	friend class NullificationEncoderGenerator;
	friend class PlayToneGenerator;
	friend class TimerGenerator;
	friend class WaitForColorGenerator;
	friend class WaitForColorIntensityGenerator;
	friend class WaitForEncoderGenerator;
	friend class WaitForSonarDistanceGenerator;
	friend class WaitForTouchSensorGenerator;
	
	friend class LoopGenerator;
	friend class IfBlockGenerator;
	friend class FunctionGenerator;

public:
	FuncOrientedGenerator(
		qrRepo::RepoControlInterface &api,
		qReal::ErrorReporterInterface &errorReporter,
		QString const &destinationPath = ""
		);

	FuncOrientedGenerator(
		QString const &pathToRepo,
		qReal::ErrorReporterInterface &errorReporter,
		QString const &destinationPath = ""
		);

	virtual ~FuncOrientedGenerator() {
	}
	
	/// main method that starts code generation.
	qReal::ErrorReporterInterface &generate();

private:
	static QString smartLineListToString(QList<SmartLine> const &list, int startIndentSize);

	void prepareIdToMethodNameMap();
	void writeGeneratedCodeToFile(QString const &resultCode, QString const &initNodeProcedureName, int initialNodeNumber);

	QMap<QString, QString> mIdToMethodNameMap;

	QList<SmartLine> mVariables;
	QList<SmartLine> mGeneratedStrings;

	QSet<qReal::Id> mAlreadyGeneratedElements;
};

}
}
