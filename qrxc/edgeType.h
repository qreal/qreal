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
#include <QtGui/QColor>

#include "graphicType.h"

class RoleType;
namespace utils {
	class OutFile;
}

class EdgeType : public GraphicType
{
public:
	explicit EdgeType(Diagram *diagram);
	virtual Type* clone() const;
	virtual ~EdgeType();
	virtual void generateCode(utils::OutFile &out);
	virtual bool generateEnumValues(utils::OutFile &/*out*/, bool /*isNotFirst*/) { return false; }
	bool copyPorts(NodeType* parent) override;
	bool copyPictures(GraphicType *parent) override;
	QList<RoleType*> getRoles();

private:
	QList<RoleType*> mRoles;

	QString mBeginArrowType;
	QString mEndArrowType;
	QString mBeginRoleName;
	QString mEndRoleName;

	QString mLineType;
	QString mShapeType;
	QColor mLineColor;
	int mLineWidth;
	QString mIsDividable;
	QStringList mFromPorts;
	QStringList mToPorts;

	virtual void generateLabels(utils::OutFile &out) const;
	virtual bool initRoles();
	virtual bool initRoleProperties();
	virtual QString propertyName(Property *property, const QString &roleName);
	virtual bool initGraphics();
	virtual bool initDividability();
	virtual bool initPortTypes();
	void initPortTypes(const QDomElement &portsElement, QStringList &ports);
	void generateEdgeStyle(const QString &styleString, utils::OutFile &out);
	void generatePorts(utils::OutFile &out, const QStringList &portTypes, const QString &direction);
	virtual bool initLabel(Label *label, const QDomElement &element, const int &count);
};
