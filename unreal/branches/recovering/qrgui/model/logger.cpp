#include "logger.h"

using namespace qReal;

Logger::Logger(QString const workingDir)
	:
	mWorkingDir(workingDir),
	enabled(false)
{
	flagsEnabled[flgEditors] = true;
	flagsEnabled[flgDiagrams] = true;
	flagsEnabled[flgUselessMessages] = false;
	flagsEnabled[flgInvalidMessages] = false;
}

Logger::~Logger()
{
	foreach(QFile* file, files.values())
		delete file;
	foreach(QString* string, names.values())
		delete string;
	foreach(QList<Message*>* log, buffer.values()) {
		foreach(Message* msg, *log)
			delete msg;
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

void Logger::log(action performed,
							const Id scene)
{
	if ((performed != actCreateDiagram) && (performed != actDestroyDiagram)) {
		write(msgInvalid +"\n",scene);
		if (flagsEnabled[flgInvalidMessages])
			log(performed, scene, Id(), QVariant(), QVariant(), QString());
	} else
		log(performed, scene, Id(), QVariant(), QVariant(), QString());
}

void Logger::log(action performed,
					const Id scene, const Id target)
{
	if ((performed != actAddElement) && (performed != actRemoveElement)) {
		write(msgInvalid + "\n",scene);
		if (flagsEnabled[flgInvalidMessages])
			log(performed, scene, target, QVariant(), QVariant(), QString());
	} else
		log(performed, scene, target, QVariant(), QVariant(), QString());
}

void Logger::log(action performed,
				 const Id scene, const Id target,
				 const QVariant prevData, const QVariant newData,
				 const QString additional)
{
	if (!pass(scene))
		return;

	log(scene, new Message(target, performed, additional, prevData, newData));
}

void Logger::log(const Id scene, Message* const data)
{
	if (!buffer.contains(scene)) {
		QList<Message*>* log = new QList<Message*>();
		log->append(data);
		buffer.insert(scene, log);
	}
	else
		buffer.value(scene)->append(data);
}

void Logger::output()
{
	foreach(Id scene, cleanDiagrams)
		remove(scene);
	foreach(Id scene, buffer.keys()) {
		QString output;
		foreach(Message* msg, *buffer.value(scene)) {
			if ((msg->performed() != actDestroyDiagram) && (msg->performed() != actCreateDiagram))
				cleanDiagrams.remove(scene);
			if ((msg->performed() == actDestroyDiagram) && (cleanDiagrams.contains(scene))) {
				cleanDiagrams.remove(scene);
				remove(scene);
				break;
				break;
			}
			if ((msg->performed() != actSetData) || (flagsEnabled[flgUselessMessages]) ||
				((msg->details() != "position") && (msg->details() != "configuration")))
					output += msg->toString() + '\n';
		}
		write(output, scene);
	}

	foreach(QList<Message*>* log, buffer.values()) {
		foreach(Message* msg, *log)
			delete msg;
		delete log;
	}
	buffer.clear();
}

bool Logger::editor(const Id scene)
{
	return (scene.editor() == QString("Meta_editor"));
}

bool Logger::pass(const Id scene)
{
	return	((!editor(scene) && flagsEnabled[flgDiagrams])
			|| (editor(scene) && flagsEnabled[flgEditors]));
}

void Logger::remove(const Id scene)
{
	if (editor(scene))
		remove(scene, "../");
	remove(scene, mWorkingDir);
}

void Logger::remove(const Id scene, QString const workingDir)
{
	buffer.remove(scene);

	QString name = scene.id();
	QFile *file;
	if (files.contains(name))
		file = files.value(name);
	else
		return;

	QDir dir;
	file->remove();
	files.remove(name);
	dir.rmdir(workingDir+"/logs/"+scene.diagram());
}

void Logger::write(const QString message, const Id scene)
{
	if (editor(scene))
		write(message, scene, "../");
	write(message, scene, mWorkingDir);
}

void Logger::write(const QString message, const Id scene, const QString workingDir)
{
	if (!enabled)
		return;

	QString path = workingDir + "/logs/" + scene.diagram() + "/";
	if (editor(scene))
		path += *names.value(scene, new QString("UnknownDiagram")) + "/";
	QString name = scene.id();
	QDir dir;
	dir.mkpath(path);

	QFile *file;
	if (!files.contains(name)) {
		file = new QFile(path+"/"+name+".log");
		files.insert(name, file);
	} else {
		file = files.value(name);
	}

	if (!file->isOpen())
		file->open(QIODevice::Append | QIODevice::Text);
	QTextStream out(file);
	out << message << "\n";

	//may be, file must be closed or smthng else
}

void Logger::setWorkingDir(QString workingDir)
{
	mWorkingDir = workingDir;
}
