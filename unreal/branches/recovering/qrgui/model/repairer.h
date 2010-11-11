#pragma once

#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QFileDialog>
#include <QPushButton>


#include "message.h"
#include "../../qrrepo/repoApi.h"
#include "classes/modelTreeItem.h"
#include "../editorManager/editorManager.h"

class PatchSaveDialog;

namespace qReal {
	class Repairer : public QObject {
		Q_OBJECT
		public:
			Repairer(qrRepo::RepoApi &api, EditorManager const &editorManager);
			bool process(Id const target);

		public slots:
			void repair();
			void patchSave();
			void patchSave(QString savePath, QString patchPath);

		private:
			qrRepo::RepoApi &mApi;
			const EditorManager &mEditorManager;
			QMultiHash<QString, QLinkedList<Message> > mLogs;

			bool isCorrect(Id const target) const;
			void repairElements(Id const target);
			void readLog(QString const diagram);
			Id correctId(Id const target);

			static QDialog* repairDialog();
	};
}

class PatchSaveDialog : public QDialog {
	Q_OBJECT
	public:
		PatchSaveDialog(QString savePath, QString patchPath, qReal::Repairer* repairer);
		~PatchSaveDialog();
	private:
		bool checkPaths();

		qReal::Repairer* mRepairer;

		QGridLayout *layout;
		QLabel* commonLabel;
		QPushButton* runButton;

		QLabel* saveCaption;
		QLineEdit* savePath;
		QPushButton* saveBrowse;

		QLabel* patchCaption;
		QLineEdit* patchPath;
		QPushButton* patchBrowse;
	private slots:
		void run();
		void releaseMemory();
		void openPatchFile();
		void openSaveDirectory();
};
