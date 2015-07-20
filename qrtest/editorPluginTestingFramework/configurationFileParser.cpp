#include "configurationFileParser.h"
#include "defs.h"

#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include "qrutils/xmlUtils.h"

using namespace editorPluginTestingFramework;

void ConfigurationFileParser::parseConfigurationFile(const QString &configurationFile)
{
	mXml = utils::xmlUtils::loadDocument(configurationFile);

	mQmakeParameter = valueByTag("qmake");
	mMakeParameter = valueByTag("make");
	mConfigurationParameter = valueByTag("configuration");
	mPluginExtension = valueByTag("pluginExtension");
	mPrefix = valueByTag("prefix");
	mQRealRootPath = valueByTag("qrealRoot");
	mGenerateHtml = valueByTag("generateHtml");
	mGeneratedCodeDirQrxc = valueByTag("generatedCodeDirQrxc");
	mGeneratedCodeDirQrmc = valueByTag("generatedCodeDirQrmc");
	mGeneratedDirHtml = valueByTag("generatedDirHtml");
}

QString ConfigurationFileParser::valueByTag(const QString &tag) const
{
	QDomNode node = mXml.elementsByTagName(tag).at(0);
	return node.toElement().text();
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

QString ConfigurationFileParser::qRealRootPath() const
{
	return mQRealRootPath;
}

QString ConfigurationFileParser::htmlGenerationParameter() const
{
	return mGenerateHtml;
}

QString ConfigurationFileParser::generatedCodeDirQrxc() const
{
	return mGeneratedCodeDirQrxc;
}
QString ConfigurationFileParser::generatedCodeDirQrmc() const
{
	return mGeneratedCodeDirQrmc;
}

QString ConfigurationFileParser::generatedDirHtml() const
{
	return mGeneratedDirHtml;
}
