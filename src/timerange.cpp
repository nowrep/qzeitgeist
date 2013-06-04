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
#include <limits>
#include <QDateTime>

namespace QZeitgeist
{

class TimeRangePrivate
{
public:
    explicit TimeRangePrivate();
    bool compare(const TimeRangePrivate &other) const;
    void copy(const TimeRangePrivate &other);

    qint64 start;
    qint64 end;
};

TimeRangePrivate::TimeRangePrivate()
    : start(-1)
    , end(-1)
{
}

bool TimeRangePrivate::compare(const TimeRangePrivate &other) const
{
    return start == other.start && end == other.end;
}

void TimeRangePrivate::copy(const TimeRangePrivate &other)
{
    if (this != &other) {
        start = other.start;
        end = other.end;
    }
}

// class TimeRange
TimeRange::TimeRange(qint64 start, qint64 end)
    : d(new TimeRangePrivate)
{
    d->start = start;
    d->end = end;
}

TimeRange::TimeRange(const TimeRange &other)
    : d(new TimeRangePrivate)
{
    d->copy(*other.d);
}

TimeRange::~TimeRange()
{
}

TimeRange &TimeRange::operator=(const TimeRange &other)
{
    d->copy(*other.d);
    return *this;
}

bool TimeRange::operator==(const TimeRange &other) const
{
    return d->compare(*other.d);
}

qint64 TimeRange::start() const
{
    return d->start;
}

qint64 TimeRange::end() const
{
    return d->end;
}

bool TimeRange::isValid() const
{
    return d->start > -1 && d->end > -1 && d->start <= d->end;
}

TimeRange TimeRange::intersect(const TimeRange &timeRange) const
{
    // Make sure both time ranges are valid and do intersects
    if (!isValid() || !timeRange.isValid() || start() > timeRange.end()) {
        return TimeRange();
    }

    return TimeRange(qMax(start(), timeRange.start()), qMin(end(), timeRange.end()));
}

HANDLE TimeRange::createHandle() const
{
    return zeitgeist_time_range_new(start(), end());
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

