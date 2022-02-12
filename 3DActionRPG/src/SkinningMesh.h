#ifndef SKINNING_MESH_H_
#define SKINNING_MESH_H_

#include <string>
#include <DxLib.h>

class Vector3;

class SkinningMesh
{
public:
    SkinningMesh() = default;
    SkinningMesh(int model_handle, float supposed_fps);
    ‾SkinningMesh();

    //メッシュの登録
    void store_model_handle(int model_handle);
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
    //アニメーションを変更
    void change_anim(int anim_index, bool loop = true, bool reset = false);
    //現在のアニメーションの総再生時間を取得
    float anim_total_sec();


private:
    int model_handle_{ -1 };      //メッシュのハンドル
    float supposed_fps_{ 60.0f };  //アニメーションの想定fps
    float anim_total_frame_{ -1 }; //アニメーションの総フレーム数
    float anim_now_frame_{ -1 };   //アニメーションの現在フレーム数
    int anim_attach_index_{ -1 }; //アニメーションアタッチハンドル
    int motion_index_{ -1 };      //アニメーション番号
    bool motion_loop_{ false };
};
#endif//!SKINNING_MESH_H_

