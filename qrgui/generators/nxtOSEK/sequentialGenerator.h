#pragma once

#include "nxtOSEKgenerator.h"

#include <QString>
#include <QPair>
#include <QMap>
#include <QStack>

#include "../../../qrkernel/ids.h"
#include "../../mainwindow/errorReporter.h"
#include "../../../qrrepo/repoApi.h"

#include "smartLine.h"

namespace qReal {
namespace generators {
namespace nxtOSEK {

/*! Class for generate a nxtOSEK code from Robot Language Diagram.
 * Creates code that contains one main function with sequentially writed
 * strings mapped from diagram elements.
 */
class SequentialGenerator: public NxtOSEKgenerator {
public:
	explicit SequentialGenerator(qrRepo::RepoControlInterface &api, QString const &destinationPath = "");
	explicit SequentialGenerator(QString const &pathToRepo, QString const &destinationPath = "");

	virtual	~SequentialGenerator() {
	}

	//! main method that starts a code generation.
	gui::ErrorReporter &generate();

private:
	//! AbstractElementGenerator - robot diagram element generator abstraction.
	friend class AbstractElementGenerator;
	class AbstractElementGenerator {
	public:
		explicit AbstractElementGenerator(SequentialGenerator *emboxGen, qReal::Id const &elementId);

		virtual ~AbstractElementGenerator() {}

		virtual bool generate();

	protected:
		virtual QList<SmartLine> loopPrefixCode() = 0;

		virtual QList<SmartLine> loopPostfixCode() = 0;

		/*!
		 * Creates new lists in mGeneratedStringSet
		 * and connects it with mElementId in mElementToStringListNumbers
		 * in case element have more than 1 incoming connection.
		*/
		virtual void createListsForIncomingConnections();

		virtual bool preGenerationCheck() = 0;
		virtual bool nextElementsGeneration() = 0;

		SequentialGenerator *mNxtGen;
		qReal::Id mElementId;
	};

	//! Realization of AbstractElementGenerator for Beep, Engines etc.
	class SimpleElementGenerator: public AbstractElementGenerator {
	public:
		explicit SimpleElementGenerator(SequentialGenerator *emboxGen, qReal::Id elementId);

	protected:
		virtual QList<SmartLine> loopPrefixCode();

		virtual QList<SmartLine> loopPostfixCode();

		virtual bool preGenerationCheck();
		virtual bool nextElementsGeneration();

		virtual QList<SmartLine> simpleCode();

		QList<QString> portsToEngineNames(QString const &portsProperty);

	private:
		QString transformSign(QString const &inequalitySign);
	};

	//! Realization of AbstractElementGenerator for Function.
	class FunctionElementGenerator: public SimpleElementGenerator {
	public:
		explicit FunctionElementGenerator(SequentialGenerator *emboxGen, qReal::Id elementId);

	protected:
		virtual QList<SmartLine> simpleCode();
		void variableAnalysis(QByteArray const &);
	};

	//! Realization of AbstractElementGenerator for Loop.
	class LoopElementGenerator: public AbstractElementGenerator {
	public:
		explicit LoopElementGenerator(SequentialGenerator *emboxGen, qReal::Id elementId);

	protected:
		virtual QList<SmartLine> loopPrefixCode();

		virtual QList<SmartLine> loopPostfixCode();

		virtual bool preGenerationCheck();
		virtual bool nextElementsGeneration();
	};

	//! Realization of AbstractElementGenerator for If block.
	class IfElementGenerator : public AbstractElementGenerator {
	public:
		explicit IfElementGenerator(SequentialGenerator *emboxGen, qReal::Id elementId);

	protected:
		virtual QList<SmartLine> loopPrefixCode();

		virtual QList<SmartLine> loopPostfixCode();

		virtual bool preGenerationCheck();
		virtual bool nextElementsGeneration();

		bool generateBranch(int branchNumber);

		QPair<bool, qReal::Id> checkBranchForBackArrows(qReal::Id const &curElementId); //initial step of checking
		QPair<bool, qReal::Id> checkBranchForBackArrows(qReal::Id const &curElementId, qReal::IdList* checkedElements);
	};

	//! Element generator factory that returns for a diagram element ID a connected generator.
	friend class ElementGeneratorFactory;
	class ElementGeneratorFactory {
	public:
		static AbstractElementGenerator* generator(SequentialGenerator *emboxGen, qReal::Id elementId)
		{
			if (elementId.element() == "IfBlock")
				return new IfElementGenerator(emboxGen, elementId);
			if (elementId.element() == "Loop")
				return new LoopElementGenerator(emboxGen, elementId);
			if (elementId.element() == "Function")
				return new FunctionElementGenerator(emboxGen, elementId);

			return new SimpleElementGenerator(emboxGen, elementId);
		}
	};

	void addToGeneratedStringSetVariableInit();

	//! Set of already generated strings united for take a same critical places position (start of loop etc)
	QList< QList<SmartLine> > mGeneratedStringSet;

	//! Set of elements that have been already observed, but can create a regular loop (If blocks, Loop etc)
	QStack<qReal::Id> mPreviousLoopElements;
	qReal::Id mPreviousElement;

	/*!
	 * Mapped element with lists in mGeneratedStringSet
	 * QString in this case is qReal::Id string presentation.
	 */
	QMap< QString, QStack<int> > mElementToStringListNumbers;

	QList<SmartLine> mVariables;
	int mVariablePlaceInGenStrSet;
};
}
}
}
