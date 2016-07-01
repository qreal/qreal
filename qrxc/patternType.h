/* Copyright 2016 Dmitry Mordvinov
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

#include "graphicType.h"

namespace utils {
	class OutFile;
}

class PatternType : public GraphicType
{
public:
	PatternType(Diagram *diagram, const QString &xml);
	Type *clone() const override;
	~PatternType() override;

	void generateCode(utils::OutFile &out) override;

	bool init(const QDomElement &element, const QString &context);
	bool initGraphics() override;
	bool initAssociations() override;
	bool initDividability() override;
	bool initPortTypes() override;
	bool initLabel(Label *label, const QDomElement &element, const int &count) override;

	bool copyPorts(NodeType *parent) override;
	bool copyPictures(GraphicType *parent) override;

private:
	QString mXml;
};
