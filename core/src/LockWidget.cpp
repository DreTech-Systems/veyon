/*
 *  LockWidget.cpp - widget for locking a client
 *
 *  Copyright (c) 2006-2016 Tobias Junghans <tobydox@users.sf.net>
 *
 *  This file is part of Veyon - http://veyon.io
 *
 *  This is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 *  USA.
 */

#include "LockWidget.h"
#include "PlatformCoreFunctions.h"
#include "PlatformInputDeviceFunctions.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>


LockWidget::LockWidget( Mode mode, QWidget* parent ) :
	QWidget( parent, Qt::X11BypassWindowManagerHint ),
	m_background(),
	m_mode( mode )
{
	VeyonCore::platform().inputDeviceFunctions().disableInputDevices();

	switch( mode )
	{
	case Black:
		m_background = QPixmap( QStringLiteral( ":/resources/locked_bg.png" ) );
		break;
	case DesktopVisible:
		QPixmap::grabWindow( qApp->desktop()->winId() );
		break;
	default:
		break;
	}

	setWindowTitle( tr( "screen lock" ) );
	showFullScreen();
	move( 0, 0 );
	setFixedSize( qApp->desktop()->size() );
	VeyonCore::platform().coreFunctions().raiseWindow( this );
	setFocusPolicy( Qt::StrongFocus );
	setFocus();
	grabMouse();
	grabKeyboard();
	setCursor( Qt::BlankCursor );
	QGuiApplication::setOverrideCursor( Qt::BlankCursor );

	QCursor::setPos( mapToGlobal( QPoint( 0, 0 ) ) );

	VeyonCore::platform().coreFunctions().disableScreenSaver();
}



LockWidget::~LockWidget()
{
	VeyonCore::platform().inputDeviceFunctions().enableInputDevices();
	VeyonCore::platform().coreFunctions().restoreScreenSaverSettings();

	QGuiApplication::restoreOverrideCursor();
}



void LockWidget::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	switch( m_mode )
	{
		case DesktopVisible:
			p.drawPixmap( 0, 0, m_background );
			break;

		case Black:
			p.fillRect( rect(), QColor( 64, 64, 64 ) );
			p.drawPixmap( ( width() - m_background.width() ) / 2,
				( height() - m_background.height() ) / 2,
								m_background );
			break;

		default:
			break;
	}
}
