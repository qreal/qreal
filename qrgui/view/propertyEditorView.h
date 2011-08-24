#pragma once

#include <QAbstractItemView>
#include <QMap>

#include "propertyEditorProxyModel.h"

#include "../thirdparty/qtpropertybrowser/src/qttreepropertybrowser.h"
#include "../models/logicalModelAssistApi.h"
#include "pushButtonProperty.h"

namespace qReal {
	class MainWindow;
}

/** @brief This widget imitates QtCreator's property editor */
class PropertyEditorView : public QWidget
{
	Q_OBJECT

public:
	explicit PropertyEditorView(QWidget *parent = 0);
	~PropertyEditorView();

	// QAbstractItemView's methods
	void setModel(PropertyEditorModel *model);
	/** @brief editor initialization */
	void init(qReal::MainWindow *mainWindow,
		qReal::models::LogicalModelAssistApi * const logicalModelAssistApi);
	/** @brief unimplemented */
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

	void buttonClicked(QtProperty *);

private:
	/** @brief returns index of value in list of possible values for index  */
	int enumPropertyIndexOf(QModelIndex const &, QString const &);
	void setPropertyValue(QtVariantProperty *property, QVariant const &value);

	bool mChangingPropertyValue;
	PropertyEditorModel *mModel;
	QtTreePropertyBrowser *mPropertyEditor;
	qReal::MainWindow *mMainWindow;
	qReal::models::LogicalModelAssistApi *mLogicalModelAssistApi;
};
