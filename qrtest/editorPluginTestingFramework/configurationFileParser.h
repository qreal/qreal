#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>

namespace editorPluginTestingFramework {
class ConfigurationFileParser
{
public:
	void parseConfigurationFile(QString const &configurationFile);

	QString qmakeParameter() const;
	QString makeParameter() const;
	QString configurationParameter() const;
	QString pluginExtension() const;

private:
	QString valueByTag(QString const &tag) const;

	QString mQmakeParameter;
	QString mMakeParameter;
	QString mConfigurationParameter;
	QString mPluginExtension;

	QDomDocument mXml;
};

}
