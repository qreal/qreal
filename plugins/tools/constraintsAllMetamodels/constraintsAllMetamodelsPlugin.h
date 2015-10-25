#pragma once

#include <qrgui/plugins/editorPluginInterface/editorInterface.h>
#include "constraintsAllLanguages.h"

namespace constraints {

class ConstraintsPlugin : public QObject, public qReal::ConstraintsPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ConstraintsPluginInterface)
	Q_PLUGIN_METADATA(IID "constraints::ConstraintsPlugin")

public:
	ConstraintsPlugin();
	~ConstraintsPlugin() override;

	QList<qReal::CheckStatus> check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager) override;
	QString metamodelName() const override;
	QString id() const override;

private:
	ConstraintsAllLanguages mConstraintsAllLanguages;

};

}
