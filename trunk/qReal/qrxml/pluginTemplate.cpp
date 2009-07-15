#include "@@HEADER@@"

QStringList EditorTest_@@NAME@@_Plugin::elements () const
{
    QStringList myElements;
    myElements << "@@NAME@@ -> element 1";
    myElements << "@@NAME@@ -> element 2";
    return myElements;
}

void EditorTest_@@NAME@@_Plugin::testLoad(QWidget *parent)
{
    QMessageBox::information(parent, "QReal Plugin", "Plugin @@NAME@@ loaded");
}

Q_EXPORT_PLUGIN2(qreal_editors, EditorTest_@@NAME@@_Plugin)
