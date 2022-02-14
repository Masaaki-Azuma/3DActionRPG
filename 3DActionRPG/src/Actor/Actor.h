#ifndef ACTOR_H_
#define ACTOR_H_

#include <DxLib.h>
#include <string>
#include "Sphere.h"
#include "Util/Vector3.h"

class IWorld;   // ワールド抽象インターフェースの前方宣言

// アクタークラス
class Actor
{
public:
    // コンストラクタ
    Actor() = default;
    // 仮想デストラクタ
    virtual ‾Actor() = default;
    // 更新
    virtual void update(float delta_time);
    // 遅延更新
    virtual void late_update(float delta_time);
    // 描画
    virtual void draw() const;
    // 半透明の描画
    virtual void draw_transparent() const;
    // GUIの描画
    virtual void draw_gui() const;
    // 衝突リアクション
    virtual void react(Actor& other);
    // メッセージ処理
    virtual void handle_message(const std::string& message, void* param);

    // 衝突判定
    void collide(Actor& other);
    // 死亡する
    void die();
    // 衝突しているか？
    bool is_collide(const Actor& other) const;
    // 死亡しているか？
    bool is_dead() const;
    // 名前を取得
    const std::string& name() const;
    // タグ名を取得
    const std::string& tag() const;
    //位置を取得
    Vector3 position() const;
    // 移動量を取得
    Vector3 velocity() const;
    //回転角度を取得
    Vector3 rotation() const;
    // 現在の位置の応じた衝突判定データを取得
    Sphere collider() const;
    //前方向を取得
    Vector3 forward() const;
protected:
    //壁との押し出し処理
    void react_wall();
    //ポリゴンヒット情報を削除
    void delete_poly_hit_info();

    // コピー禁止
    Actor(const Actor& other) = delete;
    Actor& operator = (const Actor& other) = delete;

protected:
    // ワールド
    IWorld* world_{ nullptr };
    // 名前
    std::string     name_;
    // タグ名
    std::string     tag_;
    //位置
    Vector3       position_{ 0.0f, 0.0f, 0.0f };
    // 移動量
    Vector3       velocity_{ 0.0f, 0.0f, 0.0f };
    //回転角度（度数法）
    Vector3       rotation_{ 0.0f, 0.0f, 0.0f };
    // 衝突判定が有効か？
    bool            enable_collider_{ true };
    // 衝突判定
    Sphere collider_;
    //ポリゴンヒット情報
    DxLib::MV1_COLL_RESULT_POLY_DIM poly_hit_info_;
    // 死亡フラグ
    bool            dead_{ false };
};

#endif