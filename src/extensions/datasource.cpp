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

#include "datasource.h"
#include "tools.h"
#include <QtCore/QAtomicInt>
#include <QtCore/QDebug>

namespace QZeitgeist
{

class DataSourcePrivate
{
public:
    DataSourcePrivate();
    DataSourcePrivate(const DataSourcePrivate &other);
    bool compare(const DataSourcePrivate &other) const;

    QAtomicInt ref;
    bool enabled;
    bool running;
    qint64 timestamp;
    QString uniqueId;
    QString name;
    QString description;
    QList<Event> eventTemplates;
};

DataSourcePrivate::DataSourcePrivate()
    : ref(1)
    , enabled(false)
    , running(false)
    , timestamp(0)
{
}

DataSourcePrivate::DataSourcePrivate(const DataSourcePrivate &other)
    : ref(1)
    , enabled(other.enabled)
    , running(other.running)
    , timestamp(other.timestamp)
    , uniqueId(other.uniqueId)
    , name(other.name)
    , description(other.description)
    , eventTemplates(other.eventTemplates)
{
}

bool DataSourcePrivate::compare(const DataSourcePrivate &other) const
{
    return enabled == other.enabled
           && running == other.running
           && timestamp == other.timestamp
           && uniqueId == other.uniqueId
           && name == other.name
           && description == other.description
           && eventTemplates == other.eventTemplates;
}

// class DataSource
DataSource::DataSource()
    : d(new DataSourcePrivate)
{
    qRegisterMetaType<DataSource>("QZeitgeist::DataSource");
}

DataSource::DataSource(const DataSource &other)
    : d(other.d)
{
    d->ref.ref();
}

DataSource::~DataSource()
{
    if (!d->ref.deref()) {
        delete d;
    }
}

DataSource &DataSource::operator=(const DataSource &other)
{
    qAtomicAssign(d, other.d);
    return *this;
}

bool DataSource::operator==(const DataSource &other) const
{
    return d->compare(*other.d);
}

bool DataSource::isEnabled() const
{
    return d->enabled;
}

void DataSource::setEnabled(bool enabled)
{
    detach();

    d->enabled = enabled;
}

bool DataSource::isRunning() const
{
    return d->running;
}

void DataSource::setRunning(bool running)
{
    detach();

    d->running = running;
}

qint64 DataSource::timestamp() const
{
    return d->timestamp;
}

void DataSource::setTimestamp(qint64 timestamp)
{
    detach();

    d->timestamp = timestamp;
}

QString DataSource::uniqueId() const
{
    return d->uniqueId;
}

void DataSource::setUniqueId(const QString &id)
{
    detach();

    d->uniqueId = id;
}

QString DataSource::name() const
{
    return d->name;
}

void DataSource::setName(const QString &name)
{
    detach();

    d->name = name;
}

QString DataSource::description() const
{
    return d->description;
}

void DataSource::setDescription(const QString &description)
{
    detach();

    d->description = description;
}

QList<Event> DataSource::eventTemplates() const
{
    return d->eventTemplates;
}

void DataSource::setEventTemplates(const QList<Event> &eventTemplates)
{
    detach();

    d->eventTemplates = eventTemplates;
}

HANDLE DataSource::createHandle() const
{
    QByteArray idData = d->uniqueId.toUtf8();
    QByteArray nameData = d->name.toUtf8();
    QByteArray descriptionData = d->description.toUtf8();
    GPtrArray *events = Tools::eventsToPtrArray(d->eventTemplates);

    ZeitgeistDataSource *ds = zeitgeist_data_source_new();
    zeitgeist_data_source_set_enabled(ds, d->enabled);
    zeitgeist_data_source_set_running(ds, d->running);
    zeitgeist_data_source_set_timestamp(ds, d->timestamp);
    zeitgeist_data_source_set_unique_id(ds, idData.constData());
    zeitgeist_data_source_set_name(ds, nameData.constData());
    zeitgeist_data_source_set_description(ds, descriptionData.constData());
    zeitgeist_data_source_set_event_templates(ds, events);

    g_ptr_array_unref(events);

    return ds;
}

// static
DataSource DataSource::fromHandle(HANDLE handle)
{
    ZeitgeistDataSource *ds = (ZeitgeistDataSource *)handle;
    DataSource dataSource;

    dataSource.d->enabled = zeitgeist_data_source_get_enabled(ds);
    dataSource.d->running = zeitgeist_data_source_get_running(ds);
    dataSource.d->timestamp = zeitgeist_data_source_get_timestamp(ds);
    dataSource.d->uniqueId = zeitgeist_data_source_get_unique_id(ds);
    dataSource.d->name = zeitgeist_data_source_get_name(ds);
    dataSource.d->description = zeitgeist_data_source_get_description(ds);
    dataSource.d->eventTemplates = Tools::eventsFromPtrArray(zeitgeist_data_source_get_event_templates(ds));

    return dataSource;
}

static const int streamVersion = 1;

QDataStream &operator<<(QDataStream &stream, const DataSource &dataSource)
{
    stream << streamVersion;
    stream << dataSource.d->enabled;
    stream << dataSource.d->running;
    stream << dataSource.d->timestamp;
    stream << dataSource.d->uniqueId;
    stream << dataSource.d->name;
    stream << dataSource.d->description;
    stream << dataSource.d->eventTemplates;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, DataSource &dataSource)
{
    int version = -1;
    stream >> version;

    if (version != streamVersion) {
        qWarning() << "DataSource: Invalid stream version!";
        return stream;
    }

    stream >> dataSource.d->enabled;
    stream >> dataSource.d->running;
    stream >> dataSource.d->timestamp;
    stream >> dataSource.d->uniqueId;
    stream >> dataSource.d->name;
    stream >> dataSource.d->description;
    stream >> dataSource.d->eventTemplates;

    return stream;
}

void DataSource::detach()
{
    qAtomicDetach(d);
}

} // namespace QZeitgeist

