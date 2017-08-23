/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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

#pragma once

#include <QtCore/QHash>

#include <qrkernel/definitions.h>
#include <qrkernel/ids.h>

#include "classes/graphicalObject.h"
#include "classes/logicalObject.h"
#include "serializer.h"

namespace qrRepo {
namespace details {

/// Class that actually contains all data, supports low-level queries and can serialize/deserialize data.
class Repository
{
public:
	explicit Repository(const QString &workingFile);
	~Repository();

	/// Returns a reference to serializer component used for low-level saving operations.
	Serializer &serializer();

	/// replacing property values that contains input value with new value
	/// @param toReplace - id list that contains ids of elements that properties should be replaced
	/// @param value - input value that should be contained by any property of each element
	/// @param newValue - string representation of value with what property values should be replaced
	void replaceProperties(const qReal::IdList &toReplace, const QString &value, const QString &newValue);

	/// returning IdList of elements that names contains input string
	/// @param name - string that should be contained by names of elements that Id's are in the output list
	qReal::IdList findElementsByName(const QString &name, bool sensitivity, bool regExpression) const;

	/// Returns IdList of elements that have given property
	/// @param name - string that should be contained by names of elements that have input property
	qReal::IdList elementsByProperty(const QString &property, bool sensitivity, bool regExpression) const;

	/// returning IdList of elements that have input property content
	/// @param name - string that should be contained by names of elements that have input property content
	qReal::IdList elementsByPropertyContent(const QString &property, bool sensitivity, bool regExpression) const;

	qReal::IdList children(const qReal::Id &id) const;
	qReal::Id parent(const qReal::Id &id) const;

	/// Clones object recursively in internal structures.
	/// This doesn't mean it'll appear on your diagrams
	qReal::Id cloneObject(const qReal::Id &id);

	void setParent(const qReal::Id &id, const qReal::Id &parent);
	void addChild(const qReal::Id &id, const qReal::Id &child);
	void addChild(const qReal::Id &id, const qReal::Id &child, const qReal::Id &logicalId);
	void removeParent(const qReal::Id &id);
	void remove(const qReal::Id &id);
	void removeChild(const qReal::Id &id, const qReal::Id &child);

	/// Stacks element child before sibling (element id shold be parent of them both)
	void stackBefore(const qReal::Id &id, const qReal::Id &child, const qReal::Id &sibling);

	void setProperty(const qReal::Id &id, const QString &name, const QVariant &value) const;
	void copyProperties(const qReal::Id &dest, const qReal::Id &src);
	QVariant property(const qReal::Id &id, const QString &name) const;
	QMap<QString, QVariant> properties(const qReal::Id &id) const;
	void setProperties(const qReal::Id &id, QMap<QString, QVariant> const &properties);
	bool hasProperty(const qReal::Id &id, const QString &name, bool sensitivity = false
			, bool regExpression = false) const;
	void removeProperty(const qReal::Id &id, const QString &name);
	QMapIterator<QString, QVariant> propertiesIterator(const qReal::Id &id) const;

	void setBackReference(const qReal::Id &id, const qReal::Id &reference) const;
	void removeBackReference(const qReal::Id &id, const qReal::Id &reference) const;

	void setTemporaryRemovedLinks(const qReal::Id &id, const QString &direction, const qReal::IdList &linkIdList);
	qReal::IdList temporaryRemovedLinksAt(const qReal::Id &id, const QString &direction) const;
	qReal::IdList temporaryRemovedLinks(const qReal::Id &id) const;
	void removeTemporaryRemovedLinks(const qReal::Id &id);

	qReal::IdList elements() const;
	bool isLogicalId(const qReal::Id &elem) const;
	qReal::Id logicalId(const qReal::Id &elem) const;

	void printDebug() const;

	void exterminate();
	void open(const QString &saveFile);

	bool exist(const qReal::Id &id) const;

	/// Opens file into existing project
	/// @param importedFile - name of file to be imported
	void importFromDisk(const QString &importedFile);

	void saveAll() const;
	void save(const qReal::IdList &list) const;
	void saveWithLogicalId(const qReal::IdList &list) const;
	void saveDiagramsById(QHash<QString, qReal::IdList> const &diagramIds);
	void remove(const qReal::IdList &list) const;
	void setWorkingFile(const QString &workingDir);
	void exportToXml(const QString &targetFile) const;

	/// Returns current working file name
	QString workingFile() const;

	/// Creates empty graphical part with given index inside given object.
	/// @param id - id of an object where we shall create graphical part.
	/// @param partIndex - index of created part in given object.
	void createGraphicalPart(const qReal::Id &id, int partIndex);

	/// Returns a list of indexes of graphical parts for given element.
	QList<int> graphicalParts(const qReal::Id &id) const;

	/// Returns the value of graphical part property of a given object.
	/// @param id - id of an object where graphical part is located.
	/// @param partIndex - index of a graphical part.
	/// @param propertyName - name of a property which value we want to get.
	QVariant graphicalPartProperty(const qReal::Id &id, int partIndex, const QString &propertyName) const;

	/// Sets the value of graphical part property of a given object. If a property already exists, its value
	/// will be overwritten, otherwise new property will be created with given value.
	/// @param id - id of an object where graphical part is located.
	/// @param partIndex - index of a graphical part.
	/// @param propertyName - name of a property which value we want to set.
	/// @param value - new value of a property.
	void setGraphicalPartProperty(
			const qReal::Id &id
			, int partIndex
			, const QString &propertyName
			, const QVariant &value
			);

	/// Returns a list of keys by that stored some meta-information.
	QStringList metaInformationKeys() const;

	/// Returns the meta-information about current model stored by the given key.
	QVariant metaInformation(const QString &key) const;

	/// Stores the meta-information for current stored binded to the given key.
	void setMetaInformation(const QString &key, const QVariant &info);

private:
	void init();

	void loadFromDisk();
	void addChildrenToRootObject();

	qReal::IdList idsOfAllChildrenOf(qReal::Id id) const;
	QList<Object*> allChildrenOf(qReal::Id id) const;
	QList<Object*> allChildrenOfWithLogicalId(qReal::Id id) const;

	QHash<qReal::Id, Object *> mObjects;
	QHash<QString, QVariant> mMetaInfo;

	/// Name of the current save file for project.
	QString mWorkingFile;
	Serializer mSerializer;
};

}
}
