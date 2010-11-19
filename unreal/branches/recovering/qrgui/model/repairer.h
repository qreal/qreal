#pragma once

#include "message.h"
#include "repairerDialogs.h"

#include "classes/modelTreeItem.h"
#include "../editorManager/editorManager.h"
#include "../../qrrepo/repoApi.h"

class RepairerDialog;
class PatchSaveDialog;

namespace qReal {
	class Repairer : public QObject {
		Q_OBJECT
		public:
			Repairer(qrRepo::RepoApi &api, EditorManager const &editorManager);

			bool isBusy();
			bool checkIds(Id const target);
			void repairElements(Id const target);

			MainWindow* getMainWindow() const;

		public slots:
			void repair();
			void patchSave();
			void patchSave(QString savePath, QString patchPath);

		private:
			bool inProgress;

			qrRepo::RepoApi &mApi;
			const EditorManager &mEditorManager;

			Id correctId(Id const target);
			bool checkId(Id const target) const;

			void readLog(QString const diagram);

			RepairerDialog* mRepairerDialog;
			PatchSaveDialog* mPatchSaveDialog;

		private slots:
			void releaseRepairerDialog();
			void releasePatchSaveDialog();

		signals:
			void workFinished();
	};
}
