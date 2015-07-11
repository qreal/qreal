#include "qrmcLauncher.h"
#include "qrmc/metaCompiler.h"
#include "defs.h"

using namespace qReal;
using namespace qrmc;
using namespace qrRepo;
using namespace editorPluginTestingFramework;

void QrmcLauncher::launchQrmc(const QString &fileName, const QString &pathToQrmc, const QString &pathToGeneratedCode)
{
	qDebug() << "STARTING QRMC LAUNCHING";
	QString normalizedFileName = fileName;
	if (!fileName.contains(".qrs")) {
		normalizedFileName += ".qrs";
	}

	RepoApi *const mRepoApi = new RepoApi(normalizedFileName);
	MetaCompiler metaCompiler(pathToQrmc, mRepoApi);
	IdList const metamodels = mRepoApi->children(Id::rootId());

	foreach (Id const &key, metamodels) {
		const QString &objectType = key.element();
		if (objectType == "MetamodelDiagram" && mRepoApi->isLogicalElement(key)) {
			const QString &nameOfMetamodel = mRepoApi->stringProperty(key, "name");

			if (!metaCompiler.compile(nameOfMetamodel)) {
				qDebug() << "compilation failed";
			}
		}
	}

	qDebug() << stringSeparator;
}
