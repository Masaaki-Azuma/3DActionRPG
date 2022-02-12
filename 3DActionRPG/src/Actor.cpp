#include "Actor.h"
#include "DxConverter.h"
#include "Mesh.h"
#include "Matrix4x4.h"

// 更新
void Actor::update(float delta_time) {}

// 遅延更新
void Actor::late_update(float delta_time) {}

// 描画
void Actor::draw() const {}

// 半透明オブジェクトの描画
void Actor::draw_transparent() const {}

// GUIの描画
void Actor::draw_gui() const {}

// 衝突リアクション
void Actor::react(Actor&) {}

// メッセージ処理
void Actor::handle_message(const std::string& message, void* param) {}

// 衝突判定
void Actor::collide(Actor& other)
{
    // どちらのアクターも衝突判定が有効か？
    if (enable_collider_ && other.enable_collider_) {
        // 衝突判定をする
        if (is_collide(other)) {
            // 衝突した場合は、お互いに衝突リアクションをする
            react(other);
            other.react(*this);
        }
    }
}

// 死亡する
void Actor::die()
{
    dead_ = true;
}

// 衝突しているか？
bool Actor::is_collide(const Actor& other) const
{
    return collider().is_collide(other.collider());
    //return collider().intersects(other.collider());
}

// 死亡しているか？
bool Actor::is_dead() const
{
    return dead_;
}

// 名前を取得
const std::string& Actor::name() const
{
    return name_;
}

// タグ名を取得
const std::string& Actor::tag() const
{
    return tag_;
}

Vector3 Actor::position() const
{
    return position_;
}

// 移動量を取得
Vector3 Actor::velocity() const
{
    return velocity_;
}

Vector3 Actor::rotation() const
{
    return rotation_;
}

// 衝突判定データを取得
Sphere Actor::collider() const
{
    return Sphere{ collider_.radius, collider_.center + position_ };
}

//前方向（正規化）を取得。xz平面に平行なベクトルであることに注意
Vector3 Actor::forward() const
{
    Vector3 forward = Vector3{ 0.0f, 0.0f, 1.0f } * Matrix4x4::rotateY(rotation_.y);
    return forward.Normalize();
}

void Actor::react_wall()
{
    //ポリゴンヒット情報をリセット
    delete_poly_hit_info();
    //壁から押し戻し
    poly_hit_info_ = DxLib::MV1CollCheck_Sphere(Mesh::stage_handle, -1, DxConverter::GetVECTOR(collider().center), collider().radius);
    if (poly_hit_info_.HitNum >= 1) {
        //当たっているポリゴンの数だけ繰り返す
        for (int i = 0; i < poly_hit_info_.HitNum; ++i) {
            //交点
            Vector3 intersect = DxConverter::GetVector3(poly_hit_info_.Dim[i].HitPosition);
            //ヒットポリゴンの法線
            Vector3 normal = DxConverter::GetVector3(poly_hit_info_.Dim[i].Normal);
            //押し戻し距離
            float distance = collider().radius - (intersect - collider().center).Length();
            position_ += normal * distance;
        }
    }
}

void Actor::delete_poly_hit_info()
{
    //ポリゴンヒット情報を削除
    DxLib::MV1CollResultPolyDimTerminate(poly_hit_info_);
}

