//
// Created by Philip on 4/9/2023.
//

#pragma once

#include "Component.hpp"
#include <vector>
#include <optional>

namespace Doge {

    /**
     * Represents how entities relate to each other
     */
    class HierarchyComponent : public Component{
    private:
        std::vector<Entity> children;
        std::optional<Entity> parent;
    public:

        HierarchyComponent() {
            parent = std::nullopt;
        };
        explicit HierarchyComponent(Entity parent) : parent(parent){};

        std::vector<Entity> getChildren() const {
            return children;
        }

        void addChild(Entity child) {
            children.push_back(child);
        }

        /**
         * Get the parent
         * @return Parent unless node is top level(has no parent)
         */
        std::optional<Entity> getParent() const {
            return parent;
        }

        void setParent(const Entity &new_parent) {
            parent = new_parent;
        }


    };

} // Doge
