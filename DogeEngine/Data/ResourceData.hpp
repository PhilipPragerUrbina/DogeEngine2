//
// Created by Philip on 4/17/2023.
//

#pragma once

namespace Doge {

    /**
     * Represents data that can be stored as a shared resource
     * Extend this to create data that is shared between components.
     * Also expected to have a constructor that takes in a ResourceRequester and a string ID, where one can request additional dependencies.
     */
    class ResourceData {
    private:
        unsigned int reference_count = 0; //todo reference counting
    public:
        virtual ~ResourceData() = default;
    };

} // Doge
