#include "createItemPushButton.h"

using namespace qReal::shapeEdit;

CreateItemPushButton::CreateItemPushButton(Item *specifyButton, QWidget *parent)
    : QPushButton(parent)
    , mItem(specifyButton)
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(emitClickedWithItem(bool)));
}

void CreateItemPushButton::emitClickedWithItem(bool checked)
{
    emit clickedItemButton(checked, mItem->clone());
}
