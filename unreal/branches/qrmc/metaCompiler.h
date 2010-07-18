#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QList>

#include "plugin.h"

class MetaCompiler
{
public:
	MetaCompiler(QString const &workingCopyDir);
	~MetaCompiler();
	bool compile();

private:
	bool init();
	bool initPlugins();
	bool loadTemplates();
	bool loadPluginHeaderTemplate();
	bool loadPluginSourceTemplate();
	bool loadTemplateUtils();

	bool generateCode();

	bool changeDir(QString const &dir);
	void makeDir(QString const &dir);

	qrRepo::RepoApi mApi;
	QList<Plugin*> mPlugins;

	QString mPluginHeaderTemplate;
	QString mPluginSourceTemplate;
	QMap<QString, QString> mTemplateUtils;

	QDir mDirectory;
};
