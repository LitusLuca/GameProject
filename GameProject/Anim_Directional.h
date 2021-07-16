#pragma once
#include "Anim_Base.h"
#include "Direction.h"
class Anim_Directional :
    public Anim_Base
{
protected:
    void frameStep();
    void cropSprite();
    void readIn(std::stringstream& l_stream);
};

