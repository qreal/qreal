#pragma once

#include "../../qrkernel/definitions.h"
#include "../../qrkernel/ids.h"
#include "classes/object.h"
#include "qrRepoGlobal.h"
#include "serializer.h"

#include <QHash>

namespace qrRepo {

namespace details {

class Client
{
public:
	QRREPO_EXPORT Client(QString const &workingFile);
	QRREPO_EXPORT ~Client();

	/// replacing property values that contains input value with new value
	/// @param toReplace - id list that contains ids of elements that properties should be replaced
	/// @param value - input value that should be contained by any property of each element
	/// @param newValue - string representation of value with what property values should be replaced
	void replaceProperties(qReal::IdList const &toReplace, QString const value, QString const newValue);

	/// returning IdList of elements that names contains input string
	/// @param name - string that should be contained by names of elements that Id's are in the output list
	qReal::IdList findElementsByName(QString const &name, bool sensitivity) const;

	/// returning IdList of elements that have input property
	/// @param name - string that should be contained by names of elements that have input property
	qReal::IdList elementsByProperty(QString const &property, bool sensitivity) const;

	/// returning IdList of elements that have input property content
	/// @param name - string that should be contained by names of elements that have input property content
	qReal::IdList elementsByPropertyContent(QString const &property, bool sensitivity) const;

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

	void setProperty(const qReal::Id &id, const QString &name, const QVariant &value) const;
	void copyProperties(const qReal::Id &dest, const qReal::Id &src);
	QVariant property(const qReal::Id &id, const QString &name) const;
	bool hasProperty(const qReal::Id &id, const QString &name, bool sensitivity = false) const;
	void removeProperty(const qReal::Id &id, const QString &name);
	QMapIterator<QString, QVariant> propertiesIterator(qReal::Id const &id) const;

	void setTemporaryRemovedLinks(qReal::Id const &id, QString const &direction, qReal::IdList const &linkIdList);
	qReal::IdList temporaryRemovedLinksAt(qReal::Id const &id, QString const &direction) const;
	qReal::IdList temporaryRemovedLinks(qReal::Id const &id) const;
	void removeTemporaryRemovedLinks(qReal::Id const &id);

	qReal::IdList elements() const;
	bool isLogicalId(qReal::Id const &elem) const;
	qReal::Id logicalId(qReal::Id const &elem) const;

	void svnUpdate();
	void svnCommit();

	void printDebug() const;

	void exterminate();
	void open(QString const &saveFile);

	bool exist(qReal::Id const &id) const;

	/// Opens file into existing project
	/// @param importedFile - name of file to be imported
	void importFromDisk(QString const &importedFile);

	void saveAll() const;
	void save(qReal::IdList list) const;
	void saveWithLogicalId(qReal::IdList list) const;
	void saveDiagramsById(QHash<QString, qReal::IdList> const &diagramIds);
	void remove(qReal::IdList list) const;
	void setWorkingFile(QString const &workingDir);

	/// Returns current working file name
	QString workingFile() const;

private:
	void init();

	void loadFromDisk();
	void addChildrenToRootObject();

	qReal::IdList idsOfAllChildrenOf(qReal::Id id) const;
	QList<Object*> allChildrenOf(qReal::Id id) const;
	QList<Object*> allChildrenOfWithLogicalId(qReal::Id id) const;

	QHash<qReal::Id, Object*> mObjects;

	/// This term keeps name of current save file for project
	QString mWorkingFile;
	Serializer serializer;
};
}
}
