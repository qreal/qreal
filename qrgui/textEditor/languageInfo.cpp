#include "languageInfo.h"

using namespace qReal::text;

QList<LanguageInfo> Languages::mUserDefinedLanguages;

QList<LanguageInfo> Languages::knownLanguages()
{
	return mUserDefinedLanguages + QList<LanguageInfo>({
			// Append here all languages declared in Languages class
			c(), russianC(), python(), qtScript(), fSharp()
	});
}

void Languages::registerLanguage(LanguageInfo const &language)
{
	mUserDefinedLanguages << language;
}

LanguageInfo Languages::pickByExtension(QString const &extension)
{
	for (LanguageInfo const &language : knownLanguages()) {
		if (language.extension == extension) {
			return language;
		}
	}

	return textFileInfo(extension);
}
