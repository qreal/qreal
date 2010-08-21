#pragma once
#include <QString>
#include <QTextStream>

#include "../../kernel/ids.h"
#include "../../mainwindow/errorReporter.h"
#include "../../editorManager/editorManager.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "call_scilab.h"
#include <QProcessEnvironment>

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
                gui::ErrorReporter generate(QString const &dirName);

            private:
                void VisitIntegralNode(Id const &id, QTextStream &out);
                void Visit(Id const &id, QTextStream &out);
                Id getLinkByPortName(Id const &id, QString const portName);
                QString getLinkOtherEntityValue(Id const &link, Id const &id);
                QString getPropertyString(Id const &id, QString const &propertyName);
                qrRepo::RepoApi const &mApi;
                qReal::EditorManager const &mEditorManager;
                gui::ErrorReporter mErrorReporter;
                void generateNativeCode(QString const &dirName);
            };

        }
    }
