#pragma once
#include "IRepoBrowser.h"
#include "../ResourcePropertiesDb/FileResourcePayload.h"

#ifndef BROWSERFILTERS_H
#define BROWSERFILTERS_H

namespace SoftwareRepository
{
    class FilenameFilter : public IBrowserFilter<FileResourcePayload>
    {
    public:
        virtual NoSqlDb::Query<FileResourcePayload> apply(const NoSqlDb::Query<FileResourcePayload>&) override;
        static FilenameFilter create(const ResourceName& filename) { return FilenameFilter(filename); }

    private:
        FilenameFilter(const ResourceName& filename) : filename_(filename) {}
        ResourceName filename_;
    };

    class FileVersionFilter : public IBrowserFilter<FileResourcePayload>
    {
    public:
        virtual NoSqlDb::Query<FileResourcePayload> apply(const NoSqlDb::Query<FileResourcePayload>&) override;
        static FileVersionFilter create(const ResourceVersion& version) { return FileVersionFilter(version); }

    private:
        FileVersionFilter(const ResourceVersion& version) : version_(version) {}
        ResourceVersion version_;
    };

    class PackageFilter : public IBrowserFilter<FileResourcePayload>
    {
    public:
        virtual NoSqlDb::Query<FileResourcePayload> apply(const NoSqlDb::Query<FileResourcePayload>&) override;
        static PackageFilter create(const PackageName& package) { return PackageFilter(package); }

    private:
        PackageFilter(const PackageName& package) : package_(package) {}
        PackageName package_;
    };
}

#endif // !BROWSERFILTERS_H
