#include "NewType.h"

#include <QtCore/QVariant>

using namespace qReal;

NewType NewType::loadFromString(QString const &string)
{
	QStringList path = string.split('/');
	Q_ASSERT(path.count() > 0 && path.count() <= 5);
	Q_ASSERT(path[0] == "qrm:");

	NewType result;
	switch (path.count()) {
	case 4: result.mElement = path[3];
		// Fall-thru
	case 3: result.mDiagram = path[2];
		// Fall-thru
	case 2: result.mEditor = path[1];
		// Fall-thru
	}
	Q_ASSERT(string == result.toString());
	Q_ASSERT(string == result.toUrl().toString());
	return result;
}


NewType NewType::rootType()
{
	return NewType("ROOT_ID","ROOT_ID","ROOT_ID");
}

NewType::NewType(QString const &editor, QString  const &diagram, QString  const &element)
	: mEditor(editor), mDiagram(diagram), mElement(element)
{
	Q_ASSERT(checkIntegrity());
}

NewType::NewType(NewType const &base, QString const &additional)
	: mEditor(base.mEditor), mDiagram(base.mDiagram), mElement(base.mElement)
{
	unsigned baseSize = base.typeSize();
	switch (baseSize) {
	case 0:
		mEditor = additional;
		break;
	case 1:
		mDiagram = additional;
		break;
	case 2:
		mElement = additional;
		break;
		//default:
		//Q_ASSERT(!"Can not add a part to NewType, it will be too long");
	}
	Q_ASSERT(checkIntegrity());
}

QString NewType::editor() const
{
	return mEditor;
}

NewType NewType::editorId() const
{
	return NewType(mEditor);
}

NewType NewType::diagramId() const
{
	return NewType(mEditor,mDiagram);
}

QString NewType::diagram() const
{
	return mDiagram;
}

QString NewType::element() const
{
	return mElement;
}

unsigned NewType::typeSize() const
{
	if (mElement != "")
		return 3;
	if (mDiagram != "")
		return 2;
	if (mEditor != "")
		return 1;
	return 0;
}

QUrl NewType::toUrl() const
{
	return QUrl(toString());
}

QString NewType::toString() const
{
	QString path = "qrm:/" + mEditor;
	if (mDiagram != "")
		path += "/" + mDiagram;
	if (mElement != "")
		path += "/" + mElement;
	return path;
}

bool NewType::checkIntegrity() const
{
	bool emptyPartsAllowed = true;

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

QVariant NewType::toVariant() const
{
	QVariant result;
	result.setValue(*this);
	return result;
}

QVariant TypeListHelper::toVariant(TypeList const &list)
{
	QVariant v;
	v.setValue(list);
	return v;
}

