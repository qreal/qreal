#pragma once

#include <QtCore/QHash>
#include <QtGui/QWidget>
#include <QtGui/QIcon>

#include "../kernel/NewType.h"

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

	void addDiagramType(NewType const &type, QString const &name);
	void addItemType(NewType const &type, QString const &name, QString const &description, QIcon const &icon);
	void initDone();
	void deleteDiagramType(NewType const &type);

	QComboBox* getComboBox();

	QVector<QString> getTabNames();

public slots:
	void setActiveEditor(int const editorIndex);

private:
	class DraggableElement : public QWidget {
	public:
		DraggableElement(NewType const &type, QString const &name, QString const &description,
						 QIcon const &icon, QWidget *parent = NULL);

		QIcon icon() const
		{
			return mIcon;
		}

		QString text() const
		{
			return mText;
		}

		NewType type() const
		{
			return mType;
		}

	private:
		NewType mType;
		QIcon mIcon;
		QString mText;
	};

	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);

	QHash<NewType, int> mCategories;
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
};

}

}
