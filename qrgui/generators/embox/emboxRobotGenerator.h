#include <QString>
#include <QPair>
#include <QLinkedList>

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
				//QString generateByElement(qReal::Id node, qReal::Id previousNode = qReal::Id());

				class AbstractElementGenerator {
					public:
						explicit AbstractElementGenerator(qrRepo::RepoApi *api,
								qReal::Id elementId);

						virtual ~AbstractElementGenerator() {};

						virtual QLinkedList< QPair<QString, qReal::Id> > generate() = 0;

					protected:
						qrRepo::RepoApi *mApi;
						qReal::Id mElementId;
				};

				//for Beep, Engines
				class SimpleElementGenerator: public AbstractElementGenerator {
					public:
						explicit SimpleElementGenerator(qrRepo::RepoApi *api,
								qReal::Id elementId);

						virtual ~SimpleElementGenerator() {};

						virtual QLinkedList< QPair<QString, qReal::Id> > generate();

					protected:
						QLinkedList< QPair<QString, qReal::Id> > simpleCode();
				};

				class ElementGeneratorFactory {
					public:
						static AbstractElementGenerator* generator(qrRepo::RepoApi *api,
								qReal::Id elementId) {
							return new SimpleElementGenerator(api, elementId); //TODO: добавить обработку для других классов
						}
				};

				qrRepo::RepoApi *mApi;
				bool mIsNeedToDeleteMApi;
				QString mDestinationPath;
		};
	}
}
