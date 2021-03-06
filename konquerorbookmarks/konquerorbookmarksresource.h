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

#ifndef KONQUERORBOOKMARKSRESOURCE_H
#define KONQUERORBOOKMARKSRESOURCE_H

#include <QtCore/QSharedDataPointer>
#include <Akonadi/ResourceBase>
#include <nepomuk/queryserviceclient.h>

class KonquerorBookmarksResource : public Akonadi::ResourceBase,
    public Akonadi::AgentBase::Observer
{
Q_OBJECT

public:
    KonquerorBookmarksResource(const QString &id);
    ~KonquerorBookmarksResource();

public Q_SLOTS:
    virtual void configure(WId windowId);

protected Q_SLOTS:
    void retrieveCollections();
    void retrieveItems(const Akonadi::Collection &col);
    bool retrieveItem(const Akonadi::Item &item, const QSet<QByteArray> &parts);

protected:
    virtual void aboutToQuit();

    virtual void itemAdded(const Akonadi::Item &item, const Akonadi::Collection &collection);
    virtual void itemChanged(const Akonadi::Item &item, const QSet<QByteArray> &parts);
    virtual void itemRemoved(const Akonadi::Item &item);

    virtual void collectionAdded(const Akonadi::Collection &collection, const Akonadi::Collection
        &parent);
    virtual void collectionChanged(const Akonadi::Collection &collection);
    virtual void collectionRemoved(const Akonadi::Collection &collection);

private:
    class Private;
    QSharedDataPointer<Private> d;
};

#endif
