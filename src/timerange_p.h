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
#ifndef QZEITGEIST_TIMERANGE_P_H_
#define QZEITGEIST_TIMERANGE_P_H_

namespace QZeitgeist
{

struct ZeitgeistTimeRange;

class TimeRangePrivate
{
public:
    explicit TimeRangePrivate(qint64 start_, qint64 end_);
    TimeRangePrivate(const TimeRangePrivate &other);

    bool compare(const TimeRangePrivate &other) const;

    qint64 start;
    qint64 end;
};

::ZeitgeistTimeRange *timeRangeToNative(const TimeRange &timeRange);

}; // namespace QZeitgeist

#endif // QZEITGEIST_TIMERANGE_P_H_

