#include "model.h"
#include "logger.h"

#include <QDebug>

using namespace qReal;
using namespace model;

Logger::Logger(Model *model)
	: mModel(model), enabled(false)
{
}

void Logger::enable()
{
	enabled = true;
}

void Logger::disable()
{
	enabled = false;
}

void Logger::log(const action performed,
							const Id scene)
{
	log(performed, scene, Id(), QVariant(), QString());
}

void Logger::log(const action performed,
					const Id scene, const Id target)
{
	log(performed, scene, target, QVariant(), QString());
}

void Logger::log(const action performed, const Id scene,
						const Id target, const QVariant data)
{
	log(performed, scene, target, data, QString());
}

void Logger::log(const action performed,
				 const Id scene, const Id target,
				 const QVariant data, const QString additional)
{
	QString message;
	switch (performed) {
		case setData:
			message += QString("SetData");
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
