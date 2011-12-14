#pragma once

#include "nxtOSEKgenerator.h"

#include <QString>
#include <QList>
#include <QMap>

#include "../../../qrkernel/ids.h"
#include "../../mainwindow/errorReporter.h"
#include "../../../qrrepo/repoApi.h"

#include "smartLine.h"

namespace qReal {
namespace generators {
namespace nxtOSEK {

class FuncOrientedGenerator: public NxtOSEKgenerator {
	friend class AbstractElementGenerator;
	
	friend class SimpleElementGenerator;
	
	friend class Beep_gen;
	friend class EnginesBackward_gen;
	friend class EnginesForward_gen;
	friend class EnginesStop_gen;
	friend class FinalNode_gen;
	friend class InitialBlock_gen;
	friend class InitialNode_gen;
	friend class NullificationEncoder_gen;
	friend class PlayTone_gen;
	friend class Timer_gen;
	friend class WaitForColor_gen;
	friend class WaitForColorIntensity_gen;
	friend class WaitForEncoder_gen;
	friend class WaitForSonarDistance_gen;
	friend class WaitForTouchSensor_gen;
	
	friend class Loop_gen;
	friend class IfBlock_gen;
	friend class Function_gen;

public:
	explicit FuncOrientedGenerator(qrRepo::RepoControlInterface &api, QString const &destinationPath = "");
	explicit FuncOrientedGenerator(QString const &pathToRepo, QString const &destinationPath = "");
	virtual ~FuncOrientedGenerator() {
	}
	
	gui::ErrorReporter &generate();

private:
	static QString smartLineListToString(QList<SmartLine> list, int startIndentSize);

	void prepareIdToMethodNameMap();
	void writeGeneratedCodeToFile(QString const &resultCode, QString const &initNodeProcedureName, int initialNodeNumber);

	QMap<QString, QString> mIdToMethodNameMap;

	QList<SmartLine> mVariables;
	QList<SmartLine> mGeneratedStrings;

	QSet<Id> mAlreadyGeneratedElements;
};

}
}
}
