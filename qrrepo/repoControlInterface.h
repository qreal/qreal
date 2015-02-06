#pragma once

#include "../qrkernel/roles.h"

namespace qrRepo {

/// Provides repository control methods, like save or open saved contents.
class RepoControlInterface
{
public:
	virtual ~RepoControlInterface() {}

	virtual void exterminate() = 0;

	/// virtual, for returning IdList of elements that names contains input string
	/// @param name - string that should be contained by names of elements that Id's are in the output list
	virtual qReal::IdList findElementsByName(const QString &name, bool sensitivity, bool regExp) const = 0;

	/// virtual, for returning IdList of elements that have input property
	/// @param name - string that should be contained by names of elements that have input property
	virtual qReal::IdList elementsByProperty(const QString &property, bool sensitivity, bool regExp) const = 0;

	/// virtual, for returning IdList of elements that have input property content
	/// @param name - string that should be contained by names of elements that have input property content
	virtual qReal::IdList elementsByPropertyContent(const QString &propertyContent, bool sensitivity, bool regExp) const = 0;

	/// virtual, for import *.qrs file into current project
	/// @param importedFile - file to be imported
	virtual void importFromDisk(const QString &importedFile) = 0;
	virtual void saveAll() const = 0;
	virtual void save(const qReal::IdList &list) const = 0;
	virtual void saveTo(const QString &workingFile) = 0;

	/// exports repo contents to a single XML file
	virtual void exportToXml(const QString &targetFile) const = 0;

	/// saves choosen diagrams to target directory and file
	/// @param diagramIds - map of the following structure:
	/// key is a file path to save into, value is a list of diagrams to save
	virtual void saveDiagramsById(QHash<QString, qReal::IdList> const &diagramIds) = 0;

	virtual void open(const QString &workingFile) = 0;

	/// Returns current working file name, to which model is saved
	virtual QString workingFile() const = 0;
};

}
