//
// Created by Philip on 4/9/2023.
//

#pragma once
#include <vector>
namespace Doge {

    /**
     * Simple wrapper for std::vector that does not contain the type
     */
    class GenericVector {
    public:
        /**
         * Get the size of the vector
         */
        virtual size_t size() const {
            return 0;
        }

        virtual ~GenericVector() = default;
    };

    /**
     * Allows storing a vector in a generic vector
     */
    template<typename T> class GenericVectorImplementation : public GenericVector{
    public:

        /**
         * Start with empty vector
         */
        GenericVectorImplementation() = default;

        /**
         * Create from existing vector
         * @param vector Vector to copy
         */
        explicit GenericVectorImplementation(const std::vector<T>& vector) : vector(vector){}

        /**
         * The underlying vector itself
         */
        std::vector<T> vector;

        size_t size() const override {
            return vector.size();
        }
    };

} // Doge
