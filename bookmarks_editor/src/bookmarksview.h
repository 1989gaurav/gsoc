/*
    Copyright (c) 2009 Eduardo Robles Elvira <edulix@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; see the file COPYING.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef BOOKMARKSVIEW_H
#define BOOKMARKSVIEW_H

#include <QtGui/QWidget>
#include <QtCore/QSharedDataPointer>
#include <Akonadi/Collection>
#include <konqbookmark/konqbookmark.h>

#include "ui_bookmarksview_base.h"

class QPainter;
class QString;
class KUrl;
class Job;

/**
 * This is the main view class for bookmarks.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author Eduardo Robles Elvira <edulix@gmail.com>
 * @version 0.1
 */

class BookmarksView : public QWidget, public Ui::bookmarksview_base
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    BookmarksView(QWidget *parent);

    /**
     * Destructor
     */
    virtual ~BookmarksView();

    void createModels();
    void addBookmark();

Q_SIGNALS:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString &text);

public Q_SLOTS:
    void slotAddFolder(const QString &folderName);
    void slotDelete();

protected Q_SLOTS:
    void slotBookmarkAdded(const QModelIndex &index);

private:
    /**
     * Returns the collection that should be used as a parent collection for new
     * items being added (subcollection, bookmarks) taing into consideration
     * current selection.
     */
    Akonadi::Collection getParentCollection(QModelIndex current);

    /**
     * Helper function for the one above, you shouldn't use it.
     */
    Akonadi::Collection getParentCollection(QModelIndex current, Akonadi::Collection
        defaultCollection);

private:
    Ui::bookmarksview_base ui_bookmarksview_base;

    class Private;
    QSharedDataPointer<Private> d;

    Q_PRIVATE_SLOT(d, void expandCollectionsView(const QModelIndex& index))
    Q_PRIVATE_SLOT(d, void expandBookmarksView(const QModelIndex &index))
    Q_PRIVATE_SLOT(d, void selectBookmarkFolder(const QModelIndex &index = QModelIndex(),
        int start = -1))
};

#endif // BOOKMARSVIEW_H
