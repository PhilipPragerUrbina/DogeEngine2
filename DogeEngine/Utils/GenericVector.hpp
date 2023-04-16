//
// Created by Philip on 4/9/2023.
//

#pragma once
#include <vector>
namespace Doge {

    class GenericVector {
    public:
        virtual size_t size() const {
            return 0;
        }

        virtual ~GenericVector() = default;
    };

    template<typename T> class GenericVectorImplementation : public GenericVector{
    public:
        std::vector<T> vector;

        size_t size() const override {
            return vector.size();
        }
    };

} // Doge
