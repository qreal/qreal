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

#include "paletteElement.h"

using namespace qReal;
using namespace gui;

PaletteElement::PaletteElement(const Id &id
		, const QString &name
		, const QString &description
		, const QIcon &icon
		, const QSize &preferredSize
		, const Id &explosionTarget)
	: mId(id)
	, mName(name)
	, mDescription(description)
	, mIcon(icon)
	, mPreferredSize(preferredSize)
	, mExplosionTarget(explosionTarget)
{
}

PaletteElement::PaletteElement(const EditorManagerInterface &manager, const Id &element)
	: PaletteElement(
		element
		, manager.friendlyName(element)
		, manager.description(element)
		, manager.icon(element)
		, manager.iconSize(element)
		, Id())
{
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

QSize PaletteElement::preferredSize() const
{
	return mPreferredSize;
}

Id PaletteElement::explosionTarget() const
{
	return mExplosionTarget;
}
