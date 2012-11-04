#include "qrmcLauncher.h"
#include "../../qrmc/metaCompiler.h"

using namespace qReal;
using namespace qrmc;
using namespace qrRepo;

QrmcLauncher::QrmcLauncher()
{
}

void QrmcLauncher::launchQrmc(QString fileName, QString const directoryToGeneratedCode, QString const pathToQrmc)
{
	//pathToQrmc = qreal-testtool/qrmc (от bin считается)
	//fileName = .qrs (тоже от bin)
	//directoryToGeneratedCode = "../qrtest/editorPluginTestingFramework/plugins"
	//pathToQrealRoot = ../../../../ от сгенеренного кода
	if (!fileName.contains(".qrs")) {
		fileName += ".qrs";
	}
	qrRepo::RepoApi *mRepoApi = new qrRepo::RepoApi(fileName);
	MetaCompiler metaCompiler(pathToQrmc, mRepoApi);
	IdList const metamodels = mRepoApi->children(Id::rootId());

	foreach (Id const &key, metamodels) {
		QString const objectType = key.element();
		if (objectType == "MetamodelDiagram" && mRepoApi->isLogicalElement(key)) {
			QString nameOfMetamodel = mRepoApi->stringProperty(key, "name");
			QString pathToQrealRoot = "../../../../";
			if (!metaCompiler.compile(nameOfMetamodel, pathToQrealRoot, directoryToGeneratedCode)) {
				qDebug() << "compilation failed";
			}
		}
	}
}
