#pragma once

#include <QString>
#include <QPair>
#include <QMap>
//#include <QLinkedList>
#include <QStack>

#include "../../kernel/ids.h"
#include "../../mainwindow/errorReporter.h"
#include "../../../qrrepo/repoApi.h"

namespace qReal {
namespace generators {
class NxtOSEKRobotGenerator {
public:
	explicit NxtOSEKRobotGenerator(qrRepo::RepoApi *api,
								   QString const &destinationPath = "");
	explicit NxtOSEKRobotGenerator(QString const &pathToRepo,
								   QString const &destinationPath = "");

	~NxtOSEKRobotGenerator();

	gui::ErrorReporter &generate();

private:
	friend class AbstractElementGenerator;
	class AbstractElementGenerator {
	public:
		explicit AbstractElementGenerator(NxtOSEKRobotGenerator *emboxGen,
										  qReal::Id elementId);

		virtual ~AbstractElementGenerator() {};

		virtual bool generate();
		//must change mNxtGen->mGeneratedStringSet

	protected:
		virtual QList< QPair<QString, qReal::Id> >
		loopPrefixCode() = 0;

		virtual QList< QPair<QString, qReal::Id> >
		loopPostfixCode() = 0;

		virtual void createListsForIncomingConnections();
		//creates new lists in mGeneratedStringSet
		//and connects it with mElementId in mElementToStringListNumbers
		//if element have more than 1 incoming connection

		virtual bool preGenerationCheck() = 0;
		virtual bool nextElementsGeneration() = 0;

		NxtOSEKRobotGenerator *mNxtGen;
		qReal::Id mElementId;
	};

	//for Beep, Engines etc
	class SimpleElementGenerator: public AbstractElementGenerator {
	public:
		explicit SimpleElementGenerator(NxtOSEKRobotGenerator *emboxGen,
										qReal::Id elementId);

	protected:
		virtual QList< QPair<QString, qReal::Id> >
		loopPrefixCode();

		virtual QList< QPair<QString, qReal::Id> >
		loopPostfixCode();

		virtual bool preGenerationCheck();
		virtual bool nextElementsGeneration();

		QList< QPair<QString, qReal::Id> > simpleCode();

		QList<QString> portsToEngineNames(QString const &portsProperty);
	};

	//for loops
	class LoopElementGenerator: public AbstractElementGenerator {
	public:
		explicit LoopElementGenerator(NxtOSEKRobotGenerator *emboxGen,
									  qReal::Id elementId);

	protected:
		virtual QList< QPair<QString, qReal::Id> >
		loopPrefixCode();

		virtual QList< QPair<QString, qReal::Id> >
		loopPostfixCode();

		virtual bool preGenerationCheck();
		virtual bool nextElementsGeneration();
	};

	//for if blocks
	class IfElementGenerator : public AbstractElementGenerator {
	public:
		explicit IfElementGenerator(NxtOSEKRobotGenerator *emboxGen,
									qReal::Id elementId);

	protected:
		virtual QList< QPair<QString, qReal::Id> >
		loopPrefixCode();

		virtual QList< QPair<QString, qReal::Id> >
		loopPostfixCode();

		virtual bool preGenerationCheck();
		virtual bool nextElementsGeneration();

		bool generateBranch(int branchNumber);
	};

	friend class ElementGeneratorFactory;
	class ElementGeneratorFactory {
	public:
		static AbstractElementGenerator* generator(NxtOSEKRobotGenerator *emboxGen,
												   qReal::Id elementId) {
			if (elementId.element() == "IfBlock")
				return new IfElementGenerator(emboxGen, elementId);
			if (elementId.element() == "Loop")
				return new LoopElementGenerator(emboxGen, elementId);

			return new SimpleElementGenerator(emboxGen, elementId);
			//TODO: добавить обработку для других классов
		}
	};

	qrRepo::RepoApi *mApi;
	bool mIsNeedToDeleteMApi;
	QString mDestinationPath;
	QList< QList< QPair<QString, qReal::Id> > > mGeneratedStringSet;
	QStack<qReal::Id> mPreviousLoopElements;
	qReal::Id mPreviousElement;

	QMap< QString, QStack<int> > mElementToStringListNumbers;
	//mapped element with lists in mGeneratedStringSet
	//QString in this case is qReal::Id string presentation

	gui::ErrorReporter mErrorReporter;
};
}
}
