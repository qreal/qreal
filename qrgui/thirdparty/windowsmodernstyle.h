/****************************************************************************
* Modern Qt style for Windows
* Copyright (C) 2008-2009 Michał Męciński
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright
*      notice, this list of conditions and the following disclaimer in the
*      documentation and/or other materials provided with the distribution.
*   3. Neither the name of the copyright holder nor the names of the
*      contributors may be used to endorse or promote products derived from
*      this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
****************************************************************************/

#ifndef WINDOWSMODERNSTYLE_H
#define WINDOWSMODERNSTYLE_H

#include <QtGlobal>

#if !defined( Q_OS_WIN ) || ( QT_VERSION < 0x040300 ) || defined( QT_NO_STYLE_WINDOWSVISTA )
#define NO_STYLE_WINDOWSMODERN
#endif

#if !defined( NO_STYLE_WINDOWSMODERN )

// TODO: replace this spiky style with OK one

#include <QProxyStyle>

#include "thirdPartyDeclSpec.h"

/**
* Modern Qt style for Windows.
*
* %WindowsModernStyle is a Qt style for Windows which imitates the look
* of MS Office 2003. It changes the style of toolbars, menus, docked
* windows and toolboxes. The color scheme used by this style is
* automatically adjusted to system settings.
*/
class QRGUI_THIRDPARTY_EXPORT WindowsModernStyle : public QProxyStyle
{
	Q_OBJECT
public:
	/**
	* Default constructor.
	*/
	WindowsModernStyle();

	/**
	* Destructor.
	*/
	~WindowsModernStyle();

public: // overrides
	void polish( QApplication* application );
	void unpolish( QApplication* application );

	void polish( QPalette& palette );

	void polish( QWidget* widget );
	void unpolish( QWidget* widget );

	int pixelMetric( PixelMetric metric, const QStyleOption* option, const QWidget* widget ) const;

	int styleHint( StyleHint hint, const QStyleOption* option, const QWidget* widget,
		QStyleHintReturn* returnData = nullptr ) const;

	QSize sizeFromContents( ContentsType type, const QStyleOption* option,
		const QSize& contentsSize, const QWidget* widget ) const;

	QRect subElementRect( SubElement element, const QStyleOption* option, const QWidget* widget ) const;
	QRect subControlRect( ComplexControl control, const QStyleOptionComplex* option,
		SubControl subControl, const QWidget* widget ) const;

	void drawPrimitive( PrimitiveElement element, const QStyleOption* option,
		QPainter* painter, const QWidget* widget ) const;
	void drawControl( ControlElement element, const QStyleOption* option,
		QPainter* painter, const QWidget* widget ) const;
	void drawComplexControl( ComplexControl control, const QStyleOptionComplex* option,
		QPainter* painter, const QWidget* widget ) const;

public:
	enum {
		/**
		* Primitive element filled with the main window's background gradient.
		*/
		PE_WindowGradient = PE_CustomBase + 1
	};

protected slots:
	int layoutSpacingImplementation( QSizePolicy::ControlType control1, QSizePolicy::ControlType control2,
		Qt::Orientation orientation, const QStyleOption* option, const QWidget* widget ) const;

private:
	enum Mode
	{
		Blue,
		Silver,
		Olive,
		Classic
	};

private:
	Mode m_mode;

	// main window, toolbox
	QColor m_colorBackgroundBegin;
	QColor m_colorBackgroundEnd;

	// menu
	QColor m_colorMenuBorder;
	QColor m_colorMenuBackground;
	QColor m_colorMenuTitleBegin;
	QColor m_colorMenuTitleEnd;

	// toolbar, tab, toolbox
	QColor m_colorBarBegin;
	QColor m_colorBarMiddle;
	QColor m_colorBarEnd;

	// toolbar handle
	QColor m_colorHandle;
	QColor m_colorHandleLight;

	// menu, toolbar
	QColor m_colorSeparator;
	QColor m_colorSeparatorLight;

	// menu, toolbar, tab, toolbox
	QColor m_colorItemBorder;
	QColor m_colorItemBackgroundBegin;
	QColor m_colorItemBackgroundMiddle;
	QColor m_colorItemBackgroundEnd;
	QColor m_colorItemCheckedBegin;
	QColor m_colorItemCheckedMiddle;
	QColor m_colorItemCheckedEnd;
	QColor m_colorItemSunkenBegin;
	QColor m_colorItemSunkenMiddle;
	QColor m_colorItemSunkenEnd;

	// toolbar shadow, tab, toolbox tab
	QColor m_colorBorder;
	QColor m_colorBorderLight;
};

#endif // !defined( NO_STYLE_WINDOWSMODERN )

#endif
