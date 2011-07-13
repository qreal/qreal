#pragma once

#include <QString>
#include <QPair>
#include <QMap>
//#include <QLinkedList>
#include <QStack>

#include "../../kernel/ids.h"
#include "../../../qrrepo/repoApi.h"

namespace qReal {
	namespace generators {
		class EmboxRobotGenerator {
			public:
				explicit EmboxRobotGenerator(qrRepo::RepoApi *api,
						QString const &destinationPath = "");
				explicit EmboxRobotGenerator(QString const &pathToRepo,
						QString const &destinationPath = "");

				~EmboxRobotGenerator();

				void generate();

			private:
				friend class AbstractElementGenerator;
				class AbstractElementGenerator {
					public:
						explicit AbstractElementGenerator(EmboxRobotGenerator *emboxGen,
								qReal::Id elementId);

						virtual ~AbstractElementGenerator() {};

						virtual bool generate();
							//must change mEmboxGen->mGeneratedStringSet

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

						EmboxRobotGenerator *mEmboxGen;
						qReal::Id mElementId;
				};

				//for Beep, Engines etc
				class SimpleElementGenerator: public AbstractElementGenerator {
					public:
						explicit SimpleElementGenerator(EmboxRobotGenerator *emboxGen,
								qReal::Id elementId);

						virtual ~SimpleElementGenerator() {};
						
					protected:
						virtual QList< QPair<QString, qReal::Id> >
							loopPrefixCode();

						virtual QList< QPair<QString, qReal::Id> >
							loopPostfixCode();

						virtual bool preGenerationCheck();
						virtual bool nextElementsGeneration();

						QList< QPair<QString, qReal::Id> > simpleCode();
				};

				//for loops
				class LoopElementGenerator: public AbstractElementGenerator {
					public:
						explicit LoopElementGenerator(EmboxRobotGenerator *emboxGen,
								qReal::Id elementId);

						virtual ~LoopElementGenerator() {};
						
					protected:
						virtual QList< QPair<QString, qReal::Id> >
							loopPrefixCode();

						virtual QList< QPair<QString, qReal::Id> >
							loopPostfixCode();

						virtual bool preGenerationCheck();
						virtual bool nextElementsGeneration();
				};

				friend class ElementGeneratorFactory;
				class ElementGeneratorFactory {
					public:
						static AbstractElementGenerator* generator(EmboxRobotGenerator *emboxGen,
								qReal::Id elementId) {
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

		};
	}
}
