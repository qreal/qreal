#pragma once
#include <QComboBox>

class QColor;
class QWidget;

class ColorListEditor : public QComboBox
{
	Q_OBJECT
public:
	ColorListEditor(QWidget *widget = 0);

public:
	QColor color() const;
	void setColor(QColor c);

private:
	QColor mColor;
	void populateList();
};
