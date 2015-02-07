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

void Languages::registerLanguage(const LanguageInfo &language)
{
	mUserDefinedLanguages << language;
}

LanguageInfo Languages::pickByExtension(const QString &extension)
{
	for (const LanguageInfo &language : knownLanguages()) {
		if (language.extension == extension) {
			return language;
		}
	}

	return textFileInfo(extension);
}
