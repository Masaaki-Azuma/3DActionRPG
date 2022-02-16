#ifndef PLAYER_DATABASE_H_
#define PLAYER_DATABASE_H_

#include "Singleton.h"

#include <string>

/*扱いの説明

ゲームで実際に用いるデータはmaster_parameterを疑似的なセーブデータとして扱う
*/

//プレイヤーに関するセーブデータや進行データの管理クラス
class PlayerDatabase : public Singleton<PlayerDatabase>
{
public:
	struct Parameter
	{
		int hp;
		int attack;
		int total_gem;
	};
public:
	void set_dst_file(const std::string& file_name);
	void load_master_data();
	void save_master_data();
	//1ゲーム開始時に呼ばれるパラメータ読み込み関数
	void set_initial_parameter();

	//1ゲームプレイ中のパラメーター増減
	void add_hp(int rise_value);
	void add_attack(int rise_value);
	void add_gem(int rise_value);
	void set_hp(int hp);
	//1ゲームプレイ中のパラメータ取得
	const Parameter& get_current_parameter();
	//セーブデータのパラメータ取得
	const Parameter& get_master_parameter();

	//ジェム増加をセーブ
	void add_possessed_jem(int rise_gem);
	//HPを増加させセーブ
	void enhance_hp(int rise_hp);
	//攻撃力を増加させセーブ
	void enhance_attack(int rize_attack);


private:
	//1プレイ中の残りライフ等パラメーターを保持する
	Parameter current_parameter_{ -1, -1, -1 };
	//セーブデータに残るパラメーターを保持する
	Parameter master_parameter_{ -1, -1, -1 };
	//データ元ファイル
	std::string master_file_name_{ "" };

private:
	PlayerDatabase() = default;
	‾PlayerDatabase() = default;
private:
	friend class Singleton<PlayerDatabase>;

};
#endif//!PLAYER_DATABASE_H_

