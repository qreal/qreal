#pragma once

#include <QList>
#include <QPair>

#include "../smartLine.h"
#include "../funcOrientedGenerator.h"

namespace robots {
namespace generator {

class AbstractElementGenerator {
public:	
	AbstractElementGenerator(FuncOrientedGenerator* gen, qReal::Id const &elementId): mNxtGen(gen){
		if (mNxtGen->mApi->isLogicalElement(elementId))
			mElementId = elementId;
		else
			mElementId = mNxtGen->mApi->logicalId(elementId);
	}

	virtual ~AbstractElementGenerator() {
	}

	//changes mElementCounter
	virtual QString nextMethodName();
	
	//changes mVariables, mGeneratedStrings, mAlreadyGeneratedElements, mErrorReporter in mNxtGen
	virtual void generate();

protected:
	QList<QString> portsToEngineNames(QString const &portsProperty) const;
	QString transformSign(QString const &sign) const;

	virtual void generateMethodBody() = 0;

	static int mElementCounter;

	FuncOrientedGenerator* mNxtGen;
	qReal::Id mElementId;
};

}
}
