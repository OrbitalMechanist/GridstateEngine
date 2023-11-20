#pragma once

enum class ComponentType {  //Components can be searched by these types
    Position,       //related to world position
    Render,         //related to rendering static or mostly static elements
    Physics,        //related to the use of physics for animations
    Animation,      //related to calling/controlling animation playback
    Effect,         //related to rendering dynamic or short lived elements

    UnitLogic,      //related to enemy AI behaviour
    UnitMath,       //related to caculations made for gameplay purposes
    UnitValue,      //related to the values asociated with units for gameplay purposes

    //maybe some for UI and menus
};