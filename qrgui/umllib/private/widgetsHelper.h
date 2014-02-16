#pragma once

#include <QtCore/QMap>
#include <QtGui/QPixmap>
#include <qrgui/umllib/nodeElement.h>
#include <editorPluginInterface/widgetsHelperInterface.h>
#include <editorPluginInterface/sdfRendererInterface.h>
#include <editorPluginInterface/propertyEditorInterface.h>

namespace qReal {
class WidgetsHelper : public WidgetsHelperInterface
{
	Q_OBJECT

public:
	explicit WidgetsHelper(NodeElement * const element = 0 /* Accepts ownership on this */);
	virtual ~WidgetsHelper();

	QWidget *widget() const;
	virtual bool initWidget(QString const &filename);
	QMap<QString, PropertyEditorInterface *> propertyEditors() const;

	void setEditorManager(EditorManager const *editorManager);
	void onIdChanged();

	void onElementGeometryChanged();

private:
	void initEnumEditors();
	void initLayoutFactoryPropertyEditor();

	NodeElement *mElement;
	EditorManager const *mEditorManager;
	QWidget *mWidget;
	QMap<QString, PropertyEditorInterface *> mPropertyEditors;
	QList<PropertyEditorInterface *> mGeometryEditors;
	QRectF mOldGeometry;
};

class WtfIconEngineV2: public SdfIconEngineV2Interface
{
public:
	WtfIconEngineV2(QString const &fileName);
	virtual void paint(QPainter *painter, QRect const &rect, QIcon::Mode mode, QIcon::State state);
	virtual QIconEngine *clone() const;

private:
	QPixmap mPixmap;
};

class WtfIconLoader
{
public:
	/// Returns a pixmap of widget in specified wtf-file
	static QPixmap pixmapOf(QString const &fileName);

private:
	static WtfIconLoader *instance();

	WtfIconLoader();
	~WtfIconLoader();

	WidgetsHelper *mHelper;
	QMap<QString, QPixmap> mLoadedIcons;
};
}
