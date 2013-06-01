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

#include "timerange.h"
#include "timerange_p.h"
#include <limits>
#include <QDateTime>

namespace QZeitgeist
{

TimeRangePrivate::TimeRangePrivate(qint64 start_, qint64 end_)
    : start(start_)
    , end(end_)
{
}

TimeRangePrivate::TimeRangePrivate(const TimeRangePrivate &other)
    : start(other.start)
    , end(other.end)
{
}

bool TimeRangePrivate::compare(const TimeRangePrivate &other) const
{
    return start == other.start && end == other.end;
}

::ZeitgeistTimeRange *timeRangeToNative(const TimeRange &other)
{
    return zeitgeist_time_range_new(other.start(), other.end());
}

// class TimeRange
TimeRange::TimeRange(qint64 start, qint64 end)
    : d(new TimeRangePrivate(start, end))
{
}

TimeRange::TimeRange(const TimeRange &other)
    : d(new TimeRangePrivate(*other.d))
{
}

TimeRange::~TimeRange()
{
}

TimeRange &TimeRange::operator=(const TimeRange &other)
{
    if (this != &other) {
        d->start = other.d->start;
        d->end = other.d->end;
    }

    return *this;
}

bool TimeRange::operator==(const TimeRange &other) const
{
    return d->compare(*other.d);
}

bool TimeRange::isValid() const
{
    return d->start > -1 && d->end > -1 && d->start <= d->end;
}

qint64 TimeRange::start() const
{
    return d->start;
}

qint64 TimeRange::end() const
{
    return d->end;
}

TimeRange TimeRange::intersect(const TimeRange &timeRange) const
{
    ::ZeitgeistTimeRange *thisTr = timeRangeToNative(*this);
    ::ZeitgeistTimeRange *otherTr = timeRangeToNative(timeRange);
    ::ZeitgeistTimeRange *resultTr = zeitgeist_time_range_intersect(thisTr, otherTr);

    if (!resultTr) {
        g_object_unref(thisTr);
        g_object_unref(otherTr);

        return TimeRange(-1, -1);
    }

    TimeRange result(zeitgeist_time_range_get_start(resultTr),
                     zeitgeist_time_range_get_end(resultTr));

    g_object_unref(thisTr);
    g_object_unref(otherTr);
    g_object_unref(resultTr);

    return result;
}

// static
TimeRange TimeRange::timeRangeAnytime()
{
    return TimeRange(0, std::numeric_limits<qint64>::max());
}

// static
TimeRange TimeRange::timeRangeToNow()
{
    return TimeRange(0, QDateTime::currentDateTime().toMSecsSinceEpoch());
}

// static
TimeRange TimeRange::timeRangeFromNow()
{
    return TimeRange(QDateTime::currentDateTime().toMSecsSinceEpoch(),
                     std::numeric_limits<qint64>::max());
}

}; // namespace QZeitgeist

