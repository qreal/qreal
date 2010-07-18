#pragma once

#include <QtCore/QString>
#include "../../trunk/qrrepo/repoApi.h"

class PluginHeader
{
public:
	PluginHeader(QString const &pluginName);
	~PluginHeader();

	bool generate(QString const &headerTemplate);
	void init(qrRepo::RepoApi &repo);

private:
	QString mName;
};
