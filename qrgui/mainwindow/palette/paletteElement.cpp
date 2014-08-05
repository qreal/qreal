#include "paletteElement.h"

#include <dialogs/gesturesShow/gesturePainter.h>
#include <pluginManager/editorManagerInterface.h>

using namespace qReal;
using namespace gui;

PaletteElement::PaletteElement(Id const &id
		, QString const &name
		, QString const &description
		, QIcon const &icon
		, QSize const &preferredSize
		, Id const &explosionTarget)
	: mId(id)
	, mName(name)
	, mDescription(description)
	, mIcon(icon)
	, mPreferredSize(preferredSize)
	, mExplosionTarget(explosionTarget)
{
}

PaletteElement::PaletteElement(EditorManagerInterface const &manager, Id const &element)
	: mId(element)
	, mName(manager.friendlyName(element))
	, mDescription(manager.description(element))
	, mIcon(manager.icon(element))
	, mPreferredSize(manager.iconSize(element))
	, mExplosionTarget()
{
	GesturePainter *paint = new GesturePainter(manager.mouseGesture(element), Qt::white, Qt::blue, 18);
	mGesture = paint->pixmap(QSize(18, 18), QIcon::Mode::Normal, QIcon::State::Off);
	delete paint;
}

Id PaletteElement::id() const
{
	return mId;
}

QString PaletteElement::name() const
{
	return mName;
}

QString PaletteElement::description() const
{
	return mDescription;
}

QIcon PaletteElement::icon() const
{
	return mIcon;
}

QIcon PaletteElement::gesture() const
{
	return mGesture;
}

QSize PaletteElement::preferredSize() const
{
	return mPreferredSize;
}

Id PaletteElement::explosionTarget() const
{
	return mExplosionTarget;
}
