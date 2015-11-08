#pragma once

#include <QtWidgets/QWidget>

#include <qrutils/graphicsUtils/abstractItemView.h>

#include "editor/editorView.h"

namespace qReal {

class IShapeEdit : public QWidget
{

    Q_OBJECT

public:
    virtual graphicsUtils::AbstractView* getView() = 0;

    //virtual ~newShapeEdit(); //нужен ли?

signals:
    void shapeSaved(const QString &shape, const QPersistentModelIndex &index, const int &role);

    /// To tell MainWindow to update pallete, for example.
    void needUpdate();

protected:
    /// Protected constructor because this is an abstract class.
    /// Need to construct QWidget.
    //explicit IShapeEdit(QWidget *parent = nullptr);

};
}
