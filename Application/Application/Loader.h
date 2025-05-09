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

		// 他のデータを追加

	};

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

private:
	/// <summary>
	/// 再帰的にオブジェクトを解析
	/// </summary>
	void ParseObjectRecursive(LevelData* levelData, const nlohmann::json& objectArray);
};