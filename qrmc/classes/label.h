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

#include <QtXml/QDomElement>
#include "scalableCoordinate.h"

namespace qrmc {
	class MetaCompiler;

	class Label
	{
	public:
		// Default copy constructor is ok.

		bool init(const QDomElement &element, int index, bool nodeLabel, int width, int height);

		QString generateInit(MetaCompiler *compiler, bool isNode) const;
		QString generateUpdate(MetaCompiler *compiler) const;
		QString generateDefinition(MetaCompiler *compiler) const;

	private:
		void initCoordinate(ScalableCoordinate &field, QString coordinate, int maxValue);
		QString titleName() const;
		QStringList getListOfStr(const QString &strToParse) const;
		QString generateCodeForUpdateData() const;

		ScalableCoordinate mX;
		ScalableCoordinate mY;
		int mIndex;
		QString mText;
		QString mTextBinded;
		QString mReadOnly;
		QString mBackground;
		QString mCenter;
		QString scalingX;
		QString scalingY;
		qreal mRotation;
	};
}
