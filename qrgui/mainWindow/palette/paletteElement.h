#pragma once

#include <QtCore/QSize>
#include <QtGui/QIcon>

#include <qrkernel/ids.h>

#include "plugins/pluginManager/editorManagerInterface.h"

namespace qReal {
namespace gui {

/// A class-storage for palette element properties (such as icon and text)
class PaletteElement
{
public:
	/// Stores given parameters in self
	PaletteElement(const Id &id
			, const QString &name
			, const QString &description
			, const QIcon &icon
			, const QSize &preferredSize
			, const Id &explosionTarget);

	/// Stores editor manager-derived parameters of given elements
	PaletteElement(const EditorManagerInterface &manager, const Id &element);

	Id id() const;
	QString name() const;
	QString description() const;
	QIcon icon() const;
	QSize preferredSize() const;
	Id explosionTarget() const;

private:
	Id mId;
	QString mName;
	QString mDescription;
	QIcon mIcon;
	QSize mPreferredSize;
	Id mExplosionTarget;
};

}
}
