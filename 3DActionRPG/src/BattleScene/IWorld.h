#ifndef IWORLD_H_
#define IWORLD_H_

#include <string>
#include <vector>
#include <memory>

class Actor;
class Field;
class Sphere;
class Vector3;

// ワールド抽象インターフェース
class IWorld
{
public:
    // 仮想デストラクタ
    virtual ‾IWorld() = default;

    // アクターを追加
    virtual void add_actor(std::shared_ptr<Actor> actor) = 0;
    // アクターの検索
    virtual std::shared_ptr<Actor> find_actor(const std::string& name) const = 0;
    // 指定したタグ名を持つアクターの検索
    virtual std::vector<std::shared_ptr<Actor>> find_actor_with_tag(const std::string& tag) const = 0;
    // アクター数を返す
    virtual int count_actor() const = 0;
    // 指定したタグ名を持つアクター数を返す
    virtual int count_actor_with_tag(const std::string& tag) const = 0;
    // メッセージの送信
    virtual void send_message(const std::string& message, void* param = nullptr) = 0;
    //討伐数の加算
    virtual void add_basterd(const std::string& enemy_name) = 0;
    //フィールドとの押し出し判定
    virtual void collide_field(const Sphere& sphere, Vector3& position) = 0;
    //バトルシーンが終了したか？
    virtual bool is_battle_end() const = 0;
    // フィールドの取得
    //virtual Field* field() = 0;
    // カメラの取得
    virtual std::shared_ptr<Actor> camera() = 0;
    // ライトの取得
    virtual std::shared_ptr<Actor> light() = 0;
};

#endif

