#pragma once

#include <QtCore/QHash>

#include "../../qrkernel/definitions.h"
#include "../../qrkernel/ids.h"
#include "classes/graphicalObject.h"
#include "classes/logicalObject.h"
#include "qrRepoGlobal.h"
#include "serializer.h"

namespace qrRepo {
namespace details {

/// Class that actually contains all data, supports low-level queries and can serialize/deserialize data.
class Repository
{
public:
	QRREPO_EXPORT Repository(QString const &workingFile);
	QRREPO_EXPORT virtual ~Repository();

	/// replacing property values that contains input value with new value
	/// @param toReplace - id list that contains ids of elements that properties should be replaced
	/// @param value - input value that should be contained by any property of each element
	/// @param newValue - string representation of value with what property values should be replaced
	void replaceProperties(qReal::IdList const &toReplace, QString const value, QString const newValue);

	/// returning IdList of elements that names contains input string
	/// @param name - string that should be contained by names of elements that Id's are in the output list
	qReal::IdList findElementsByName(QString const &name, bool sensitivity, bool regExpression) const;

	/// Returns IdList of elements that have given property
	/// @param name - string that should be contained by names of elements that have input property
	qReal::IdList elementsByProperty(QString const &property, bool sensitivity, bool regExpression) const;

	/// returning IdList of elements that have input property content
	/// @param name - string that should be contained by names of elements that have input property content
	qReal::IdList elementsByPropertyContent(QString const &property, bool sensitivity, bool regExpression) const;

	qReal::IdList children(const qReal::Id &id) const;
	qReal::Id parent(const qReal::Id &id) const;
	/**
	  Clones object recursively in internal structures.
	  This doesn't mean it'll appear on your diagrams
	  */
	qReal::Id cloneObject(const qReal::Id &id);
	void setParent(const qReal::Id &id, const qReal::Id &parent);
	void addChild(const qReal::Id &id, const qReal::Id &child);
	void addChild(const qReal::Id &id, const qReal::Id &child, qReal::Id const &logicalId);
	void removeParent(const qReal::Id &id);
	void remove(const qReal::Id &id);
	void removeChild(const qReal::Id &id, const qReal::Id &child);

	/// Stacks element child before sibling (element id shold be parent of them both)
	void stackBefore(qReal::Id const &id, qReal::Id const &child, qReal::Id const &sibling);

	void setProperty(const qReal::Id &id, QString const &name, const QVariant &value) const;
	void copyProperties(const qReal::Id &dest, const qReal::Id &src);
	QVariant property(const qReal::Id &id, QString const &name) const;
	QMap<QString, QVariant> properties(qReal::Id const &id);
	void setProperties(qReal::Id const &id, QMap<QString, QVariant> const &properties);
	bool hasProperty(const qReal::Id &id, QString const &name, bool sensitivity = false
			, bool regExpression = false) const;
	void removeProperty(const qReal::Id &id, QString const &name);
	QMapIterator<QString, QVariant> propertiesIterator(qReal::Id const &id) const;

	void setBackReference(qReal::Id const &id, qReal::Id const &reference) const;
	void removeBackReference(qReal::Id const &id, qReal::Id const &reference) const;

	void setTemporaryRemovedLinks(qReal::Id const &id, QString const &direction, qReal::IdList const &linkIdList);
	qReal::IdList temporaryRemovedLinksAt(qReal::Id const &id, QString const &direction) const;
	qReal::IdList temporaryRemovedLinks(qReal::Id const &id) const;
	void removeTemporaryRemovedLinks(qReal::Id const &id);

	qReal::IdList elements() const;
	bool isLogicalId(qReal::Id const &elem) const;
	qReal::Id logicalId(qReal::Id const &elem) const;

	void printDebug() const;

	void exterminate();
	void open(QString const &saveFile);

	bool exist(qReal::Id const &id) const;

	/// Opens file into existing project
	/// @param importedFile - name of file to be imported
	void importFromDisk(QString const &importedFile);

	void saveAll() const;
	void save(qReal::IdList const &list) const;
	void saveWithLogicalId(qReal::IdList const &list) const;
	void saveDiagramsById(QHash<QString, qReal::IdList> const &diagramIds);
	void remove(qReal::IdList list) const;
	void setWorkingFile(QString const &workingDir);
	void exportToXml(QString const &targetFile) const;

	/// Returns current working file name
	QString workingFile() const;

	/// Creates empty graphical part with given index inside given object.
	/// @param id - id of an object where we shall create graphical part.
	/// @param partIndex - index of created part in given object.
	virtual void createGraphicalPart(qReal::Id const &id, int partIndex);

	/// Returns a list of indexes of graphical parts for given element.
	virtual QList<int> graphicalParts(qReal::Id const &id) const;

	/// Returns the value of graphical part property of a given object.
	/// @param id - id of an object where graphical part is located.
	/// @param partIndex - index of a graphical part.
	/// @param propertyName - name of a property which value we want to get.
	virtual QVariant graphicalPartProperty(qReal::Id const &id, int partIndex, QString const &propertyName) const;

	/// Sets the value of graphical part property of a given object. If a property already exists, its value
	/// will be overwritten, otherwise new property will be created with given value.
	/// @param id - id of an object where graphical part is located.
	/// @param partIndex - index of a graphical part.
	/// @param propertyName - name of a property which value we want to set.
	/// @param value - new value of a property.
	virtual void setGraphicalPartProperty(
			qReal::Id const &id
			, int partIndex
			, QString const &propertyName
			, QVariant const &value
			);

	void generateGraphicalModel();

private:
	void removeAllGraphicalElements();
	void moveElements(const QMap<qReal::Id, qReal::Id> &graphicalByLogical);

	void init();

	void loadFromDisk();
	void addChildrenToRootObject();

	qReal::IdList idsOfAllChildrenOf(qReal::Id id) const;
	QList<Object*> allChildrenOf(qReal::Id id) const;
	QList<Object*> allChildrenOfWithLogicalId(qReal::Id id) const;

	QHash<qReal::Id, Object*> mObjects;

	/// Name of the current save file for project.
	QString mWorkingFile;
	Serializer mSerializer;
};

}
}
