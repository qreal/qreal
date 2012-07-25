#pragma once

#include "../qrkernel/roles.h"

namespace qrRepo {

class RepoControlInterface
{
public:
	virtual ~RepoControlInterface() {}

	virtual void exterminate() = 0;

	/// virtual, for returning IdList of elements that names contains input string
	/// @param name - string that should be contained by names of elements that Id's are in the output list
	virtual qReal::IdList findElementsByName(QString const &name, bool sensitivity, bool regExp) const = 0;

	/// virtual, for returning IdList of elements that have input property
	/// @param name - string that should be contained by names of elements that have input property
	virtual qReal::IdList elementsByProperty(QString const &property, bool sensitivity, bool regExp) const = 0;

	/// virtual, for returning IdList of elements that have input property content
	/// @param name - string that should be contained by names of elements that have input property content
	virtual qReal::IdList elementsByPropertyContent(QString const &propertyContent, bool sensitivity, bool regExp) const = 0;

	/// virtual, for import *.qrs file into current project
	/// @param importedFile - file to be imported
	virtual void importFromDisk(QString const &importedFile) = 0;
	virtual void saveAll() const = 0;
	virtual void save(qReal::IdList list) const = 0;
	virtual void saveTo(QString const &workingFile) = 0;
	/// saves choosen diagrams to target directory and file
	/// @param diagramIds - map of the following structure:
	/// key is a file path to save into, value is a list of diagrams to save
	virtual void saveDiagramsById(QHash<QString, qReal::IdList> const &diagramIds) = 0;

	virtual void open(QString const &workingFile) = 0;

	/// Returns current working file name, to which model is saved
	virtual QString workingFile() const = 0;
};

}
