#pragma once

#include <QtWidgets/QWidget>

#include "commonTwoDModel/engine/twoDModelDisplayInterface.h"
#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {
namespace engine {

class COMMON_TWO_D_MODEL_EXPORT TwoDModelDisplayWidget : public QWidget, public TwoDModelDisplayInterface
{
	Q_OBJECT

public:
	TwoDModelDisplayWidget(QWidget *parent = nullptr)
		: QWidget(parent)
	{
	}
};

}
}
