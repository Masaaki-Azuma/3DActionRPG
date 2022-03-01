#include "MapScene.h"

#include "Util/MyRandom.h"
#include "Util/Input.h"
#include "Util/AppearAnimation.h"
#include "AssetsManager/Image.h"
#include "AssetsManager/PlayerDatabase.h"
#include "MapManager.h"

MapScene::MapScene():
    map_{MapManager::GetInstance()},
    p_DB_{PlayerDatabase::GetInstance()},
    hp_gauge_{Texture_GaugeFrame, Texture_GaugeBarGreen, 150, 100, 540, 40}
{
}

void MapScene::start(void* data)
{
    Image::load("MapScene");
    is_end_ = false;

    //HPゲージ
    hp_gauge_ = ExtendableBarGauge{ 150, 100, 540, 40 , Texture_GaugeFrame, Texture_GaugeBarGreen, Texture_GaugeBarGray };
    hp_gauge_.extend(p_DB_.get_master_parameter().hp, p_DB_.limit_hp());
    hp_gauge_.set_edge_width(10);

    //バトルシーンから帰ってきたときの更新処理
    map_.enter_map();

}

void MapScene::update(float delta_time)
{
    //最後のエリアから戻ってきたときはシーン終了
    if (map_.is_final_area()) {
        is_end_ = true;
    }

    //マップの更新
    map_.update(delta_time);
    //遭遇テキスト更新
    encount_text_.update(delta_time);

    //エリアを選択したらエリア公開
    if (map_.is_picked() && encount_text_.is_wait()) {
        encount_text_ = SlideInAnimation{ Texture_GaugeBarGreen, 100, 100, 10.0f, 300.0f };
        encount_text_.start();
    }
    //演出終了後、決定キーで進む
    if (Input::get_button_down(PAD_INPUT_1) && encount_text_.is_end()) {
        is_end_ = true;
        return;
    }
}

void MapScene::draw() const
{
    //マップ描画
    map_.draw();
    //HPバー描画
    hp_gauge_.draw_gui(static_cast<float>(p_DB_.get_current_parameter().hp));
    //遭遇モンスター名表示
    encount_text_.draw();
}

bool MapScene::is_end() const
{
    return is_end_;
}

std::string MapScene::next() const
{
    if (map_.is_final_area()) return "GameResultScene";
    else                      return "BattleScene";
}

void MapScene::end()
{
    Image::clear();
}

void* MapScene::data()
{
    //次に出現する敵の名前を送信
    return &map_.selected_enemy();
}

void MapScene::restart_area()
{
    if (Input::get_button(PAD_INPUT_4)) { //A
        is_end_ = true;
    }
}
