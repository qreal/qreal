#pragma once

#include <QtWidgets/QWidget>

#include "twoDModel/engine/twoDModelDisplayInterface.h"
#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {
namespace engine {

/// Base class for all widgets that emulate diaplay`s behaviour. Implements 'clear' function.
class TWO_D_MODEL_EXPORT TwoDModelDisplayWidget : public QWidget, public TwoDModelDisplayInterface
{
	Q_OBJECT

public:
	explicit TwoDModelDisplayWidget(QWidget *parent = nullptr);

	void setPainter(graphicsUtils::PainterInterface *painter) override;
	void clear() const override;

private:
	graphicsUtils::PainterInterface *mPainter;
};

}
}
