#pragma once

#include <QTcpSocket>
#include "../common/defs.h"
#include "../common/classes.h"
#include "../common/realrepoapiclasses.h"

class XmiHandler : public QObject
{
	Q_OBJECT;

public:
	XmiHandler(QString const &addr, const int port);

	void exportToXmi(QString const &pathToFile);
private:
	RealRepoClient *client;

	QString serializeObject(QString const &id);
	QString serializeChildren(QString const &id);
	QString serializeOutcomingLink(QString const &id);
	QString serializeIncomingLink(QString const &id);
	QString serializeLinkBodies(QString const &id);
	QString serializeLink(QString const &id);

	QPair<QString, QString> multiplicityValues(QString const &multiplicity) const;
};
