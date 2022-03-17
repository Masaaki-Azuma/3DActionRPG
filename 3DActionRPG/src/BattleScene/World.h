#ifndef WORLD_H_
#define WORLD_H_

#include "IWorld.h"

#include <unordered_map>

#include "ActorManager.h"

class Field;

// ワールドクラス
class World : public IWorld
{
public:
    // コンストラクタ
    World() = default;
    // デストラクタ
    ‾World();
    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;
    // 消去
    void clear();
    // カメラの追加
    void add_camera(ActorPtr camera);
    // ライトの追加
    void add_light(ActorPtr light);
    // フィールドの追加
    void add_field(std::shared_ptr<Field> field);
    //敵討伐数を取得
    std::unordered_map<std::string, int>& basterd_list();
    bool is_battle_win() const;
    bool is_battle_lose() const;
    virtual bool is_battle_end() const;

    // アクターを追加
    virtual void add_actor(ActorPtr actor) override;
    // アクターの検索
    virtual ActorPtr find_actor(const std::string& name) const override;
    // 指定したタグ名を持つアクターの検索
    virtual std::vector<ActorPtr> find_actor_with_tag(const std::string& tag) const override;
    // アクター数を返す
    virtual int count_actor() const override;
    // 指定したタグ名を持つアクター数を返す
    virtual int count_actor_with_tag(const std::string& tag) const override;
    // メッセージの送信
    virtual void send_message(const std::string& message, void* param = nullptr) override;
    //討伐数の加算
    virtual void add_basterd(const std::string& enemy_name) override;
    //フィールドとの押し出し判定
    virtual void collide_field(const Sphere& sphere, Vector3& position) override;

    // カメラの取得
    virtual ActorPtr camera() override;
    // ライトの取得
    virtual ActorPtr light() override;
    // フィールドの取得
    //virtual Field* field() override;

    // コピー禁止
    World(const World& other) = delete;
    World& operator = (const World& other) = delete;

private:
    // アクターマネージャー
    ActorManager  actors_;
    // ライト
    ActorPtr light_{ nullptr };
    // カメラ
    ActorPtr camera_{ nullptr };
    // フィールド
    std::shared_ptr<Field> field_{ nullptr };
    //討伐敵リスト
    std::unordered_map<std::string, int> basterd_list_;

};

#endif