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

#include "konquerorbookmarksresource.h"

#include "settings.h"
#include "settingsadaptor.h"

#include <konqbookmark/konqbookmark.h>
#include <akonadi/changerecorder.h>
#include <akonadi/collectiondeletejob.h>
#include <akonadi/itemdeletejob.h>
#include <akonadi/itemfetchscope.h>
#include <KUrl>

#include <nepomuk/ontologies/bookmark.h>
#include <nepomuk/ontologies/bookmarkfolder.h>
#include <nepomuk/ontologies/dataobject.h>
#include <nepomuk/result.h>
#include <Nepomuk/Resource>
#include <Nepomuk/Types/Class>
#include <Nepomuk/ResourceManager>
#include <Nepomuk/Variant>
#include <Soprano/Vocabulary/Xesam>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Vocabulary/XMLSchema>
#include <Soprano/Model>
#include <Soprano/QueryResultIterator>

#include <QtDBus/QDBusConnection>
#include <QObject>
#include <QtGlobal>
#include <QUrl>
#include <QLatin1String>

using namespace Akonadi;

class KonquerorBookmarksResource::Private : public QSharedData
{
public:
    Private(KonquerorBookmarksResource *parent);
    
public:
    Collection mBookmarksRootCollection;
    Collection mMenuCollection;
    Collection mToolbarCollection;
    Collection mSavedSearchesCollection;
    Collection mMostRecentCollection;
    Collection mRecentTagsCollection;
    Collection mMostVisitedCollection;
    Collection mUnclasifiedCollection;
    Collection::List mList;
    
    /**
     * We use this to query the nepomuk database everytime we need to fetch
     * nfo:Bookmark's belonging to a nfo:BookmarksFolder.
     */
//     Nepomuk::Search::QueryServiceClient *mQueryServiceClient;

private:
    KonquerorBookmarksResource *mParent;
};

KonquerorBookmarksResource::Private::Private(KonquerorBookmarksResource *parent) :
    mParent( parent )
{
//     mQueryServiceClient = new Nepomuk::Search::QueryServiceClient( parent );
//     connect( mQueryServiceClient, SIGNAL(newEntries( const QList<Nepomuk::Search::Result>& )),
//              parent, SLOT(slotNewEntries( const QList<Nepomuk::Search::Result>& )) );
}

KonquerorBookmarksResource::KonquerorBookmarksResource( const QString &id )
  : ResourceBase( id ),  d( new Private ( this ) )
{
    Nepomuk::ResourceManager::instance()->init();
    new SettingsAdaptor( Settings::self() );
    QDBusConnection::sessionBus().registerObject( QLatin1String( "/Settings" ),
        Settings::self(), QDBusConnection::ExportAdaptors ); 
    changeRecorder()->itemFetchScope().fetchFullPayload();

    QStringList mimeTypes;
    mimeTypes << "application/x-vnd.kde.konqbookmark" << Collection::mimeType();
    
    d->mBookmarksRootCollection.setParent( Collection::root() );
    d->mBookmarksRootCollection.setRemoteId( "konqbookmark:/" );
    d->mBookmarksRootCollection.setName( "Konqueror Bookmarks" );
    d->mBookmarksRootCollection.setContentMimeTypes( mimeTypes );
    
//     d->mMenuCollection.setParent( d->mBookmarksRootCollection );
//     d->mMenuCollection.setRemoteId( "/konquerorbookmarks/menu/" );
//     d->mMenuCollection.setName( "Bookmarks Menu" );
//     d->mMenuCollection.setContentMimeTypes( mimeTypes );
//     
//     d->mToolbarCollection.setParent( d->mBookmarksRootCollection );
//     d->mToolbarCollection.setRemoteId( "/konquerorbookmarks/toolbar/" );
//     d->mToolbarCollection.setName( "Bookmarks Toolbar" );
//     d->mToolbarCollection.setContentMimeTypes( mimeTypes );
//     
//     d->mSavedSearchesCollection.setParent( d->mMenuCollection );
//     d->mSavedSearchesCollection.setRemoteId( "/konquerorbookmarks/menu/savedsearches/" );
//     d->mSavedSearchesCollection.setName( "Saved Searches" );
//     d->mSavedSearchesCollection.setContentMimeTypes( mimeTypes );
//     
//     d->mMostRecentCollection.setParent( d->mMenuCollection );
//     d->mMostRecentCollection.setRemoteId( "/konquerorbookmarks/menu/mostrecent/" );
//     d->mMostRecentCollection.setName( "Most Recent Bookmarks" );
//     d->mMostRecentCollection.setContentMimeTypes( mimeTypes );
//     
//     d->mRecentTagsCollection.setParent( d->mMenuCollection );
//     d->mRecentTagsCollection.setRemoteId( "/konquerorbookmarks/menu/mostrecenttags/" );
//     d->mRecentTagsCollection.setName( "Most Recent Tags" );
//     d->mRecentTagsCollection.setContentMimeTypes( mimeTypes );
// 
//     d->mMostVisitedCollection.setParent( d->mMenuCollection );
//     d->mMostVisitedCollection.setRemoteId( "/konquerorbookmarks/menu/mostvisited/" );
//     d->mMostVisitedCollection.setName( "Most Visited Bookmarks" );
//     d->mMostVisitedCollection.setContentMimeTypes( mimeTypes );
//     
//     d->mUnclasifiedCollection.setParent( d->mMenuCollection );
//     d->mUnclasifiedCollection.setRemoteId( "/konquerorbookmarks/menu/unclasified/" );
//     d->mUnclasifiedCollection.setName( "Unclasified Bookmarks" );
//     d->mUnclasifiedCollection.setContentMimeTypes( mimeTypes );
    
    d->mList << d->mBookmarksRootCollection;
//     d->mList << d->mMenuCollection;
//     d->mList << d->mToolbarCollection;
//     d->mList << d->mSavedSearchesCollection;
//     d->mList << d->mMostRecentCollection;
//     d->mList << d->mRecentTagsCollection;
//     d->mList << d->mMostVisitedCollection;
//     d->mList << d->mUnclasifiedCollection;
}

KonquerorBookmarksResource::~KonquerorBookmarksResource()
{
}


Collection::List listRecursive( const Nepomuk::BookmarkFolder& parent, const Collection& parentCol )
{
    Collection::List list;
    QStringList mimeTypes;
    
    
    if(!parent.hasProperty(Nepomuk::BookmarkFolder::containsBookmarkFolderUri()))
        return list;
    
    QList<Nepomuk::BookmarkFolder> bookmarkFolders = parent.containsBookmarkFolders();
    
    mimeTypes << "application/x-vnd.kde.konqbookmark" << Collection::mimeType();

    foreach( const Nepomuk::BookmarkFolder& bookmarkFolder, bookmarkFolders )
    {
        Collection col;
        if(!bookmarkFolder.titles().empty())
            col.setName( bookmarkFolder.titles().first() );
        else  // shouldn't happen
            col.setName( "" );
        
        col.setRemoteId( bookmarkFolder.resourceUri().toString() );
        col.setParent( parentCol );
        col.setContentMimeTypes( mimeTypes );
        list << col;
        list << listRecursive( bookmarkFolder, col );
    }

    return list;
}

void KonquerorBookmarksResource::retrieveCollections()
{
    Nepomuk::BookmarkFolder bookmarkFolder( d->mBookmarksRootCollection.remoteId() );

    Collection::List list;
    list << d->mBookmarksRootCollection;
    list += listRecursive( bookmarkFolder, d->mBookmarksRootCollection );
    collectionsRetrieved( list );
}

void KonquerorBookmarksResource::retrieveItems( const Akonadi::Collection &collection )
{
    // this method is called when Akonadi wants to know about all the
    // items in the given collection. You can but don't have to provide all the
    // data for each item, remote ID and MIME type are enough at this stage.
    // Depending on how your resource accesses the data, there are several
    // different ways to tell Akonadi when you are done.
  
    Nepomuk::BookmarkFolder folder( collection.remoteId() );
    QList<Nepomuk::Bookmark> bookmarks = folder.containsBookmarks();
    Item::List items;
    foreach( const Nepomuk::Bookmark& bookmark, bookmarks )
    {
        KonqBookmark konqBookmark;
        Item item;
        item.setRemoteId( bookmark.resourceUri().toString() );
        item.setMimeType( "application/x-vnd.kde.konqbookmark" );
        if(!bookmark.bookmarkses().empty())
            konqBookmark.setUrl( bookmark.bookmarkses().first().resourceUri() );
        
        if(!bookmark.titles().empty())
            konqBookmark.setTitle( bookmark.titles().first() );
        
        item.setPayload<KonqBookmark>( konqBookmark );
        items << item;
    }
    itemsRetrieved( items );
}

bool KonquerorBookmarksResource::retrieveItem( const Akonadi::Item &item, const QSet<QByteArray> &parts )
{
    Q_UNUSED( item );
    Q_UNUSED( parts );

    // this method is called when Akonadi wants more data for a given item.
    // You can only provide the parts that have been requested but you are allowed
    // to provide all in one go
    // As the current implementation goes, we should have already provided the info for that item in
    //  retrieveItems() so we do nothing here.

    return true;
}

void KonquerorBookmarksResource::aboutToQuit()
{
    // TODO: any cleanup you need to do while there is still an active
    // event loop. The resource will terminate after this method returns
}

void KonquerorBookmarksResource::configure( WId windowId )
{
    Q_UNUSED( windowId );
    
    // TODO Here we will show the "Organize bookmarks.." dialog.
}

void KonquerorBookmarksResource::itemAdded( const Akonadi::Item &item, const Akonadi::Collection &collection )
{
    kDebug();
    if(!item.hasPayload<KonqBookmark>())
    {
        kDebug() << "!item.hasPayload<KonqBookmark>()";
        return;
    }
    
    KonqBookmark konqBookmark = item.payload<KonqBookmark>();
    Nepomuk::Bookmark bookmark( konqBookmark.uniqueUri() );
    bookmark.setTitles( QStringList(konqBookmark.title()) );
    Nepomuk::DataObject url(konqBookmark.url().toString());
    bookmark.setBookmarkses( QList<Nepomuk::DataObject>() << url );
    
    Nepomuk::BookmarkFolder folder( collection.remoteId() );
    folder.addContainsBookmark(bookmark);
    
    // Set the remote Id (as suggested by vkrause)
    Akonadi::Item itemCopy = item;
    itemCopy.setRemoteId( bookmark.resourceUri().toString() );
    changeCommitted(itemCopy);
}

void KonquerorBookmarksResource::itemChanged( const Akonadi::Item &item, const QSet<QByteArray> &parts )
{
    Q_UNUSED( parts );
    if(!item.hasPayload<KonqBookmark>())
    {
        kDebug() << "!item.hasPayload<KonqBookmark>()";
        return;
    }
    
    KonqBookmark konqBookmark = item.payload<KonqBookmark>();
    Nepomuk::Bookmark bookmark( konqBookmark.uniqueUri() );
    bookmark.setTitles( QStringList(konqBookmark.title()) );
    Nepomuk::DataObject url(konqBookmark.url().toString());
    bookmark.setBookmarkses( QList<Nepomuk::DataObject>() << url );
}

void KonquerorBookmarksResource::itemRemoved( const Akonadi::Item &item )
{
    if(!item.hasPayload<KonqBookmark>())
    {
        kDebug() << "!item.hasPayload<KonqBookmark>()";
        return;
    }
    
    KonqBookmark konqBookmark = item.payload<KonqBookmark>();
    Nepomuk::Bookmark bookmark( konqBookmark.uniqueUri() );
    bookmark.remove();
}

void KonquerorBookmarksResource::collectionAdded( const Akonadi::Collection &collection, const Akonadi::Collection &parent )
{
    QString uniqueUri = KonqBookmark::generateUniqueUri();
    kDebug() << "adding '" << collection.name() << "' with remoteId = " << uniqueUri;
    
    // Create bookmark folder
    Nepomuk::BookmarkFolder bookmarkFolder( uniqueUri );
    bookmarkFolder.setTitles( QStringList(collection.name()) );
    
    // Set parent folder
    Nepomuk::BookmarkFolder parentBookmarkFolder( parent.remoteId() );
    if(!parentBookmarkFolder.containsBookmarkFolders().contains(bookmarkFolder))
        parentBookmarkFolder.addContainsBookmarkFolder( bookmarkFolder );
    
    // Set the remote Id (as suggested by vkrause)
    Akonadi::Collection collectionCopy = collection;
    collectionCopy.setRemoteId( uniqueUri );
    changeCommitted(collectionCopy);
}

void KonquerorBookmarksResource::collectionChanged( const Akonadi::Collection &collection )
{
    // At the moment, the only thing a collection can change is it's name
    Nepomuk::BookmarkFolder bookmarkFolder( collection.remoteId() );
    
    // Set parent folder;
    if(bookmarkFolder.titles() != QStringList(collection.name()))
        bookmarkFolder.setTitles( QStringList(collection.name()) );
}

void KonquerorBookmarksResource::collectionRemoved( const Akonadi::Collection &collection )
{
    // Root collection cannot be removed
    if(collection.remoteId() == "konqbookmark:/")
        return;
    
    // Remove all the bookmarks inside this folder
    Nepomuk::BookmarkFolder bookmarkFolder( collection.remoteId() );
    QList<Nepomuk::Bookmark> bookmarks = bookmarkFolder.containsBookmarks();
    foreach( const Nepomuk::Bookmark& bookmark, bookmarks )
    {
        Item item;
        item.setRemoteId( bookmark.resourceUri().toString() );
        Akonadi::ItemDeleteJob *job = new Akonadi::ItemDeleteJob( item );
        if ( !job->exec() )
            kDebug() << "Error deleting an bookmark item";
    }
    
    // Remove recursively all the subfolders
    QList<Nepomuk::BookmarkFolder> bookmarkFolders = bookmarkFolder.containsBookmarkFolders();
    foreach( const Nepomuk::BookmarkFolder& subFolder, bookmarkFolders )
    {
        Collection subCollection;
        subCollection.setRemoteId( subFolder.resourceUri().toString() );
        
        Akonadi::CollectionDeleteJob *job = new Akonadi::CollectionDeleteJob( subCollection );
        if ( !job->exec() )
            kDebug() << "Error deleting a bookmark folder";
    }
    bookmarkFolder.remove();
    changeCommitted(collection);
}

AKONADI_RESOURCE_MAIN( KonquerorBookmarksResource )

#include "konquerorbookmarksresource.moc"
