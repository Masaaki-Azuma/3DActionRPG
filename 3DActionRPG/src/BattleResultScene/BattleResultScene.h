#ifndef BATTLE_RESULT_SCENE_H_
#define BATTLE_RESULT_SCENE_H_

#include "IScene.h"

#include <string>
#include <unordered_map>

class BattleResultScene :
    public IScene
{
public:
    struct BattleResultData
    {
        std::string battle_result;
        std::unordered_map<std::string, int> basterd_list;
    };
public:
    virtual void start(void* data = nullptr) override;
    virtual void update(float delta_time) override;
    virtual void draw() const override;
    virtual bool is_end() const override;
    virtual std::string next() const override;
    virtual void end() override;
    virtual void* data() override;

private:
    bool is_end_{ true };
    BattleResultData result_;
};
#endif//!BATTLE_RESULT_SCENE_H_

