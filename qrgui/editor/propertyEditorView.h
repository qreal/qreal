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

#include <QtCore/QMap>
#include <QtWidgets/QAbstractItemView>
#include <QtGui/QResizeEvent>

#include <qrgui/models/propertyEditorModel.h>
#include <qrgui/models/logicalModelAssistApi.h>

#include <qrgui/controller/controller.h>

#include <qrgui/thirdparty/qtpropertybrowser/src/qttreepropertybrowser.h>

#include "qrgui/editor/editorDeclSpec.h"
#include "qrgui/editor/private/pushButtonProperty.h"

namespace qReal {
namespace gui {
namespace editor {

/** @brief This widget imitates QtCreator's property editor */
class QRGUI_EDITOR_EXPORT PropertyEditorView : public QWidget
{
	Q_OBJECT

public:
	explicit PropertyEditorView(QWidget *parent = 0);
	~PropertyEditorView();

	// QAbstractItemView's methods
	void setModel(PropertyEditorModel *model);
	/** @brief editor initialization */
	void init(qReal::models::LogicalModelAssistApi &logicalModelAssistApi, qReal::Controller &controller);
	/** @brief unimplemented */
	void scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint = QAbstractItemView::EnsureVisible);

	PropertyEditorModel* model() {
		return mModel;
	}

	void installEventFilter(QObject *obj);

signals:
	/// Emitted when user wants to edits 'Shape' property.
	void shapeEditorRequested(const QPersistentModelIndex &index, int role
		, const QString &propertyValue, bool useTypedPorts);

	/// Emitted when user wants to enter a code into a property.
	void textEditorRequested(const QPersistentModelIndex &index, const int role, const QString &propertyValue);

	/// Emitted when user wants to edit some reference property.
	void referenceListRequested(const QPersistentModelIndex &index, const QString &referenceType
			, const QString &propertyValue, int role);

public slots:

	// QAbstractItemView's methods
	void update(const QModelIndex &index);
	void setRootIndex(const QModelIndex &index);
	void reset();

protected:
	virtual void resizeEvent(QResizeEvent *event);

protected slots:
	// QAbstractItemView's methods
	void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
	void editorValueChanged(QtProperty *, QVariant);

	void buttonClicked(QtProperty *);

private:
	/** @brief returns index of value in list of possible values for index  */
	int enumPropertyIndexOf(const QModelIndex &, const QString &);
	void setPropertyValue(QtVariantProperty *property, const QVariant &value);

	QString propertyDescription(const int cellIndex) const;

	bool mChangingPropertyValue;
	PropertyEditorModel *mModel;
	QtTreePropertyBrowser *mPropertyEditor;
	qReal::models::LogicalModelAssistApi *mLogicalModelAssistApi;
	QtVariantPropertyManager *mVariantManager;
	QtVariantEditorFactory *mVariantFactory;
	PushButtonPropertyManager *mButtonManager;
	PushButtonFactory *mButtonFactory;
	qReal::Controller *mController;
};

}
}
}
