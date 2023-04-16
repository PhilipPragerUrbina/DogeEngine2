//
// Created by Philip on 4/9/2023.
//

#pragma once
#include <iostream>
#include <glm.hpp>
#include <gtx/string_cast.hpp>
namespace Doge {

    /**
     *  IOstream overload for glm some math types
     *  Makes debugging a lot easier
     **/


    std::ostream& operator<<(std::ostream& os, const glm::vec3& to_print)
    {
        os << glm::to_string(to_print);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const glm::vec4& to_print)
    {
        os << glm::to_string(to_print);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const glm::vec2& to_print)
    {
        os << glm::to_string(to_print);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const glm::mat4& to_print)
    {
        os << glm::to_string(to_print);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const glm::mat3& to_print)
    {
        os << glm::to_string(to_print);
        return os;
    }



} // Doge
