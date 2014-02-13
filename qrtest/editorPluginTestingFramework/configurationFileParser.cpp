#include "configurationFileParser.h"
#include "defs.h"

#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include "qrutils/xmlUtils.h"

using namespace editorPluginTestingFramework;

void ConfigurationFileParser::parseConfigurationFile(QString const &configurationFile)
{
	mXml = utils::xmlUtils::loadDocument(pathToSourceCode + configurationFile);

	mQmakeParameter = valueByTag("qmake");
	mMakeParameter = valueByTag("make");
	mConfigurationParameter = valueByTag("configuration");
	mPluginExtension = valueByTag("pluginExtension");
	mPrefix = valueByTag("prefix");
}

QString ConfigurationFileParser::valueByTag(QString const &tag) const
{
	QDomNode node = mXml.elementsByTagName(tag).at(0);
	QString const &value = node.toElement().text();

	return value;
}

QString ConfigurationFileParser::qmakeParameter() const
{
	return mQmakeParameter;
}

QString ConfigurationFileParser::makeParameter() const
{
	return mMakeParameter;
}

QString ConfigurationFileParser::configurationParameter() const
{
	return mConfigurationParameter;
}

QString ConfigurationFileParser::pluginExtension() const
{
	return mPluginExtension;
}

QString ConfigurationFileParser::prefix() const
{
	return mPrefix;
}
