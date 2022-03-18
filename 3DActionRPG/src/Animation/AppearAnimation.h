#ifndef APPEAR_ANIMATION
#define APPEAR_ANIMATION

//シルエット状態から徐々に上から公開するアニメーションクラス

class Vector2;

class AppearAnimation
{
public:
	AppearAnimation() = default;
	AppearAnimation(float x, float y, int color_id, int shilhouette_id, int image_width, int image_height, float appear_speed);
	AppearAnimation(const Vector2& position, int color_id, int silhouette_id, const Vector2& image_size, float appear_speed);

	//公開し始める
	void appear();
	//隠す
	void reset();
	//アニメーション更新
	void update(float delta_time);
	//描画
	void draw() const;
	//アニメーションが終了したか？
	bool is_end() const;

private:
	//描画位置x(中央)
	float x_;
	//描画位置y（中央）
	float y_;
	//色つき画像ID
	int color_id_;
	//シルエット画像ID
	int shilhouette_id_;
	//アニメーション状態
	unsigned int state_;
	//画像幅
	int width_;
	//画像高さ
	int height_;
	//アニメーション速さ
	float appear_speed_{ 1.0f };
	//公開部分の高さ
	float appear_height_{ 0.0f };
};
#endif//!APPEAR_ANIMATION

