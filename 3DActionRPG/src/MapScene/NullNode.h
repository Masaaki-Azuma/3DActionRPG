#ifndef NULL_NODE_H_
#define NULL_NODE_H_

#include "AreaNode.h"

class NullNode :
    public AreaNode
{
public:
    NullNode():
        AreaNode{ Vector3{0,0,0}, ""}
    {
    }
    virtual void update(float delta_time) override{}
    virtual void draw() const override {}
    virtual void appear() override {}
    virtual bool is_appeared() const override { return false; }
    virtual void add_next(PtrNode next) override {};
};
#endif//!NULL_NODE_H_

