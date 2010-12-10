#include "logger.h"

using namespace qReal;

Logger::Logger(QString const workingDir)
	:
	mWorkingDir(workingDir),
	enabled(false)
{
	flagsEnabled[flgEditors] = true;
	flagsEnabled[flgDiagrams] = false;
	flagsEnabled[flgUselessMessages] = false;
}
Logger::~Logger()
{
	foreach(QFile* file, files.values())
		delete file;
	foreach(QString* string, names.values())
		delete string;
	foreach(QList<Message*>* log, buffer.values()) {
		foreach(Message* message, *log)
			delete message;
		delete log;
	}
}

void Logger::enable()
{
	enabled = true;
}
void Logger::disable()
{
	enabled = false;
}
void Logger::setFlag(flag flg, bool arg)
{
	flagsEnabled[flg] = arg;
}

void Logger::rememberNameOfScene(Id scene, QString name)
{
	if (!names.contains(scene))
		names.insert(scene, new QString(name));
	else
		names.replace(scene, new QString(name));
}

void Logger::log(Message* const message)
{
	Id scene = message->scene();
	if (!pass(*message)) {
//		qDebug() << "Logger::log() error | Message filtered.";
		return;
	}
	if (!buffer.contains(scene)) {
		QList<Message*>* log = new QList<Message*>();
		log->append(message);
		buffer.insert(scene, log);
	}
	else
		buffer.value(scene)->append(message);
}

void Logger::output()
{
	foreach(Id scene, buffer.keys()) {
		QString log;
		QString patch;

		bool editor = isEditor(scene);
		foreach(Message* message, *buffer.value(scene)) {
			log += message->toString();
			if (editor) {
				Message patchMessage = message->generatePatchMessage(*names.value(scene));
				if (patchMessage.valid())
					patch += patchMessage.toString();
			}
		}
		write(log, scene, false);
		if (editor)
			write(patch, scene, true);
	}

	foreach(QList<Message*>* log, buffer.values()) {
		foreach(Message* message, *log)
			delete message;
		delete log;
	}
	buffer.clear();
}

bool Logger::isEditor(const Id scene) const
{
	return (scene.editor() == QString("Meta_editor"));
}

bool Logger::pass(const Message message) const
{
	bool editor = isEditor(message.scene());

	if ((!flagsEnabled[flgDiagrams]) && (!editor))
		return false;
	if ((!flagsEnabled[flgEditors]) && (editor))
		return false;

	bool useless = (message.performed() == actSetData) &&
		(((message.details() == "position") || (message.details() == "configuration")));

	if ((!flagsEnabled[flgUselessMessages]) && (useless))
		return false;

	return true;
}

void Logger::write(const QString message, const Id scene, const bool patch)
{
	if (!enabled)
		return;
	if (isEditor(scene))
		write(message, scene, "../", patch);
	write(message, scene, mWorkingDir, patch);
}

void Logger::write(const QString message, const Id scene, const QString workingDir, const bool patch)
{
	QString path = workingDir + "/logs/" + scene.diagram() + '/';
	if (isEditor(scene))
		path += *names.value(scene, new QString("UnknownDiagram")) + '/';
	QDir dir;
	dir.mkpath(path);

	path += '/'+scene.id()+'.';
	if (patch)
		path += qReal::extensionPatch;
	else
		path += qReal::extensionLog;

	QFile *file;
	if (!files.contains(path)) {
		file = new QFile(path);
		files.insert(path, file);
	} else {
		file = files.value(path);
	}

	if (!file->isOpen())
		file->open(QIODevice::Append | QIODevice::Text);
	QTextStream out(file);
	out << message << '\n';

	//may be, file must be closed or smthng else
}

void Logger::setWorkingDir(QString workingDir)
{
	mWorkingDir = workingDir;
}
