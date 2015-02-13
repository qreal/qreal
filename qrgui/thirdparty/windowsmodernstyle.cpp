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

#include "windowsmodernstyle.h"

#if !defined( NO_STYLE_WINDOWSMODERN )

#include <QStyleOption>
#include <QPainter>
#include <QLibrary>
#include <QApplication>
#include <QMainWindow>
#include <QAbstractButton>
#include <QToolBox>
#include <QScrollArea>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QLayout>

#if !defined( WMSTYLE_NO_PLUGIN )
#include <QStylePlugin>
#endif

#include <qt_windows.h>

typedef bool (WINAPI* PtrIsAppThemed)();
typedef HRESULT (WINAPI* PtrGetCurrentThemeName)( OUT LPWSTR pszThemeFileName, int cchMaxNameChars, OUT OPTIONAL LPWSTR pszColorBuff,
	int cchMaxColorChars, OUT OPTIONAL LPWSTR pszSizeBuff, int cchMaxSizeChars );

static PtrIsAppThemed pIsAppThemed = nullptr;
static PtrGetCurrentThemeName pGetCurrentThemeName = nullptr;

// :(
#define QWindowsVistaStyle QProxyStyle
#define QWindowsXPStyle QProxyStyle
#define QWindowsStyle QProxyStyle


static void resolveSymbols()
{
	static bool tried = false;
	if ( !tried ) {
		tried = true;
		QLibrary library( "uxtheme" );
		pIsAppThemed = (PtrIsAppThemed)library.resolve( "IsAppThemed" );
		pGetCurrentThemeName = (PtrGetCurrentThemeName)library.resolve( "GetCurrentThemeName" );
	}
}

WindowsModernStyle::WindowsModernStyle() : QWindowsVistaStyle(),
	m_mode( Classic )
{
	resolveSymbols();
}

WindowsModernStyle::~WindowsModernStyle()
{
}

static bool useVista()
{
	return QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA
		&& QSysInfo::WindowsVersion < QSysInfo::WV_NT_based;
}

static QColor colorRole( QPalette::ColorRole role )
{
	return QApplication::palette().color( role );
}

static QColor blendColors( const QColor& src, const QColor& dest, qreal alpha )
{
	qreal red = alpha * src.red() + ( 1.0 - alpha ) * dest.red();
	qreal green = alpha * src.green() + ( 1.0 - alpha ) * dest.green();
	qreal blue = alpha * src.blue() + ( 1.0 - alpha ) * dest.blue();
	return QColor( (int)( red + 0.5 ), (int)( green + 0.5 ), (int)( blue + 0.5 ) );
}

static QColor blendRoles( QPalette::ColorRole src, QPalette::ColorRole dest, qreal alpha )
{
	QPalette palette = QApplication::palette();
	return blendColors( palette.color( src ), palette.color( dest ), alpha );
}

void WindowsModernStyle::polish( QApplication* application )
{
	if ( useVista() )
		QWindowsVistaStyle::polish( application );
	else
		QWindowsXPStyle::polish( application );
}

void WindowsModernStyle::unpolish( QApplication* application )
{
	if ( useVista() )
		QWindowsVistaStyle::unpolish( application );
	else
		QWindowsXPStyle::unpolish( application );
}

void WindowsModernStyle::polish( QPalette& palette )
{
	if ( useVista() )
		QWindowsVistaStyle::polish( palette );
	else
		QWindowsXPStyle::polish( palette );

	m_mode = Classic;

	const int maxLength = 256;
	WCHAR themeFileName[ maxLength ];
	WCHAR themeColor[ maxLength ];

	if ( pIsAppThemed && pIsAppThemed() && pGetCurrentThemeName( themeFileName, maxLength, themeColor, maxLength, nullptr, 0 ) == S_OK ) {
		QString name = QString::fromWCharArray( themeFileName );
		QString color = QString::fromWCharArray( themeColor );
		if ( name.endsWith( "Luna.msstyles" ) ) {
			if ( color == QLatin1String( "HomeStead" ) )
				m_mode = Olive;
			else if ( color == QLatin1String( "Metallic" ) )
				m_mode = Silver;
			else
				m_mode = Blue;
		} else if ( name.endsWith( "Aero.msstyles" ) ) {
			m_mode = Blue;
		}
	}

	switch ( m_mode ) {
		case Blue:
			m_colorBackgroundBegin = qRgb( 158, 190, 245 );
			m_colorBackgroundEnd = qRgb( 196, 218, 250 );
			m_colorMenuBorder = qRgb( 0, 45, 150 );
			m_colorMenuBackground = qRgb( 246, 246, 246 );
			m_colorMenuTitleBegin = qRgb( 227, 239, 255 );
			m_colorMenuTitleEnd = qRgb( 123, 164, 224 );
			m_colorBarBegin = qRgb( 227, 239, 255 );
			m_colorBarMiddle = qRgb( 203, 225, 252 );
			m_colorBarEnd = qRgb( 123, 164, 224 );
			m_colorHandle = qRgb( 39, 65, 118 );
			m_colorHandleLight = qRgb( 255, 255, 255 );
			m_colorSeparator = qRgb( 106, 140, 203 );
			m_colorSeparatorLight = qRgb( 241, 249, 255 );
			m_colorItemBorder = qRgb( 0, 0, 128 );
			m_colorItemBackgroundBegin = qRgb( 255, 238, 190 );
			m_colorItemBackgroundMiddle = qRgb( 255, 225, 172 );
			m_colorItemBackgroundEnd = qRgb( 255, 200, 125 );
			m_colorItemCheckedBegin = qRgb( 255, 200, 125 );
			m_colorItemCheckedMiddle = qRgb( 255, 180, 100 );
			m_colorItemCheckedEnd = qRgb( 255, 150, 70 );
			m_colorItemSunkenBegin = qRgb( 254, 128, 62 );
			m_colorItemSunkenMiddle = qRgb( 255, 177, 109 );
			m_colorItemSunkenEnd = qRgb( 255, 223, 154 );
			m_colorBorder = qRgb( 68, 86, 134 );
			m_colorBorderLight = qRgb( 106, 140, 203 );
			break;

		case Silver:
			m_colorBackgroundBegin = qRgb( 215, 215, 229 );
			m_colorBackgroundEnd = qRgb( 243, 243, 247 );
			m_colorMenuBorder = qRgb( 124, 124, 148 );
			m_colorMenuBackground = qRgb( 253, 250, 255 );
			m_colorMenuTitleBegin = qRgb( 232, 233, 242 );
			m_colorMenuTitleEnd = qRgb( 172, 170, 194 );
			m_colorBarBegin = qRgb( 252, 252, 252 );
			m_colorBarMiddle = qRgb( 232, 233, 242 );
			m_colorBarEnd = qRgb( 172, 170, 194 );
			m_colorHandle = qRgb( 84, 84, 117 );
			m_colorHandleLight = qRgb( 255, 255, 255 );
			m_colorSeparator = qRgb( 110, 109, 143 );
			m_colorSeparatorLight = qRgb( 255, 255, 255 );
			m_colorItemBorder = qRgb( 75, 75, 111 );
			m_colorItemBackgroundBegin = qRgb( 255, 238, 190 );
			m_colorItemBackgroundMiddle = qRgb( 255, 225, 172 );
			m_colorItemBackgroundEnd = qRgb( 255, 200, 125 );
			m_colorItemCheckedBegin = qRgb( 255, 200, 125 );
			m_colorItemCheckedMiddle = qRgb( 255, 180, 100 );
			m_colorItemCheckedEnd = qRgb( 255, 150, 70 );
			m_colorItemSunkenBegin = qRgb( 254, 128, 62 );
			m_colorItemSunkenMiddle = qRgb( 255, 177, 109 );
			m_colorItemSunkenEnd = qRgb( 255, 223, 154 );
			m_colorBorder = qRgb( 110, 109, 143 );
			m_colorBorderLight = qRgb( 145, 144, 173 );
			break;

		case Olive:
			m_colorBackgroundBegin = qRgb( 217, 217, 167 );
			m_colorBackgroundEnd = qRgb( 242, 241, 228 );
			m_colorMenuBorder = qRgb( 117, 141, 94 );
			m_colorMenuBackground = qRgb( 244, 244, 238 );
			m_colorMenuTitleBegin = qRgb( 237, 240, 214 );
			m_colorMenuTitleEnd = qRgb( 181, 196, 143 );
			m_colorBarBegin = qRgb( 255, 255, 237 );
			m_colorBarMiddle = qRgb( 206, 220, 167 );
			m_colorBarEnd = qRgb( 181, 196, 143 );
			m_colorHandle = qRgb( 81, 94, 51 );
			m_colorHandleLight = qRgb( 255, 255, 255 );
			m_colorSeparator = qRgb( 96, 128, 88 );
			m_colorSeparatorLight = qRgb( 244, 247, 222 );
			m_colorItemBorder = qRgb( 63, 93, 56 );
			m_colorItemBackgroundBegin = qRgb( 255, 238, 190 );
			m_colorItemBackgroundMiddle = qRgb( 255, 225, 172 );
			m_colorItemBackgroundEnd = qRgb( 255, 200, 125 );
			m_colorItemCheckedBegin = qRgb( 255, 200, 125 );
			m_colorItemCheckedMiddle = qRgb( 255, 180, 100 );
			m_colorItemCheckedEnd = qRgb( 255, 150, 70 );
			m_colorItemSunkenBegin = qRgb( 254, 128, 62 );
			m_colorItemSunkenMiddle = qRgb( 255, 177, 109 );
			m_colorItemSunkenEnd = qRgb( 255, 223, 154 );
			m_colorBorder = qRgb( 96, 128, 88 );
			m_colorBorderLight = qRgb( 151, 166, 123 );
			break;

		case Classic:
			m_colorBackgroundBegin = colorRole( QPalette::Button );
			m_colorBackgroundEnd = blendRoles( QPalette::Button, QPalette::Base, 0.205 );
			m_colorMenuBorder = blendRoles( QPalette::Text, QPalette::Dark, 0.2 );
			m_colorMenuBackground = blendRoles( QPalette::Button, QPalette::Base, 0.143 );
			m_colorMenuTitleBegin = blendRoles( QPalette::Button, QPalette::Base, 0.2 );
			m_colorMenuTitleEnd = blendRoles( QPalette::Button, QPalette::Base, 0.5 );
			m_colorBarBegin = blendRoles( QPalette::Button, QPalette::Base, 0.2 );
			m_colorBarMiddle = blendRoles( QPalette::Button, QPalette::Base, 0.5 );
			m_colorBarEnd = colorRole( QPalette::Button );
			m_colorHandle = blendRoles( QPalette::Dark, QPalette::Base, 0.75 );
			m_colorHandleLight = colorRole( QPalette::Light );
			m_colorSeparator = blendRoles( QPalette::Dark, QPalette::Base, 0.7 );
			m_colorSeparatorLight = colorRole( QPalette::Light );
			m_colorItemBorder = colorRole( QPalette::Highlight );
			m_colorItemBackgroundBegin = blendRoles( QPalette::Highlight, QPalette::Base, 0.3 );
			m_colorItemBackgroundMiddle = m_colorItemBackgroundEnd = m_colorItemBackgroundBegin;
			m_colorItemCheckedBegin = blendRoles( QPalette::Highlight, QPalette::Base, 0.15 );
			m_colorItemCheckedMiddle = m_colorItemCheckedEnd = m_colorItemCheckedBegin;
			m_colorItemSunkenBegin = blendRoles( QPalette::Highlight, QPalette::Base, 0.45 );
			m_colorItemSunkenMiddle = m_colorItemSunkenEnd = m_colorItemSunkenBegin;
			m_colorBorder = colorRole( QPalette::Dark );
			m_colorBorderLight = blendRoles( QPalette::Dark, QPalette::Base, 0.8 );
			break;
	}
}

static const QTabWidget* isStyledTabWidget( const QWidget* widget )
{
	const QTabWidget* tabWidget = qobject_cast<const QTabWidget*>( widget );
	if ( tabWidget && tabWidget->tabShape() == QTabWidget::Rounded
		&& ( tabWidget->tabPosition() == QTabWidget::North || tabWidget->tabPosition() == QTabWidget::South ) ) {
		if ( qobject_cast<const QMainWindow*>( tabWidget->window() ) )
			return tabWidget;
	}
	return nullptr;
}

static const QTabBar* isStyledTabBar( const QWidget* widget )
{
	const QTabBar* tabBar = qobject_cast<const QTabBar*>( widget );
	if ( tabBar && ( tabBar->shape() == QTabBar::RoundedNorth || tabBar->shape() == QTabBar::RoundedSouth ) ) {
		if ( qobject_cast<const QMainWindow*>( tabBar->window() ) )
			return tabBar;
	}
	return nullptr;
}

static bool isToolBoxButton( const QWidget* widget )
{
	const QAbstractButton* button = qobject_cast<const QAbstractButton*>( widget );
	if ( button && qobject_cast<const QToolBox*>( button->parentWidget() ) )
		return true;
	return false;
}

static bool isToolBoxPanel( const QWidget* widget )
{
	const QWidget* viewport = widget->parentWidget();
	if ( viewport ) {
		const QScrollArea* scrollArea = qobject_cast<const QScrollArea*>( viewport->parentWidget() );
		if ( scrollArea && qobject_cast<const QToolBox*>( scrollArea->parentWidget() ) )
			return true;
	}
	return false;
}

void WindowsModernStyle::polish( QWidget* widget )
{
	if ( qobject_cast<QMainWindow*>( widget ) )
		widget->setAttribute( Qt::WA_StyledBackground );

	if ( qobject_cast<QToolBox*>( widget ) ) {
		widget->setAttribute( Qt::WA_StyledBackground );
		widget->layout()->setMargin( 2 );
	}

	if ( isToolBoxButton( widget ) ) {
		widget->setAttribute( Qt::WA_Hover );
		QSizePolicy policy = widget->sizePolicy();
		policy.setControlType( QSizePolicy::PushButton );
		widget->setSizePolicy( policy );
	}

	if ( isToolBoxPanel( widget ) )
		widget->setAttribute( Qt::WA_StyledBackground );

	if ( isStyledTabBar( widget ) )
		widget->setAttribute( Qt::WA_Hover );

	if ( useVista() )
		QWindowsVistaStyle::polish( widget );
	else
		QWindowsXPStyle::polish( widget );
}

void WindowsModernStyle::unpolish( QWidget* widget )
{
	if ( qobject_cast<QMainWindow*>( widget ) )
		widget->setAttribute( Qt::WA_StyledBackground, false );

	if ( qobject_cast<QMainWindow*>( widget ) )
		widget->setAttribute( Qt::WA_StyledBackground, false );

	if ( qobject_cast<QToolBox*>( widget ) )
		widget->setAttribute( Qt::WA_StyledBackground, false );

	if ( isToolBoxButton( widget ) )
		widget->setAttribute( Qt::WA_Hover, false );

	if ( isToolBoxPanel( widget ) )
		widget->setAttribute( Qt::WA_StyledBackground, false );

	if ( isStyledTabBar( widget ) )
		widget->setAttribute( Qt::WA_Hover, false );

	if ( useVista() )
		QWindowsVistaStyle::unpolish( widget );
	else
		QWindowsXPStyle::unpolish( widget );
}

int WindowsModernStyle::pixelMetric( PixelMetric metric, const QStyleOption* option, const QWidget* widget ) const
{
	switch ( metric ) {
		case PM_MenuBarPanelWidth:
			return 0;
		case PM_MenuBarVMargin:
		case PM_MenuBarHMargin:
			return 2;
		case PM_MenuPanelWidth:
			return 1;
		case PM_MenuHMargin:
			return 0;
		case PM_MenuVMargin:
			return 1;

		case PM_ToolBarFrameWidth:
			return 2;
		case PM_ToolBarItemMargin:
		case PM_ToolBarItemSpacing:
			return 0;
		case PM_ToolBarIconSize:
			return 16;

		case PM_MenuButtonIndicator:
			return 12;

		case PM_ButtonShiftVertical:
		case PM_ButtonShiftHorizontal:
			if ( widget && qobject_cast<QToolBar*>( widget->parentWidget() ) )
				return 0;
			break;

		case PM_DockWidgetSeparatorExtent:
			return 4;
		case PM_DockWidgetTitleBarButtonMargin:
			return 4;
		case PM_DockWidgetTitleMargin:
			return 3;

		case PM_LayoutVerticalSpacing:
			if ( qobject_cast<const QToolBox*>( widget ) )
				return -1;
			break;

		case PM_TabBarBaseOverlap:
			if ( isStyledTabWidget( widget ) || isStyledTabBar( widget ) )
				return 0;
			break;
		case PM_TabBarTabShiftVertical:
			if ( const QTabBar* tabBar = isStyledTabBar( widget ) )
				return ( tabBar->shape() == QTabBar::RoundedSouth ) ? -2 : 2;
			break;

		default:
			break;
	}

	if ( useVista() )
		return QWindowsVistaStyle::pixelMetric( metric, option, widget );
	else
		return QWindowsXPStyle::pixelMetric( metric, option, widget );
}

int WindowsModernStyle::styleHint( StyleHint hint, const QStyleOption* option, const QWidget* widget,
	QStyleHintReturn* returnData ) const
{
	if ( useVista() )
		return QWindowsVistaStyle::styleHint( hint, option, widget, returnData );
	else
		return QWindowsXPStyle::styleHint( hint, option, widget, returnData );
}

QSize WindowsModernStyle::sizeFromContents( ContentsType type, const QStyleOption* option,
	const QSize& contentsSize, const QWidget* widget ) const
{
	switch ( type ) {
		case CT_MenuBar:
			return contentsSize - QSize( 0, 1 );

		case CT_Menu:
#if ( QT_VERSION < 0x040400 )
			return contentsSize - QSize( 0, 1 );
#else
			return contentsSize;
#endif

		case CT_MenuBarItem:
			return contentsSize + QSize( 16, 6 );

		case CT_MenuItem:
			if ( const QStyleOptionMenuItem* menuItem = qstyleoption_cast<const QStyleOptionMenuItem*>( option ) ) {
				if ( menuItem->menuItemType == QStyleOptionMenuItem::Separator )
					return QSize( 10, 3 );
				int space = 32 + 16;
				if ( menuItem->text.contains( '\t' ) )
					space += 12;
				return QSize( contentsSize.width() + space, 22 );
			}
			break;

		default:
			break;
	}

	if ( useVista() )
		return QWindowsVistaStyle::sizeFromContents( type, option, contentsSize, widget );
	else
		return QWindowsXPStyle::sizeFromContents( type, option, contentsSize, widget );
}

QRect WindowsModernStyle::subElementRect( SubElement element, const QStyleOption* option, const QWidget* widget ) const
{
	QRect rect;
	if ( useVista() )
		rect = QWindowsVistaStyle::subElementRect( element, option, widget );
	else
		rect = QWindowsXPStyle::subElementRect( element, option, widget );

	switch ( element ) {
		case SE_DockWidgetCloseButton:
		case SE_DockWidgetFloatButton:
			rect.translate( -2, 0 );
			break;

		case SE_TabWidgetTabContents:
			if ( isStyledTabWidget( widget ) )
				rect = QWindowsStyle::subElementRect( SE_TabWidgetTabPane, option, widget );
			break;

		case SE_TabWidgetTabBar:
			if ( isStyledTabWidget( widget ) )
				rect = QWindowsStyle::subElementRect( SE_TabWidgetTabBar, option, widget );
			break;

		default:
			break;
	}

	return rect;
}

QRect WindowsModernStyle::subControlRect( ComplexControl control, const QStyleOptionComplex* option,
	SubControl subControl, const QWidget* widget ) const
{
	if ( useVista() )
		return QWindowsVistaStyle::subControlRect( control, option, subControl, widget );
	else
		return QWindowsXPStyle::subControlRect( control, option, subControl, widget );
}

int WindowsModernStyle::layoutSpacingImplementation( QSizePolicy::ControlType control1, QSizePolicy::ControlType control2,
	Qt::Orientation /*orientation*/, const QStyleOption* /*option*/, const QWidget* widget ) const
{
	if ( qobject_cast<const QToolBox*>( widget ) ) {
		if ( control1 == QSizePolicy::PushButton && control2 == QSizePolicy::DefaultType )
			return 0;
		else if ( control1 == QSizePolicy::DefaultType && control2 == QSizePolicy::PushButton )
			return 3;
		else
			return 2;
	}

	return 6;
}

void WindowsModernStyle::drawPrimitive( PrimitiveElement element, const QStyleOption* option,
	QPainter* painter, const QWidget* widget ) const
{
	switch ( element ) {
		case PE_Widget:
			if ( qobject_cast<const QMainWindow*>( widget ) ) {
				QRect rect = option->rect;
				if ( QStatusBar* statusBar = widget->findChild<QStatusBar*>() ) {
					rect.adjust( 0, 0, 0, -statusBar->height() );
					painter->setPen( option->palette.light().color() );
					painter->drawLine( rect.bottomLeft() + QPoint( 0, 1 ),
						rect.bottomRight() + QPoint( 0, 1 ) );
				}
				QLinearGradient gradient( option->rect.topLeft(), option->rect.topRight() );
				gradient.setColorAt( 0.0, m_colorBackgroundBegin );
				gradient.setColorAt( 0.6, m_colorBackgroundEnd );
				painter->fillRect( rect, gradient );
				return;
			}

			if ( qobject_cast<const QToolBox*>( widget ) ) {
				QLinearGradient gradient( option->rect.topLeft(), option->rect.topRight() );
				gradient.setColorAt( 0.4, m_colorBackgroundBegin );
				gradient.setColorAt( 1.0, m_colorBackgroundEnd );
				painter->fillRect( option->rect, gradient );
				return;
			}

			if ( isToolBoxPanel( widget ) ) {
				QLinearGradient gradient( option->rect.topLeft(), option->rect.topRight() );
				gradient.setColorAt( 0.4, m_colorBarMiddle );
				gradient.setColorAt( 1.0, m_colorBarBegin );
				painter->fillRect( option->rect, gradient );
				return;
			}
			break;

		case PE_WindowGradient: {
			QLinearGradient gradient( option->rect.topLeft(), option->rect.topRight() );
			gradient.setColorAt( 0.0, m_colorBackgroundBegin );
			gradient.setColorAt( 0.6, m_colorBackgroundEnd );
			painter->fillRect( option->rect, gradient );
			return;
		}

		case PE_PanelMenuBar:
			return;

		case PE_FrameMenu:
			painter->setPen( m_colorMenuBorder );
			painter->setBrush( Qt::NoBrush );
			painter->drawRect( option->rect.adjusted( 0, 0, -1, -1 ) );

			if ( const QMenu* menu = qobject_cast<const QMenu*>( widget ) ) {
				if ( const QMenuBar* menuBar = qobject_cast<const QMenuBar*>( menu->parent() ) ) {
					QRect rect = menuBar->actionGeometry( menu->menuAction() );
					if ( !rect.isEmpty() ) {
						painter->setPen( m_colorMenuBackground );
						painter->drawLine( 1, 0, rect.width() - 2, 0 );
					}
				}
			}

			if ( const QToolBar* toolBar = qobject_cast<const QToolBar*>( widget ) ) {
				QRect rect = option->rect.adjusted( 1, 1, -1, -1 );
				QLinearGradient gradient;
				if ( toolBar->orientation() == Qt::Vertical )
					gradient = QLinearGradient( rect.topLeft(), rect.topRight() );
				else
					gradient = QLinearGradient( rect.topLeft(), rect.bottomLeft() );
				gradient.setColorAt( 0.0, m_colorBarBegin );
				gradient.setColorAt( 0.4, m_colorBarMiddle );
				gradient.setColorAt( 0.6, m_colorBarMiddle );
				gradient.setColorAt( 1.0, m_colorBarEnd );
				painter->fillRect( rect, gradient );
			}
			return;

		case PE_IndicatorToolBarHandle:
			if ( option->state & State_Horizontal ) {
				for ( int i = option->rect.height() / 5; i <= 4 * ( option->rect.height() / 5 ); i += 5 ) {
					int x = option->rect.left() + 3;
					int y = option->rect.top() + i + 1;
					painter->fillRect( x + 1, y, 2, 2, m_colorHandleLight );
					painter->fillRect( x, y - 1, 2, 2, m_colorHandle );
				}
			} else {
				for ( int i = option->rect.width() / 5; i <= 4 * ( option->rect.width() / 5 ); i += 5 ) {
					int x = option->rect.left() + i + 1;
					int y = option->rect.top() + 3;
					painter->fillRect( x, y + 1, 2, 2, m_colorHandleLight );
					painter->fillRect( x - 1, y, 2, 2, m_colorHandle );
				}
			}
			return;

		case PE_IndicatorToolBarSeparator:
			painter->setPen( m_colorSeparator );
			if ( option->state & State_Horizontal )
				painter->drawLine( ( option->rect.left() + option->rect.right() - 1 ) / 2, option->rect.top() + 2,
					( option->rect.left() + option->rect.right() - 1 ) / 2, option->rect.bottom() - 2 );
			else
				painter->drawLine( option->rect.left() + 2, ( option->rect.top() + option->rect.bottom() - 1 ) / 2,
					option->rect.right() - 2, ( option->rect.top() + option->rect.bottom() - 1 ) / 2 );
			painter->setPen( m_colorSeparatorLight );
			if ( option->state & State_Horizontal )
				painter->drawLine( ( option->rect.left() + option->rect.right() + 1 ) / 2, option->rect.top() + 2,
					( option->rect.left() + option->rect.right() + 1 ) / 2, option->rect.bottom() - 2 );
			else
				painter->drawLine( option->rect.left() + 2, ( option->rect.top() + option->rect.bottom() + 1 ) / 2,
					option->rect.right() - 2, ( option->rect.top() + option->rect.bottom() + 1 ) / 2 );
			return;

		case PE_IndicatorButtonDropDown: {
			QToolBar* toolBar;
			if ( widget && ( toolBar = qobject_cast<QToolBar*>( widget->parentWidget() ) ) ) {
				QRect rect = option->rect.adjusted( -1, 0, -1, -1 );
				bool selected = option->state & State_MouseOver && option->state & State_Enabled;
				bool sunken = option->state & State_Sunken;
				if ( selected || sunken ) {
					painter->setPen( m_colorItemBorder );
					if ( toolBar->orientation() == Qt::Vertical ) {
						if ( sunken )
							painter->setBrush( m_colorItemSunkenEnd );
						else
							painter->setBrush( m_colorItemBackgroundEnd );
					} else {
						QLinearGradient gradient( rect.topLeft(), rect.bottomLeft() );
						if ( sunken ) {
							gradient.setColorAt( 0.0, m_colorItemSunkenBegin );
							gradient.setColorAt( 0.5, m_colorItemSunkenMiddle );
							gradient.setColorAt( 1.0, m_colorItemSunkenEnd );
						} else {
							gradient.setColorAt( 0.0, m_colorItemBackgroundBegin );
							gradient.setColorAt( 0.5, m_colorItemBackgroundMiddle );
							gradient.setColorAt( 1.0, m_colorItemBackgroundEnd );
						}
						painter->setBrush( gradient );
					}
					painter->drawRect( rect );
				}
				QStyleOption optionArrow = *option;
				optionArrow.rect.adjust( 2, 2, -2, -2 );
				drawPrimitive( PE_IndicatorArrowDown, &optionArrow, painter, widget );
				return;
			}
		}

		case PE_IndicatorDockWidgetResizeHandle:
			return;

		case PE_PanelButtonTool:
			if ( widget && widget->inherits( "QDockWidgetTitleButton" ) ) {
				if ( option->state & ( QStyle::State_MouseOver | QStyle::State_Sunken ) ) {
					painter->setPen( m_colorItemBorder );
					painter->setBrush( ( option->state & QStyle::State_Sunken ) ? m_colorItemSunkenMiddle : m_colorItemBackgroundMiddle );
					painter->drawRect( option->rect.adjusted( 0, 0, -1, -1 ) );
				}
				return;
			}
			break;

		case PE_FrameTabWidget:
			if ( isStyledTabWidget( widget ) ) {
				painter->fillRect( option->rect, option->palette.window() );
				return;
			}
			break;

		case PE_FrameTabBarBase:
			if ( isStyledTabBar( widget ) )
				return;
			break;

		default:
			break;
	}

	if ( useVista() )
		QWindowsVistaStyle::drawPrimitive( element, option, painter, widget );
	else
		QWindowsXPStyle::drawPrimitive( element, option, painter, widget );
}

void WindowsModernStyle::drawControl( ControlElement element, const QStyleOption* option,
	QPainter* painter, const QWidget* widget ) const
{
	switch ( element ) {
		case CE_MenuBarEmptyArea:
			return;

		case CE_MenuBarItem:
			if ( option->state & QStyle::State_Sunken && option->state & QStyle::State_Enabled ) {
				painter->setPen( m_colorMenuBorder );
				QLinearGradient gradient( option->rect.topLeft(), option->rect.bottomLeft() );
				gradient.setColorAt( 0.0, m_colorMenuTitleBegin );
				gradient.setColorAt( 1.0, m_colorMenuTitleEnd );
				painter->setBrush( gradient );
				painter->drawRect( option->rect.adjusted( 0, 0, -1, 0 ) );
			} else if ( option->state & QStyle::State_Selected && option->state & QStyle::State_Enabled ) {
				painter->setPen( m_colorItemBorder );
				QLinearGradient gradient( option->rect.topLeft(), option->rect.bottomLeft() );
				gradient.setColorAt( 0.0, m_colorItemBackgroundBegin );
				gradient.setColorAt( 1.0, m_colorItemBackgroundEnd );
				painter->setBrush( gradient );
				painter->drawRect( option->rect.adjusted( 0, 0, -1, -1 ) );
			}
			if ( const QStyleOptionMenuItem* optionItem = qstyleoption_cast<const QStyleOptionMenuItem*>( option ) ) {
				int flags = Qt::AlignCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
				if ( !styleHint( SH_UnderlineShortcut, option, widget ) )
					flags |= Qt::TextHideMnemonic;
				if ( !optionItem->icon.isNull() ) {
					QPixmap pixmap = optionItem->icon.pixmap( pixelMetric( PM_SmallIconSize, option, widget ), QIcon::Normal );
					drawItemPixmap( painter, option->rect, flags, pixmap );
				} else {
					drawItemText( painter, option->rect, flags, option->palette, true, optionItem->text, QPalette::Text );
				}
			}
			return;

		case CE_MenuEmptyArea:
			painter->fillRect( option->rect, m_colorMenuBackground );
			return;

		case CE_MenuItem: {
			if ( option->state & QStyle::State_Selected && option->state & QStyle::State_Enabled ) {
				painter->setPen( m_colorItemBorder );
				painter->setBrush( m_colorItemBackgroundBegin );
				painter->drawRect( option->rect.adjusted( 1, 0, -3, -1 ) );
			} else {
				QLinearGradient gradient( QPoint( 0, 0 ), QPoint( 25, 0 ) );
				gradient.setColorAt( 0.0, m_colorBarBegin );
				gradient.setColorAt( 1.0, m_colorBarEnd );
				QRect margin = option->rect;
				margin.setWidth( 25 );
				painter->fillRect( margin, gradient );
				QRect background = option->rect;
				background.setLeft( margin.right() + 1 );
				painter->fillRect( background, m_colorMenuBackground );
			}
			if ( const QStyleOptionMenuItem* optionItem = qstyleoption_cast<const QStyleOptionMenuItem*>( option ) ) {
				if ( optionItem->menuItemType == QStyleOptionMenuItem::Separator ) {
					painter->setPen( m_colorSeparator );
					painter->drawLine( option->rect.left() + 32, ( option->rect.top() + option->rect.bottom() ) / 2,
						option->rect.right(), ( option->rect.top() + option->rect.bottom() ) / 2 );
					return;
				}
				QRect checkRect = option->rect.adjusted( 2, 1, -2, -2 );
				checkRect.setWidth( 20 );
				if ( optionItem->checked && option->state & QStyle::State_Enabled ) {
					painter->setPen( m_colorItemBorder );
					if ( option->state & QStyle::State_Selected && option->state & QStyle::State_Enabled )
						painter->setBrush( m_colorItemSunkenBegin );
					else
						painter->setBrush( m_colorItemCheckedBegin );
					painter->drawRect( checkRect );
				}
				if ( !optionItem->icon.isNull() ) {
					QIcon::Mode mode;
					if ( optionItem->state & State_Enabled )
						mode = ( optionItem->state & State_Selected ) ? QIcon::Active : QIcon::Normal;
					else
						mode = QIcon::Disabled;
					QIcon::State state = optionItem->checked ? QIcon::On : QIcon::Off;
					QPixmap pixmap = optionItem->icon.pixmap( pixelMetric( PM_SmallIconSize, option, widget ), mode, state );
					QRect rect = pixmap.rect();
					rect.moveCenter( checkRect.center() );
					painter->drawPixmap( rect.topLeft(), pixmap );
				} else if ( optionItem->checked ) {
					QStyleOption optionCheckMark;
					optionCheckMark.initFrom( widget );
					optionCheckMark.rect = checkRect;
					if ( !( option->state & State_Enabled ) )
						optionCheckMark.palette.setBrush( QPalette::Text, optionCheckMark.palette.brush( QPalette::Disabled, QPalette::Text ) );
					drawPrimitive( PE_IndicatorMenuCheckMark, &optionCheckMark, painter, widget );
				}
				QRect textRect = option->rect.adjusted( 32, 1, -16, -1 );
				int flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
				if ( !styleHint( SH_UnderlineShortcut, option, widget ) )
					flags |= Qt::TextHideMnemonic;
				QString text = optionItem->text;
				int pos = text.indexOf( '\t' );
				if ( pos >= 0 ) {
					drawItemText( painter, textRect, flags | Qt::AlignRight, option->palette, option->state & State_Enabled,
						text.mid( pos + 1 ), QPalette::Text );
					text = text.left( pos );
				}
				drawItemText( painter, textRect, flags, option->palette, option->state & State_Enabled, text, QPalette::Text );
				if ( optionItem->menuItemType == QStyleOptionMenuItem::SubMenu ) {
					QStyleOption optionArrow;
					optionArrow.initFrom( widget );
					optionArrow.rect = option->rect.adjusted( 0, 4, -4, -4 );
					optionArrow.rect.setLeft( option->rect.right() - 12 );
					optionArrow.state = option->state & State_Enabled;
					drawPrimitive( PE_IndicatorArrowRight, &optionArrow, painter, widget );
				}
			}
			return;
		}

		case CE_ToolBar: {
			QRect rect = option->rect;
			bool vertical = false;
			if ( const QToolBar* toolBar = qobject_cast<const QToolBar*>( widget ) ) {
				vertical = ( toolBar->orientation() == Qt::Vertical );
				if ( vertical )
					rect.setBottom( toolBar->childrenRect().bottom() + 2 );
				else
					rect.setRight( toolBar->childrenRect().right() + 2 );
			}
			painter->save();
			QRegion region = rect.adjusted( 2, 0, -2, 0 );
			region += rect.adjusted( 0, 2, 0, -2 );
			region += rect.adjusted( 1, 1, -1, -1 );
			painter->setClipRegion( region );
			QLinearGradient gradient;
			if ( vertical )
				gradient = QLinearGradient( rect.topLeft(), rect.topRight() );
			else
				gradient = QLinearGradient( rect.topLeft(), rect.bottomLeft() );
			gradient.setColorAt( 0.0, m_colorBarBegin );
			gradient.setColorAt( 0.4, m_colorBarMiddle );
			gradient.setColorAt( 0.6, m_colorBarMiddle );
			gradient.setColorAt( 1.0, m_colorBarEnd );
			painter->fillRect( rect, gradient );

			painter->setPen( vertical ? m_colorBorderLight : m_colorBorder );
			painter->drawLine( rect.bottomLeft() + QPoint( 2, 0 ), rect.bottomRight() - QPoint( 2, 0 ) );
			painter->setPen( vertical ? m_colorBorder : m_colorBorderLight );
			painter->drawLine( rect.topRight() + QPoint( 0, 2 ), rect.bottomRight() - QPoint( 0, 2 ) );
			painter->setPen( m_colorBorderLight );
			painter->drawPoint( rect.bottomRight() - QPoint( 1, 1 ) );
			painter->restore();
			return;
		}

		case CE_DockWidgetTitle: {
			QLinearGradient gradient( option->rect.topLeft(), option->rect.bottomLeft() );
			gradient.setColorAt( 0.0, m_colorBarBegin );
			gradient.setColorAt( 1.0, m_colorBarEnd );
			painter->fillRect( option->rect, gradient );
			if ( const QStyleOptionDockWidget* optionDockWidget = qstyleoption_cast<const QStyleOptionDockWidget*>( option ) ) {
				QRect rect = option->rect.adjusted( 6, 0, -4, 0 );
				if ( optionDockWidget->closable )
					rect.adjust( 0, 0, -16, 0 );
				if ( optionDockWidget->floatable )
					rect.adjust( 0, 0, -16, 0 );
				QString text = painter->fontMetrics().elidedText( optionDockWidget->title, Qt::ElideRight, rect.width() );
				drawItemText( painter, rect, Qt::AlignLeft | Qt::AlignVCenter, option->palette,
					option->state & State_Enabled, text, QPalette::WindowText );
			}
			return;
		}

		case CE_TabBarTabShape:
			if ( isStyledTabBar( widget ) ) {
				bool firstTab = false;
				bool lastTab = false;
				bool bottom = false;
				if ( const QStyleOptionTab* optionTab = qstyleoption_cast<const QStyleOptionTab*>( option ) ) {
					if ( optionTab->position == QStyleOptionTab::Beginning )
						firstTab = true;
					else if ( optionTab->position == QStyleOptionTab::End )
						lastTab = true;
					else if ( optionTab->position == QStyleOptionTab::OnlyOneTab )
						firstTab = lastTab = true;
					if ( optionTab->shape == QTabBar::RoundedSouth )
						bottom = true;
				}
				QRect rect = option->rect;
				painter->save();
				if ( option->state & State_Selected ) {
					if ( bottom )
						rect.adjust( firstTab ? 0 : -2, -1, lastTab ? -1 : 1, -1 );
					else
						rect.adjust( firstTab ? 0 : -2, 0, lastTab ? -1 : 1, 1 );
				} else {
					if ( bottom ) {
						rect.adjust( 0, -1, lastTab ? -1 : 0, -2 );
						painter->setClipRect( rect.adjusted( 0, 1, 1, 1 ) );
					} else {
						rect.adjust( 0, 1, lastTab ? -1 : 0, 0 );
						painter->setClipRect( rect.adjusted( 0, 0, 1, 0 ) );
					}
				}
				QLinearGradient gradient;
				if ( bottom )
					gradient = QLinearGradient( rect.bottomLeft(), rect.topLeft() );
				else
					gradient = QLinearGradient( rect.topLeft(), rect.bottomLeft() );
				if ( option->state & State_Selected ) {
					gradient.setColorAt( 0.0, m_colorItemBackgroundBegin );
					gradient.setColorAt( 1.0, option->palette.window().color() );
					painter->setPen( m_colorBorder );
				} else if ( option->state & State_MouseOver && option->state & State_Enabled ) {
					gradient.setColorAt( 0.0, m_colorItemBackgroundBegin );
					gradient.setColorAt( 1.0, m_colorItemBackgroundEnd );
					painter->setPen( m_colorBorderLight );
				} else {
					gradient.setColorAt( 0.0, m_colorBarMiddle );
					gradient.setColorAt( 1.0, m_colorBarEnd );
					painter->setPen( m_colorBorderLight );
				}
				painter->setBrush( gradient );
				painter->drawRect( rect );
				painter->restore();
				return;
			}
			break;

		case CE_ToolBoxTabShape: {
			QRect rect = option->rect.adjusted( 0, 0, -1, -1 );
			QLinearGradient gradient( rect.topLeft(), rect.bottomLeft() );
			if ( option->state & QStyle::State_Sunken ) {
				gradient.setColorAt( 0.0, m_colorItemSunkenBegin );
				gradient.setColorAt( 1.0, m_colorItemSunkenEnd );
				painter->setPen( m_colorBorder );
			} else if ( option->state & State_MouseOver && option->state & State_Enabled ) {
				gradient.setColorAt( 0.0, m_colorItemBackgroundBegin );
				gradient.setColorAt( 1.0, m_colorItemBackgroundEnd );
				painter->setPen( m_colorBorder );
			} else {
				gradient.setColorAt( 0.0, m_colorBarMiddle );
				gradient.setColorAt( 1.0, m_colorBarEnd );
				painter->setPen( m_colorBorderLight );
			}
			painter->setBrush( gradient );
			painter->drawRect( rect );
			return;
		}

		case CE_Splitter:
			if ( qobject_cast<const QMainWindow*>( widget->window() ) )
				return;
			break;

		default:
			break;
	}

	if ( useVista() )
		QWindowsVistaStyle::drawControl( element, option, painter, widget );
	else
		QWindowsXPStyle::drawControl( element, option, painter, widget );
}

void WindowsModernStyle::drawComplexControl( ComplexControl control, const QStyleOptionComplex* option,
	QPainter* painter, const QWidget* widget ) const
{
	switch ( control ) {
		case CC_ToolButton: {
			QToolBar* toolBar;
			if ( widget && ( toolBar = qobject_cast<QToolBar*>( widget->parentWidget() ) ) ) {
				if ( const QStyleOptionToolButton* optionToolButton = qstyleoption_cast<const QStyleOptionToolButton*>( option ) ) {
					QRect buttonRect = subControlRect( control, option, SC_ToolButton, widget );
					QStyle::State buttonState = option->state & ~State_Sunken;
					if ( option->state & State_Sunken ) {
						if ( optionToolButton->activeSubControls & SC_ToolButton )
							buttonState |= State_Sunken;
						else if ( optionToolButton->activeSubControls & SC_ToolButtonMenu )
							buttonState |= State_MouseOver;
					}
					bool selected = buttonState & State_MouseOver && option->state & State_Enabled;
					bool checked = buttonState & State_On;
					bool sunken = buttonState & State_Sunken;
					if ( selected || checked || sunken ) {
						QRect rect = buttonRect.adjusted( 0, 0, -1, -1 );
						painter->setPen( m_colorItemBorder );
						QLinearGradient gradient;
						if ( toolBar->orientation() == Qt::Vertical )
							gradient = QLinearGradient( rect.topLeft(), rect.topRight() );
						else
							gradient = QLinearGradient( rect.topLeft(), rect.bottomLeft() );
						if ( sunken || (selected && checked) ) {
							gradient.setColorAt( 0.0, m_colorItemSunkenBegin );
							gradient.setColorAt( 0.5, m_colorItemSunkenMiddle );
							gradient.setColorAt( 1.0, m_colorItemSunkenEnd );
						} else if ( checked ) {
							gradient.setColorAt( 0.0, m_colorItemCheckedBegin );
							gradient.setColorAt( 0.5, m_colorItemCheckedMiddle );
							gradient.setColorAt( 1.0, m_colorItemCheckedEnd );
						} else {
							gradient.setColorAt( 0.0, m_colorItemBackgroundBegin );
							gradient.setColorAt( 0.5, m_colorItemBackgroundMiddle );
							gradient.setColorAt( 1.0, m_colorItemBackgroundEnd );
						}
						painter->setBrush( gradient );
						painter->drawRect( rect );
					}
					QStyleOptionToolButton optionLabel = *optionToolButton;
					int fw = pixelMetric( PM_DefaultFrameWidth, option, widget );
					optionLabel.rect = buttonRect.adjusted( fw, fw, -fw, -fw );
					drawControl( CE_ToolButtonLabel, &optionLabel, painter, widget );
					if ( optionToolButton->subControls & SC_ToolButtonMenu ) {
						QStyleOption optionMenu = *optionToolButton;
						optionMenu.rect = subControlRect( control, option, SC_ToolButtonMenu, widget );
						optionMenu.state = optionToolButton->state & ~State_Sunken;
						if ( optionToolButton->state & State_Sunken ) {
							if ( optionToolButton->activeSubControls & SC_ToolButton )
								optionMenu.state |= State_MouseOver | State_Sunken;
							else if ( optionToolButton->activeSubControls & SC_ToolButtonMenu )
								optionMenu.state |= State_Sunken;
						}
						drawPrimitive( PE_IndicatorButtonDropDown, &optionMenu, painter, widget );
					} else if ( optionToolButton->features & QStyleOptionToolButton::HasMenu ) {
						int size = pixelMetric( PM_MenuButtonIndicator, option, widget );
						QRect rect = optionToolButton->rect;
						QStyleOptionToolButton optionArrow = *optionToolButton;
						optionArrow.rect = QRect( rect.right() + 4 - size, rect.height() - size + 4, size - 5, size - 5 );
						drawPrimitive( PE_IndicatorArrowDown, &optionArrow, painter, widget );
					}
					return;
				}
			}
			break;
		}

		default:
			break;
	}

	if ( useVista() )
		QWindowsVistaStyle::drawComplexControl( control, option, painter, widget );
	else
		QWindowsXPStyle::drawComplexControl( control, option, painter, widget );
}

#if !defined( WMSTYLE_NO_PLUGIN )

class WindowsModernStylePlugin : public QStylePlugin
{
public: // overrides
	QStringList keys() const;
	QStyle* create( const QString &key );
};

QStringList WindowsModernStylePlugin::keys() const
{
	return QStringList() << "WindowsModernStyle";
}

QStyle* WindowsModernStylePlugin::create( const QString &key )
{
	if ( key.toLower() == QLatin1String( "windowsmodernstyle" ) )
		return new WindowsModernStyle();
	return nullptr;
}

#if !defined( WMSTYLE_EXPORT_PLUGIN )

QObject* qt_plugin_instance_windowsmodernstyle()
{
	static QPointer<QObject> instance;
	if ( !instance )
		instance = new WindowsModernStylePlugin();
	return instance;
}

//Q_IMPORT_PLUGIN( windowsmodernstyle )

#else

Q_EXPORT_PLUGIN2( windowsmodernstyle, WindowsModernStylePlugin )

#endif // !defined( WMSTYLE_EXPORT_PLUGIN )

#endif // !defined( WMSTYLE_NO_PLUGIN )

#endif // !defined( NO_STYLE_WINDOWSMODERN )
