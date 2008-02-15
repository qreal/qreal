#include <QPluginLoader>
#include <QStringList>
#include <QDir>

#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>

#include "plugindialog.h"

PluginDialog::PluginDialog(const QString &path, const QStringList &fileNames,
                           QWidget *parent) :
    QDialog(parent),
    label(new QLabel),
    treeWidget(new QTreeWidget),
    okButton(new QPushButton(tr("OK")))    
{
    treeWidget->setAlternatingRowColors(false);
    treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    treeWidget->setColumnCount(1);
    treeWidget->header()->hide();

    okButton->setDefault(true);

    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(2, 1);
    mainLayout->addWidget(label, 0, 0, 1, 3);
    mainLayout->addWidget(treeWidget, 1, 0, 1, 3);
    mainLayout->addWidget(okButton, 2, 1);
    setLayout(mainLayout);

    interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                            QIcon::Normal, QIcon::On);
    interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                            QIcon::Normal, QIcon::Off);
    featureIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

    setWindowTitle(tr("Plugin Information"));
    findPlugins(path, fileNames);
}

void PluginDialog::findPlugins(const QString &path,
                               const QStringList &fileNames)
{
    label->setText(tr("QReal found the following plugins\n"
                      "(looked in %1):")
                   .arg(QDir::toNativeSeparators(path)));

    const QDir dir(path);

    foreach (QObject *plugin, QPluginLoader::staticInstances())
        populateTreeWidget(plugin, tr("%1 (Static Plugin)")
                                   .arg(plugin->metaObject()->className()));

    foreach (QString fileName, fileNames) {
        QPluginLoader loader(dir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
            populateTreeWidget(plugin, fileName);
    }
}

void PluginDialog::populateTreeWidget(QObject *plugin, const QString &text)
{
    QTreeWidgetItem *pluginItem = new QTreeWidgetItem(treeWidget);
    pluginItem->setText(0, text);
    treeWidget->setItemExpanded(pluginItem, true);

    QFont boldFont = pluginItem->font(0);
    boldFont.setBold(true);
    pluginItem->setFont(0, boldFont);

    if (plugin) {
//        BrushInterface *iBrush = qobject_cast<BrushInterface *>(plugin);
//        if (iBrush)
//            addItems(pluginItem, "BrushInterface", iBrush->brushes());
	addItems(pluginItem, "SomeInterface", QStringList() );
    }
}

void PluginDialog::addItems(QTreeWidgetItem *pluginItem,
                            const char *interfaceName,
                            const QStringList &features)
{
    QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(pluginItem);
    interfaceItem->setText(0, interfaceName);
    interfaceItem->setIcon(0, interfaceIcon);

    foreach (QString feature, features) {
        if (feature.endsWith("..."))
            feature.chop(3);
        QTreeWidgetItem *featureItem = new QTreeWidgetItem(interfaceItem);
        featureItem->setText(0, feature);
        featureItem->setIcon(0, featureIcon);
    }
}
