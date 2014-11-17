#pragma once

#include <QtCore/QObject>

#include <qrgui/versioning/diffPluginBase.h>

namespace versioning
{

class DiffPlugin : public QObject, public qReal::DiffPluginBase
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.versioning.diffPlugin")

public:
	DiffPlugin();
	virtual ~DiffPlugin() {}
	QList<qReal::ActionInfo> actions();

private slots:
	void diffClicked();
	void diffBetweenClicked();

private:
	QList<qReal::ActionInfo> mMenu;
};

}
