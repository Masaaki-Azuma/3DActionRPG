#ifndef PARAMETER_SCENE_H_
#define PARAMETER_SCENE_H_

#include "IScene.h"
#include "AssetsManager/PlayerDatabase.h"
#include "Actor/ExtendableBarGauge.h"

class PadInput;

class ParameterScene :
    public IScene
{
private:
    //メニュー選択状態
    enum class State
    {
        SelectMenu = 0,
        CheckParameter,
        EnhanceParamter,
    };

public:
    ParameterScene();
    virtual void start(void* data = nullptr) override;
    virtual void update(float delta_time) override;
    virtual void draw() const override;
    virtual bool is_end() const override;
    virtual std::string next() const override;
    virtual void end() override;
    virtual void* data() override;
private:
    //パラメータ閲覧
    void check_parameter();
    //メニュー選択
    void select_menu();
    //強化するパラメーターを選択
    void select_enhanced_parameter();
    //体力の強化を試みる
    void try_enhance_hp();
    //攻撃力のの強化を試みる
    void try_enhance_attack();
    //画面左のメニューを描画
    void draw_menu() const;
    //画面右の詳細を描画
    void draw_detail_parameter()const;
    //文章を改行する
    std::string restruct_string(const std::string& str) const;

private:
    bool is_end_{ false };
    int selected_parameter_index{ 0 };
    int selected_menu_index{ 0 };
    State menu_state{ State::SelectMenu };
    ExtendableBarGauge hp_gauge_;
    ExtendableBarGauge attack_gauge_;

    PlayerDatabase& p_DB_{ PlayerDatabase::GetInstance() };
    PadInput& input_;

};
#endif//!PARAMETER_SCENE_H_

