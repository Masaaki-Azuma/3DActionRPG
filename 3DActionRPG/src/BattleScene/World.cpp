#include "World.h"
#include "Field.h"
#include "Actor/Actor.h"

// デストラクタ
World::‾World()
{
    clear();
}

// 更新
void World::update(float delta_time)
{
    // アクターの更新
    actors_.update(delta_time);
    // アクターの衝突
    actors_.collide();
    // アクターの遅延更新
    actors_.late_update(delta_time);
    // アクターの消去
    actors_.remove();
    // カメラの更新
    camera_->update(delta_time);
}

// 描画
void World::draw() const
{
    // カメラの描画
    camera_->draw();
    // フィールドの描画
    field_->draw();
    // アクターの描画
    actors_.draw();
    // 半透明アクターの描画
    actors_.draw_transparent();
    // GUIの描画
    actors_.draw_gui();
}

// 消去
void World::clear()
{
    // アクターを消去
    actors_.clear();
    // カメラを消去
    camera_ = nullptr;
    // ライトを消去
    light_ = nullptr;
    // フィールドを消去
    field_ = nullptr;
    //敵討伐数リスト
    basterd_list_.clear();
}

// カメラの追加
void World::add_camera(ActorPtr camera)
{
    camera_ = camera;
}

// ライトの追加
void World::add_light(ActorPtr light)
{
    light_ = light;
}

//// フィールドの追加
void World::add_field(std::shared_ptr<Field> field)
{
    field_ = field;
}

std::unordered_map<std::string, int>& World::basterd_list()
{
    return basterd_list_;
}

bool World::is_battle_win() const
{
    return count_actor_with_tag("EnemyTag") == 0;
}

bool World::is_battle_lose() const
{
    return find_actor("Player") == nullptr;
}

bool World::is_battle_end() const
{
    return is_battle_win() || is_battle_lose();
}

// アクターの追加
void World::add_actor(ActorPtr actor)
{
    actors_.add(actor);
}

// アクターの検索
ActorPtr World::find_actor(const std::string& name) const
{
    return actors_.find(name);
}

// 指定したタグ名を持つアクターの検索
std::vector<ActorPtr> World::find_actor_with_tag(const std::string& tag) const
{
    return actors_.find_with_tag(tag);
}

// アクター数を返す
int World::count_actor() const
{
    return actors_.count();
}

// 指定したタグ名を持つアクター数を返す
int World::count_actor_with_tag(const std::string& tag) const
{
    return actors_.count_with_tag(tag);
}

// メッセージ送信
void World::send_message(const std::string& message, void* param)
{
    actors_.send_message(message, param);
}

void World::add_basterd(const std::string& enemy_name)
{
    if (basterd_list_.count(enemy_name) == 0) basterd_list_[enemy_name] = 1;
    else                                      basterd_list_[enemy_name]++;
}

void World::collide_field(const Sphere& sphere, Vector3& position)
{
    field_->collide_sphere(sphere, position);
}

// カメラの取得
ActorPtr World::camera()
{
    return camera_;
}

// ライトの取得
ActorPtr World::light()
{
    return light_;
}

// フィールドの取得
//Field* World::field()
//{
//    return field_;
//}

