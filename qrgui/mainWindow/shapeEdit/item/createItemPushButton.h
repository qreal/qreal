#pragma once

#include <QtWidgets/QPushButton>

#include "mainWindow/shapeEdit/item/item.h"

namespace qReal {
namespace shapeEdit {

class CreateItemPushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CreateItemPushButton(Item *specifyButton, QWidget *parent = 0);

signals:
    void clickedItemButton(bool checked, Item *newItem);

private slots:
    void emitClickedWithItem(bool checked);

private:
    Item* mItem;
};

}
}
