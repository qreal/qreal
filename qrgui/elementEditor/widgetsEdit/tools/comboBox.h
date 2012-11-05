#pragma once

#include <QtGui/QComboBox>

#include "tool.h"

namespace qReal
{
namespace widgetsEdit
{

class ComboBox : public Tool
{
	Q_OBJECT

	Q_PROPERTY(bool duplicatesEnabled READ duplicatesEnabled WRITE setDuplicatesEnabled USER true DESIGNABLE true)
	Q_PROPERTY(bool editable READ isEditable WRITE setEditable USER true DESIGNABLE true)
	Q_PROPERTY(bool frame READ hasFrame WRITE setFrame USER true DESIGNABLE true)
	Q_PROPERTY(int maxCount READ maxCount WRITE setMaxCount USER true DESIGNABLE true)
	Q_PROPERTY(int maxVisibleItems READ maxVisibleItems WRITE setMaxVisibleItems USER true DESIGNABLE true)
	Q_PROPERTY(int minimumContentsLength READ minimumContentsLength WRITE setMinimumContentsLength USER true DESIGNABLE true)
	Q_PROPERTY(QComboBox::SizeAdjustPolicy sizeAdjustPolicy READ sizeAdjustPolicy WRITE setSizeAdjustPolicy USER true DESIGNABLE true)
	Q_PROPERTY(QString bindedPropertyName READ bindedPropertyName WRITE setBindedPropertyName USER true DESIGNABLE true)

public:
	explicit ComboBox(ToolController *controller);

private:

	bool duplicatesEnabled() const;
	bool isEditable() const;
	bool hasFrame() const;
	int maxCount() const;
	int maxVisibleItems() const;
	int minimumContentsLength() const;
	QComboBox::SizeAdjustPolicy sizeAdjustPolicy() const;
	QString bindedPropertyName() const;

	void setDuplicatesEnabled(bool enabled);
	void setEditable(bool editable);
	void setFrame(bool hasFrame);
	void setMaxCount(int maxCount);
	void setMaxVisibleItems(int maxVisibleItems);
	void setMinimumContentsLength(int length);
	void setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy policy);
	void setBindedPropertyName(QString const &name);

	QComboBox *mComboBox;
	QString mBindedPropertyName;
};

}
}
