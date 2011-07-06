#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"

#include "ui_preferencesBehaviourForm.h"
#include "ui_preferencesCompilerForm.h"
#include "ui_preferencesDebuggerForm.h"
#include "ui_preferencesEditorForm.h"
#include "ui_preferencesMiscellaniousForm.h"
#include "ui_preferencesRobotSettingsForm.h"

#include <QSettings>
#include <QFileDialog>

#include "../../thirdparty/qextserialport/src/qextserialenumerator.h"

using namespace qReal::interpreters::robots;

PreferencesDialog::PreferencesDialog(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent)
	: QDialog(parent), ui(new Ui::PreferencesDialog), mShowGridAction(showGridAction), mShowAlignmentAction(showAlignmentAction)
	, mActivateGridAction(activateGridAction), mActivateAlignmentAction(activateAlignmentAction)
{
	ui->setupUi(this);
	// Seems like god object
	initBehaviourPage();
	initCompilerPage();
	initDebugPage();
	initEditorPage();
	initMiscellaniousPage();
	initRobotSettingsPage();

	connect(ui->listWidget, SIGNAL(clicked(const QModelIndex &)), this, SLOT(chooseTab(const QModelIndex &)));
	connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));

	mLastIconsetPath = mMiscellaniousUi->imagesPathEdit->text();

	QSettings settings("SPbSU", "QReal");
	int currentTab = settings.value("currentPreferencesTab", 0).toInt();
	ui->listWidget->setCurrentRow(currentTab);
	chooseTab(ui->listWidget->currentIndex());
}

void PreferencesDialog::initBehaviourPage()
{
	mBehaviourPage = new QWidget(ui->pageContentWigdet);
	mBehaviourUi = new Ui::BehaviourForm;
	mBehaviourUi->setupUi(mBehaviourPage);
	mBehaviourPage->hide();

	QSettings settings("SPbSU", "QReal");
	mBehaviourUi->saveExitCheckBox->setChecked(settings.value("SaveExitSuggestion", true).toBool());
	mBehaviourUi->warningWindowBox->setChecked(settings.value("warningWindow", true).toBool());
	mBehaviourUi->chooseDiagramsToSaveCheckBox->setChecked(settings.value("ChooseDiagramsToSave", true).toBool());
	mBehaviourUi->diagramCreateCheckBox->setChecked(settings.value("DiagramCreateSuggestion", true).toBool());
	mBehaviourUi->paletteTabCheckBox->setChecked(settings.value("PaletteTabSwitching", true).toBool());
}

void PreferencesDialog::initCompilerPage()
{
	mCompilerPage = new QWidget(ui->pageContentWigdet);
	mCompilerUi = new Ui::CompilerForm;
	mCompilerUi->setupUi(mCompilerPage);
	mCompilerPage->hide();

	connect(mCompilerUi->linuxButton, SIGNAL(clicked()), this, SLOT(changeSystem()));
	connect(mCompilerUi->windowsButton, SIGNAL(clicked()), this, SLOT(changeSystem()));
	connect(mCompilerUi->otherButton, SIGNAL(clicked()), this, SLOT(changeSystem()));

	QSettings settings("SPbSU", "QReal");
	mCompilerUi->windowsButton->setChecked(settings.value("windowsButton", false).toBool());
	mCompilerUi->linuxButton->setChecked(settings.value("linuxButton", false).toBool());
	mCompilerUi->otherButton->setChecked(settings.value("otherButton", false).toBool());
	mCompilerUi->pathToQmake->setText(settings.value("pathToQmake", "").toString());
	mCompilerUi->pathToMake->setText(settings.value("pathToMake", "").toString());
	mCompilerUi->pluginExtension->setText(settings.value("pluginExtension", "").toString());
	mCompilerUi->prefix->setText(settings.value("prefix", "").toString());
}

void PreferencesDialog::initDebugPage()
{
	mDebuggerPage = new QWidget(ui->pageContentWigdet);
	mDebuggerUi = new Ui::DebuggerForm;
	mDebuggerUi->setupUi(mDebuggerPage);
	mDebuggerPage->hide();

	QSettings settings("SPbSU", "QReal");
	mDebuggerUi->timeoutLineEdit->setText(settings.value("debuggerTimeout", 750).toString());
	mDebuggerUi->colorComboBox->addItems(QColor::colorNames());
	QString curColor = settings.value("debugColor", "red").toString();
	int curColorIndex = mDebuggerUi->colorComboBox->findText(curColor);
	mDebuggerUi->colorComboBox->setCurrentIndex(curColorIndex);
	settings.value("debugColor", mDebuggerUi->colorComboBox->currentText());
}

void PreferencesDialog::initEditorPage()
{
	mEditorPage = new QWidget(ui->pageContentWigdet);
	mEditorUi = new Ui::EditorForm;
	mEditorUi->setupUi(mEditorPage);
	mEditorPage->hide();

	// changing grid size in QReal:Robots is forbidden
	connect(mEditorUi->gridWidthSlider, SIGNAL(sliderMoved(int)), this, SLOT(widthGridSliderMoved(int)));
	connect(mEditorUi->indexGridSlider, SIGNAL(sliderMoved(int)), this, SLOT(indexGridSliderMoved(int)));
	mEditorUi->indexGridSlider->setVisible(false);
	mEditorUi->label_20->setVisible(false);

	QSettings settings("SPbSU", "QReal");
	mEditorUi->showGridCheckBox->setChecked(settings.value("ShowGrid", true).toBool());
	mEditorUi->showAlignmentCheckBox->setChecked(settings.value("ShowAlignment", true).toBool());
	mEditorUi->activateGridCheckBox->setChecked(settings.value("ActivateGrid", false).toBool());
	mEditorUi->activateAlignmentCheckBox->setChecked(settings.value("ActivateAlignment", true).toBool());
	mEditorUi->embeddedLinkerIndentSlider->setValue(settings.value("EmbeddedLinkerIndent", 8).toInt());
	mEditorUi->embeddedLinkerSizeSlider->setValue(settings.value("EmbeddedLinkerSize", 6).toInt());
	mEditorUi->gridWidthSlider->setValue(settings.value("GridWidth", 10).toInt());
	mEditorUi->indexGridSlider->setValue(settings.value("IndexGrid", 30).toInt());
	mEditorUi->zoomFactorSlider->setValue(settings.value("zoomFactor", 2).toInt());
	mWithGrid = mEditorUi->gridWidthSlider->value();
	mIndexGrid = mEditorUi->indexGridSlider->value();
}

void PreferencesDialog::initMiscellaniousPage()
{
	mMiscellaniousPage = new QWidget(ui->pageContentWigdet);
	mMiscellaniousUi = new Ui::MiscellaniousForm;
	mMiscellaniousUi->setupUi(mMiscellaniousPage);
	mMiscellaniousPage->hide();

	connect(mMiscellaniousUi->imagesPathBrowseButton, SIGNAL(clicked()), this, SLOT(browseImagesPath()));

	QSettings settings("SPbSU", "QReal");
	mMiscellaniousUi->imagesPathEdit->setText(settings.value("pathToImages", QDir::currentPath() + "/images/iconset1").toString());
	mMiscellaniousUi->chaoticEditionCheckBox->setChecked(settings.value("ChaoticEdition", false).toBool());
	mMiscellaniousUi->antialiasingCheckBox->setChecked(settings.value("Antialiasing", true).toBool());
	mMiscellaniousUi->splashScreenCheckBox->setChecked(settings.value("Splashscreen", true).toBool());
	mMiscellaniousUi->openGLCheckBox->setChecked(settings.value("OpenGL", true).toBool());
	mMiscellaniousUi->squareLineModeCheckBox->setChecked(settings.value("SquareLine", false).toBool());
}

void PreferencesDialog::initRobotSettingsPage()
{
	mRobotSettingsPage = new QWidget(ui->pageContentWigdet);
	mRobotSettingsUi = new Ui::RobotSettingsForm;
	mRobotSettingsUi->setupUi(mRobotSettingsPage);
	mRobotSettingsPage->hide();

	connect(mRobotSettingsUi->nullModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(activatedUnrealModel(bool)));
	connect(mRobotSettingsUi->d2ModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(activatedUnrealModel(bool)));
	connect(mRobotSettingsUi->manualComPortCheckbox, SIGNAL(toggled(bool)), this, SLOT(manualComPortCheckboxChecked(bool)));

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
	QSettings settings("SPbSU", "QReal");
	QString const defaultPortName = settings.value("bluetoothPortName", "").toString();

	if (ports.isEmpty()) {
		mRobotSettingsUi->comPortComboBox->hide();
		mRobotSettingsUi->comPortLabel->hide();
		mRobotSettingsUi->manualComPortCheckbox->hide();
		mRobotSettingsUi->noComPortsFoundLabel->show();
		mRobotSettingsUi->directInputComPortLabel->show();
		mRobotSettingsUi->directInputComPortLineEdit->show();
		mRobotSettingsUi->directInputComPortLineEdit->setText(defaultPortName);
	} else {
		mRobotSettingsUi->comPortComboBox->show();
		mRobotSettingsUi->comPortLabel->show();
		mRobotSettingsUi->manualComPortCheckbox->show();
		mRobotSettingsUi->directInputComPortLabel->hide();
		mRobotSettingsUi->directInputComPortLineEdit->hide();
		mRobotSettingsUi->noComPortsFoundLabel->hide();

		foreach (QextPortInfo info, ports) {
			QRegExp const portNameRegexp("COM\\d+", Qt::CaseInsensitive);
			if (portNameRegexp.indexIn(info.portName) != -1) {
				QString const portName = portNameRegexp.cap();
				mRobotSettingsUi->comPortComboBox->addItem(portName);
			}
		}
		int const defaultIndex = mRobotSettingsUi->comPortComboBox->findText(defaultPortName);
		if (defaultIndex != -1)
			mRobotSettingsUi->comPortComboBox->setCurrentIndex(defaultIndex);
	}

	if (settings.value("manualComPortCheckboxChecked", "false").toBool())
		mRobotSettingsUi->manualComPortCheckbox->setChecked(true);

	QStringList sensorNames;
	sensorNames << tr("Unused")
			<< tr("Touch sensor (boolean value)")
			<< tr("Touch sensor (raw value)")
			<< tr("Sonar sensor")
			<< tr("Color sensor (full colors)")
			<< tr("Color sensor (red)")
			<< tr("Color sensor (green)")
			<< tr("Color sensor (blue)")
			<< tr("Color sensor (passive)")
	;

	mRobotSettingsUi->port1ComboBox->addItems(sensorNames);
	mRobotSettingsUi->port2ComboBox->addItems(sensorNames);
	mRobotSettingsUi->port3ComboBox->addItems(sensorNames);
	mRobotSettingsUi->port4ComboBox->addItems(sensorNames);

	sensorType::SensorTypeEnum const port1 = static_cast<sensorType::SensorTypeEnum>(settings.value("port1SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port2 = static_cast<sensorType::SensorTypeEnum>(settings.value("port2SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port3 = static_cast<sensorType::SensorTypeEnum>(settings.value("port3SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port4 = static_cast<sensorType::SensorTypeEnum>(settings.value("port4SensorType", "0").toInt());

	mRobotSettingsUi->port1ComboBox->setCurrentIndex(port1);
	mRobotSettingsUi->port2ComboBox->setCurrentIndex(port2);
	mRobotSettingsUi->port3ComboBox->setCurrentIndex(port3);
	mRobotSettingsUi->port4ComboBox->setCurrentIndex(port4);

	robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<robotModelType::robotModelTypeEnum>(settings.value("robotModel", "1").toInt());
	initRobotModelType(typeOfRobotModel);

}

// Initialization END

PreferencesDialog::~PreferencesDialog()
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("currentPreferencesTab", ui->listWidget->currentRow());

	delete ui;

	delete mBehaviourUi;
	delete mCompilerUi;
	delete mDebuggerUi;
	delete mEditorUi;
	delete mMiscellaniousUi;

	delete mBehaviourPage;
	delete mCompilerPage;
	delete mDebuggerPage;
	delete mEditorPage;
	delete mMiscellaniousPage;
}

void PreferencesDialog::widthGridSliderMoved(int value)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("GridWidth", value);
	emit gridChanged();
}

void PreferencesDialog::indexGridSliderMoved(int value)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("IndexGrid", value);
	emit gridChanged();
}

void PreferencesDialog::applyChanges()
{
	QSettings settings("SPbSU", "QReal");

	settings.setValue("ShowGrid", mEditorUi->showGridCheckBox->isChecked());
	settings.setValue("ShowAlignment", mEditorUi->showAlignmentCheckBox->isChecked());
	settings.setValue("ActivateGrid", mEditorUi->activateGridCheckBox->isChecked());
	settings.setValue("ActivateAlignment", mEditorUi->activateAlignmentCheckBox->isChecked());
	settings.setValue("EmbeddedLinkerIndent", mEditorUi->embeddedLinkerIndentSlider->value());
	settings.setValue("EmbeddedLinkerSize", mEditorUi->embeddedLinkerSizeSlider->value());
	settings.setValue("GridWidth", mEditorUi->gridWidthSlider->value());
	settings.setValue("IndexGrid", mEditorUi->indexGridSlider->value());
	settings.setValue("zoomFactor", mEditorUi->zoomFactorSlider->value());
	mWithGrid = mEditorUi->gridWidthSlider->value();
	mIndexGrid = mEditorUi->indexGridSlider->value();

	mShowGridAction->setChecked(mEditorUi->showGridCheckBox->isChecked());
	mShowAlignmentAction->setChecked(mEditorUi->showAlignmentCheckBox->isChecked());
	mActivateGridAction->setChecked(mEditorUi->activateGridCheckBox->isChecked());
	mActivateAlignmentAction->setChecked(mEditorUi->activateAlignmentCheckBox->isChecked());

	settings.setValue("warningWindow", mBehaviourUi->warningWindowBox->isChecked());
	settings.setValue("ChooseDiagramsToSave", mBehaviourUi->chooseDiagramsToSaveCheckBox->isChecked());
	settings.setValue("DiagramCreateSuggestion", mBehaviourUi->diagramCreateCheckBox->isChecked());
	settings.setValue("PaletteTabSwitching", mBehaviourUi->paletteTabCheckBox->isChecked());
	settings.setValue("SaveExitSuggestion", mBehaviourUi->saveExitCheckBox->isChecked());


	settings.setValue("Splashscreen", mMiscellaniousUi->splashScreenCheckBox->isChecked());
	settings.setValue("ShowGrid", mEditorUi->showGridCheckBox->isChecked());
	settings.setValue("ShowAlignment", mEditorUi->showAlignmentCheckBox->isChecked());
	settings.setValue("ActivateGrid", mEditorUi->activateGridCheckBox->isChecked());
	settings.setValue("ActivateAlignment", mEditorUi->activateAlignmentCheckBox->isChecked());
	settings.setValue("Antialiasing", mMiscellaniousUi->antialiasingCheckBox->isChecked());
	settings.setValue("OpenGL", mMiscellaniousUi->openGLCheckBox->isChecked());
	settings.setValue("SquareLine", mMiscellaniousUi->squareLineModeCheckBox->isChecked());

	settings.setValue("warningWindow", mBehaviourUi->warningWindowBox->isChecked());

	settings.setValue("windowsButton", mCompilerUi->windowsButton->isChecked());
	settings.setValue("linuxButton", mCompilerUi->linuxButton->isChecked());
	settings.setValue("otherButton", mCompilerUi->otherButton->isChecked());
	settings.setValue("pathToQmake", mCompilerUi->pathToQmake->text());
	settings.setValue("pathToMake", mCompilerUi->pathToMake->text());
	settings.setValue("pluginExtension", mCompilerUi->pluginExtension->text());
	settings.setValue("prefix", mCompilerUi->prefix->text());

	settings.setValue("debuggerTimeout", mDebuggerUi->timeoutLineEdit->text());
	settings.setValue("debugColor", mDebuggerUi->colorComboBox->currentText());

	settings.setValue("ChaoticEdition", mMiscellaniousUi->chaoticEditionCheckBox->isChecked());
	settings.setValue("Splashscreen", mMiscellaniousUi->splashScreenCheckBox->isChecked());
	settings.setValue("Antialiasing", mMiscellaniousUi->antialiasingCheckBox->isChecked());
	settings.setValue("OpenGL", mMiscellaniousUi->openGLCheckBox->isChecked());
	settings.setValue("pathToImages", mMiscellaniousUi->imagesPathEdit->text());

	settings.setValue("bluetoothPortName", selectedPortName());
	settings.setValue("port1SensorType", selectedPort1Sensor());
	settings.setValue("port2SensorType", selectedPort2Sensor());
	settings.setValue("port3SensorType", selectedPort3Sensor());
	settings.setValue("port4SensorType", selectedPort4Sensor());
	settings.setValue("robotModel", selectedRobotModel());


	if (mLastIconsetPath != mMiscellaniousUi->imagesPathEdit->text())
		emit iconsetChanged();

	emit settingsApplied();
}

void PreferencesDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		mBehaviourUi->retranslateUi(mBehaviourPage);
		mCompilerUi->retranslateUi(mCompilerPage);
		mDebuggerUi->retranslateUi(mDebuggerPage);
		mEditorUi->retranslateUi(mEditorPage);
		mMiscellaniousUi->retranslateUi(mMiscellaniousPage);
		mRobotSettingsUi->retranslateUi(mRobotSettingsPage);

		break;
	default:
		break;
	}
}

void PreferencesDialog::saveAndClose()
{
	applyChanges();
	close();
}

void PreferencesDialog::cancel()
{
	mEditorUi->gridWidthSlider->setValue(mWithGrid);
	mEditorUi->indexGridSlider->setValue(mIndexGrid);
	QSettings settings("SPbSU", "QReal");
	settings.setValue("GridWidth", mWithGrid);
	settings.setValue("IndexGrid", mIndexGrid);
	close();
}

void PreferencesDialog::changeSystem()
{
	if(mCompilerUi->linuxButton->isChecked())
		initCompilersSettings("qmake", "make", "so", "lib");
	if(mCompilerUi->windowsButton->isChecked())
		initCompilersSettings("qmake", "mingw32-make", "dll", "");
	if(mCompilerUi->otherButton->isChecked())
		mCompilerUi->compilerSettingsWidget->setEnabled(true);
}

void PreferencesDialog::initCompilersSettings(const QString &pathToQmake,
		const QString &pathToMake, const QString &pluginExtension, const QString &prefix)
{
	mCompilerUi->pathToQmake->setText(pathToQmake);
	mCompilerUi->pathToMake->setText(pathToMake);
	mCompilerUi->pluginExtension->setText(pluginExtension);
	mCompilerUi->prefix->setText(prefix);
	mCompilerUi->compilerSettingsWidget->setEnabled(false);
}

void PreferencesDialog::browseImagesPath()
{
	QString path = QFileDialog::getExistingDirectory(this, "Open Directory");
	mMiscellaniousUi->imagesPathEdit->setText(path.replace("\\", "/"));
}

void PreferencesDialog::chooseTab(const QModelIndex &index)
{
	mBehaviourPage->hide();
	mCompilerPage->hide();
	mEditorPage->hide();
	mDebuggerPage->hide();
	mMiscellaniousPage->hide();
	mRobotSettingsPage->hide();

	switch(static_cast<PageIndexes>(index.row())){
	case behaviour:
		mBehaviourPage->show();
		break;

	case compiler:
		mCompilerPage->show();
		break;

	case debugger:
		mDebuggerPage->show();
		break;

	case editor:
		mEditorPage->show();
		break;

	case miscellanious:
		mMiscellaniousPage->show();
		break;

	case robotSettings:
		mRobotSettingsPage->show();
		break;
	}
}

void PreferencesDialog::initRobotModelType(robotModelType::robotModelTypeEnum type)
{
	if (type == robotModelType::null) {
		mRobotSettingsUi->nullModelRadioButton->setChecked(true);
		activatedUnrealModel(true);
	} else if (type == robotModelType::unreal) {
		mRobotSettingsUi->d2ModelRadioButton->setChecked(true);
		activatedUnrealModel(true);
	} else
		mRobotSettingsUi->realModelRadioButton->setChecked(true);
}

robotModelType::robotModelTypeEnum PreferencesDialog::selectedRobotModel() const
{
	if (mRobotSettingsUi->nullModelRadioButton->isChecked())
		return robotModelType::null;
	else if (mRobotSettingsUi->d2ModelRadioButton->isChecked())
		return robotModelType::unreal;
	else
		return robotModelType::real;
}


void PreferencesDialog::activatedUnrealModel(bool checked)
{
	if (checked)
		mRobotSettingsUi->bluetoothSettingsGroupBox->setEnabled(false);
	else
		mRobotSettingsUi->bluetoothSettingsGroupBox->setEnabled(true);
}

void PreferencesDialog::manualComPortCheckboxChecked(bool state)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("manualComPortCheckboxChecked", state);
	QString const defaultPortName = settings.value("bluetoothPortName", "").toString();

	if (state) {
		mRobotSettingsUi->comPortComboBox->hide();
		mRobotSettingsUi->comPortLabel->hide();
		mRobotSettingsUi->directInputComPortLabel->show();
		mRobotSettingsUi->directInputComPortLineEdit->show();
		mRobotSettingsUi->directInputComPortLineEdit->setText(defaultPortName);
	} else {
		mRobotSettingsUi->comPortComboBox->show();
		mRobotSettingsUi->comPortLabel->show();
		mRobotSettingsUi->directInputComPortLabel->hide();
		mRobotSettingsUi->directInputComPortLineEdit->hide();
		mRobotSettingsUi->noComPortsFoundLabel->hide();
	}
}

QString PreferencesDialog::selectedPortName() const
{
	if (!isVisible()) {
		QSettings settings("SPbSU", "QReal");
		return settings.value("bluetoothPortName", "").toString();
	}

	if (mRobotSettingsUi->comPortComboBox->isVisible())
		return mRobotSettingsUi->comPortComboBox->currentText();
	else
		return mRobotSettingsUi->directInputComPortLineEdit->text();
}

sensorType::SensorTypeEnum PreferencesDialog::selectedPort1Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mRobotSettingsUi->port1ComboBox->currentIndex());
}

sensorType::SensorTypeEnum PreferencesDialog::selectedPort2Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mRobotSettingsUi->port2ComboBox->currentIndex());
}

sensorType::SensorTypeEnum PreferencesDialog::selectedPort3Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mRobotSettingsUi->port3ComboBox->currentIndex());
}

sensorType::SensorTypeEnum PreferencesDialog::selectedPort4Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mRobotSettingsUi->port4ComboBox->currentIndex());
}
