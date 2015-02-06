#pragma once

#include <QtCore/QString>

#include "utilsDeclSpec.h"

namespace utils {

/// Helper class that is meant to transform arbitrary string to a correct C++/C# identifier.
class QRUTILS_EXPORT NameNormalizer
{
public:
	/// Makes from given name correct C++/C# identifier.
	/// @param name String to transform to an identifier.
	/// @param upperFirst Makes first letter capital when true or small when false.
	/// @return Transformed string.
	static QString normalize(const QString &name, const bool upperFirst = true);

	/// Makes from given name correct C++/C# identifier. Illegal characters would
	/// be thrown away
	/// @param name String to transform to an identifier.
	/// @param upperFirst Makes first letter capital when true or small when false.
	/// @return Transformed string.
	static QString normalizeStrongly(const QString &name, const bool upperFirst = true);

private:
	/// Makes all letters that start words capital and replaces spaces with underscore,
	/// for example tranforms "some text" to "Some_Text".
	static QString upperFirst(const QString &string);

	/// Makes all letters that start words lower and replaces spaces with underscore,
	/// for example tranforms "some text" to "some_text".
	static QString lowerFirst(const QString &string);

	/// Spells given id in translit-style (e.g. 'идентификатор' -> 'identificator')
	static QString russianTranslit(const QString &russianString);
};

}
