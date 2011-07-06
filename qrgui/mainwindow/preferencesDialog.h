#pragma once

#include <QDialog>
#include <QSlider>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>

#include "../../interpreters/robots/sensorConstants.h"

using namespace qReal::interpreters::robots;

namespace Ui {
	class PreferencesDialog;
	class BehaviourForm;
	class CompilerForm;
	class DebuggerForm;
	class EditorForm;
	class MiscellaniousForm;
	class RobotSettingsForm;
}

class PreferencesDialog : public QDialog {
	Q_OBJECT
public:
	PreferencesDialog(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent = 0);
	~PreferencesDialog();

	enum PageIndexes {
		editor = 0,
		behaviour = 1,
		miscellanious = 2,
		compiler = 3,
		debugger = 4,
		robotSettings = 5
	};

protected:
	void changeEvent(QEvent *e);

signals:
	void gridChanged();
	void iconsetChanged();
	void settingsApplied();

private slots:
	void cancel();
	void applyChanges();
	void saveAndClose();
	void changeSystem();
	void widthGridSliderMoved(int value);
	void indexGridSliderMoved(int value);

	void browseImagesPath();
	void chooseTab(const QModelIndex &);

	void activatedUnrealModel(bool checked);
	void manualComPortCheckboxChecked(bool state);

private:
	void initBehaviourPage();
	void initCompilerPage();
	void initDebugPage();
	void initEditorPage();
	void initMiscellaniousPage();
	void initRobotSettingsPage();
	void initRobotModelType(robotModelType::robotModelTypeEnum type);

	void initCompilersSettings(QString const &pathToQmake,
			QString const &pathToMake, QString const &pluginExtension, QString const &prefix);

	QString selectedPortName() const;
	sensorType::SensorTypeEnum selectedPort1Sensor() const;
	sensorType::SensorTypeEnum selectedPort2Sensor() const;
	sensorType::SensorTypeEnum selectedPort3Sensor() const;
	sensorType::SensorTypeEnum selectedPort4Sensor() const;
	robotModelType::robotModelTypeEnum selectedRobotModel() const;

	Ui::PreferencesDialog *ui;
	Ui::BehaviourForm *mBehaviourUi;
	Ui::CompilerForm *mCompilerUi;
	Ui::DebuggerForm *mDebuggerUi;
	Ui::EditorForm *mEditorUi;
	Ui::MiscellaniousForm *mMiscellaniousUi;
	Ui::RobotSettingsForm *mRobotSettingsUi;

	QWidget *mBehaviourPage;
	QWidget *mCompilerPage;
	QWidget *mDebuggerPage;
	QWidget *mEditorPage;
	QWidget *mMiscellaniousPage;
	QWidget *mRobotSettingsPage;

	QAction * const mShowGridAction;
	QAction * const mShowAlignmentAction;
	QAction * const mActivateGridAction;
	QAction * const mActivateAlignmentAction;
	int mWithGrid;
	int mIndexGrid;

	QString mLastIconsetPath;
};
