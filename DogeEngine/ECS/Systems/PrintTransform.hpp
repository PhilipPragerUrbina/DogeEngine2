//
// Created by Philip on 4/15/2023.
//

#pragma once

#include "System.hpp"
#include "../Components/TransformComponent.hpp"
#include <iostream>
#include "../../Utils/GLMPrints.hpp"
namespace Doge {

    class PrintTransform : public System<TransformComponent, HierarchyComponent>{
    public:
        void process(TransformComponent& comp, HierarchyComponent& comp2) override {
            std::cout << comp.pos << "    " << comp.getEntity().getID() << " " << comp2.child.getID() << "\n";
        }

    };

} // Doge
