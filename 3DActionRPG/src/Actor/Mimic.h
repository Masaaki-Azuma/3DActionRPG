#ifndef MIMIC_H_
#define MIMIC_H_

#include "Actor/Enemy.h"
class Mimic :
    public Enemy
{
    // Enemy を介して継承されました
    virtual void update_state(float delta_time) override;
};
#endif//!MIMIC_H_

