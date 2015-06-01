/* Copyright 2007-2015 QReal Research Group
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
