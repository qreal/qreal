#pragma once

#include "repairer.h"

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>

#include "constants.h"
#include "../mainwindow/mainwindow.h"

namespace qReal {
	class Repairer;
}

class PatchSaveDialog;

class RepairerDialog : public QDialog {
	Q_OBJECT
	public:
		RepairerDialog(qReal::Repairer* repairer, QString const savePath);
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
