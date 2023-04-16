//
// Created by Philip on 4/6/2023.
//

#pragma once

namespace Doge {

    /**
     * Simply represents a grouping of components
     * Nothing more than a unique id. Can be passed around like a pointer.
     * @details https://t-machine.org/index.php/2007/12/22/entity-systems-are-the-future-of-mmog-development-part-3/
     * This class should not be extended.
     */
    class Entity {
    private:
        unsigned int entity_id;

        /**
        * Create an entity from an ID
        * @param entity_id Supplied by ECS manager
        */
        explicit Entity(const unsigned int entity_id) : entity_id(entity_id){}

        friend class ECSManager;
    public:

        /**
        * Create an entity with id 0.
        */
        Entity() : entity_id(0){}

        /**
         * Get the entity id
         * @return A number unique to this instance.
         */
        unsigned int getID() const {return entity_id;}
    };

} // Doge
