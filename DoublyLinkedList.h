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
			outputLine += current->line;
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
	struct Node {
		std::string line = {};  //ノード内の内容
		Node* prevNode = nullptr;    //一つ前のノードのポインタ
		Node* nextNode = nullptr;    //一つ後のノードのポインタ
	};

	Node* head = nullptr;  //先頭アドレス
	Node* tail = nullptr;  //末尾アドレス
	size_t listSize = 0;  //現在のリストのサイズ

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
		node->line = newLine;
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