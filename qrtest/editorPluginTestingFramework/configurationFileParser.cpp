#include "configurationFileParser.h"
#include "defs.h"

#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include "../../qrutils/xmlUtils.h"

using namespace editorPluginTestingFramework;

void ConfigurationFileParser::parseConfigurationFile()
{
	mXml = utils::xmlUtils::loadDocument(pathToSourceCode + configurationFileName);

	mQmakeParameter = valueByTag("qmake");
	mMakeParameter = valueByTag("make");
	mConfigurationParameter = valueByTag("configuration");
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
