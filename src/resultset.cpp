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

ResultSet::ResultSet(HANDLE handle)
    : m_handle(handle)
{
    Q_ASSERT(m_handle);

    g_object_ref((ZeitgeistResultSet *)m_handle);
}

ResultSet::~ResultSet()
{
    g_object_unref((ZeitgeistResultSet *)m_handle);
}

int ResultSet::size() const
{
    return (int) zeitgeist_result_set_size((ZeitgeistResultSet *)m_handle);
}

int ResultSet::estimatedMatches() const
{
    return (int) zeitgeist_result_set_estimated_matches((ZeitgeistResultSet *)m_handle);
}

Event ResultSet::nextValue()
{
    ZeitgeistEvent *ev = zeitgeist_result_set_next_value((ZeitgeistResultSet *)m_handle);
    return Event::fromHandle(ev);
}

bool ResultSet::hasNext() const
{
    return zeitgeist_result_set_has_next((ZeitgeistResultSet *)m_handle);
}

int ResultSet::currentPosition() const
{
    return (int) zeitgeist_result_set_tell((ZeitgeistResultSet *)m_handle);
}

void ResultSet::reset()
{
    zeitgeist_result_set_reset((ZeitgeistResultSet *)m_handle);
}

} // namespace QZeitgeist

