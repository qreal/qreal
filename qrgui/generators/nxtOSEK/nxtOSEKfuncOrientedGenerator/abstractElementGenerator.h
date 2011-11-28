#pragma once

#include <QList>
#include <QPair>

#include "../smartLine.h"
#include "../nxtOSEKfuncOrientedGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEKgenerator {

class AbstractElementGenerator {
public:	
	AbstractElementGenerator(NxtOSEKRobotGenerator* gen, Id elementId): mNxtGen(gen){
		if (mNxtGen->mApi->isLogicalElement(elementId))
			mElementId = elementId;
		else
			mElementId = mNxtGen->mApi->logicalId(elementId);
	}

	virtual ~AbstractElementGenerator() {
	}

	virtual QString nextMethodName();
	
	//changes mVariables, mGeneratedStrings, mAlreadyGeneratedElements, mErrorReporter in mNxtGen
	virtual void generate();

protected:
	QList<QString> portsToEngineNames(QString const &portsProperty);
	QString transformSign(QString const &sign);

	virtual void generateMethodBody() = 0;

	static int mElementCounter;

	NxtOSEKRobotGenerator* mNxtGen;
	Id mElementId;
};

}
}
}
