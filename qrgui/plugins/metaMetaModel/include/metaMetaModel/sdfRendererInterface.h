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

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QPainter>
#include <QtGui/QIconEngineV2>
#include <QtXml/QDomDocument>

#include "metaMetaModel/elementRepoInterface.h"

namespace qReal {

class SdfRendererInterface : public QObject
{
public:
	virtual bool load(const QString &filename) = 0;
	virtual bool load(const QDomDocument &document) = 0;
	virtual void render(QPainter *painter, const QRectF &bounds, bool isIcon = false) = 0;
	virtual void setElementRepo(ElementRepoInterface *elementRepo) = 0;
};

// TODO: ???
class SdfIconEngineV2Interface : public QIconEngine
{
public:
};

}
