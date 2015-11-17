#pragma once

#include <QtWidgets/QWidget>

#include <qrutils/graphicsUtils/abstractItemView.h>

#include "editor/editorView.h"

namespace qReal {
namespace shapeEdit {

class IShapeEdit : public QWidget
{
    Q_OBJECT

public:
    virtual graphicsUtils::AbstractView* getView() const = 0;

    virtual Id getId() const = 0;

    virtual ~IShapeEdit() {}

signals:
    void shapeSaved(const QString &shape, const QPersistentModelIndex &index, const int &role);

    /// To tell MainWindow to update pallete, for example.
    void needUpdate();

protected:
    const Id mId;
};

}
}
