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

#include "pushButtonProperty.h"

using namespace qReal::gui::editor;

PushButtonPropertyManager::PushButtonPropertyManager(QObject *parent)
	: QtAbstractPropertyManager(parent)
{
	d_ptr = nullptr;
}

PushButtonPropertyManager::~PushButtonPropertyManager()
{
	clear();
	delete d_ptr;
}

bool PushButtonPropertyManager::value(const QtProperty *) const
{
	return false;
}

QString PushButtonPropertyManager::valueText(const QtProperty *) const
{
	return tr("Click to choose");
}

QIcon PushButtonPropertyManager::valueIcon(const QtProperty *) const
{
	return QIcon();
}

PushButtonFactory::PushButtonFactory(QObject *parent)
	: QtAbstractEditorFactory<PushButtonPropertyManager>(parent)
{
	d_ptr = nullptr; // new PushButtonFactoryPrivate();
}

PushButtonFactory::~PushButtonFactory()
{
}

void PushButtonFactory::connectPropertyManager(PushButtonPropertyManager *)
{
}

QWidget *PushButtonFactory::createEditor(PushButtonPropertyManager *manager, QtProperty *property, QWidget *)
{
	emit manager->buttonClicked(property);
	return nullptr;
}

void PushButtonFactory::disconnectPropertyManager(PushButtonPropertyManager *)
{
}
