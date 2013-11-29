#pragma once

#include <QtCore/QObject>
#include <QtCore/QProcess>

#include "update.h"

class UpdatesInstaller : public QObject
{
	Q_OBJECT
public:
	explicit UpdatesInstaller(QObject *parent = 0);

signals:

public slots:

};

