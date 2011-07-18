#pragma once

#include <QAbstractItemView>
#include <QMap>
#include "propertyEditorProxyModel.h"

#include "../thirdparty/qtpropertybrowser/src/qttreepropertybrowser.h"
#include "../thirdparty/qtpropertybrowser/src/qtvariantproperty.h"

class PropertyEditorView : public QWidget
{
	Q_OBJECT

public:
	explicit PropertyEditorView(QWidget *parent = 0);

	// QAbstractItemView's methods
	void setModel(PropertyEditorModel *model);
	void scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint = QAbstractItemView::EnsureVisible);

	PropertyEditorModel* model(){
		return mModel;
	}

public slots:

	// QAbstractItemView's methods
	void update(const QModelIndex &index);
	void setRootIndex(const QModelIndex &index);
	void reset();

protected:

protected slots:
	// QAbstractItemView's methods
	void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
	void editorValueChanged(QtProperty *, QVariant);

private:
	PropertyEditorModel *mModel;
	QtTreePropertyBrowser *mPropertyEditor;
	QMap<QModelIndex, QtProperty*> mPropertiesRows;
};
