/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "languageInfo.h"

using namespace qReal::text;

QList<LanguageInfo> Languages::mUserDefinedLanguages;

QList<LanguageInfo> Languages::knownLanguages()
{
	return mUserDefinedLanguages + QList<LanguageInfo>({
			// Append here all languages declared in Languages class
			c(), russianC(), python(), qtScript(), javaScript(), fSharp()
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
