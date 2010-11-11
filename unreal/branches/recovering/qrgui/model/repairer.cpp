#include "repairer.h"

#include <QProgressDialog>

using namespace qReal;

Repairer::Repairer(qrRepo::RepoApi &api, const EditorManager &editorManager)
	:
	mApi(api),
	mEditorManager(editorManager)
{
}

void Repairer::patchSave()
{
	PatchSaveDialog* dialog = new PatchSaveDialog("","",this);
	dialog->show();
	//пропатчить редактор с помощью специального лога
	//просто выполнить последовательно всё, что в логе
	//следует избегать ситуаций с двумя разными ветками
}

void Repairer::patchSave(QString savePath, QString patchPath)
{
	//todo:
}

void Repairer::repair()
{
	qDebug() << "Repairing...";
	//предложить Автопочинку/ПропатчитьСейв/ПропатчитьРедактор
	repairElements(Id::getRootId());
	qDebug() << "Finished.";
}

QDialog* Repairer::repairDialog()
{
	QDialog* result = new QDialog();
	result->setFixedSize(360,480);
	QGridLayout layout;
	result->setLayout(&layout);

	return result;
}

void Repairer::repairElements(const Id target)
{
	foreach(Id child, mApi.children(target)) {
		//надо это оптимизировать, чтобы не проверять по 2 раза элементы
		if (!isCorrect(child)) {
			Id newId = correctId(child);
			mApi.replace(child, newId);
			repairElements(newId);
			continue;
		}
		repairElements(child);
	}
}

void Repairer::readLog(const QString diagram)
{
	if (mLogs.keys().contains(diagram))
		return;
	mLogs.insert(diagram, Message::parseLog("../logs/MetaEditor/" + diagram + "/"));
}

bool Repairer::isCorrect(Id target) const
{
	return (mEditorManager.elements(target.diagramId()).contains(target.type()));
}

bool Repairer::process(Id const target)
{
	//критерии проверки надо будет изменить и вынести в отдельные методы
	if ((target != Id::getRootId()) && (!isCorrect(target))) {
		repair();
		return false;
	}
	foreach(Id child, mApi.children(target)) {
		if (!process(child)) {
			repair();
			return false;
		}
	}

	return true;
}

Id Repairer::correctId(const Id target)
{
	readLog(target.diagram());

	Id curr = Id::loadFromString(target.toString().remove('/' + target.id()));
	foreach(Message msg, mLogs.value(target.diagram()))
		if ((msg.prevValue().type() == QVariant::String) && (msg.newValue().type() == QVariant::String)
			&& (msg.prevValue().toString() == curr.element()))
				curr = Id::loadFromString(target.diagramId().toString() + '/' + msg.newValue().toString());

	if (mEditorManager.elements(target.diagramId()).contains(curr))
		return Id::loadFromString(curr.toString() + '/' + target.id());
	else {
		qDebug() << "Repairer::getCorrectId() error | There is no final element in editor (incorrect log?).";
		return Id();
	}
}


/**		PatchSaveDialog		**/


PatchSaveDialog::PatchSaveDialog(QString savePathStr, QString patchPathStr, qReal::Repairer *repairer)
{
	mRepairer = repairer;

	layout = new QGridLayout();
	savePath = new QLineEdit(savePathStr);
	patchPath = new QLineEdit(patchPathStr);
	saveError = new QLabel();
	patchError = new QLabel();
	saveBrowse = new QPushButton("Browse");
	patchBrowse = new QPushButton("Browse");
	saveCaption = new QLabel("Path to save: ");
	patchCaption = new QLabel("Path to patch: ");
	commonLabel = new QLabel("Choose paths of save and patch.");
	runButton = new QPushButton("Patch");

	saveError->setStyleSheet("QLabel {color : red; }");
	patchError->setStyleSheet("QLabel {color : red; }");

	connect(runButton, SIGNAL(clicked()), this, SLOT(run()));
	connect(this, SIGNAL(finished(int)), this, SLOT(releaseMemory()));
	connect(patchBrowse, SIGNAL(clicked()), this, SLOT(openPatchFile()));
	connect(saveBrowse, SIGNAL(clicked()), this, SLOT(openSaveDirectory()));

	layout->addWidget(commonLabel,0,0,1,-1,Qt::AlignLeft);
	layout->addWidget(saveCaption,1,0);
	layout->addWidget(savePath,1,1);
	layout->addWidget(saveBrowse,1,2);
	layout->addWidget(patchCaption,3,0);
	layout->addWidget(patchPath,3,1);
	layout->addWidget(patchBrowse,3,2);
	layout->addWidget(runButton,5,0,1,-1,Qt::AlignRight);

	setLayout(layout);

	int width = this->width();
	setFixedSize(width,heightForWidth(width));
}

PatchSaveDialog::~PatchSaveDialog()
{
	releaseMemory();
}

bool PatchSaveDialog::checkPaths()
{
	//todo:
	layout->removeWidget(saveError);
	layout->removeWidget(patchError);
	if (false)
		return true;
	if (true) {
		saveError->setText(tr("Path is incorrect."));
		layout->addWidget(saveError,2,1,1,-1,Qt::AlignLeft);
	}
	if (true) {
		patchError->setText(tr("Path is incorrect."));
		layout->addWidget(patchError,4,1,1,-1,Qt::AlignLeft);
	}
	return false;
}

void PatchSaveDialog::run()
{
	if (checkPaths()) {
		mRepairer->patchSave(savePath->text(), patchPath->text());
		close();
	}
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
}

void PatchSaveDialog::openPatchFile() {
	patchPath->setText(
		QFileDialog::getOpenFileName(
			this, tr("Choose patch file"), patchPath->text(), tr("QReal patch files (*.patch)")));
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
}
