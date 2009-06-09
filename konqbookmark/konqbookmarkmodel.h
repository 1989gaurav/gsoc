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

#ifndef KONQBOOKMARK_MODEL_H
#define KONQBOOKMARK_MODEL_H

#include "konqbookmark_export.h"
#include <akonadi/itemmodel.h>

class KonqBookmark;

namespace Akonadi
{

    class KONQBOOKMARK_EXPORT KonqBookmarkModel : public ItemModel
    {
    public:
        enum Column
        {
            Title,
            Url,
            UniqueUri,
            Tags,
            Description,
            NumVisits,
            Created,
            LastModified,
            LastVisited
        };
        enum { ColumnCount =  9 };

        KonqBookmarkModel( QObject *parent = 0 );

        virtual ~KonqBookmarkModel();

        virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
        virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
        virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
        
        // We let the delegate know that we are editable
        Qt::ItemFlags flags(const QModelIndex &index) const;
        
        // With this function we make the model editable
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
        
        bool addBookmark( const KonqBookmark &konqBookmark );
        
        // Drag & drop related functions
        bool dropMimeData(const QMimeData *data, Qt::DropAction action,
            int row, int column, const QModelIndex &parent);
        QMimeData *mimeData(const QModelIndexList &indexes) const;
        QStringList mimeTypes() const;
        QString mimeType() const;
        Qt::DropActions supportedDropActions() const;
    private:
        class Private;
        Private* const d;
    };

}

#endif // KONQ_BOOKMARK_MODEL_H
