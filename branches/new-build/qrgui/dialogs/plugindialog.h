#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <QDialog>
#include <QIcon>

class QLabel;
class QPushButton;
class QStringList;
class QTreeWidget;
class QTreeWidgetItem;

class PluginDialog : public QDialog
{
    Q_OBJECT

public:
    PluginDialog(const QString &path, const QStringList &fileNames,
                 QWidget *parent = 0);

private:
    void findPlugins(const QString &path, const QStringList &fileNames);
    void populateTreeWidget(QObject *plugin, const QString &text);
    void addItems(QTreeWidgetItem *pluginItem, const char *interfaceName,
                  const QStringList &features);

    QLabel *label;
    QTreeWidget *treeWidget;
    QPushButton *okButton;
    QIcon interfaceIcon;
    QIcon featureIcon;
};

#endif
