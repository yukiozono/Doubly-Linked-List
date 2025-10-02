#pragma once
#include <string>
#include <fstream>

struct ScoreData {
	int            score = 0;
	std::string    userName = {};
};

/**
 * @class DoublyLinkedList
 * @brief 指定したファイルを取り込める双方向リスト
 *
 * @note 入力と出力はstd::stringのみ
 * txtファイルを指定し、読み込むことで、
 * 再度同順に出力できます
 */
class DoublyLinkedList {
private:
	struct Node {
		Node* prevNode = nullptr;    //一つ前のノードのポインタ
		Node* nextNode = nullptr;    //一つ後のノードのポインタ
		ScoreData data = {};
	};

	//ノードへのポインタ
	Node* head = nullptr;  //先頭アドレス
	Node* tail = nullptr;  //末尾アドレス
	size_t listSize = 0;   //現在のリストのサイズ

	

public:

	class iterator {
		//DoublyLinkedListからアクセスするための宣言
		friend class DoublyLinkedList;

	protected:
		Node* node = nullptr;

		//コンストラクタ(initの値で初期化)
		explicit iterator(Node* init) : node(init) {}

	public:
		//初期値はdefaultのnullptrにお任せ
		iterator() = default;

		//ScoreDataにアクセスするための間接参照
		ScoreData& operator*() { return node->data; }

	};

	class const_iterator : public iterator {
		//DoublyLinkedListからアクセスするための宣言
		friend class DoublyLinkedList;

		//コンストラクタ(initの値で初期化)
		explicit const_iterator(Node* init) : iterator(init) {}

	public:
		//デフォルトコンストラクタ(iterator()呼出時、nodeをnullptrに)
		const_iterator() : iterator() {}

		//前置デクリメント用(it--)(リストの先頭に向かって一つ進める[operator--]())
		const_iterator& operator--() { if (this->node) this->node = this->node->prevNode; return *this; }

		//後置デクリメント用(--it)(リストの先頭に向かって一つ進める[operator--]())
		const_iterator  operator--(int) { const_iterator t = *this; --(*this); return t; }

		//前置インクリメント用(it++)(リストの末尾に向かって一つ進める[operator++]())
		const_iterator& operator++() { if (this->node) this->node = this->node->nextNode; return *this; }

		//後置インクリメント用(++it)(リストの末尾に向かって一つ進める[operator++]())
		const_iterator  operator++(int) { const_iterator t = *this; ++(*this); return t; }

		//間接参照(戻り値 const ScoreData&)（イテレータの指す要素を取得する[operator* const版]())
		const ScoreData& operator*() const { return this->node->data; }

		//コピーコンストラクタ自動生成(iteratorの位置ポインタをコピー)(コピーを行う[コピーコンストラクタ]())
		const_iterator(const const_iterator&) = default;

		//コピーコンストラクタ自動生成(iteratorの位置ポインタを上書き)(代入を行う[operator=]())
		const_iterator& operator=(const const_iterator&) = default;
		
		//等値比較(==であればtrueを返す)(同一か比較する[operator==]())
		bool operator==(const const_iterator& r) const { return this->node == r.node; }

		//非等値比較(!=であればtrueを返す)(異なるかか比較する[operator!=]()
		bool operator!=(const const_iterator& r) const { return this->node != r.node; }

		//iteratorからconst_iteratorへ変換
		const_iterator(const iterator& it) : iterator(it.node) {}
	};

public:
	//データ数の取得
	size_t size() const { return listSize; }

	/**
	 * @brief          ノード追加
	 * @param previous 挿入先のノードの位置
	 * @param datas    追加するデータ
	 * @return         追加したノードの位置
	 */
	iterator addNode(const const_iterator& nodePos, const ScoreData& datas) {
		//新規ノードにデータを代入
		Node* current = new Node{ nullptr, nullptr, datas };

		//もしリストが空であった場合
		if (listSize == 0) {
			//先頭と末尾アドレスにcurrentを代入
			head = tail = current;
		}
		//もしpreviousが末尾のノードだった場合		
		else if (nodePos.node == nullptr) {
			//currentを追加し、末尾アドレスにcurrentを代入
			current->prevNode = tail;
			tail->nextNode = current;
			tail = current;
		}
		
		//もし挿入先が先頭であった場合
		else if (nodePos.node == head) {
			//currentを先頭にし、headの位置を交代
			current->nextNode = head;
			head->prevNode = current;
			head = current;
		}
		//もしどちらでもない途中からの挿入だった場合
		else {
			//previousのノードとcurrentの前後のアドレスを再編成
			Node* prev = nodePos.node->prevNode;
			current->prevNode = prev;
			current->nextNode = nodePos.node;
			prev->nextNode = current;

			//次のノードの前要素アドレスにcurrentを代入
			nodePos.node->prevNode = current;
		}

		//リストサイズを管理する変数を+1
		++listSize;

		return iterator(current);
	}


	/**
	 * @brief          ノード削除
	 * @param previous 削除するノードの位置
	 * @return         次のノードの位置
	 */
	iterator deleteNode(const iterator& nodePos) {
		Node* current = nodePos.node;

		//見つからなかった場合は、nullptrをreturn
		if (!current) return iterator(nullptr);

		Node* next = current->nextNode;

		//ノードの前後のポインタを再編成
		if (current->prevNode) {
			current->prevNode->nextNode = current->nextNode;
		}
		else {
			head = current->nextNode;
		}

		if (current->nextNode) {
			current->nextNode->prevNode = current->prevNode;
		}
		else {
			tail = current->prevNode;
		}

		//currentを削除し、リストサイズも減らす
		delete current;
		--listSize;
		return iterator(next);
	}

	/**
	 * @brief リスト内の要素先頭から全消去
	 */
	void clear() {
		Node* current = head;
		//currentがnullptrになるまでループし、ノードを削除
		while (current != nullptr) {
			Node* next = current->nextNode;
			delete current;
			current = next;
		}
		head = nullptr;
		tail = nullptr;
		listSize = 0;
	}

	/**
	 * @brief ファイル読み込み
	 * @param ファイルパス
	 */
	void inputData(std::string filePath) {
		//まずはリストの内容物クリア
		clear();

		//ファイルパスからファイルを開く
		std::ifstream file(filePath.c_str());

		//もし正しくファイルを開けなかった場合、return
		if (!file.is_open()) {
			return;
		}

		std::string line = {};

		//一行ずつListに追加
		while (std::getline(file, line)) {
			if (line.empty()) continue;  //もし空だった場合、続行


			//文字列とデータの処理
			ScoreData data = {};
			//最初のタブキー入力の位置を検索
			std::size_t tab = line.find('\t');

			//タブを検出した場合、それまでの文字列をスコアにし、
			if (tab != std::string::npos) {
				std::string s_score = line.substr(0, tab);

				//残りをプレイヤー名に
				std::string name = line.substr(tab + 1);

				//数字の文字列だった場合、intに変換
				try {
					data.score = std::stoi(s_score);
				}
				catch (...) {
					// 数値でない場合はスキップ
					continue;
				}
				data.userName = name;
			}
			else {
				//タブがなかった場合、スコアを0にし、名前のみ代入
				data.score = 0;
				data.userName = line;
			}

			// 末尾にノードを追加
			addNode(const_iterator(nullptr), data);
		}

		//ファイルを閉じる
		file.close();
	}

	/**
	 * @brief 読み込んだ内容の出力
	 * @return 文字列(string)
	 */
	std::string outputData() {
		std::string outputLine;
		Node* current = head;

		//currentがnullptrになるまでループ
		while (current != nullptr) {
			//文字列を追加し、改行
			outputLine += std::to_string(current->data.score);  //スコアを文字列に変換し、
			outputLine += '\t';   //タブを追加
			outputLine += current->data.userName;  //名前を追加し
			outputLine += '\n';   //改行

			//次のノードへ
			current = current->nextNode;
		}

		//文字列を返す
		return outputLine;
	}

	/**
	 * @brief デフォルトコンストラクタ。
	 */
	DoublyLinkedList() = default;

	/**
	 * @brief デストラクタでリストの内容物全消去
	 */
	~DoublyLinkedList() {
		clear(); 
	}

	/**
	 * @brief コピーコンストラクタを削除。
	 */
	DoublyLinkedList(const DoublyLinkedList&) = delete;

	/**
	 * @brief コピー代入演算子を削除。
	 */
	DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;
};