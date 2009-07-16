/*
   Copyright (C) 2009 by Rahman Duran <rahman.duran@gmail.com>
   Copyright (c) 2009 Eduardo Robles Elvira <edulix@gmail.com>
                                                                          
   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or      
   ( at your option ) version 3 or, at the discretion of KDE e.V.           
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of 
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.                 

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to     
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,     
   Boston, MA 02110-1301, USA.  
*/

#ifndef KTREEMODELNAVIGATORMENU_P_H
#define KTREEMODELNAVIGATORMENU_P_H

#include <kmenu.h>

class KTreeModelNavigator;

/**
 * @brief Base class for drop-down url menus of the tree model navigator.
 *
 * This menu acts like KMenu with drag&drop support. 
 */
class KTreeModelNavigatorMenu : public KMenu
{
    Q_OBJECT

public:
    explicit KTreeModelNavigatorMenu(KTreeModelNavigator *treeModelNavigator, QWidget* parent);
    virtual ~KTreeModelNavigatorMenu();
    
Q_SIGNALS:

    /**
     * Is emitted when drop event occurs.
     */
    void mimeDataDropped(QAction* action, QDropEvent* event);

protected:
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dragMoveEvent(QDragMoveEvent* event);
    virtual void dropEvent(QDropEvent* event);    
   
private:
    KTreeModelNavigator* m_treeModelNavigator;
};

#endif // KTREEMODELNAVIGATORMENU_P_H
