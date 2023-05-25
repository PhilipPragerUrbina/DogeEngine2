//
// Created by Philip on 4/17/2023.
//

#pragma once

namespace Doge {

    /**
     * Smart pointer to shared resources
     * @tparam T Resource type
     */
    template <class T> class Resource {
    private:
        T* raw_resource;
        //todo reference counting

        friend class ResourceManager;
        /**
         * Create a pointer to the resource data
         */
        explicit Resource(T* resource_data) : raw_resource(resource_data){}

    public:

        /**
         * Operator passthrough for hashing
         */
        bool operator==(const Resource &rhs) const {
            return *raw_resource == *rhs.raw_resource;
        }

        /**
         * Access the resource data
         */
        T* operator->() const { return raw_resource; }
    };

} // Doge
