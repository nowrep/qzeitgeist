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
#ifndef QZEITGEIST_DATASOURCEREGISTRY_H_
#define QZEITGEIST_DATASOURCEREGISTRY_H_

#include <QZeitgeist/QZeitgeist>
#include <QtCore/QObject>

namespace QZeitgeist
{

class DataSource;

class QZEITGEIST_EXPORT DataSourceRegistry : public QObject
{
    Q_OBJECT

public:
    explicit DataSourceRegistry(QObject *parent = 0);
    ~DataSourceRegistry();

    int getAllDataSources();
    int getDataSourceById(const QString &uniqueId);

    int registerDataSource(const DataSource &dataSource);
    int setDataSourceEnabled(const QString &uniqueId, bool enabled);

Q_SIGNALS:
    void dataSourcesReceived(int requestId, const QList<DataSource> &dataSources);
    void dataSourceGotById(int requestId, const DataSource &dataSource);
    void dataSourceRegistered(int requestId);
    void dataSourceEnabled(int requestId);
    void error(int requestId, const QString &errorMessage);

private:
    Q_DISABLE_COPY(DataSourceRegistry)

    class DataSourceRegistryPrivate *d;
    friend class DataSourceRegistryPrivate;
};

} // namespace QZeitgeist

#endif // QZEITGEIST_DATASOURCEREGISTRY_H_
