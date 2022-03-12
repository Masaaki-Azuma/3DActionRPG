#ifndef SINGLETON_H_
#define SINGLETON_H_

/*
このクラスの派生クラスは以下の実装を強制する
1.コンストラクタ・デストラクタをprivate
2.基底クラスをフレンドクラス登録
*/

//外部での生成、コピーを許可しない、継承・テンプレート前提
template<class T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}
protected:
	Singleton() = default;
	virtual ‾Singleton() = default;
private:
	Singleton(const Singleton& other) = delete;
	Singleton& operator=(const Singleton& other) = delete;
};

/*
1.コピーを完全に禁止
2.外部からの生成・解放禁止
3.インスタンスを生成せずに唯一のインスタンスを取得する関数
*/

#endif//!SINGLETON_H_