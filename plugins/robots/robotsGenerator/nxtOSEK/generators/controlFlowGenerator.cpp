#include "controlFlowGenerator.h"

#include "../nxtOSEKRobotGenerator.h"
#include "../elementGeneratorFactory.h"

using namespace robots::generator;

ControlFlowGenerator::ControlFlowGenerator(NxtOSEKRobotGenerator * const mainGenerator
		, qReal::Id const &diagram)
	: mMainGenerator(mainGenerator)
	, mDiagram(diagram)
{
	mGeneratedStringSet.append(QList<SmartLine>()); //first list for variable initialization
}

bool ControlFlowGenerator::generate()
{
	bool generationOccured = false;
	qReal::IdList const toGenerate(mMainGenerator->api()->elementsByType("InitialNode"));

	foreach (qReal::Id const &initialNode, toGenerate) {
		if (!mMainGenerator->api()->isGraphicalElement(initialNode)
				|| mMainGenerator->api()->parent(initialNode) != mDiagram) {
			continue;
		}
		mPreviousElement = initialNode;

		generationOccured = true;

		AbstractElementGenerator * const gen = ElementGeneratorFactory::generator(mMainGenerator
				, initialNode, *mMainGenerator->api());
		gen->generate();
		delete gen;
		break;
	}

	if (!generationOccured) {
		mMainGenerator->errorReporter().addError(QObject::tr("There is nothing "\
				"to generate, diagram doesn't have Initial Node"), mDiagram);
	}
	return generationOccured;
}

qReal::Id &ControlFlowGenerator::previousElement()
{
	return mPreviousElement;
}

QList<QList<SmartLine> > &ControlFlowGenerator::generatedStringSet()
{
	return mGeneratedStringSet;
}

void ControlFlowGenerator::setGeneratedStringSet(int key, QList<SmartLine> const &list)
{
	mGeneratedStringSet[key] = list;
}

QMap<QString, QStack<int> > &ControlFlowGenerator::elementToStringListNumbers()
{
	return mElementToStringListNumbers;
}

int ControlFlowGenerator::elementToStringListNumbersPop(QString const &key)
{
	return mElementToStringListNumbers[key].pop();
}

QStack<qReal::Id> &ControlFlowGenerator::previousLoopElements()
{
	return mPreviousLoopElements;
}

qReal::Id ControlFlowGenerator::previousLoopElementsPop()
{
	return mPreviousLoopElements.pop();
}
