#include "widgetsHelper.h"
#include "../nodeElement.h"
#include "../../elementEditor/widgetsEdit/widgetsEditor.h"
#include "../../elementEditor/widgetsEdit/propertyBinding/metaPropertyBinding.h"
#include "../../../qrutils/xmlUtils.h"

WidgetsHelper::WidgetsHelper(NodeElement * const element)
	: mElement(element), mEditorManager(NULL), mWidget(NULL)
{
	setParent(element);
	initLayoutFactoryPropertyEditor();
}

WidgetsHelper::~WidgetsHelper()
{
}

void WidgetsHelper::initLayoutFactoryPropertyEditor()
{
	if (!mElement) {
		return;
	}
	layouts::NodeElementLayoutFactory *factory = mElement->layoutFactory();
	if (!factory->binding().isEmpty()) {
		mPropertyEditors.insertMulti(factory->binding(), factory);
	}
}

bool WidgetsHelper::initWidget(QString const &filename)
{
	QDomDocument const document = utils::xmlUtils::loadDocument(filename);
	if (document.isNull()) {
		return false;
	}
	mPropertyEditors.clear();
	initLayoutFactoryPropertyEditor();
	QList<PropertyEditorInterface *> editors;
	mWidget = qReal::widgetsEdit::WidgetsEditor::deserializeWidget(document, editors);
	if (!mWidget) {
		return false;
	}
	if (mElement) {
		mElement->setWidget(mWidget);
	}

	foreach (PropertyEditorInterface *iface, editors) {
		mPropertyEditors.insertMulti(iface->binding(), iface);
		// Ugly spike. Workarround for problem that
		// stupid Qt doesn`t generate widget`s geometry changed signal
		MetaPropertyBinding *metaBinding = dynamic_cast<MetaPropertyBinding *>(iface);
		if (metaBinding && metaBinding->source() == "geometry") {
			mGeometryEditors << iface;
		}
	}

	return true;
}

void WidgetsHelper::onElementGeometryChanged()
{
	if (mElement->geometry() == mOldGeometry) {
		return;
	}
	mOldGeometry = mElement->geometry();
	foreach (PropertyEditorInterface *iface, mGeometryEditors) {
		iface->setPropertyValue(mOldGeometry);
	}
}

QMap<QString, PropertyEditorInterface *> WidgetsHelper::propertyEditors() const
{
	return mPropertyEditors;
}

void WidgetsHelper::setEditorManager(EditorManager const *editorManager)
{
	mEditorManager = editorManager;
	initEnumEditors();
}

void WidgetsHelper::onIdChanged() {
	mElement->updateData();
	initEnumEditors();
}

void WidgetsHelper::initEnumEditors()
{
	if (!mElement || mElement->id() == Id() || !mEditorManager) {
		return;
	}
	foreach (QString const &propertyName, mPropertyEditors.keys()) {
		QStringList const values = mEditorManager->getEnumValues(mElement->id(), propertyName);
		foreach (PropertyEditorInterface *editor, mPropertyEditors.values(propertyName)) {
			// If property is not enum empty list would be given and
			// will be ignored by property editor
			editor->setEnumValues(values);
		}
	}
}

QWidget *WidgetsHelper::widget() const
{
	return mWidget;
}

WtfIconLoader::WtfIconLoader()
	: mHelper(new WidgetsHelper)
{
}

WtfIconLoader::~WtfIconLoader()
{
	delete mHelper;
}

WtfIconLoader *WtfIconLoader::instance()
{
	static WtfIconLoader instance;
	return &instance;
}

QPixmap WtfIconLoader::pixmapOf(const QString &fileName)
{
	if (!mLoadedIcons.contains(fileName)) {
		QPixmap pixmap;
		if (mHelper->initWidget(fileName)) {
			pixmap = QPixmap::grabWidget(mHelper->widget());
		}
		mLoadedIcons.insert(fileName, pixmap);
	}
	return mLoadedIcons[fileName];
}

WtfIconEngineV2::WtfIconEngineV2(const QString &fileName)
	: mPixmap(WtfIconLoader::instance()->pixmapOf(fileName))
{
}

void WtfIconEngineV2::paint(QPainter *painter, QRect const &rect
		, QIcon::Mode mode, QIcon::State state)
{
	// TODO: remove copy-past from SdfIconEngineV2
	Q_UNUSED(mode)
	Q_UNUSED(state)
	painter->eraseRect(rect);
	int const rh = rect.height();
	int const rw = rect.width();

	int const ph = mPixmap.height();
	int const pw = mPixmap.width();
	if (pw == 0 || ph == 0) { // SUDDENLY!!11
		return;
	}
	// Take picture aspect into account;
	QRect resRect = rect;
	if (rh * pw < ph * rw) {
		resRect.setLeft(rect.left() + (rw-rh*pw/ph)/2);
		resRect.setRight(rect.right() - (rw-rh*pw/ph)/2);
	}
	if (rh * pw > ph * rw) {
		resRect.setTop(rect.top() + (rh-rw*ph/pw)/2);
		resRect.setBottom(rect.bottom() - (rh-rw*ph/pw)/2);
	}
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->drawPixmap(resRect, mPixmap);
}
