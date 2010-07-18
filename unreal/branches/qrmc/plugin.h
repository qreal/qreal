#pragma once

#include <QtCore/QString>
#include <QtCore/QDebug>
#include "pluginHeader.h"
#include "pluginSource.h"
#include "../../trunk/qrrepo/repoApi.h"

class Plugin
{
public:
	Plugin(qReal::Id id, QString const &name);
	~Plugin();

	void init(qrRepo::RepoApi &repo);
	void generate(QString const &headerTemplate, QString const &sourceTemplate, QMap<QString, QString> const &utils);

private:
	qReal::Id mMetamodelId;
	QString mName;
	PluginHeader mHeader;
	PluginSource mSource;
//	QString mHeaderTemplate;
//	QString mSourceTemplate;
};
