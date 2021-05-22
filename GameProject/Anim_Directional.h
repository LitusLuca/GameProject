#pragma once
#include "Anim_Base.h"
class Anim_Directional :
    public Anim_Base
{
public:
    Anim_Directional();
protected:
    void frameStep();
    void cropSprite();
    void readIn(std::stringstream& l_stream);
};

