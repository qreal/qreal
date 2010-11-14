#pragma once

#include <QLabel>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QFileDialog>
#include <QPushButton>

//todo: вынести в отдельные файлы графические классы

#include "message.h"
#include "../../qrrepo/repoApi.h"
#include "classes/modelTreeItem.h"
#include "../editorManager/editorManager.h"

class RepairerDialog;
class PatchSaveDialog;

namespace qReal {
	class Repairer : public QObject {
		Q_OBJECT
		public:
			Repairer(qrRepo::RepoApi &api, EditorManager const &editorManager);
			bool process(Id const target);
			void repairElements(Id const target);

			MainWindow* getMainWindow() const;
		public slots:
			void repair();
			void patchSave();
			void patchSave(QString savePath, QString patchPath);

		private:

			/** Working **/

			qrRepo::RepoApi &mApi;
			const EditorManager &mEditorManager;
			QMultiHash<QString, QLinkedList<Message> > mLogs;

			Id correctId(Id const target);
			bool isCorrect(Id const target) const;

			void readLog(QString const diagram);

			/** Graphics **/

			RepairerDialog* mRepairerDialog;
			PatchSaveDialog* mPatchSaveDialog;

		private slots:

			void releaseRepairerDialog();
			void releasePatchSaveDialog();
	};
}

class RepairerDialog : public QDialog {
	Q_OBJECT
	public:
		RepairerDialog(qReal::Repairer* repairer);
		~RepairerDialog();

	private:
		qReal::Repairer* mRepairer;

		QGridLayout *layout;
		QLabel* mCommonText;
		QPushButton* mRunButton;

		QLabel* mAutorepairText;

		QLabel* mPatchSaveCaption;
		QLabel* mAutorepairCaption;
		QLabel* mGenerateEditorCaption;

		QWidget* mAutorepairDialog;
		QWidget* mGenerateEditorDialog;
		PatchSaveDialog* mPatchSaveDialog;

		QCheckBox* mPatchSaveSwitcher;
		QCheckBox* mAutorepairSwitcher;
		QCheckBox* mGenerateEditorSwitcher;

		QLabel* mGenerateEditorText;
		QLineEdit* mGenerateEditorPath;
		QLabel* mGenerateEditorPathCaption;
		QLabel* mGenerateEditorPathError;
		QPushButton* mGenerateEditorPathBrowse;

		QGridLayout* mAutorepairLayout;
		QGridLayout* mGenerateEditorLayout;

	private slots:
		void run();
		void openEditorXML();
		bool checkEditorPath();
};

class PatchSaveDialog : public QWidget {
	Q_OBJECT
	public:
		PatchSaveDialog(QString savePath, QString patchPath, qReal::Repairer* repairer);
		~PatchSaveDialog();

		QPushButton* getRunButton() const;

	public slots:
		void run();

	private:
		qReal::Repairer* mRepairer;

		QGridLayout *layout;
		QLabel* commonLabel;
		QPushButton* runButton;

		QLabel* saveError;
		QLabel* saveCaption;
		QLineEdit* savePath;
		QPushButton* saveBrowse;

		QLabel* patchError;
		QLabel* patchCaption;
		QLineEdit* patchPath;
		QPushButton* patchBrowse;

	private slots:
		bool checkSavePath();
		bool checkPatchPath();

		void releaseMemory();
		void openPatchFile();
		void openSaveDirectory();
};
