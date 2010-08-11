#pragma once
#include <QString>

#include "../../kernel/ids.h"
#include "../../mainwindow/errorReporter.h"
#include "../../editorManager/editorManager.h"

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
                explicit ScilabGenerator(qrRepo::RepoApi const &api, qReal::EditorManager const &editor);
                gui::ErrorReporter generate();

            private:
                void VisitIntegralNode(Id const &id, utils::OutFile &out);
                void Visit(Id const &id);
                Id getLinkByPortName(Id const &id, QString const portName);
                QString getLinkOtherEntityValue(Id const &link, Id const &id, QString const defaultValue);

                qrRepo::RepoApi const &mApi;
                qReal::EditorManager const &mEditorManager;
                gui::ErrorReporter mErrorReporter;
            };

        }
    }
