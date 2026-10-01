#include <Novice.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
const char kWindowTitle[] = "LC1B_16_ニシヤナギ_リョウマ_タイトル";

enum BulletKinds {
	ATTACK = 0,
	HEALTH = 2,

};BulletKinds bulletKinds;

struct Vector2 {
	float x;
	float y;
};

struct Player {
	Vector2 pos = { 0.0f,0.0f };
	Vector2 range = { 0.0f,0.0f };
	Vector2 halfRange = { 0.0f,0.0f };//半分の大きさ
	Vector2 speed = { 0.0f,0.0f, };
	Vector2 acceleration = { 0.0f,0.0f };
	bool isReflection = false;//反射
};Player player;

struct Enemy {
	Vector2 pos = { 0.0f,0.0f };
	Vector2 range = { 0.0f,0.0f };
	Vector2 halfRange = { 0.0f,0.0f };//半分の大きさ
	Vector2 speed = { 0.0f,0.0f, };
	Vector2 acceleration = { 0.0f,0.0f };
	bool isReflection = false;//反射する
};Enemy enemy;

struct Bullet {
	Vector2 pos = { 0.0f,0.0f };
	Vector2 range = { 0.0f,0.0f };
	Vector2 halfRange = { 0.0f,0.0f };
	float magnification = { 0.0f };
	Vector2 speed = { 0.0f,0.0f, };
	float rotate = { 0.0f };
	Vector2 acceleration = { 0.0f,0.0f };

};Bullet bullet;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1920, 1080);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	int testBulletGraphHandle = Novice::LoadTexture("./TD1game/images/bullet.png");

	float theta = 0;
	float moveX = sinf(theta);
	float moveY = cosf(theta);

	Vector2 kScreenMin{ 0.0f,0.0f };
	Vector2 kScreenMax{ 1920.0f,1080.0f };

	Vector2 kQuadMinPosition{ 0.0f,0.0f };
	bullet.pos = { 960.0f,540.0f };
	bullet.range = { 64.0f,64.0f };
	bullet.halfRange = { 32.0f,32.0f };
	bullet.magnification = 0.05f;
	bullet.speed = { 10.0f,10.0f };
	bullet.acceleration = { 0.01f,0.01f };

	Vector2 bulletLeftTop = { bullet.pos.x , bullet.pos.y };
	Vector2 bulletRightTop = { bullet.pos.x + bullet.range.x , bullet.pos.y };
	Vector2 bulletLeftUnder = { bullet.pos.x , bullet.pos.y + bullet.range.y };
	Vector2 bulletRightUnder = { bullet.pos.x + bullet.range.x , bullet.pos.y + bullet.range.y };

	Vector2 bulletToEntityDistance{ (enemy.pos.x - player.pos.x) ,(enemy.pos.y - player.pos.y) };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		bullet.pos.x += bullet.speed.x;
		bullet.pos.y += bullet.speed.y;

		bulletLeftTop = { bullet.pos.x , bullet.pos.y };
		bulletRightTop = { bullet.pos.x + bullet.range.x , bullet.pos.y };
		bulletLeftUnder = { bullet.pos.x , bullet.pos.y + bullet.range.y };
		bulletRightUnder = { bullet.pos.x + bullet.range.x , bullet.pos.y + bullet.range.y };
		
		bulletToEntityDistance.x = (enemy.pos.x - player.pos.x);
		bulletToEntityDistance.y = (enemy.pos.y - player.pos.y);
		bullet.rotate = atan2f(bulletToEntityDistance.y, bulletToEntityDistance.x);
		theta = (bullet.rotate) * static_cast<float>(M_PI);
		moveX = sinf(theta);
		moveY = cosf(theta);		
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			player.isReflection = true;
			if (player.isReflection) {
				
				bulletToEntityDistance.x = (enemy.pos.x - player.pos.x);
				bulletToEntityDistance.y = (enemy.pos.y - player.pos.y);
				bullet.rotate = atan2f(bulletToEntityDistance.y, bulletToEntityDistance.x);
				
				theta = (bullet.rotate) * static_cast<float>(M_PI);
				moveX = sinf(theta);
				moveY = cosf(theta);

				bullet.speed.x = (bullet.speed.x +bullet.speed.x * bullet.magnification) * (-1);
				bullet.halfRange.x = (bullet.halfRange.x + bullet.halfRange.x * bullet.magnification);
				bullet.halfRange.y = (bullet.halfRange.y + bullet.halfRange.y * bullet.magnification);

				player.isReflection = false;
			}
			
		}

		if (bulletLeftTop.x < kScreenMin.x ||
			bulletRightTop.x > kScreenMax.x ||
			bulletLeftUnder.x < kScreenMin.x ||
			bulletRightUnder.x > kScreenMax.x) {
			bullet.speed.x *= -1;
		}
		if (bulletLeftTop.y < kScreenMin.y ||
			bulletRightTop.y > kScreenMax.y ||
			bulletLeftUnder.y < kScreenMin.y ||
			bulletRightUnder.y > kScreenMax.y) {
			bullet.speed.y *= -1;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		Novice::DrawQuad(static_cast<int>(bulletLeftTop.x), static_cast<int>(bulletLeftTop.y),
			static_cast<int>(bulletRightTop.x), static_cast<int>(bulletRightTop.y),
			static_cast<int>(bulletLeftUnder.x), static_cast<int>(bulletLeftUnder.y),
			static_cast<int>(bulletRightUnder.x), static_cast<int>(bulletRightUnder.y),
			static_cast<int>(kQuadMinPosition.x - bullet.halfRange.x), static_cast<int>(kQuadMinPosition.y - bullet.halfRange.y),
			static_cast<int>(bullet.range.x + bullet.halfRange.x), static_cast<int>(bullet.range.y + bullet.halfRange.y),
			testBulletGraphHandle,0xffffffff);
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
