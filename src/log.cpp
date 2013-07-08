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

#include "log.h"
#include "monitor.h"
#include "tools.h"
#include "resultset.h"
#include <QtCore/QDebug>
#include <QtCore/QStringList>

namespace QZeitgeist
{

class LogRequest
{
public:
    LogRequest(int _id, LogPrivate *_d) : id(_id), d(_d) { }

    int id;
    LogPrivate *d;
};

class LogPrivate
{
public:
    explicit LogPrivate(Log *log);
    ~LogPrivate();

    int createId();
    void emitMonitorRemoved(int id) const;
    void emitError(int id, const QString &message) const;
    void emitEventsInserted(int id, const QList<quint32> &eventIds) const;
    void emitEventsFound(int id, const ResultSet &resultSet) const;
    void emitEventIdsFound(int id, const QList<quint32> &eventIds) const;
    void emitEventsGotById(int id, const ResultSet &resultSet) const;
    void emitRelatedUrlsFound(int id, const QList<QUrl> &urls) const;
    void emitEventsDeleted(int id, const TimeRange &tr) const;

    Log *q;
    ZeitgeistLog *log;
    bool isDefault;
    int requestIdCounter;
};

LogPrivate::LogPrivate(Log *log)
    : q(log)
    , log(0)
    , isDefault(false)
    , requestIdCounter(0)
{
}

LogPrivate::~LogPrivate()
{
    if (!isDefault) {
        g_object_unref(log);
    }
}

int LogPrivate::createId()
{
    return requestIdCounter++;
}

void LogPrivate::emitMonitorRemoved(int id) const
{
    Q_EMIT q->monitorRemoved(id);
}

void LogPrivate::emitError(int id, const QString &message) const
{
    Q_EMIT q->error(id, message);
}

void LogPrivate::emitEventsInserted(int id, const QList<quint32> &eventIds) const
{
    Q_EMIT q->eventsInserted(id, eventIds);
}

void LogPrivate::emitEventsFound(int id, const ResultSet &resultSet) const
{
    Q_EMIT q->eventsFound(id, resultSet);
}

void LogPrivate::emitEventIdsFound(int id, const QList<quint32> &eventIds) const
{
    Q_EMIT q->eventIdsFound(id, eventIds);
}

void LogPrivate::emitEventsGotById(int id, const ResultSet &resultSet) const
{
    Q_EMIT q->eventsGotById(id, resultSet);
}

void LogPrivate::emitRelatedUrlsFound(int id, const QList<QUrl> &urls) const
{
    Q_EMIT q->relatedUrlsFound(id, urls);
}

void LogPrivate::emitEventsDeleted(int id, const TimeRange &tr) const
{
    Q_EMIT q->eventsDeleted(id, tr);
}

// callbacks
static void on_monitor_removed(ZeitgeistLog *log, GAsyncResult *res, LogRequest *request)
{
    GError *error = 0;
    zeitgeist_log_remove_monitor_finish(log, res, &error);

    if (error) {
        request->d->emitError(request->id, error->message);

        g_error_free(error);
        delete request;
        return;
    }

    request->d->emitMonitorRemoved(request->id);

    delete request;
}

static void on_events_inserted(ZeitgeistLog *log, GAsyncResult *res, LogRequest *request)
{
    GError *error = 0;
    GArray *event_ids = zeitgeist_log_insert_events_finish(log, res, &error);

    if (error) {
        request->d->emitError(request->id, error->message);

        g_error_free(error);
        g_array_unref(event_ids);
        delete request;
        return;
    }

    request->d->emitEventsInserted(request->id, Tools::quint32ListFromGArray(event_ids));

    g_array_unref(event_ids);
    delete request;
}

static void on_events_found(ZeitgeistLog *log, GAsyncResult *res, LogRequest *request)
{
    GError *error = 0;
    ZeitgeistResultSet *result = zeitgeist_log_find_events_finish(log, res, &error);

    if (error) {
        request->d->emitError(request->id, error->message);

        g_error_free(error);
        g_object_unref(result);
        delete request;
        return;
    }

    request->d->emitEventsFound(request->id, ResultSet::fromHandle(result));

    delete request;
}

static void on_event_ids_found(ZeitgeistLog *log, GAsyncResult *res, LogRequest *request)
{
    GError *error = 0;
    int len = 0;
    guint32 *result = zeitgeist_log_find_event_ids_finish(log, res, &len, &error);

    if (error) {
        request->d->emitError(request->id, error->message);

        g_free(result);
        g_error_free(error);
        delete request;
        return;
    }

    QList<quint32> list;
    for (int i = 0; i < len; ++i) {
        list.append(result[i]);
    }
    g_free(result);

    request->d->emitEventIdsFound(request->id, list);

    delete request;
}

static void on_events_got_by_id(ZeitgeistLog *log, GAsyncResult *res, LogRequest *request)
{
    GError *error = 0;
    ZeitgeistResultSet *result = zeitgeist_log_get_events_finish(log, res, &error);

    if (error) {
        request->d->emitError(request->id, error->message);

        g_error_free(error);
        g_object_unref(result);
        delete request;
        return;
    }

    request->d->emitEventsGotById(request->id, ResultSet::fromHandle(result));

    delete request;
}

static void on_events_deleted(ZeitgeistLog *log, GAsyncResult *res, LogRequest *request)
{
    GError *error = 0;
    ZeitgeistTimeRange *result = zeitgeist_log_delete_events_finish(log, res, &error);

    if (error) {
        request->d->emitError(request->id, error->message);

        g_error_free(error);
        g_object_unref(result);
        delete request;
        return;
    }

    request->d->emitEventsDeleted(request->id, Tools::timeRangeFromHandle(result));

    g_object_unref(result);
    delete request;
}

static void on_related_urls_found(ZeitgeistLog *log, GAsyncResult *res, LogRequest *request)
{
    GError *error = 0;
    int len = 0;
    gchar **result = zeitgeist_log_find_related_uris_finish(log, res, &len, &error);

    if (error) {
        request->d->emitError(request->id, error->message);

        g_error_free(error);
        g_free(result);
        delete request;
        return;
    }

    QList<QUrl> list;
    for (int i = 0; i < len; ++i) {
        list.append(QUrl(result[i]));
    }
    g_free(result);

    request->d->emitRelatedUrlsFound(request->id, list);

    delete request;
}

// class Log
Log::Log(QObject *parent)
    : QObject(parent)
    , d(new LogPrivate(this))
{
    d->log = zeitgeist_log_new();

    qRegisterMetaType<QList<quint32> >("QList<quint32>");
    qRegisterMetaType<QList<QUrl> >("QList<QUrl>");
}

Log::~Log()
{
    delete d;
}

bool Log::installMonitor(Monitor *monitor)
{
    GError *err = 0;

    zeitgeist_log_install_monitor(d->log, (ZeitgeistMonitor *)monitor->getHandle(), &err);

    if (err) {
        qWarning() << "Log::installMonitor Error:" << err->message;
        g_error_free(err);
        return false;
    }

    return true;
}

int Log::removeMonitor(Monitor *monitor)
{
    int id = d->createId();

    zeitgeist_log_remove_monitor(d->log,
                                 (ZeitgeistMonitor *)monitor->getHandle(),
                                 (GAsyncReadyCallback)on_monitor_removed,
                                 new LogRequest(id, d));

    return id;
}

int Log::insertEvent(const Event &event)
{
    return insertEvents(QList<Event>() << event);
}

int Log::insertEvents(const QList<Event> &events)
{
    GPtrArray *arr = Tools::eventsToPtrArray(events);
    int id = d->createId();

    zeitgeist_log_insert_events(d->log, arr, 0,
                                (GAsyncReadyCallback)on_events_inserted,
                                new LogRequest(id, d));
    g_ptr_array_unref(arr);

    return id;
}

int Log::findEvents(const TimeRange &timeRange, const QList<Event> &eventTemplates,
                    StorageState storageState, ResultType resultType, int count)
{
    ZeitgeistTimeRange *tr = (ZeitgeistTimeRange *)Tools::timeRangeCreateHandle(timeRange);
    GPtrArray *arr = Tools::eventsToPtrArray(eventTemplates);
    int id = d->createId();

    zeitgeist_log_find_events(d->log, tr, arr,
                              (ZeitgeistStorageState)storageState,
                              count,
                              (ZeitgeistResultType)resultType,
                              0,
                              (GAsyncReadyCallback)on_events_found,
                              new LogRequest(id, d));
    g_object_unref(tr);
    g_ptr_array_unref(arr);

    return id;
}

int Log::findEventIds(const TimeRange &timeRange, const QList<Event> &eventTemplates,
                      StorageState storageState, ResultType resultType, int count)
{
    ZeitgeistTimeRange *tr = (ZeitgeistTimeRange *)Tools::timeRangeCreateHandle(timeRange);
    GPtrArray *arr = Tools::eventsToPtrArray(eventTemplates);
    int id = d->createId();

    zeitgeist_log_find_event_ids(d->log, tr, arr,
                                 (ZeitgeistStorageState)storageState,
                                 count,
                                 (ZeitgeistResultType)resultType,
                                 0,
                                 (GAsyncReadyCallback)on_event_ids_found,
                                 new LogRequest(id, d));
    g_object_unref(tr);
    g_ptr_array_unref(arr);

    return id;
}

int Log::getEventsByIds(const QList<quint32> &eventIds)
{
    GArray *arr = Tools::quint32ListToGArray(eventIds);
    int id = d->createId();

    zeitgeist_log_get_events(d->log, arr, 0,
                             (GAsyncReadyCallback)on_events_got_by_id,
                             new LogRequest(id, d));
    g_array_unref(arr);

    return id;
}

int Log::deleteEvents(const QList<quint32> &eventIds)
{
    GArray *arr = Tools::quint32ListToGArray(eventIds);
    int id = d->createId();

    zeitgeist_log_delete_events(d->log, arr, 0,
                                (GAsyncReadyCallback)on_events_deleted,
                                new LogRequest(id, d));
    g_array_unref(arr);

    return id;
}

int Log::findRelatedUrls(const TimeRange &timeRange, const QList<Event> &eventTemplates,
                         const QList<Event> &resultEventTemplates, StorageState storageState,
                         RelevantResultType relevantResultType, int count)
{
    ZeitgeistTimeRange *tr = (ZeitgeistTimeRange *)Tools::timeRangeCreateHandle(timeRange);
    GPtrArray *event_templates = Tools::eventsToPtrArray(eventTemplates);
    GPtrArray *result_event_templates = Tools::eventsToPtrArray(resultEventTemplates);
    int id = d->createId();

    zeitgeist_log_find_related_uris(d->log, tr, event_templates, result_event_templates,
                                    (ZeitgeistStorageState)storageState,
                                    count,
                                    (ZeitgeistRelevantResultType)relevantResultType,
                                    0,
                                    (GAsyncReadyCallback)on_related_urls_found,
                                    new LogRequest(id, d));
    g_object_unref(tr);
    g_ptr_array_unref(event_templates);
    g_ptr_array_unref(result_event_templates);

    return id;
}

QString Log::version() const
{
    gint major, minor, micro;

    zeitgeist_log_get_version(d->log, &major, &minor, &micro);

    return QString("%1.%2.%3").arg(major, minor, micro);
}

QString Log::dataPath() const
{
    gchar *res = zeitgeist_log_datapath(d->log);
    QString path = res;

    g_free((void *)res);
    return path;
}

QStringList Log::extensions() const
{
    QStringList list;
    int len = 0;
    gchar **res = zeitgeist_log_get_extensions(d->log, &len);

    for (int i = 0; i < len; ++i) {
        list.append(res[i]);
        g_free((void *)res[i]);
    }

    g_free((void *)res);
    return list;
}

// static
Log *Log::defaultLog()
{
    Log *log = new Log;
    log->d->log = zeitgeist_log_get_default();
    log->d->isDefault = true;

    return log;
}

} // namespace QZeitgeist

