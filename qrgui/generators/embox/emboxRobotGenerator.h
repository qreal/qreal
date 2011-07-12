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

						virtual bool generate() = 0;
							//must change mEmboxGen->mGeneratedStringSet
						
						virtual QList< QPair<QString, qReal::Id> >
							cyclePrefixCode() = 0;
						
						virtual QList< QPair<QString, qReal::Id> >
							cyclePostfixCode() = 0;

					protected:
						virtual void createListsForIncomingConnections();
							//creates new lists in mGeneratedStringSet
							//and connects it with mElementId in mElementToStringListNumbers
							//if element have more than 1 incoming connection

						EmboxRobotGenerator *mEmboxGen;
						qReal::Id mElementId;
				};

				//for Beep, Engines
				class SimpleElementGenerator: public AbstractElementGenerator {
					public:
						explicit SimpleElementGenerator(EmboxRobotGenerator *emboxGen,
								qReal::Id elementId);

						virtual ~SimpleElementGenerator() {};

						virtual bool generate();
						
						virtual QList< QPair<QString, qReal::Id> >
							cyclePrefixCode();

						virtual QList< QPair<QString, qReal::Id> >
							cyclePostfixCode();

					protected:
						QList< QPair<QString, qReal::Id> > simpleCode();
				};

				class ElementGeneratorFactory {
					public:
						static AbstractElementGenerator* generator(EmboxRobotGenerator *emboxGen,
								qReal::Id elementId) {
							return new SimpleElementGenerator(emboxGen, elementId); 
							//TODO: добавить обработку для других классов
						}
				};

				qrRepo::RepoApi *mApi;
				bool mIsNeedToDeleteMApi;
				QString mDestinationPath;
				QList< QList< QPair<QString, qReal::Id> > > mGeneratedStringSet;
				QStack<qReal::Id> mPreviousCycleElements;
				qReal::Id mPreviousElement;

				QMap< QString, QStack<int> > mElementToStringListNumbers;
					//mapped element with lists in mGeneratedStringSet
					//QString in this case is qReal::Id string presentation

		};
	}
}
