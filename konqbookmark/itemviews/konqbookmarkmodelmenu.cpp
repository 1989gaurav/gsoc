/*
    Copyright (c) 2009 Eduardo Robles Elvira <edulix@gmail.com>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "konqbookmarkmodelmenu.h"
#include "modelmenusearchline.h"
#include "itemmodels/konqbookmarkmodel.h"

#include <akonadi/entitytreemodel.h>
#include <kbookmarkmenu.h>
#include <kdebug.h>
#include <kaction.h>
#include <klineedit.h>
#include <kauthorized.h>
#include <QWidgetAction>
#include <QCoreApplication>
#include <QPushButton>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QStyle>
#include <QAction>
#include <QHash>
/**
 * Private class that helps to provide binary compatibility between releases.
 * @internal
 */
//@cond PRIVATE
class KonqBookmarkModelMenu::Private
{
public:
    Private(KonqBookmarkModelMenu *parent);
    virtual ~Private() {}
    
    void setChildAsRoot(const QModelIndex& index);
    
    void updateOpenInTabs(const int& count);
    void updateEditBookmarks();
    void updateNewFolder();
    void updateAddBookmark();
    
    void slotOpenFolderInTabs();
    void slotNewFolder();
    void slotAddBookmark();
    void slotEditBookmarks();
    
public:
    KonqBookmarkMenuHelper *m_helper;
    
    // This is used to "export" our actions into an actionlist
    // we got in the constructor. So that the program can show our 
    // actions in their shortcut dialog
    KActionCollection * m_actionCollection;
    
    KonqBookmarkModelMenu *q;
    
    QHash<QString, QAction *> m_helperActions;
};

KonqBookmarkModelMenu::Private::Private(KonqBookmarkModelMenu *parent)
    : q(parent)
{

}

void KonqBookmarkModelMenu::Private::setChildAsRoot(const QModelIndex& index)
{
    if (index.isValid()) {
        kDebug() << index.data();
        q->setRootIndex(index);
        kDebug() << q->rootIndex().data();
        disconnect(q->model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)),
            q, SLOT(setChildAsRoot(const QModelIndex&)));
    }
}

void KonqBookmarkModelMenu::Private::updateOpenInTabs(const int& count)
{
    // If not supported then we have nothing to do
    if (!m_helper || !m_helper->enableOption("openintabs") || !KAuthorized::authorizeKAction("bookmarks")) {
        return;
    }
    
    // If action was added previously, check if it should still be there.
    // Remove action in case no bookmark is present
    if (m_helperActions.contains("openintabs")) {
        if(count == 0) {
            delete m_helperActions["openintabs"]; 
            m_helperActions.remove("openintabs");
        }
       return;
    }
    
    // If action was not added but no bookmark is shown, then action should not
    // be added so we finish here
    if (count == 0) {
        return;
    }

    QString title = i18n("Open Folder in Tabs");

    KAction* openFolderInTabs = new KAction(title, q);
    openFolderInTabs->setIcon(KIcon("tab-new"));
    openFolderInTabs->setToolTip(i18n("Open all bookmarks in this folder as a new tab."));
    connect( openFolderInTabs, SIGNAL(triggered(bool)), q, SLOT(slotOpenFolderInTabs()));
    
//     MenuItemLocation location = (q->flags() & IsRootFlag) ? PreModelItems : PostModelItems;
    q->addAction(openFolderInTabs, PreModelItems);
    m_helperActions["openintabs"] = openFolderInTabs;
}


void KonqBookmarkModelMenu::Private::updateEditBookmarks()
{
    // If not supported or already added then we have nothing to do
    if (!m_helper || !m_helper->enableOption("editbookmarks")
        || !KAuthorized::authorizeKAction("bookmarks") || m_helperActions.contains("editbookmarks")) {
        return;
    }

    QString title = i18n("Edit Bookmarks...");

    KAction* editBookmarks = m_actionCollection->addAction(
        KStandardAction::EditBookmarks,
        "edit_bookmarks",
        q,
        SLOT( slotEditBookmarks() )
    );
    editBookmarks->setToolTip(i18n("Edit your bookmark collection in a separate window"));
    q->addAction(editBookmarks, PreModelItems);
    m_helperActions["editbookmarks"] = editBookmarks;
}

void KonqBookmarkModelMenu::Private::updateNewFolder()
{
    // If not supported or already added then we have nothing to do
    if (!m_helper || !m_helper->enableOption("newfolder")
        || !KAuthorized::authorizeKAction("bookmarks") || m_helperActions.contains("newfolder")) {
        return;
    }

    QString title = i18n("New Bookmark Folder...");

    KAction* newBookmarkFolder = new KAction(title, q);
    newBookmarkFolder->setIcon(KIcon("folder-new"));
    newBookmarkFolder->setToolTip(i18n("Create a new bookmark folder in this menu"));
    connect( newBookmarkFolder, SIGNAL(triggered(bool)), q, SLOT(slotNewFolder()));
    
//     MenuItemLocation location = (q->flags() & IsRootFlag) ? PreModelItems : PostModelItems;
    q->addAction(newBookmarkFolder, PreModelItems);
    m_helperActions["newfolder"] = newBookmarkFolder;
}

void KonqBookmarkModelMenu::Private::updateAddBookmark()
{
    // If not supported or already added then we have nothing to do
    if (!m_helper || !m_helper->enableOption("addbookmark")
        || !KAuthorized::authorizeKAction("bookmarks") || m_helperActions.contains("addbookmark")) {
        return;
    }

    KAction* addBookmark = m_actionCollection->addAction(
        KStandardAction::AddBookmark,
        "add_bookmark",
        q,
        SLOT(slotAddBookmark()));
        
//     MenuItemLocation location = (q->flags() & IsRootFlag) ? PreModelItems : PostModelItems;
    q->addAction(addBookmark, PreModelItems);
    m_helperActions["addbookmark"] = addBookmark;
}

void KonqBookmarkModelMenu::Private::slotOpenFolderInTabs()
{
    Q_ASSERT(m_helper);
    
    QList<KUrl> urls;
    int size = q->currentModel()->rowCount(q->currentRootIndex());
    
    for (int i = 0; i < size; ++i) {
        QModelIndex index = q->currentModel()->index(i, Akonadi::KonqBookmarkModel::Url, q->currentRootIndex());
        if (!q->isFolder(index)) {
            kDebug() << index.data().toString();
            urls.append(KUrl(index.data().toString()));
        }
    }
    
    m_helper->openInNewTabs(urls);
}

void KonqBookmarkModelMenu::Private::slotNewFolder()
{
    if (!m_helper) {
        return;
    }
    
    Akonadi::Collection collection = 
        qVariantValue<Akonadi::Collection>(q->currentRootIndex().data(Akonadi::EntityTreeModel::CollectionRole));
    if(!collection.isValid())
        return;
    
    m_helper->createNewFolder(collection);
}

void KonqBookmarkModelMenu::Private::slotAddBookmark()
{
    if (!m_helper) {
        return;
    }
    
    Akonadi::Collection collection = 
        qVariantValue<Akonadi::Collection>(q->currentRootIndex().data(Akonadi::EntityTreeModel::CollectionRole));
    if (!collection.isValid()) {
        return;
    }
    
    m_helper->addBookmark(collection);
}

void KonqBookmarkModelMenu::Private::slotEditBookmarks()
{
    if (!m_helper) {
        return;
    }
    
    m_helper->editBookmarks();
}
//@endcond

KonqBookmarkModelMenu::KonqBookmarkModelMenu(QAbstractItemModel* model, KonqBookmarkMenuHelper *konqBookmarkMenuHelper, KActionCollection* actionCollection, QWidget *parent)
    : ModelMenu(parent),  d( new Private(this) )
{
    setModel(model);
    setFlags(flags() | OneColumnFlag);
    setShowSearchLine(false); // Not complete
    d->m_helper = konqBookmarkMenuHelper;
    d->m_actionCollection = actionCollection;
    
    // We want to set "Konqueror Bookmarks" as root
    connect(model, SIGNAL(rowsInserted(const QModelIndex&, int, int)),
        this, SLOT(setChildAsRoot(const QModelIndex&)));
        
    init();
}

KonqBookmarkModelMenu::KonqBookmarkModelMenu(KonqBookmarkMenuHelper *konqBookmarkMenuHelper, KActionCollection* actionCollection, KonqBookmarkModelMenu *parent)
    : ModelMenu(parent),  d( new Private(this) )
{
    d->m_helper = konqBookmarkMenuHelper;
    d->m_actionCollection = actionCollection;
    
    connect(this, SIGNAL(activated(const QModelIndex &)), this, SLOT(openBookmark(const QModelIndex &)));
    connect(this, SIGNAL(rowCountChanged(const int &)), this, SLOT(updateActions(const int &)));
        
    init();
}

void KonqBookmarkModelMenu::init()
{
    int childCount = model()->rowCount(rootIndex());
    updateActions(childCount);
}

KonqBookmarkModelMenu::~KonqBookmarkModelMenu()
{
    delete d;
}

ModelMenu *KonqBookmarkModelMenu::createBaseMenu()
{
    return new KonqBookmarkModelMenu(d->m_helper, d->m_actionCollection, this);
}

QAction *KonqBookmarkModelMenu::makeAction(const QIcon &icon, const QString &text, QObject *parent)
{
    return ModelMenu::makeAction(icon, text, parent);
}

void KonqBookmarkModelMenu::openBookmark(const QModelIndex& index)
{
    if (!index.isValid()) {
        return;
    }
    
    KUrl url(index.data(Akonadi::KonqBookmarkModel::Url).toString());
    d->m_helper->openBookmark(url);
}

KMenu *KonqBookmarkModelMenu::contextMenu(const QModelIndex& index)
{
    return ModelMenu::contextMenu(index);
}

void KonqBookmarkModelMenu::updateActions(const int& count)
{
    kDebug() << count;
    if (flags() & IsRootFlag) {
        d->updateAddBookmark();
//         addAddBookmarksList();
        d->updateNewFolder();
        d->updateEditBookmarks();
    } else {
        d->updateOpenInTabs(count);
        d->updateAddBookmark();
    //         addAddBookmarksList();
        d->updateNewFolder();
    }
}

bool KonqBookmarkModelMenu::isFolder(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return false;
    }
    
    Akonadi::Collection collection = 
        qVariantValue<Akonadi::Collection>(index.data(Akonadi::EntityTreeModel::CollectionRole));
    return collection.isValid();
}

#include "konqbookmarkmodelmenu.moc"
