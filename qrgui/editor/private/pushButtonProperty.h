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

#include <QtWidgets/QPushButton>

#include "thirdparty/qtpropertybrowser/src/qtvariantproperty.h"
#include "thirdparty/qtpropertybrowser/src/qteditorfactory.h"

namespace qReal {
namespace gui {
namespace editor {

/** @brief This manager allows to use push buttons in property editor to do some stuff by click */
class PushButtonPropertyManager : public QtAbstractPropertyManager
{
	friend class PushButtonFactory;
	Q_OBJECT
public:
	PushButtonPropertyManager(QObject *parent = 0);
	~PushButtonPropertyManager();

	bool value(const QtProperty *property) const;

public Q_SLOTS:
	void setValue(QtProperty *, bool){}
Q_SIGNALS:
	void valueChanged(QtProperty *property, bool val);
	void buttonClicked(QtProperty *property);

protected:
	QString valueText(const QtProperty *property) const;
	QIcon valueIcon(const QtProperty *property) const;
	virtual void initializeProperty(QtProperty *){}
	virtual void uninitializeProperty(QtProperty *){}
private:
	class {} *d_ptr;
	Q_DISABLE_COPY(PushButtonPropertyManager)
};

/** @brief helper to PushButtonPropertyManager */
class PushButtonFactory : public QtAbstractEditorFactory<PushButtonPropertyManager>
{
	Q_OBJECT

public:
	PushButtonFactory(QObject *parent = 0);
	~PushButtonFactory();

protected:
	void connectPropertyManager(PushButtonPropertyManager *manager);
	QWidget *createEditor(PushButtonPropertyManager *manager, QtProperty *property, QWidget *parent);
	void disconnectPropertyManager(PushButtonPropertyManager *manager);

private:
	class {} *d_ptr;
	Q_DISABLE_COPY(PushButtonFactory)
};

}
}
}
