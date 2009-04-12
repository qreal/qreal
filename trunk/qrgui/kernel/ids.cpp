#include "ids.h"

using namespace qReal;

Id::Id(QString editor, QString diagram, QString element, QString id)
	: mEditor(editor), mDiagram(diagram), mElement(element), mId(id)
{
	Q_ASSERT(checkIntegrity());
}

QString Id::editor() const
{
	return mEditor;
}

QString Id::diagram() const
{
	return mDiagram;
}

QString Id::element() const
{
	return mElement;
}

QString Id::id() const
{
	return mId;
}

unsigned Id::idSize() const
{
	if (mId != "")
		return 4;
	if (mElement != "")
		return 3;
	if (mDiagram != "")
		return 2;
	if (mEditor != "")
		return 1;
	return 0;
}

QUrl Id::toUrl() const
{
	return QUrl(toString());
}

QString Id::toString() const
{
	QString path = "qrm:/" + mEditor;
	if (mDiagram != "")
		path += "/" + mDiagram;
	if (mElement != "")
		path += "/" + mElement;
	if (mId != "")
		path += "/" + mId;
	return path;
}

bool Id::checkIntegrity() const
{
	bool emptyPartsAllowed = true;

	if (mId != "")
		emptyPartsAllowed = false;

	if (mElement != "")
		emptyPartsAllowed = false;
	else if (!emptyPartsAllowed)
		return false;

	if (mDiagram != "")
		emptyPartsAllowed = false;
	else if (!emptyPartsAllowed)
		return false;

	if (mEditor == "" && !emptyPartsAllowed)
		return false;

	return true;
}
