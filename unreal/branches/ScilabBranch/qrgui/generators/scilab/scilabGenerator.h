#pragma once
#include <QString>

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
            class ScilabGenerator
            {
            public:
                explicit ScilabGenerator(qrRepo::RepoApi const &api);    
                gui::ErrorReporter generate();

            private:
                void VisitIntegralNode(Id const &id, utils::OutFile &out);
                void Visit(Id const &id);

                qrRepo::RepoApi const &mApi;
               // utils::OutFile &outFile;
            };

        }
    }
