#pragma once
#include <string>
#include <fstream>
/**
 * @class DoublyLinkedList
 * @brief 指定したファイルを取り込める双方向リスト
 *
 * @note 入力と出力はstd::stringのみ
 * txtファイルを指定し、読み込むことで、
 * 再度同順に出力できます
 */
class DoublyLinkedList {
public:

	struct ScoreData {
		int            score = 0;
		std::string    userName = {};
	};

	struct Node {
		Node* prevNode = nullptr;    //一つ前のノードのポインタ
		Node* nextNode = nullptr;    //一つ後のノードのポインタ
		ScoreData data = {};
	};


	Node* head = nullptr;  //先頭アドレス
	Node* tail = nullptr;  //末尾アドレス
	unsigned short listSize = 0;  //現在のリストのサイズ

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
			pushback(line);
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
			outputLine += current->data;
			outputLine += '\n';

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

private:
	
	
	/**
	 * @brief ノード追加
	 * @param previous 挿入先のノード
	 * @param current  挿入するノード
	 */
	void addNode(Node* previous, Node* current) {
		
		//もしリストが空であった場合
		if (listSize == 0) {
			//先頭と末尾アドレスにcurrentを代入
			head = current;
			tail = current;
			current->prevNode = current->nextNode = nullptr;
		}
		//もしpreviousの次のノードが末尾のノードではなかった場合(途中からの挿入の場合)
		else if (previous != tail) {
			//previousのノードとcurrentの前後のアドレスを再編成
			current->nextNode = previous->nextNode;
			previous->nextNode = current;
			current->prevNode = previous;
			
			//次のノードの前要素アドレスにcurrentを代入
			current->nextNode->prevNode = current;
		}
		//もしpreviousが末尾のノードだった場合
		else if (previous == tail) {
			//currentを追加し、末尾アドレスにcurrentを代入
			previous->nextNode = current;
			current->prevNode = previous;
			current->nextNode = nullptr;
			tail = current;
		}		

		//リストサイズを管理する変数を+1
		listSize++;
	}

	/**
	 * @brief リストに要素追加
	 * @param newLine 追加する文字列
	 */
	void pushback(const std::string& newLine) {
		Node* node = new Node;
		node->data = newLine;
		addNode(tail, node);
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
};