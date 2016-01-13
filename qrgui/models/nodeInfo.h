/* Copyright 2015-2016 Dmitry Mordvinov
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

#include <QtCore/QRectF>

#include "elementInfo.h"

#include "modelsDeclSpec.h"

namespace qReal {

/// Container for convenient storing node element data for passing it into methods with just one parameter.
class QRGUI_MODELS_EXPORT NodeInfo : public ElementInfo
{
public:
	NodeInfo();

	NodeInfo(const Id &id
			, const Id &logicalId
			, const Id &logicalParent
			, const Id &graphicalParent
			, const QMap<QString, QVariant> &logicalProperties
			, const QMap<QString, QVariant> &graphicalProperties
			, const Id &explosionTarget);
};

}

QRGUI_MODELS_EXPORT QDataStream &operator<< (QDataStream &out, const qReal::NodeInfo &data);
QRGUI_MODELS_EXPORT QDataStream &operator>> (QDataStream &in, qReal::NodeInfo &data);
QRGUI_MODELS_EXPORT bool operator== (const qReal::NodeInfo &first, const qReal::NodeInfo &second);
