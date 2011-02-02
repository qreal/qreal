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
                        void generateDiagram(NewType const &type);
                        void generateProcess(NewType const &type, utils::OutFile &out);
                        void generateProcessTypeBody(NewType const &type, utils::OutFile &out);
                        void generateProcessOperation(NewType const &type, utils::OutFile &out);
                        void generateLetBinding(NewType const &type, utils::OutFile &out);
                        void generateResource(NewType const &type, bool first, utils::OutFile &out);
                        void generateActivity(NewType const &type, utils::OutFile &out);
                        void generateHandler(NewType const &type, utils::OutFile &out);
                        void generateFunctor(NewType const &type, utils::OutFile &out);
                        void generateFunctorFormalParameter(NewType const &type, utils::OutFile &out);
                        void generatePortMap(NewType const &type, utils::OutFile &out);
                        NewType generateChain(NewType const &startNode, utils::OutFile &out);
                        void generateActivityNode(NewType const &type, utils::OutFile &out);
                        NewType generateIf(NewType const &type, utils::OutFile &out);

			qrRepo::RepoApi const &mApi;
			gui::ErrorReporter mErrorReporter;

                        TypeList mPortMappingDiagrams;
                        TypeList mActivityDiagrams;
		};

	}
}
