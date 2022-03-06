#ifndef SKINNING_MESH_H_
#define SKINNING_MESH_H_

#include <string>
#include <DxLib.h>

#include "Util/Vector3.h"

class Vector3;

class SkinningMesh
{
public:
    SkinningMesh() = default;
    SkinningMesh(int model_handle, float supposed_fps);
    ‾SkinningMesh();

    //メッシュの登録
    void store_model_handle(int model_handle);
    //保持しているメッシュハンドルを破棄する
    void clear();
    //更新
    void update(float delta_time);
    //描画
    void draw() const;
    //メッシュの位置を設定
    void set_position(const VECTOR& position);
    //メッシュの位置を設定
    void set_position(const Vector3& position);
    //モデルの向き（弧度法）を設定
    void set_rotation(const VECTOR& rotation);
    //モデルの向き（弧度法）を設定
    void set_rotation(const Vector3& rotation);

    void set_scale(const VECTOR& scale);
    //モデルの大きさを設定
    void set_scale(const Vector3& scale);
    //アニメーションを変更
    void change_anim(int anim_index, bool loop = true, bool reset = false);
    //現在のアニメーションの総再生時間を取得
    float anim_total_sec() const;


private:
    int model_handle_{ -1 };      //メッシュのハンドル
    float supposed_fps_{ 60.0f };  //アニメーションの想定fps
    float anim_total_frame_{ -1 }; //アニメーションの総フレーム数
    float anim_now_frame_{ -1 };   //アニメーションの現在フレーム数
    int anim_attach_index_{ -1 }; //アニメーションアタッチハンドル
    int motion_index_{ -1 };      //アニメーション番号
    bool motion_loop_{ false };

    Vector3 position_{ 0.0f, 0.0f, 0.0f };
    Vector3 rotation_{ 0.0f,0.0f,0.0f };
    Vector3 scale_{ 1.0f, 1.0f, 1.0f };
};
#endif//!SKINNING_MESH_H_

