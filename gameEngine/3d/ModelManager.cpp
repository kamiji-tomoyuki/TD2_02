#include "ModelManager.h"
#include "DirectXCommon.h"
#include "ModelCommon.h"
#include <filesystem>

ModelManager* ModelManager::instance = nullptr;

ModelManager* ModelManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ModelManager;
	}
	return instance;
}
void ModelManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void ModelManager::Initialize(DirectXCommon* dxCommon)
{
	modelCommon_ = new ModelCommon();
	modelCommon_->Initialize(dxCommon);
}

void ModelManager::LoadModel(const std::string& filePath)
{
	// --- ファイルパスからディレクトリとファイル名を抽出 ---
	std::filesystem::path path(filePath);
	std::string directory = "resources/models/" + path.parent_path().string();
	std::string fileName = path.filename().string(); // ファイル名だけを抽出

	// --- 読み込み済みモデルを検索 ---
	if (models.contains(fileName)) {
		// 読み込み済みなら早期return
		return;
	}


	// --- ディレクトリが存在しない場合は "resources/models" をデフォルトに ---
	if (directory.empty()) {
		directory = "resources/models";
	}

	// --- モデルの生成とファイル読み込み・初期化 ---
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Initialize(modelCommon_, directory, fileName);

	// --- モデルをmapコンテナに格納する ---
	models.insert(std::make_pair(fileName, std::move(model))); // 所有権を譲渡
}

void ModelManager::UploadModel()
{
    if (uploadModels_.empty()) {
        return;
    }

	uploadModels_.front()->ModelUpload();
    uploadModels_.pop();
}

Model* ModelManager::FindModel(const std::string& filePath)
{
	// --- 読み込み済みモデルを検索 ---
	if (models.contains(filePath)) {
		// 読み込みモデルを戻り値としてreturn
		return models.at(filePath).get();
	}
	// ファイル名一致無し
	return nullptr;
}