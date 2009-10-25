#pragma once

#include <QtGui/QDialog>

#include "../client/repoApi.h"

namespace Ui {
	class EditorGeneratorDialogUi;
}

namespace qReal {

	class EditorGeneratorDialog: public QDialog
	{
		Q_OBJECT

	public:
		EditorGeneratorDialog(client::RepoApi const &api);
		~EditorGeneratorDialog();
	private slots:
		void createEditor();
	private:
		Ui::EditorGeneratorDialogUi *mUi;
		client::RepoApi const &mApi;
		IdList mEditors;
	};
}
