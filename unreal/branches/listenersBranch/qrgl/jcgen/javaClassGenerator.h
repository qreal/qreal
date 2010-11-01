#include <QString>
#include <QList>

#include "../../qrrepo/repoApi.h"

class JavaClassGenerator {
	public:
		JavaClassGenerator(const qrRepo::RepoApi&);
		bool generateClass(qReal::Id classElemId /*QString pathWhereMustBeCreatedFile*/);

	private:
		bool loadTemplateFromFile(const QString& filename, QString& loadedTemplate);
		bool insertStrToTemplate(const QString& newStr, const QString& placeIndentificator, QString& templateString);

		qrRepo::RepoApi& rApi;
};
