#ifndef PARAMETER_SCENE_H_
#define PARAMETER_SCENE_H_

#include "IScene.h"
#include "AssetsManager/PlayerDatabase.h"

class ParameterScene :
    public IScene
{
public:
    virtual void start(void* data = nullptr) override;
    virtual void update(float delta_time) override;
    virtual void draw() const override;
    virtual bool is_end() const override;
    virtual std::string next() const override;
    virtual void end() override;
    virtual void* data() override;
private:
    //強化するパラメーターを選択
    void select_enhanced_parameter();
private:
    bool is_end_{ false };
    int selected_parameter_index{ 0 };
    int selected_icon_index{ 0 };
    PlayerDatabase& p_DB_{ PlayerDatabase::GetInstance() };

};
#endif//!PARAMETER_SCENE_H_

