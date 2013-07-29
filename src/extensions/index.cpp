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
extern "C" {
#include <zeitgeist.h>
}

#include "index.h"
#include "tools.h"
#include "timerange.h"
#include "resultset.h"

namespace QZeitgeist
{

class IndexPrivate
{
public:
    IndexPrivate(Index *q_);
    ~IndexPrivate();

    void emitEventsFound(const ResultSet &res);
    void emitEventsFoundWithRelevancies(const ResultSet &res, const QList<double> &r);
    void emitError(const QString &e);

    Index *q;
    ZeitgeistIndex *index;
};

IndexPrivate::IndexPrivate(Index *q_)
    : q(q_)
{
    index = zeitgeist_index_new();
}

IndexPrivate::~IndexPrivate()
{
    g_object_unref(index);
}

void IndexPrivate::emitEventsFound(const ResultSet &res)
{
    Q_EMIT q->eventsFound(res);
}

void IndexPrivate::emitEventsFoundWithRelevancies(const ResultSet &res, const QList<double> &r)
{
    Q_EMIT q->eventsFoundWithRelevancies(res, r);
}

void IndexPrivate::emitError(const QString &e)
{
    Q_EMIT q->error(e);
}

// callbacks
static void on_found(ZeitgeistIndex *index, GAsyncResult *res, IndexPrivate *d)
{
    GError *error = 0;
    ZeitgeistResultSet *result = zeitgeist_index_search_finish(index, res, &error);

    if (error) {
        d->emitError(error->message);

        g_error_free(error);
        return;
    }

    d->emitEventsFound(ResultSet::fromHandle(result));

    g_object_unref(result);
}

static void on_found_with_relevancies(ZeitgeistIndex *index, GAsyncResult *res, IndexPrivate *d)
{
    GError *error = 0;
    int len = 0;
    gdouble *array = 0;
    ZeitgeistResultSet *result = zeitgeist_index_search_with_relevancies_finish(index, res, &array, &len, &error);

    if (error) {
        d->emitError(error->message);

        g_error_free(error);
        g_free(array);
        return;
    }

    QList<double> list;

    for (int i = 0; i < len; ++i) {
        list.append(array[i]);
    }

    d->emitEventsFoundWithRelevancies(ResultSet::fromHandle(result), list);

    g_free(array);
    g_object_unref(result);
}

// class Index
Index::Index(QObject *parent)
    : QObject(parent)
    , d(new IndexPrivate(this))
{
}

Index::~Index()
{
    delete d;
}

void Index::search(const QString &query, const TimeRange &timeRange,
                   const QList<Event> &eventTemplates, quint32 offset,
                   quint32 count, ResultType resultType)
{
    ZeitgeistTimeRange *tr = (ZeitgeistTimeRange *)Tools::timeRangeCreateHandle(timeRange);
    GPtrArray *arr = Tools::eventsToPtrArray(eventTemplates);

    zeitgeist_index_search(d->index,
                           query.toUtf8().constData(),
                           tr,
                           arr,
                           offset,
                           count,
                           (ZeitgeistResultType)resultType,
                           0,
                           (GAsyncReadyCallback)on_found,
                           d);

    g_object_unref(tr);
    g_ptr_array_unref(arr);
}

void Index::searchWithRelevancies(const QString &query, const TimeRange &timeRange,
                                  const QList<Event> &eventTemplates, StorageState storageState,
                                  quint32 offset, quint32 count, ResultType resultType)
{
    ZeitgeistTimeRange *tr = (ZeitgeistTimeRange *)Tools::timeRangeCreateHandle(timeRange);
    GPtrArray *arr = Tools::eventsToPtrArray(eventTemplates);

    zeitgeist_index_search_with_relevancies(d->index,
                                            query.toUtf8().constData(),
                                            tr,
                                            arr,
                                            (ZeitgeistStorageState)storageState,
                                            offset,
                                            count,
                                            (ZeitgeistResultType)resultType,
                                            0,
                                            (GAsyncReadyCallback)on_found_with_relevancies,
                                            d);

    g_object_unref(tr);
    g_ptr_array_unref(arr);
}

} // namespace QZeitgeist

