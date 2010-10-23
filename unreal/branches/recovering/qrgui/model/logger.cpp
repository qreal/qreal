#include "model.h"
#include "logger.h"

#include <QDebug>

using namespace qReal;
using namespace model;

Logger::Logger(Model *model)
	:
	mModel(model), mWorkingDir(mModel->mutableApi().getWorkingDir()),
	enabled(false)
{
	flagsEnabled[flgEditors] = true;
	flagsEnabled[flgDiagrams] = true;
	flagsEnabled[flgUselessMessages] = false;
	flagsEnabled[flgInvalidMessages] = false;
}

Logger::~Logger()
{
	foreach(Id diagram, cleanDiagrams)
		remove(diagram);
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

	log(scene, Message(target, performed, additional, prevData, newData));
}

void Logger::log(const Id scene, const Message data)
{
	QString message = msgOperation;
	switch (data.performed()) {
		case actSetData:
			if ((!flagsEnabled[flgUselessMessages]) &&
				((data.details() == QString("position")) ||
				 (data.details() == QString("configuration"))))
				return;
			cleanDiagrams.remove(scene);
			message += msgSetData;
			break;
		case actAddElement:
			cleanDiagrams.remove(scene);
			message += msgAddElement;
			break;
		case actRemoveElement:
			cleanDiagrams.remove(scene);
			message += msgRemoveElement;
			break;
		case actCreateDiagram:
			message += msgCreateDiagram;
			break;
		case actDestroyDiagram:
			if (cleanDiagrams.contains(scene)) {
				cleanDiagrams.remove(scene);
				remove(scene);
				return;
			}
			cleanDiagrams.insert(scene);
			message += msgDestroyDiagram;	//unused?
			break;
	}

	message += "\n";
	if (data.target().idSize() > 1)
		message += msgTarget + data.target().toString() + "\n";
	if (!data.details().isNull())
		message += msgDetails + data.details() + "\n";
	if (!data.prevValue().isNull())
		message += msgPrevValue + Message::getDataString(data.prevValue()) + "\n";
	if (!data.newValue().isNull())
		message += msgNewValue + Message::getDataString(data.newValue()) + "\n";

	if (!buffer.contains(scene))
		buffer.insert(scene, new QString(message));
	else
		buffer.value(scene)->append("\n"+message);
}

void Logger::output()
{
	foreach(Id scene, cleanDiagrams)
		remove(scene);
	foreach(Id scene, buffer.keys())
		write(*buffer.value(scene), scene);
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
	else {
		qDebug() << "Serializer::logRemove() error: no such file.";
		return;
	}
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

	QString path = workingDir+"/logs/"+scene.diagram();
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
