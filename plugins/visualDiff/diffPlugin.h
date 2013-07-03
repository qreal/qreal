#pragma once

#include <QtCore/QObject>

#include "../../qrgui/versioning/diffPluginBase.h"

namespace versioning
{

class DiffPlugin : public QObject, public qReal::DiffPluginBase
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	virtual ~DiffPlugin() {}

	QList<qReal::ActionInfo> actions();

private slots:
	void diffClicked();
	void diffBetweenClicked();
};

}
