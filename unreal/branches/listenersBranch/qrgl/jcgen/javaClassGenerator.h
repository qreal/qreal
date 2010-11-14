#include <QString>
#include <QList>

#include "../../qrrepo/repoApi.h"

class JavaClassGenerator {
	public:
		JavaClassGenerator(const QString& repoDirectory);
		bool generateClass(const qReal::Id& classElemId /*, QString pathWhereMustBeCreatedFile*/);
		bool generateAllRepoClasses();

	private:
		bool loadTemplateFromFile(const QString& filename, QString& loadedTemplate);
		bool insertStrToTemplate(const QString& newStr, const QString& placeIndentificator, QString& templateString);
		
		//Похожа на insertStrToTemplate, но вместо замены placeIndentificator на newStr в шаблоне, добовляет newStr
		//после placeIndentificator
		bool appendAfterTemplatePlace(const QString& newStr, const QString& placeIndentificator, QString& templateString);

		bool generateFieldString(const qReal::Id& fieldElemId, const QString& fieldTemplate, QString& resultFieldString);
		bool generateMethodString(const qReal::Id& methodElemId, const QString& methodTemplate, QString& resultMethodString);

		qrRepo::RepoApi rApi;
};
