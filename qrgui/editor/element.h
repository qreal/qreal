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

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QAction>

#include <qrkernel/ids.h>
#include <qrkernel/settingsManager.h>

#include <qrgui/plugins/editorPluginInterface/elementRepoInterface.h>
#include <qrgui/plugins/editorPluginInterface/elementImpl.h>

#include <qrgui/models/graphicalModelAssistApi.h>
#include <qrgui/models/logicalModelAssistApi.h>
#include <qrgui/models/exploser.h>

#include <qrgui/controller/controller.h>

#include "qrgui/editor/editorDeclSpec.h"
#include "qrgui/editor/label.h"
#include "qrgui/editor/contextMenuAction.h"


namespace qReal {

/// size of a point port
const int kvadratik = 10;

/// base class for an element on a diagram
class QRGUI_EDITOR_EXPORT Element : public QObject, public QGraphicsItem, public ElementRepoInterface
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	/// Constructor
	/// @param elementImpl - pointer to implementation of the element. Takes ownership.
	Element(ElementImpl *elementImpl
			, const Id &id
			, models::GraphicalModelAssistApi &graphicalAssistApi
			, models::LogicalModelAssistApi &logicalAssistApi
			);

	virtual ~Element() {}

	void initEmbeddedControls();

	virtual void updateData();

	virtual Id id() const;
	virtual Id logicalId() const;
	virtual QString name() const;

	virtual void connectToPort() {}  // for edge
	virtual void checkConnectionsToPort() {}  // for node
	virtual QList<ContextMenuAction *> contextMenuActions(const QPointF &pos);

	virtual bool initPossibleEdges() = 0;
	virtual void initTitles();
	// for inline editing we should be able to change properties value. right now via graphical
	// representation. also labels could store indices and get data themselves
	virtual void setLogicalProperty(const QString &roleName, const QString &value
			, bool withUndoRedo = false);
	QString logicalProperty(const QString &roleName) const;

	virtual void setColorRect(bool bl) = 0;

	// TODO: Move this to constructor.
	void setController(qReal::Controller *controller);
	qReal::Controller *controller() const;

	ElementImpl* elementImpl() const;
	bool createChildrenFromMenu() const;

	/// Checks if this element is disabled from palette and if it is grayscales it.
	void updateEnabledState();

public slots:
	virtual void select(const bool singleSelected);
	virtual void setSelectionState(const bool selected);

signals:
	void switchFolding(bool);

protected:
	void setHideNonHardLabels(bool visible);

	void keyPressEvent(QKeyEvent *event) override;

	bool mMoving;
	bool mEnabled;
	const Id mId;
	ElementImpl * const mElementImpl;  // Has ownership.
	QList<Label *> mLabels;

	models::LogicalModelAssistApi &mLogicalAssistApi;
	models::GraphicalModelAssistApi &mGraphicalAssistApi;
	Controller *mController;
};

}
