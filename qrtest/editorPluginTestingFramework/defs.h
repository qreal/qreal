#pragma once
#include <QtCore/QString>

namespace editorPluginTestingFramework {
///plugins /sources
QString const sourcesDir = "/sources";
QString const pluginsDir = "/plugins";

QString const pathToQrmcGeneratedCode = "";
QString const pathToQrxcGeneratedCode = "";

QString const pathToQrmcGeneratedPlugin = "/plugins";
QString const pathToQrxcGeneratedPlugin = "/plugins";

QString const pathToQRealRootFromQrmc = "../";
QString const pathToQRealRootFromQrxc = "../../";

/// Todo: remove qrtest/binaries from this part
/// debug/plugins/editors/ /plugins/editors/qrmc
QString const destDirForQrmc = "";
QString const destDirForQrxc = "";

QString const pathToQrxcQrmcMethodsToTest = "methodsToTestQrxcQrmc.txt";
QString const pathToQrxcInterpreterMethodsToTest = "methodsToTestInterpQrxc.txt";

QString const configurationFileName = "configurationParameters.xml";
QString const travisConfigurationFileName = "travisConfigurationParameters.xml";

QString const pathToTestMetamodel = "../qrtest/editorPluginTestingFramework/fileToTestWithTravis";

QString const tempValueForSettingsManager = "/unsaved";

QString const stringSeparator = "-----------------------------------";
}
