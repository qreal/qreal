#include "model.h"
#include "logger.h"

#include <QDebug>

using namespace qReal;
using namespace model;

Logger::Logger(Model *model)
	: mModel(model), enabled(false)
{
	flagsEnabled[editors] = true;
	flagsEnabled[diagrams] = true;
	flagsEnabled[uselessMessages] = false;
	flagsEnabled[invalidMessages] = false;
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
	if ((performed != createDiagram) && (performed != destroyDiagram))
		write("#InvalidMessage\n",scene);
	else if (flagsEnabled[invalidMessages])
		log(performed, scene, Id(), QVariant(), QString());
}

void Logger::log(action performed,
					const Id scene, const Id target)
{
	if ((performed != addElement) && (performed != removeElement))
		write("#InvalidMessage\n",scene);
	else if (flagsEnabled[invalidMessages])
		log(performed, scene, target, QVariant(), QString());
}

void Logger::log(action performed,
				 const Id scene, const Id target,
				 const QVariant data, const QString additional)
{
	QString message;
	switch (performed) {
		case setData:
			message += QString("SetData");
			if ((!flagsEnabled[uselessMessages]) &&
			//may be, "name" messages are useless too
				((additional == QString("position")) ||
				 (additional == QString("configuration"))))
				return;
			break;
		case addElement:
			message += QString("AddElement");
			break;
		case removeElement:
			message += QString("RemoveElement");
			break;
		case createDiagram:
			message += QString("CreateDiagram");
			break;
		case destroyDiagram:
			message += QString("DestroyDiagram");
			break;
	}

	message += "\n";
	if (target.idSize() > 1)
		message += target.toString() + "\n";
	if (!additional.isNull())
		message += additional + "\n";
	if (!data.isNull())
		message += getDataString(data) + "\n";

	write(message, scene);
}

QString Logger::getDataString(const QVariant data) const
{
	QString output;
	QDebug qD = QDebug(&output);
	qD << data;
	return output;
}

void Logger::write(const QString message, const Id scene)
{
	if (!enabled)
		return;
	mModel->mutableApi().log(message, scene);
}
