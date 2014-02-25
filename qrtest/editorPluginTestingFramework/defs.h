#pragma once
#include <QtCore/QString>

namespace editorPluginTestingFramework {

QString const sourcesDir = "/sources";
QString const pluginsDir = "/plugins";

QString const pathToQrmcGeneratedCode = "/sources/qrmc";
QString const pathToQrxcGeneratedCode = "/sources/qrxc";

QString const pathToQrmcGeneratedPlugin = "/plugins/qrmc";
QString const pathToQrxcGeneratedPlugin = "/plugins/qrxc";

QString const pathToQRealRootFromQrmc = "../../../../../";
QString const pathToQRealRootFromQrxc = "../../../../";

/// Todo: remove qrtest/binaries from this part
QString const destDirForQrmc = "qrtest/binaries/plugins/qrmc";
QString const destDirForQrxc = "qrtest/binaries/plugins/qrxc";

QString const pathToQrxcQrmcMethodsToTest = "methodsToTestQrxcQrmc.txt";
QString const pathToQrxcInterpreterMethodsToTest = "methodsToTestInterpQrxc.txt";

QString const configurationFileName = "configurationParameters.xml";
QString const travisConfigurationFileName = "travisConfigurationParameters.xml";

QString const pathToTestMetamodel = "../qrtest/editorPluginTestingFramework/fileToTestWithTravis";

QString const tempValueForSettingsManager = "/unsaved";

QString const stringSeparator = "-----------------------------------";
}
