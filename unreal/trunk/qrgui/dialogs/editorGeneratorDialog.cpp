#include "editorGeneratorDialog.h"
#include "ui_editorGeneratorDialog.h"

#include <QListWidgetItem>
#include <QDebug>

#include "../kernel/ids.h"
#include "../generators/editors/editorGenerator.h"

using namespace qReal;

EditorGeneratorDialog::EditorGeneratorDialog(qrRepo::RepoApi const &api)
	: mUi(new Ui::EditorGeneratorDialogUi), mApi(api)
{
	mUi->setupUi(this);

	IdList const rootDiagrams = mApi.children(ROOT_ID);

	foreach (Id const diagram, rootDiagrams) {
		IdList const elements = mApi.children(diagram);
		foreach (Id const element, elements) {
			QString const elementType = mApi.typeName(element);
			if (elementType == "mednMetaEditor") {
				mEditors.append(element);
				QListWidgetItem *item = new QListWidgetItem(mApi.name(element), mUi->listWidget);
				item->setData(Qt::UserRole, element.toVariant());
				mUi->listWidget->addItem(item);
			}
		}
	}

	connect(mUi->buttonMakeEditor, SIGNAL(clicked()), SLOT(createEditor()));
}

EditorGeneratorDialog::~EditorGeneratorDialog()
{
	delete mUi;
}

void EditorGeneratorDialog::createEditor()
{
	QListWidgetItem* editorItem = mUi->listWidget->currentItem();
	Id id = editorItem->data(Qt::UserRole).value<Id>();
	generators::EditorGenerator generator(mApi);
	generator.generate(id);
}
