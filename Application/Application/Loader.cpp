#include "Loader.h"

// C++
#include <cassert>

void Loader::Initialize() {
	// 連結してフルパスを得る
	const std::string fullPath = "tools/untitled.json";

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullPath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// オブジェクト配列を解析
	if (deserialized.contains("objects")) {
		ParseObjectRecursive(levelData, deserialized["objects"]);
	}
}

void Loader::ParseObjectRecursive(LevelData* levelData, const nlohmann::json& objectArray) {
	// "objects"の全オブジェクトを走査
	for (const nlohmann::json& object : objectArray) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// 種類ごとの処理
#pragma region MESH
		if (type.compare("MESH") == 0) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			// カスタムプロパティのfile_nameを追加している場合
			if (object.contains("file_name")) {
				// ファイル名
				objectData.fileName = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			const nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];
			// 回転角
			objectData.rotation.x = (float)transform["rotation"][0];
			objectData.rotation.y = (float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][1];
			// スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];

			// TODO : コライダーのパラメータ読み込み

		}

#pragma endregion

		// 再帰的に子オブジェクトを解析
		if (object.contains("children")) {
			ParseObjectRecursive(levelData, object["children"]);
		}
	}
}
