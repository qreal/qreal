#include <QString>
#include <QPair>
#include <QLinkedList>

#include "../../kernel/ids.h"
#include "../../../qrrepo/repoApi.h"

namespace qReal {
	namespace generators {
		class EmboxRobotGenerator {
			public:
				explicit EmboxRobotGenerator(qrRepo::RepoApi const &api,
						QString const &destinationPath = "");
				void generate();

			private:
				//QString generateByElement(qReal::Id node, qReal::Id previousNode = qReal::Id());

				QString mDestinationPath;
				qrRepo::RepoApi mApi;
		};

		class AbstractElementGenerator {
			public:
				explicit AbstractElementGenerator(qrRepo::RepoApi const & api,
						qReal::Id elementId);

				virtual ~AbstractElementGenerator() = 0;

				virtual QLinkedList<QPair<QString, qReal::Id>> generate() = 0;

			protected:
				qReal::Id mElementId;
				qrRepo::RepoApi mApi;
		};

		//for Beep, Engines
		class SimpleElementGenerator {
			public:
				explicit SimpleElementGenerator(qrRepo::RepoApi const & api,
						qReal::Id elementId);

				virtual ~SimpleElementGenerator() {};

				virtual QLinkedList<QPair<QString, qReal::Id>> generate();

			protected:
				QLinkedList<QString> simpleCode();
		};

		class ElementGeneratorFactory {
			public:
				static AbstractElementGenerator* generator(qReal::Id elementId);
		};
	}
}
