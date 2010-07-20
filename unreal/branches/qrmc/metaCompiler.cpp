#include <QtCore/QFile>
#include <QtCore/QDebug>

#include "metaCompiler.h"
#include "nameNormalizer.h"
#include "defs.h"

using namespace qReal;

MetaCompiler::MetaCompiler(QString const &workingCopyDir) : mApi(workingCopyDir)
{
	mApi.setName(ROOT_ID, ROOT_ID.toString());
}

MetaCompiler::~MetaCompiler()
{
	foreach(Plugin* plugin, mPlugins)
		delete plugin;

}

bool MetaCompiler::compile()
{
	return init() && generateCode();
}

bool MetaCompiler::init()
{
	return initPlugins() && loadTemplates();
}

bool MetaCompiler::initPlugins()
{
	QString pluginName = "";
	qDebug() << mApi.children(ROOT_ID).size();
	if (mApi.children(ROOT_ID).isEmpty())
		qDebug() << "couldn't load any root diagrams";
	foreach(qReal::Id editorId, mApi.children(ROOT_ID)) {
		if (editorId.element() == "MetamodelDiagram") {
			pluginName = NameNormalizer::normalize(mApi.property(editorId, "name of the directory").toString().section("/", -1));
			Plugin *plugin = new Plugin(editorId, pluginName);
			plugin->init(mApi);
			mPlugins << plugin;
		}
	}
	return true;
}

bool MetaCompiler::loadTemplates()
{
	return loadPluginHeaderTemplate() && loadPluginSourceTemplate() && loadTemplateUtils();
}

bool MetaCompiler::changeDir(const QString &path)
{
	if (!mDirectory.exists(path)) {
		qDebug() << "cannot find directory \"" << path << "\"";
		return false;
	}
	mDirectory.cd(path);
	return true;
}

bool MetaCompiler::loadPluginHeaderTemplate()
{
	if (!changeDir(templatesDir))
		return false;

	QString fileName = mDirectory.absoluteFilePath(pluginHeaderTemplate);
	QFile pluginHeaderFile(fileName);
	if (!pluginHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << fileName << "\"";
		return false;
	}
	QTextStream in(&pluginHeaderFile);
	mPluginHeaderTemplate = in.readAll();

	pluginHeaderFile.close();
	mDirectory.cdUp();

	return true;
}

bool MetaCompiler::loadPluginSourceTemplate()
{
	if (!changeDir(templatesDir))
		return false;

	QString fileName = mDirectory.absoluteFilePath(pluginSourceTemplate);
	QFile pluginSourceFile(fileName);
	if (!pluginSourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << fileName << "\"";
		return false;
	}
	QTextStream in(&pluginSourceFile);
	mPluginSourceTemplate = in.readAll();

	pluginSourceFile.close();
	mDirectory.cdUp();

	return true;
}

bool MetaCompiler::loadTemplateUtils()
{
	if (!changeDir(templatesDir))
		return false;

	QString fileName = mDirectory.absoluteFilePath(utilsTemplate);
	QFile utilsFile(fileName);
	if (!utilsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << fileName << "\"";
		return false;
	}
	QTextStream in(&utilsFile);
	QString line = in.readLine();
	while (!line.isNull()) {
		QString name = line.section("=", 0, 0);
		QString definition = line.section("=", 1);
		mTemplateUtils[name] = definition;
		line = in.readLine();
	}

	utilsFile.close();
	mDirectory.cdUp();

	return true;
}

bool MetaCompiler::generateCode()
{
	foreach(Plugin *plugin, mPlugins)
		plugin->generate(mPluginHeaderTemplate, mPluginSourceTemplate, mTemplateUtils);
	return true;
}
