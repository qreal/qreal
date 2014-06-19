#pragma once

#include <QtWidgets/QComboBox>
#include <qrgui/elementEditor/widgetsEdit/propertyBinding/propertyProxyBase.h>

#include "tool.h"
#include "propertyEditor.h"

namespace qReal
{
namespace widgetsEdit
{

class ComboBoxWidget : public QComboBox, public PropertyEditor
{
	Q_OBJECT

public:
	explicit ComboBoxWidget();
	~ComboBoxWidget() override;

	void setPropertyValue(QVariant const &value) override;
	void setEnumValues(QList<QPair<QString, QString>> const &values) override;

private slots:
	void onSelectionChanged(int index);
};

class ComboBoxProxy : public ToolProxy
{
	Q_OBJECT

	Q_PROPERTY(bool duplicatesEnabled READ duplicatesEnabled WRITE setDuplicatesEnabled USER true DESIGNABLE true)
	Q_PROPERTY(bool editable READ isEditable WRITE setEditable USER true DESIGNABLE true)
	Q_PROPERTY(bool frame READ hasFrame WRITE setFrame USER true DESIGNABLE true)
	Q_PROPERTY(int maxCount READ maxCount WRITE setMaxCount USER true DESIGNABLE true)
	Q_PROPERTY(int maxVisibleItems READ maxVisibleItems WRITE setMaxVisibleItems USER true DESIGNABLE true)
	Q_PROPERTY(int minimumContentsLength READ minimumContentsLength
			   WRITE setMinimumContentsLength USER true DESIGNABLE true)
	Q_PROPERTY(QComboBox::SizeAdjustPolicy sizeAdjustPolicy READ sizeAdjustPolicy
			   WRITE setSizeAdjustPolicy USER true DESIGNABLE true)
	Q_PROPERTY(QString binding READ propertyName WRITE setPropertyName USER true DESIGNABLE true)
	BINDING_TOOL(mComboBox)

public:
	explicit ComboBoxProxy(ComboBoxWidget *comboBox);

private:
	bool duplicatesEnabled() const;
	bool isEditable() const;
	bool hasFrame() const;
	int maxCount() const;
	int maxVisibleItems() const;
	int minimumContentsLength() const;
	QComboBox::SizeAdjustPolicy sizeAdjustPolicy() const;

	void setDuplicatesEnabled(bool enabled);
	void setEditable(bool editable);
	void setFrame(bool hasFrame);
	void setMaxCount(int maxCount);
	void setMaxVisibleItems(int maxVisibleItems);
	void setMinimumContentsLength(int length);
	void setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy policy);

	ComboBoxWidget *mComboBox;
};

class ComboBox : public Tool
{
	Q_OBJECT

public:
	explicit ComboBox(ToolController *controller);

private:
	ComboBoxWidget *mComboBox;
};

}
}
