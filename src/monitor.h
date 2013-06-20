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
#ifndef QZEITGEIST_MONITOR_H_
#define QZEITGEIST_MONITOR_H_

#include "qzeitgeist.h"
#include "timerange.h"
#include "event.h"
#include <QtCore/QObject>
#include <QtCore/QList>

namespace QZeitgeist
{

class ResultSet;

class QZEITGEIST_EXPORT Monitor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<Event> eventTemplates READ eventTemplates WRITE setEventTemplates)
    Q_PROPERTY(TimeRange timeRange READ timeRange WRITE setTimeRange)

public:
    explicit Monitor(const TimeRange &timeRange, const QList<Event> &eventTemplates, QObject *parent = 0);
    ~Monitor();

    QList<Event> eventTemplates() const;
    void setEventTemplates(const QList<Event> &eventTemplates);

    TimeRange timeRange() const;
    void setTimeRange(const TimeRange &timeRange);

    HANDLE getHandle() const;

Q_SIGNALS:
#ifndef Q_MOC_RUN
public: // Hack to make signals public
#endif
    void eventsDeleted(const TimeRange &timeRange, const QList<quint32> &ids);
    void eventsInserted(const TimeRange &timeRange, const ResultSet &events);

private:
    Q_DISABLE_COPY(Monitor)

    HANDLE m_handle;
};

} // namespace QZeitgeist

#endif // QZEITGEIST_MONITOR_H_
