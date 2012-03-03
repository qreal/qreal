#pragma once

#include <QtCore/QHash>
#include <QtGui/QWidget>
#include <QtGui/QIcon>

#include "../pluginManager/editorManager.h"
#include "../pluginManager/interpreterEditorManager.h"
#include "../../qrkernel/ids.h"

class QVBoxLayout;
class QComboBox;
class QScrollArea;
class QDragEnterEvent;
class QDropEvent;

namespace qReal {

namespace gui {

class PaletteToolbox : public QWidget
{
	Q_OBJECT

public:
	explicit PaletteToolbox(QWidget *parent = NULL);
	~PaletteToolbox();

	void addDiagramType(Id const &id, QString const &name);
	void addItemType(Id const &id, QString const &name, QString const &description, QIcon const &icon);
	void initDone();
	void deleteDiagramType(Id const &id);
	QComboBox* getComboBox();

	/// Gets item types of diagram, sorts by displayed name and shows them
	void addSortedItemTypes(InterpreterEditorManager &editorManager, const Id &diagram);
	QVector<QString> getTabNames();

	Id currentTab();
	void setComboBox(Id id);

public slots:
	void setActiveEditor(int const editorIndex);
	void setActiveEditor(Id id);
	void recreateTabs();

private:
	class DraggableElement : public QWidget {
	public:
		DraggableElement(Id const &id, QString const &name, QString const &description,
				QIcon const &icon, QWidget *parent = NULL);

		QIcon icon() const
		{
			return mIcon;
		}

		QString text() const
		{
			return mText;
		}

		Id id() const
		{
			return mId;
		}

	private:
		Id mId;
		QIcon mIcon;
		QString mText;
	};

	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	void createPalette();
	void deletePalette();

	/// Method-comparator for sorting Ids by displayed name. Needs EditorManager instance to work,
	/// but qSort() prohibits it to be a member of an object. So making it static does the trick.
	static bool idLessThan(const Id &s1, const Id &s2);

	QHash<Id, int> mCategories;

	/// Vector of editors' contents
	QVector<QWidget*> mTabs;

	/// Vector of editors' names
	QVector<QString> mTabNames;

	/// Main layout
	QVBoxLayout *mLayout;

	/// Combobox with editors
	QComboBox *mComboBox;

	/// Area of current editor
	QScrollArea *mScrollArea;
	int mCurrentTab;

	/// EditorManager instance used to sort palette's content. Made static to be used inside idLessThan()
	static InterpreterEditorManager *mEditorManager;

};

}

}

