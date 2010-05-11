#pragma once

#include <QtGui/QDialog>

#include "../../qrrepo/repoApi.h"

namespace Ui {
    class EditorGeneratorDialogUi;
}

namespace qReal {

    class EditorGeneratorDialog: public QDialog
    {
        Q_OBJECT

    public:
        EditorGeneratorDialog(qrRepo::RepoApi const &api);
        ~EditorGeneratorDialog();
    private Q_SLOTS:
        void createEditor();
    private:
        Ui::EditorGeneratorDialogUi *mUi;
        qrRepo::RepoApi const &mApi;
        IdList mEditors;
    };
}
