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

#include <QtCore/QList>

#include "graphicType.h"
#include "port.h"

class Port;

namespace utils {
	class OutFile;
}

class NodeType : public GraphicType
{
public:
	NodeType(Diagram *diagram);
	virtual Type* clone() const;
	virtual ~NodeType();
	virtual void generateCode(utils::OutFile &out);
	virtual bool generateEnumValues(utils::OutFile &/*out*/, bool /*isNotFirst*/) { return false; }
	virtual bool generatePorts(utils::OutFile &out, bool isNotFirst);
	bool copyPorts(NodeType *port) override;
	bool copyPictures(GraphicType *parent) override;
	QList<Port *> ports() const override;

private:
	QList<Port*> mPorts;
	QDomElement mSdfDomElement;
	QDomElement mPortsDomElement;
	bool mIsResizeable;

	virtual bool initAssociations();
	virtual bool initGraphics();
	virtual bool initDividability();
	virtual bool initPortTypes();

	bool initSdf();
	void generateSdf() const;

	bool initPorts();
	bool initPointPorts(const QDomElement &portsElement);
	bool initLinePorts(const QDomElement &portsElement);
	virtual bool initLabel(Label *label, const QDomElement &element, const int &count);
	bool initBooleanProperties();
};
