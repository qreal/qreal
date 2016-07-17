/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include "port.h"

#include <QtXml/QDomElement>

namespace qrmc {

class MetaCompiler;

/// Represents point port.
class PointPort : public Port
{
public:
	bool init(const QDomElement &element, int width, int height) override;
	virtual Port* clone() const override;

	virtual QString generateSdf(const MetaCompiler &compiler) const override;
	virtual QString generateInit(const MetaCompiler &compiler) const override;
	virtual void generatePortList(const QStringList &portTypes) override;

private:
	qreal mX = 0;
	qreal mY = 0;
	int mWidth = 0;
	int mHeight = 0;
};

}
