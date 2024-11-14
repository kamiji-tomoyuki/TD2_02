#pragma once
#include <Framework.h>
#include <BaseScene.h>
#include <Sprite.h>
#include <Object3d.h>

class GamePlayScene : public BaseScene
{
public:
	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

private: // メンバ変数
	// カメラ
	Camera* camera = nullptr;

	// 2Dスプライト
	std::vector<Sprite*> sprites;
	// 3Dオブジェクト
	std::vector<Object3d*> object3ds;

	// サウンド
	SoundData soundDataSet;
	SoundData soundDataSet2;
};

