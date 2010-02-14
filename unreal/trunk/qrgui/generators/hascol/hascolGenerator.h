#pragma once

#include <QtCore/QString>

#include "../../kernel/ids.h"

namespace qrRepo {
	class RepoApi;
}

namespace utils {
	class OutFile;
}

namespace qReal {

	namespace generators {

		class HascolGenerator
		{
		public:
			explicit HascolGenerator(qrRepo::RepoApi const &api);

			QString generate();
		private:
			void generateDiagram(Id const &id);
			void generateProcess(Id const &id, utils::OutFile &out);
			void generateProcessTypeBody(Id const &id, utils::OutFile &out);
			void generateProcessOperation(Id const &id, utils::OutFile &out);
			void generateLetBinding(Id const &id, utils::OutFile &out);
			void generateResource(Id const &id, bool first, utils::OutFile &out);
			void generateActivity(Id const &id, utils::OutFile &out);
			void generateHandler(Id const &id, utils::OutFile &out);
			void generateFunctor(Id const &id, utils::OutFile &out);
			void generateFunctorFormalParameter(Id const &id, utils::OutFile &out);
			void generatePortMap(Id const &id, utils::OutFile &out);
			Id generateChain(Id const &startNode, utils::OutFile &out);
			void generateActivityNode(Id const &id, utils::OutFile &out);
			Id generateIf(Id const &id, utils::OutFile &out);

			void addError(QString const &errorText);

			qrRepo::RepoApi const &mApi;
			QString mErrorText;

			QList<Id> mPortMappingDiagrams;
			QList<Id> mActivityDiagrams;
		};

	}
}
