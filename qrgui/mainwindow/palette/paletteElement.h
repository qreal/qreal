#pragma once

#include <QtCore/QSize>
#include <QtGui/QIcon>

#include <qrkernel/ids.h>
#include <pluginManager/editorManagerInterface.h>

namespace qReal {
namespace gui {

/// A class-storage for palette element properties (such as icon and text)
class PaletteElement
{
public:
	/// Stores given parameters in self
	PaletteElement(Id const &id
			, QString const &name
			, QString const &description
			, QIcon const &icon
			, QSize const &preferredSize
			, Id const &explosionTarget);

	/// Stores editor manager-derived parameters of given elements
	PaletteElement(EditorManagerInterface const &manager, Id const &element);

	Id id() const;
	QString name() const;
	QString description() const;
	QIcon icon() const;	
	QIcon gesture() const;
	QSize preferredSize() const;
	Id explosionTarget() const;

private:
	Id mId;
	QString mName;
	QString mDescription;
	QIcon mIcon;
	QIcon mGesture;
	QSize mPreferredSize;
	Id mExplosionTarget;
};

}
}
