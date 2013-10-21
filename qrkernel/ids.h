#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QMetaType>
#include <QtCore/QDebug>

#include "kernelDeclSpec.h"

namespace qReal {

/// Identifier of model element or element type. Consists of four parts ---
/// editor (metamodel to which our element belongs to), diagram in that editor
/// (a tab in palette where this element will appear), element (type of
/// an element, actually), id (id of an element).
class QRKERNEL_EXPORT Id
{
public:
	/// Loads Id from string in format "qrm:/<editor>/<diagram>/<element>/<id>".
	/// @param string A string from which we must load Id.
	/// @returns Loaded Id.
	static Id loadFromString(QString const &string);

	/// Creates element Id, assigning to its first three parts values passed as
	/// parameters and generating new GUID for "id" part.
	/// @param editor Editor name of an element (its metamodel).
	/// @param diagram Diagram name of an element (tab in a palette).
	/// @param element Type name of an element ("Class", for example).
	/// @returns Created element Id.
	static Id createElementId(QString const &editor, QString const &diagram, QString const &element);

	/// Root Id for a model tree stored in repository.
	/// @returns Root id.
	static Id rootId();

	/// Constructor. Takes all four parts of an Id as optional parameters.
	/// @param editor Editor part of an Id (metamodel name).
	/// @param diagram Diagram part of an Id (palette tab name).
	/// @param element Element part of an Id (element type name).
	/// @param id Id part of an, mm, Id. Represents identity of a model element.
	explicit Id(QString const &editor = "", QString  const &diagram = ""
			, QString  const &element = "", QString  const &id = "");

	/// Convenience constructor, creates Id by given base Id adding new part as
	/// a next section. For example, by given editor Id and diagram name
	/// constructs diagram Id.
	/// @param base Id that will be extended.
	/// @param additional Part of Id that will be added to base Id.
	Id(Id const &base, QString const &additional);

	/// Returns true if this id is an empty one (same as == Id())
	bool isNull() const;

	/// Converts Id to URI in format "qrm:/<editor>/<diagram>/<element>/<id>".
	QUrl toUrl() const;

	/// Returns unique part of an Id.
	QString id() const;

	/// Returns editor (metamodel) part of an Id.
	QString editor() const;

	/// Returns diagram (palette tab) part of an Id.
	QString diagram() const;

	/// Returns element (type) part of an Id.
	QString element() const;

	/// Converts Id to string in format "qrm:/<editor>/<diagram>/<element>/<id>".
	QString toString() const;

	/// Returns number of parts Id has.
	unsigned idSize() const;

	/// Applicable only to element's Ids. Returns type's Id.
	Id type() const;

	/// Creates Id of the same type as given Id.
	Id sameTypeId() const;

	/// Cast to QVariant. Not an operator, to avoid problems with autocasts.
	QVariant toVariant() const;

	// default destructor and copy constuctor are OK
private:
	/// Used only for debug. Checks that Id is correct.
	bool checkIntegrity() const;

	QString mEditor;
	QString mDiagram;
	QString mElement;
	QString mId;
};

/// Id equality operator. Ids are equal when all their parts are equal.
inline bool operator==(Id const &i1, Id const &i2)
{
	return i1.editor() == i2.editor()
			&& i1.diagram() == i2.diagram()
			&& i1.element() == i2.element()
			&& i1.id() == i2.id();
}

/// Id inequality operator.
inline bool operator!=(Id const &i1, Id const &i2)
{
	return !(i1 == i2);
}

/// Comparison operator for using Id in maps
inline bool operator<(Id const &i1, Id const &i2)
{
	return i1.toString() < i2.toString();
}

/// Hash function for Id for using it in QHash.
inline uint qHash(Id const &key)
{
	return qHash(key.editor()) ^ qHash(key.diagram()) ^ qHash(key.element())
			^ qHash(key.id());
}

/// Operator for printing Id in QDebug.
inline QDebug operator<<(QDebug dbg, Id const &id)
{
	dbg << id.toString();
	return dbg.space();
}

/// Convenience typedef for list of Ids.
typedef QList<Id> IdList;

/// Static class with convenience functions for Id lists.
class QRKERNEL_EXPORT IdListHelper {
public:
	static QVariant toVariant(IdList const &list);
};

typedef Id Metatype;
}

// qReal::Id and qReal::IdList could be used straight in QVariant
Q_DECLARE_METATYPE(qReal::Id)

Q_DECLARE_METATYPE(qReal::IdList)

QRKERNEL_EXPORT QDataStream& operator<< (QDataStream &out, qReal::Id const &id);
QRKERNEL_EXPORT QDataStream& operator>> (QDataStream &in, qReal::Id &id);
