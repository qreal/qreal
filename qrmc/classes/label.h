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

#include <QtXml/QDomElement>

#include "scalableCoordinate.h"

namespace qrmc {

class MetaCompiler;

/// Represents label on a node or edge.
class Label
{
public:
	/// Constructor.
	/// @param element - XML node that contains label information.
	/// @param index - index of a label in a list of labels of given node or edge.
	/// @param nodeLabel - is it node or edge label.
	/// @param width - default width of a figure this label belongs to.
	/// @param height - default height of a figure this label belongs to.
	bool init(const QDomElement &element, int index, bool nodeLabel, int width, int height);

	QString generateInit(const MetaCompiler &compiler, bool isNode) const;
	QString generateUpdate(const MetaCompiler &compiler) const;
	QString generateDefinition(const MetaCompiler &compiler) const;

private:
	void initCoordinate(ScalableCoordinate &field, const QString &coordinate, int maxValue);
	QString titleName() const;
	QStringList getListOfStr(const QString &strToParse) const;
	QString generateCodeForUpdateData() const;

	ScalableCoordinate mX;
	ScalableCoordinate mY;
	int mIndex = 0;
	QString mText;
	QString mTextBinded;
	QString mReadOnly;
	QString mBackground;
	QString mCenter;
	QString mScalingX;
	QString mScalingY;
	qreal mRotation = 0;
};

}
