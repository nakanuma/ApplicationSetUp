#pragma once

// C++
#include <fstream>
#include <sstream>
#include <string>

// Engine
#include <MyMath.h>

// Externals
#include <externals/nlohmann/json.hpp>

/// <summary>
/// JSONファイルの読み込み
/// </summary>
class Loader {
public:
	struct LevelData {
		// オブジェクトデータ
		struct ObjectData {
			std::string fileName;
			Float3 translation;
			Float3 rotation;
			Float3 scaling;
		};
		std::vector<ObjectData> objects; // オブジェクトのリスト
	};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// レベルデータの取得
	/// </summary>
	LevelData* GetLevelData() const { return levelData_; }

private:
	/// <summary>
	/// 再帰的にオブジェクトを解析
	/// </summary>
	void ParseObjectRecursive(LevelData* levelData, const nlohmann::json& objectArray);

private:
	LevelData* levelData_;
};