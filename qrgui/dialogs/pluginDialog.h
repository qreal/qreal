#pragma once

#include <QDialog>
#include <QIcon>

class QLabel;
class QPushButton;
class QStringList;
class QTreeWidget;
class QTreeWidgetItem;

namespace qReal {

	class EditorManagerInterface;

	class PluginDialog : public QDialog
	{
		Q_OBJECT

	public:
		PluginDialog(const EditorManagerInterface *mgr, QWidget *parent = 0);

	private:
		QLabel *label;
		QTreeWidget *treeWidget;
		QPushButton *okButton;
		QIcon interfaceIcon;
		QIcon featureIcon;
	};

}
