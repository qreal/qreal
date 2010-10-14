#include "model.h"
#include "logger.h"

#include <QDebug>

using namespace qReal;
using namespace model;

Logger::Logger(Model *model)
	: mModel(model), enabled(false),
	msgInvalid("Invalid"),
	msgSetData("SetData"),
	msgAddElement("AddElement"),
	msgRemoveElement("RemoveElement"),
	msgCreateDiagram("CreateDiagram"),
	msgDestroyDiagram("DestroyDiagram")
{
	flagsEnabled[editors] = true;
	flagsEnabled[diagrams] = true;
	flagsEnabled[uselessMessages] = false;
	flagsEnabled[invalidMessages] = false;
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

void Logger::setFlag(flag name, bool arg)
{
	flagsEnabled[name] = arg;
}

void Logger::log(action performed,
							const Id scene)
{
	if ((performed != createDiagram) && (performed != destroyDiagram)) {
		write(msgInvalid +"\n",scene);
		if (flagsEnabled[invalidMessages])
			log(performed, scene, Id(), QVariant(), QVariant(), QString());
	} else
		log(performed, scene, Id(), QVariant(), QVariant(), QString());
}

void Logger::log(action performed,
					const Id scene, const Id target)
{
	if ((performed != addElement) && (performed != removeElement)) {
		write(msgInvalid + "\n",scene);
		if (flagsEnabled[invalidMessages])
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

	QString message = "Operation: ";
	switch (performed) {
		case setData:
			if ((!flagsEnabled[uselessMessages]) &&
				((additional == QString("position")) ||
				 (additional == QString("configuration"))))
				return;
			cleanDiagrams.remove(scene);
			message += msgSetData;
			break;
		case addElement:
			cleanDiagrams.remove(scene);
			message += msgAddElement;
			break;
		case removeElement:
			cleanDiagrams.remove(scene);
			message += msgRemoveElement;
			break;
		case createDiagram:
			message += msgCreateDiagram;
			break;
		case destroyDiagram:
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
	if (target.idSize() > 1)
		message += "Target: " + target.toString() + "\n";
	if (!additional.isNull())
		message += "Details: " + additional + "\n";
	if (!prevData.isNull())
		message += "PrevValue: " + getDataString(prevData) + "\n";
	if (!newData.isNull())
		message += "NewValue: " + getDataString(newData) + "\n";

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

bool Logger::pass(const Id scene)
{
	const bool editor = scene.editor() == QString("Meta_editor");
	return
		((!editor && flagsEnabled[diagrams])
			|| (editor && flagsEnabled[editors]));
}

void Logger::remove(const Id scene)
{
	buffer.remove(scene);
	mModel->mutableApi().logRemove(scene);
}

void Logger::write(const QString message, const Id scene)
{
	if (!enabled)
		return;
	mModel->mutableApi().log(message, scene);
}

QString Logger::getDataString(const QVariant data) const
{
	QString output;
	QDebug qD = QDebug(&output);
	qD << data;
	return output;
}
