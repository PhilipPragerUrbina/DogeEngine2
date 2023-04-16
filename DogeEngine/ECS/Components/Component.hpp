//
// Created by Philip on 4/6/2023.
//

#pragma once

#include <typeinfo>
#include <string>
#include "../Entity.hpp"


namespace Doge {

    /**
     * A component is just some data
     * A component can contain any data, but must have a unique type ID
     * @details https://t-machine.org/index.php/2007/12/22/entity-systems-are-the-future-of-mmog-development-part-3/
     * To create a component, simply extend this base class with whatever data you want.
     * @details Make sure to balance keeping data directly in the component for cache friendliness, and sharing data between components with pointers
     * You can also add some small helper methods such as getters and setter, but it should be limited as most functionality should go into systems.
     */
    class Component {
    private:
        Entity entity; //A 'pointer' to the entity this component belongs to

        /**
         * Set the entity that this component belongs to
         * @warning Make sure to set entity or the entity will be entity 0 by default
         */
        void setEntity(const Entity &new_entity) {
            entity = new_entity;
        }

        friend class ECSManager;
    public:

        /**
         * Get the entity that this component belongs to
         */
        Entity getEntity() const {
            return entity;
        }

        /**
         * Get the id for this type of component
         * @return A unique identifier shared by all instances of such component.
         */
        std::string getComponentTypeId() const {
             return typeid(*this).name();
        }

    };

} // Doge
