#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>

namespace editorPluginTestingFramework {
class ConfigurationFileParser
{
public:
	/// parses configuration file
	void parseConfigurationFile(QString const &configurationFile);

	/// returns qmake parameter from configuration file
	QString qmakeParameter() const;
	/// returns make parameter from configuration file
	QString makeParameter() const;
	/// returns configuration parameter (release or debug)
	QString configurationParameter() const;
	/// returns generated plugin extension (dll, so...)
	QString pluginExtension() const;
	/// returns prefix for generated plugin name (lib)
	QString prefix() const;
	/// returns path to QReal root
	QString qRealRootPath() const;
	/// returns "yes" if we want to generate html with testing results and "no" otherwise
	QString htmlGenerationParameter() const;
	/// returns path to generated binaries and plugins
	QString generatedCodeDirQrxc() const;
	QString generatedCodeDirQrmc() const;

	QString generatedDirHtml() const;


private:
	/// given tag, returns tag value from configuration file
	QString valueByTag(QString const &tag) const;

	QString mQmakeParameter;
	QString mMakeParameter;
	QString mConfigurationParameter;
	QString mPluginExtension;
	QString mPrefix;
	QString mQRealRootPath;
	QString mGenerateHtml;
	QString mGeneratedCodeDirQrxc;
	QString mGeneratedCodeDirQrmc;
	QString mGeneratedDirHtml;

	QDomDocument mXml;
};

}
