#pragma once

#include <QtCore/QHash>
#include <QtGui/QWidget>
#include <QtGui/QIcon>
#include <stdlib.h>
#include <../pluginManager/editorManager.h>
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
        void addSortedItemTypes(EditorManager &editorManager, const Id &diagram);
        static bool idLessThan(const Id &s1, const Id &s2);
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

	QHash<Id, int> mCategories;
	/** @brief vector of editors' contents */
	QVector<QWidget*> mTabs;
	/** @brief vector of editors' names */
	QVector<QString> mTabNames;
	/** @brief main layout */
	QVBoxLayout *mLayout;
	/** @brief Combobox with editors */
	QComboBox *mComboBox;
	/** @brief Area of current editor */
	QScrollArea *mScrollArea;
	int mCurrentTab;

};

}

}

