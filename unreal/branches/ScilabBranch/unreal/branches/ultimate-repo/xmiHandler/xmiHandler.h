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

	QString exportToXmi(QString const &pathToFile);
private:
	RealRepoClient *client;
	QString errorText;


	QString serializeObject(QString const &id, QString const &parentId);
	QString serializeChildren(QString const &id);
	QString serializeOutcomingLink(QString const &id);
	QString serializeIncomingLink(QString const &id);
	QString serializeLinkBodies(QString const &id);
	QString serializeLink(QString const &id);
	QString initPrimitiveTypes() const;

	QString serializeMultiplicity(QString const &id, QString const &multiplicity) const;
	bool isTypeSuitable(QString const &type) const;
	bool isVisibilitySuitable(QString const &type) const;

	void addError(QString const &errorText);
};
