/*
 * This file is part of QZeitgeist.
 *
 * Copyright (C) 2013 David Rosca <nowrep@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef QZEITGEIST_INDEX_H_
#define QZEITGEIST_INDEX_H_

#include "qzeitgeist.h"
#include "event.h"
#include <QtCore/QObject>

namespace QZeitgeist
{

class ResultSet;
class TimeRange;

class QZEITGEIST_EXPORT Index : public QObject
{
    Q_OBJECT

public:
    explicit Index(QObject *parent = 0);
    ~Index();

    void search(const QString &query, const TimeRange &timeRange,
                const QList<Event> &eventTemplates, quint32 offset,
                quint32 count, ResultType resultType);

    void searchWithRelevancies(const QString &query, const TimeRange &timeRange,
                               const QList<Event> &eventTemplates, StorageState storageState,
                               quint32 offset, quint32 count, ResultType resultType);

Q_SIGNALS:
    void eventsFound(const ResultSet &resultSet);
    void eventsFoundWithRelevancies(const ResultSet &resultSet, const QList<double> &relevancies);
    void error(const QString &errorMessage);

private:
    Q_DISABLE_COPY(Index)

    class IndexPrivate *d;
    friend class IndexPrivate;
};

} // namespace QZeitgeist

#endif // QZEITGEIST_INDEX_H_
