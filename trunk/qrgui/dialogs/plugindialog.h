#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <QDialog>
#include <QIcon>

class QLabel;
class QPushButton;
class QStringList;
class QTreeWidget;
class QTreeWidgetItem;

class EditorManager;

class PluginDialog : public QDialog
{
	Q_OBJECT

	public:
		PluginDialog(const EditorManager &mgr, QWidget *parent = 0);

	private:
		QLabel *label;
		QTreeWidget *treeWidget;
		QPushButton *okButton;
		QIcon interfaceIcon;
		QIcon featureIcon;
};

#endif
