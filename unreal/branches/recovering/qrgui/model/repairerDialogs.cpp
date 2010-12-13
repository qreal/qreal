#include "repairerDialogs.h"

RepairerDialog::RepairerDialog(qReal::Repairer *repairer, QString const savePath)
{
	mRepairer = repairer;
	mSavePath = savePath;

	layout = new QGridLayout();

	mPatchSaveDialog = new PatchSaveDialog(savePath, "", mRepairer);
	mPatchSaveDialog->setMaximumSize(0,0);
	mPatchSaveDialog->setMaximumSize(maximumWidth(),maximumHeight());

	mAutorepairDialog = new QWidget();
	mAutorepairLayout = new QGridLayout();
	mAutorepairText = new QLabel(tr("QReal will use logs maded during working with editor."));	//todo: autodetect
	mAutorepairLayout->addWidget(mAutorepairText,0,0,1,-1,Qt::AlignCenter);
	mAutorepairDialog->setLayout(mAutorepairLayout);

	mGenerateEditorDialog = new QWidget();
	mGenerateEditorLayout = new QGridLayout();
	mGenerateEditorText = new QLabel(tr("Choose path to editor XML"));
	mGenerateEditorPath = new QLineEdit();
	mGenerateEditorPathError = new QLabel(tr("Path is incorrect"));
	mGenerateEditorPathCaption = new QLabel(tr("Path to XML:    "));
	mGenerateEditorPathBrowse = new QPushButton(tr("Browse"));
	mGenerateEditorLayout->addWidget(mGenerateEditorText,0,0,1,-1,Qt::AlignLeft);
	mGenerateEditorLayout->addWidget(mGenerateEditorPathCaption,1,0);
	mGenerateEditorLayout->addWidget(mGenerateEditorPath,1,1);
	mGenerateEditorLayout->addWidget(mGenerateEditorPathBrowse,1,2);
	mGenerateEditorLayout->addWidget(mGenerateEditorPathError,2,1,1,-1,Qt::AlignLeft);
	mGenerateEditorPathError->setStyleSheet("QLabel {color : red; }");
	mGenerateEditorPathError->hide();
	mGenerateEditorDialog->setLayout(mGenerateEditorLayout);

	mPatchSaveSwitcher = new QCheckBox(tr("Patch save"));
	mAutorepairSwitcher = new QCheckBox(tr("Try to repair save automatically"));
	mGenerateEditorSwitcher = new QCheckBox(tr("Generate new editor from file"));

	mCommonText = new QLabel(tr("Current save cannot be loaded.\nChoose ways to fix it:"));
	mRunButton = new QPushButton(tr("Run"));

	layout->addWidget(mCommonText,0,0,1,-1,Qt::AlignLeft);
	layout->addWidget(mPatchSaveSwitcher,1,0,1,-1,Qt::AlignLeft);
	layout->addWidget(mPatchSaveDialog,2,0);
	layout->addWidget(mAutorepairSwitcher,3,0,1,-1,Qt::AlignLeft);
	layout->addWidget(mAutorepairDialog,4,0);
	layout->addWidget(mGenerateEditorSwitcher,5,0,1,-1,Qt::AlignLeft);
	layout->addWidget(mGenerateEditorDialog,6,0);
	layout->addWidget(mRunButton,7,0,1,-1,Qt::AlignCenter);

	setLayout(layout);

	mPatchSaveDialog->hide();
	mAutorepairDialog->hide();
	mGenerateEditorDialog->hide();

	QPushButton* patchSaveRunButton = mPatchSaveDialog->getRunButton();
	patchSaveRunButton->hide();

	connect(mRunButton, SIGNAL(clicked()),this,SLOT(run()));
	connect(mGenerateEditorPathBrowse, SIGNAL(clicked()),this,SLOT(openEditorXML()));
	connect(mGenerateEditorPath, SIGNAL(editingFinished()), this, SLOT(checkEditorPath()));

	connectSwitchers();

	int width = this->width() + 20;
	setFixedSize(width,heightForWidth(width));

	mAutorepairSwitcher->toggle();
}

RepairerDialog::~RepairerDialog()
{
	delete layout;
}

void RepairerDialog::run()
{
	mGenerateEditorPathError->hide();
	if (mPatchSaveSwitcher->checkState() == Qt::Checked) {
		hide();
		mPatchSaveDialog->run();
	} else if (mAutorepairSwitcher->checkState() == Qt::Checked) {
		hide();
		mRepairer->repairElements(qReal::Id::getRootId());
	} else if ((mGenerateEditorSwitcher->checkState() == Qt::Checked) && (checkEditorPath())) {
		hide();
		mRepairer->getMainWindow()->generateEditorFromXML(mGenerateEditorPath->text());
		mRepairer->getMainWindow()->open(mSavePath);
	}
	close();
}

void RepairerDialog::openEditorXML()
{
	mGenerateEditorPath->setText(QFileDialog::getOpenFileName(this,
		tr("Select xml file to parse"), "../qrxml", "XML files (*.xml)"));
	checkEditorPath();
}

bool RepairerDialog::checkEditorPath()
{
	mGenerateEditorPathError->hide();
	QFileInfo fi(mGenerateEditorPath->text());
	if ((!fi.exists()) || (!fi.isFile()) || (QString::compare(fi.suffix(),"xml",Qt::CaseInsensitive))) {
		mGenerateEditorPathError->show();
		return false;
	}
	return true;
}

void RepairerDialog::switchPatchSave()
{
	disconnectSwitchers();
	if (mPatchSaveSwitcher->checkState() == Qt::Checked) {
		mAutorepairSwitcher->setCheckState(Qt::Unchecked);
		mGenerateEditorSwitcher->setCheckState(Qt::Unchecked);
		mPatchSaveDialog->setVisible(true);
		mAutorepairDialog->setVisible(false);
		mGenerateEditorDialog->setVisible(false);
	} else
		mPatchSaveSwitcher->setCheckState(Qt::Checked);
	connectSwitchers();
}

void RepairerDialog::switchAutorepair()
{
	disconnectSwitchers();
	if (mAutorepairSwitcher->checkState() == Qt::Checked) {
		mPatchSaveSwitcher->setCheckState(Qt::Unchecked);
		mGenerateEditorSwitcher->setCheckState(Qt::Unchecked);
		mPatchSaveDialog->setVisible(false);
		mAutorepairDialog->setVisible(true);
		mGenerateEditorDialog->setVisible(false);
	} else
		mAutorepairSwitcher->setCheckState(Qt::Checked);
	connectSwitchers();
}

void RepairerDialog::switchGenerateEditor()
{
	disconnectSwitchers();
	if (mGenerateEditorSwitcher->checkState() == Qt::Checked) {
		mPatchSaveSwitcher->setCheckState(Qt::Unchecked);
		mAutorepairSwitcher->setCheckState(Qt::Unchecked);
		mPatchSaveDialog->setVisible(false);
		mAutorepairDialog->setVisible(false);
		mGenerateEditorDialog->setVisible(true);
	} else
		mGenerateEditorSwitcher->setCheckState(Qt::Checked);
	connectSwitchers();
}

void RepairerDialog::connectSwitchers()
{
	connect(mPatchSaveSwitcher, SIGNAL(toggled(bool)), this, SLOT(switchPatchSave()));
	connect(mAutorepairSwitcher, SIGNAL(toggled(bool)), this, SLOT(switchAutorepair()));
	connect(mGenerateEditorSwitcher, SIGNAL(toggled(bool)), this, SLOT(switchGenerateEditor()));
}

void RepairerDialog::disconnectSwitchers()
{
	disconnect(mPatchSaveSwitcher, SIGNAL(toggled(bool)), this, SLOT(switchPatchSave()));
	disconnect(mAutorepairSwitcher, SIGNAL(toggled(bool)), this, SLOT(switchAutorepair()));
	disconnect(mGenerateEditorSwitcher, SIGNAL(toggled(bool)), this, SLOT(switchGenerateEditor()));
}

/**		PatchSaveDialog		**/


PatchSaveDialog::PatchSaveDialog(QString savePathStr, QString patchPathStr, qReal::Repairer *repairer)
{
	mRepairer = repairer;

	layout = new QGridLayout();
	savePath = new QLineEdit(savePathStr);
	patchPath = new QLineEdit(patchPathStr);
	saveError = new QLabel(tr("Path is incorrect"));
	patchError = new QLabel(tr("Path is incorrect"));
	saveBrowse = new QPushButton(tr("Browse"));
	patchBrowse = new QPushButton(tr("Browse"));
	saveCaption = new QLabel(tr("Path to save: "));
	patchCaption = new QLabel(tr("Path to patch: "));
	commonLabel = new QLabel(tr("Choose paths to save and patch"));
	runButton = new QPushButton(tr("Patch"));

	saveError->setStyleSheet("QLabel {color : red; }");
	patchError->setStyleSheet("QLabel {color : red; }");

	connect(runButton, SIGNAL(clicked()), this, SLOT(run()));
	connect(this, SIGNAL(destroyed()), this, SLOT(releaseMemory()));
	connect(patchBrowse, SIGNAL(clicked()), this, SLOT(openPatchFile()));
	connect(saveBrowse, SIGNAL(clicked()), this, SLOT(openSaveDirectory()));
	connect(savePath, SIGNAL(editingFinished()), this, SLOT(checkSavePath()));
	connect(patchPath, SIGNAL(editingFinished()), this, SLOT(checkPatchPath()));

	layout->addWidget(commonLabel,0,0,1,-1,Qt::AlignLeft);
	layout->addWidget(saveCaption,1,0);
	layout->addWidget(savePath,1,1);
	layout->addWidget(saveBrowse,1,2);
	layout->addWidget(saveError,2,1,1,-1,Qt::AlignLeft);
	layout->addWidget(patchCaption,3,0);
	layout->addWidget(patchPath,3,1);
	layout->addWidget(patchBrowse,3,2);
	layout->addWidget(patchError,4,1,1,-1,Qt::AlignLeft);
	layout->addWidget(runButton,5,0,1,-1,Qt::AlignRight);

	saveError->hide();
	patchError->hide();

	setLayout(layout);

	int width = this->width();
	setFixedSize(width,heightForWidth(width));
}

PatchSaveDialog::~PatchSaveDialog()
{
	releaseMemory();
}

QPushButton* PatchSaveDialog::getRunButton() const
{
	return runButton;
}

bool PatchSaveDialog::checkSavePath()
{
	saveError->hide();
	QFileInfo fi(savePath->text());
	if ((!fi.exists()) || (!fi.isDir())) {
		saveError->show();
		return false;
	}
	return true;
}

bool PatchSaveDialog::checkPatchPath()
{
	patchError->hide();
	QFileInfo fi(patchPath->text());
	if ((!fi.exists()) || (!fi.isFile()) || (QString::compare(fi.suffix(),qReal::extensionPatch,Qt::CaseInsensitive))) {
		patchError->show();
		return false;
	}
	return true;
}

void PatchSaveDialog::run()
{	if (checkSavePath() && checkPatchPath()) {
		mRepairer->patchSave(savePath->text(), patchPath->text());
		close();
	}
	checkPatchPath();
}

void PatchSaveDialog::releaseMemory()
{
	delete layout;
	delete commonLabel;
	delete runButton;

	delete saveCaption;
	delete savePath;
	delete saveBrowse;

	delete patchCaption;
	delete patchPath;
	delete patchBrowse;

	delete mRepairer;
}

void PatchSaveDialog::openPatchFile() {
	patchPath->setText(
		QFileDialog::getOpenFileName(this, tr("Choose patch file"), patchPath->text(),
				tr("QReal patch files (*."+qReal::extensionPatch.toAscii()+')')));
	checkPatchPath();
}

void PatchSaveDialog::openSaveDirectory() {
	//todo: improve it with QDir, current solution is incorrect
	QString startPath = savePath->text();
	if (startPath.length() > 1) {
		if (!startPath.endsWith("/"))
			startPath += '/';
		startPath += '.' + '.';
	}
	savePath->setText(QFileDialog::getExistingDirectory(this, tr("Choose save directory"), startPath));
	checkSavePath();
}
