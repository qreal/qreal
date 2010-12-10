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
			void rememberSave(QString const savePath);

			MainWindow* getMainWindow() const;

		public slots:
			void repair();
			void patchSave();
			void patchSave(QString savePath, QString patchPath);

		private:
			bool inProgress;

			QString lastOpenedSave;

			qrRepo::RepoApi &mApi;
			const EditorManager &mEditorManager;

			Id correctId(Id const target);
			void replace(Id const from, Id const to) const;
			void changeType(Id const target, QString const &diagramName,
				QString const from, QString const to) const;

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
