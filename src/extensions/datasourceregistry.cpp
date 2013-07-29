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

#include "datasourceregistry.h"
#include "tools.h"
#include <QtCore/QDebug>

namespace QZeitgeist
{

class DataSourceRequest
{
public:
    DataSourceRequest(int _id, DataSourceRegistryPrivate *_d) : id(_id), d(_d) { }

    int id;
    DataSourceRegistryPrivate *d;
};

class DataSourceRegistryPrivate
{
public:
    DataSourceRegistryPrivate(DataSourceRegistry *q_);
    ~DataSourceRegistryPrivate();

    int createId();
    void emitError(int id, const QString &e);
    void emitDataSourcesReceived(int id, const QList<DataSource> &ds);
    void emitDataSourceGotById(int id, const DataSource &ds);
    void emitDataSourceRegistered(int id);
    void emitDataSourceEnabled(int id);

    DataSourceRegistry *q;
    ZeitgeistDataSourceRegistry *registry;
    int requestIdCounter;
};

DataSourceRegistryPrivate::DataSourceRegistryPrivate(DataSourceRegistry *q_)
    : q(q_)
    , registry(zeitgeist_data_source_registry_new())
{
    Q_ASSERT(registry);
}

DataSourceRegistryPrivate::~DataSourceRegistryPrivate()
{
    g_object_unref(registry);
}

int DataSourceRegistryPrivate::createId()
{
    return requestIdCounter++;
}

void DataSourceRegistryPrivate::emitError(int id, const QString &e)
{
    Q_EMIT q->error(id, e);
}

void DataSourceRegistryPrivate::emitDataSourcesReceived(int id, const QList<DataSource> &ds)
{
    Q_EMIT q->dataSourcesReceived(id, ds);
}

void DataSourceRegistryPrivate::emitDataSourceGotById(int id, const DataSource &ds)
{
    Q_EMIT q->dataSourceGotById(id, ds);
}

void DataSourceRegistryPrivate::emitDataSourceRegistered(int id)
{
    Q_EMIT q->dataSourceRegistered(id);
}

void DataSourceRegistryPrivate::emitDataSourceEnabled(int id)
{
    Q_EMIT q->dataSourceEnabled(id);
}

// callbacks
static void on_all_sources_got(ZeitgeistDataSourceRegistry *registry, GAsyncResult *res,
                               DataSourceRequest *request)
{
    GError *error = 0;
    GPtrArray *result = zeitgeist_data_source_registry_get_data_sources_finish(registry, res, &error);

    if (error) {
        request->d->emitError(request->id, error->message);

        g_error_free(error);
        delete request;
        return;
    }

    request->d->emitDataSourcesReceived(request->id, Tools::dataSourcesFromPtrArray(result));

    g_ptr_array_unref(result);
    delete request;
}

static void on_source_got_by_id(ZeitgeistDataSourceRegistry *registry, GAsyncResult *res,
                                DataSourceRequest *request)
{
    GError *error = 0;
    ZeitgeistDataSource *result = zeitgeist_data_source_registry_get_data_source_from_id_finish(registry, res, &error);

    if (error) {
        request->d->emitError(request->id, error->message);

        g_error_free(error);
        delete request;
        return;
    }

    request->d->emitDataSourceGotById(request->id, Tools::dataSourceFromHandle(result));

    g_object_unref(result);
    delete request;
}

static void on_registered_source(ZeitgeistDataSourceRegistry *registry, GAsyncResult *res,
                                 DataSourceRequest *request)
{
    GError *error = 0;
    gboolean result = zeitgeist_data_source_registry_register_data_source_finish(registry, res, &error);

    if (error) {
        request->d->emitError(request->id, error->message);

        g_error_free(error);
        delete request;
        return;
    }

    if (result) {
        request->d->emitDataSourceRegistered(request->id);
    }
    else {
        request->d->emitError(request->id, QString("Unknown error"));
    }

    delete request;
}

static void on_source_enabled(ZeitgeistDataSourceRegistry *registry, GAsyncResult *res,
                              DataSourceRequest *request)
{
    GError *error = 0;
    zeitgeist_data_source_registry_set_data_source_enabled_finish(registry, res, &error);

    if (error) {
        request->d->emitError(request->id, error->message);

        g_error_free(error);
        delete request;
        return;
    }

    request->d->emitDataSourceEnabled(request->id);

    delete request;
}

// class DataSourceRegistry
DataSourceRegistry::DataSourceRegistry(QObject *parent)
    : QObject(parent)
    , d(new DataSourceRegistryPrivate(this))
{
}

DataSourceRegistry::~DataSourceRegistry()
{
    delete d;
}

int DataSourceRegistry::getAllDataSources()
{
    int id = d->createId();

    zeitgeist_data_source_registry_get_data_sources(d->registry,
            0,
            (GAsyncReadyCallback)on_all_sources_got,
            new DataSourceRequest(id, d));

    return id;
}

int DataSourceRegistry::getDataSourceById(const QString &uniqueId)
{
    QByteArray idData = uniqueId.toUtf8();
    int id = d->createId();

    zeitgeist_data_source_registry_get_data_source_from_id(d->registry,
            idData.constData(),
            0,
            (GAsyncReadyCallback)on_source_got_by_id,
            new DataSourceRequest(id, d));

    return id;
}

int DataSourceRegistry::registerDataSource(const DataSource &dataSource)
{
    ZeitgeistDataSource *ds = (ZeitgeistDataSource *)Tools::dataSourceCreateHandle(dataSource);
    int id = d->createId();

    zeitgeist_data_source_registry_register_data_source(d->registry,
            ds,
            0,
            (GAsyncReadyCallback)on_registered_source,
            new DataSourceRequest(id, d));

    g_object_unref(ds);

    return id;
}

int DataSourceRegistry::setDataSourceEnabled(const QString &uniqueId, bool enabled)
{
    QByteArray idData = uniqueId.toUtf8();
    int id = d->createId();

    zeitgeist_data_source_registry_set_data_source_enabled(d->registry,
            idData.constData(),
            enabled,
            0,
            (GAsyncReadyCallback)on_source_enabled,
            new DataSourceRequest(id, d));

    return id;
}

} // namespace QZeitgeist

