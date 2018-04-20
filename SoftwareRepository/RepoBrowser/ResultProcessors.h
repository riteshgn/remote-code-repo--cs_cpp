#pragma once

#ifndef RESULTPROCESSORS_H
#define RESULTPROCESSORS_H

#include "IRepoBrowser.h"
#include "../FileResource/FileResource.h"

namespace SoftwareRepository
{

    /////////////////////////////////////////////////////////////////////
    // ConsoleResultProcessor
    // - prints each navigation step on the standard console output

    class ConsoleResultProcessor : public IBrowserResultProcessor<FileResource>
    {
    public:
        virtual bool operator()(FileResource, ResourceVersion, Level) override;
    };

}

#endif // !RESULTPROCESSORS_H
