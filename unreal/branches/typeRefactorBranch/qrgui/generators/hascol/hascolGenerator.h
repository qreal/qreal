#pragma once

#include <QtCore/QString>

#include "../../kernel/ids.h"
#include "../../mainwindow/errorReporter.h"

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

			gui::ErrorReporter &generate();
		private:
                        void generateDiagram(Id const &type);
                        void generateProcess(Id const &type, utils::OutFile &out);
                        void generateProcessTypeBody(Id const &type, utils::OutFile &out);
                        void generateProcessOperation(Id const &type, utils::OutFile &out);
                        void generateLetBinding(Id const &type, utils::OutFile &out);
                        void generateResource(Id const &type, bool first, utils::OutFile &out);
                        void generateActivity(Id const &type, utils::OutFile &out);
                        void generateHandler(Id const &type, utils::OutFile &out);
                        void generateFunctor(Id const &type, utils::OutFile &out);
                        void generateFunctorFormalParameter(Id const &type, utils::OutFile &out);
                        void generatePortMap(Id const &type, utils::OutFile &out);
                        Id generateChain(Id const &startNode, utils::OutFile &out);
                        void generateActivityNode(Id const &type, utils::OutFile &out);
                        Id generateIf(Id const &type, utils::OutFile &out);

			qrRepo::RepoApi const &mApi;
			gui::ErrorReporter mErrorReporter;

                        IdList mPortMappingDiagrams;
                        IdList mActivityDiagrams;
		};

	}
}
