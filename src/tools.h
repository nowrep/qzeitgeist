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
#ifndef QZEITGEIST_TOOLS_H_
#define QZEITGEIST_TOOLS_H_

#include "qzeitgeist.h"
#include "subject.h"
#include "event.h"
#include <QtCore/QList>

namespace QZeitgeist
{

class Tools
{
public:
    static GPtrArray *subjectsToPtrArray(const QList<Subject> &subjects);
    static QList<Subject> subjectsFromPtrArray(GPtrArray *array);

    static GByteArray *convertToByteArray(const QByteArray &array);
    static QByteArray convertFromByteArray(GByteArray *array);

    static GPtrArray *eventsToPtrArray(const QList<Event> &events);
    static QList<Event> eventsFromPtrArray(GPtrArray *array);

    static QList<QUrl> urlsFromGList(GList *list);
};

}; // namespace QZeitgeist

#endif // QZEITGEIST_TOOLS_H_
