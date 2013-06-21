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

#include "resultset.h"
#include "event.h"

namespace QZeitgeist
{

class ResultSetPrivate
{
public:
    explicit ResultSetPrivate(ZeitgeistResultSet *r);
    ~ResultSetPrivate();

    ZeitgeistResultSet *resultSet;
};

ResultSetPrivate::ResultSetPrivate(ZeitgeistResultSet *r)
    : resultSet(r)
{
    Q_ASSERT(r);
}

ResultSetPrivate::~ResultSetPrivate()
{
    g_object_unref(resultSet);
}

// class ResultSet
ResultSet::ResultSet()
{
}

ResultSet::~ResultSet()
{
}

ResultSet &ResultSet::operator=(const ResultSet &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

int ResultSet::size() const
{
    return (int) zeitgeist_result_set_size(d->resultSet);
}

int ResultSet::estimatedMatches() const
{
    return (int) zeitgeist_result_set_estimated_matches(d->resultSet);
}

Event ResultSet::nextValue()
{
    ZeitgeistEvent *ev = zeitgeist_result_set_next_value(d->resultSet);
    return Event::fromHandle(ev);
}

bool ResultSet::hasNext() const
{
    return zeitgeist_result_set_has_next(d->resultSet);
}

int ResultSet::currentPosition() const
{
    return (int) zeitgeist_result_set_tell(d->resultSet);
}

void ResultSet::reset()
{
    zeitgeist_result_set_reset(d->resultSet);
}

// static
ResultSet ResultSet::fromHandle(HANDLE handle)
{
    ResultSet r;
    r.d = QSharedPointer<ResultSetPrivate>(new ResultSetPrivate((ZeitgeistResultSet *)handle));
    return r;
}

} // namespace QZeitgeist

