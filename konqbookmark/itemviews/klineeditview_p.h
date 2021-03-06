/* This file is part of the KDE libraries

   Copyright (C) 2007 Aaron Seigo <aseigo@kde.org>
   Copyright (c) 2009 Benjamin C. Meyer  <ben@meyerhome.net>
   Copyright (c) 2009 Eduardo Robles Elvira <edulix@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License (LGPL) as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KLINEEDITVIEW_P_H
#define KLINEEDITVIEW_P_H

#include <QWidget>
#include <QEvent>

class SideWidget : public QWidget
{
    Q_OBJECT
public:
    SideWidget(QWidget *parent = 0);

protected:
    bool event(QEvent *event);
    
Q_SIGNALS:
    void sizeHintChanged();
};

#endif // KLINEEDITVIEW_P_H
