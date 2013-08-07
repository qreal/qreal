#pragma once

#include <QtCore/QMap>
#include <QtCore/QStack>

#include "../../../../qrkernel/ids.h"
#include "../smartLine.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

/// Generates code for one specified diagram
class ControlFlowGenerator
{
public:
	ControlFlowGenerator(NxtOSEKRobotGenerator * const mainGenerator
			, qReal::Id const &diagram);

	/// Starts control flow generation process
	bool generate();

	/// Returns generation process result
	QList<SmartLine> &generatedCode();

	qReal::Id &previousElement();
	QList<QList<SmartLine> > &generatedStringSet();
	QMap<QString, QStack<int> > &elementToStringListNumbers();
	QStack<qReal::Id> &previousLoopElements();
	qReal::Id previousLoopElementsPop();
	int elementToStringListNumbersPop(QString const &key);
	void setGeneratedStringSet(int key, QList<SmartLine> const &list);

private:
	NxtOSEKRobotGenerator *mMainGenerator; // Doesn`t take ownership
	qReal::Id mDiagram;

	QList<SmartLine> mGeneratedCode;

	/// Mapped element with lists in mGeneratedStringSet
	/// QString in this case is qReal::Id string presentation.
	QMap<QString, QStack<int> > mElementToStringListNumbers;

	/// Set of already generated strings united for take a same critical places position (start of loop etc)
	QList< QList<SmartLine> > mGeneratedStringSet;

	/// Set of elements that have been already observed, but can create a regular loop (If blocks, Loop etc)
	QStack<qReal::Id> mPreviousLoopElements;
	qReal::Id mPreviousElement;
};

}
}
