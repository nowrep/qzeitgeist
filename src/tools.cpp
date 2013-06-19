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

#include "tools.h"
#include "event.h"
#include <QtCore/QByteArray>
#include <QtCore/QUrl>

namespace QZeitgeist
{

GPtrArray *Tools::subjectsToPtrArray(const QList<Subject> &subjects)
{
    GPtrArray *array = g_ptr_array_sized_new(subjects.size());

    foreach (const Subject &subject, subjects) {
        g_ptr_array_add(array, (gpointer)subject.createHandle());
    }

    return array;
}

QList<Subject> Tools::subjectsFromPtrArray(GPtrArray *array)
{
    QList<Subject> list;

    for (unsigned i = 0; i < array->len; ++i) {
        Subject subject = Subject::fromHandle(g_ptr_array_index(array, i));
        list.append(subject);
    }

    return list;
}

GByteArray *Tools::convertToByteArray(const QByteArray &array)
{
    GByteArray *garray = g_byte_array_new();
    g_byte_array_append(garray, (guint8 *)array.constData(), array.size());
    return garray;
}

QByteArray Tools::convertFromByteArray(GByteArray *array)
{
    return QByteArray((const char *)array->data, (int) array->len);
}

GPtrArray *Tools::eventsToPtrArray(const QList<Event> &events)
{
    GPtrArray *array = g_ptr_array_sized_new(events.size());

    foreach (const Event &event, events) {
        g_ptr_array_add(array, (gpointer)event.createHandle());
    }

    return array;
}

QList<Event> Tools::eventsFromPtrArray(GPtrArray *array)
{
    QList<Event> list;

    for (unsigned i = 0; i < array->len; ++i) {
        Event event = Event::fromHandle(g_ptr_array_index(array, i));
        list.append(event);
    }

    return list;
}

QList<QUrl> Tools::urlsFromGList(GList *list)
{
    QList<QUrl> urls;
    GList *tmp = list;

    while (tmp) {
        gchar *uri = (gchar *) tmp->data;
        urls.append(QUrl(uri));
        tmp = tmp->next;
    }

    g_list_free_full(list, g_free);
    return urls;
}


}; // namespace QZeitgeist

