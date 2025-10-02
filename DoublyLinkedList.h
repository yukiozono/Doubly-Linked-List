#pragma once
#include <string>
#include <fstream>

struct ScoreData {
	int            score = 0;
	std::string    userName = {};
};

/**
 * @class DoublyLinkedList
 * @brief �w�肵���t�@�C������荞�߂�o�������X�g
 *
 * @note ���͂Əo�͂�std::string�̂�
 * txt�t�@�C�����w�肵�A�ǂݍ��ނ��ƂŁA
 * �ēx�����ɏo�͂ł��܂�
 */
class DoublyLinkedList {
private:
	struct Node {
		Node* prevNode = nullptr;    //��O�̃m�[�h�̃|�C���^
		Node* nextNode = nullptr;    //���̃m�[�h�̃|�C���^
		ScoreData data = {};
	};

	//�m�[�h�ւ̃|�C���^
	Node* head = nullptr;  //�擪�A�h���X
	Node* tail = nullptr;  //�����A�h���X
	size_t listSize = 0;   //���݂̃��X�g�̃T�C�Y

	

public:

	class iterator {
		//DoublyLinkedList����A�N�Z�X���邽�߂̐錾
		friend class DoublyLinkedList;

	protected:
		Node* node = nullptr;

		//�R���X�g���N�^(init�̒l�ŏ�����)
		explicit iterator(Node* init) : node(init) {}

	public:
		//�����l��default��nullptr�ɂ��C��
		iterator() = default;

		//ScoreData�ɃA�N�Z�X���邽�߂̊ԐڎQ��
		ScoreData& operator*() { return node->data; }

	};

	class const_iterator : public iterator {
		//DoublyLinkedList����A�N�Z�X���邽�߂̐錾
		friend class DoublyLinkedList;

		//�R���X�g���N�^(init�̒l�ŏ�����)
		explicit const_iterator(Node* init) : iterator(init) {}

	public:
		//�f�t�H���g�R���X�g���N�^(iterator()�ďo���Anode��nullptr��)
		const_iterator() : iterator() {}

		//�O�u�f�N�������g�p(it--)(���X�g�̐擪�Ɍ������Ĉ�i�߂�[operator--]())
		const_iterator& operator--() { if (this->node) this->node = this->node->prevNode; return *this; }

		//��u�f�N�������g�p(--it)(���X�g�̐擪�Ɍ������Ĉ�i�߂�[operator--]())
		const_iterator  operator--(int) { const_iterator t = *this; --(*this); return t; }

		//�O�u�C���N�������g�p(it++)(���X�g�̖����Ɍ������Ĉ�i�߂�[operator++]())
		const_iterator& operator++() { if (this->node) this->node = this->node->nextNode; return *this; }

		//��u�C���N�������g�p(++it)(���X�g�̖����Ɍ������Ĉ�i�߂�[operator++]())
		const_iterator  operator++(int) { const_iterator t = *this; ++(*this); return t; }

		//�ԐڎQ��(�߂�l const ScoreData&)�i�C�e���[�^�̎w���v�f���擾����[operator* const��]())
		const ScoreData& operator*() const { return this->node->data; }

		//�R�s�[�R���X�g���N�^��������(iterator�̈ʒu�|�C���^���R�s�[)(�R�s�[���s��[�R�s�[�R���X�g���N�^]())
		const_iterator(const const_iterator&) = default;

		//�R�s�[�R���X�g���N�^��������(iterator�̈ʒu�|�C���^���㏑��)(������s��[operator=]())
		const_iterator& operator=(const const_iterator&) = default;
		
		//���l��r(==�ł����true��Ԃ�)(���ꂩ��r����[operator==]())
		bool operator==(const const_iterator& r) const { return this->node == r.node; }

		//�񓙒l��r(!=�ł����true��Ԃ�)(�قȂ邩����r����[operator!=]()
		bool operator!=(const const_iterator& r) const { return this->node != r.node; }

		//iterator����const_iterator�֕ϊ�
		const_iterator(const iterator& it) : iterator(it.node) {}
	};

public:
	//�f�[�^���̎擾
	size_t size() const { return listSize; }

	/**
	 * @brief          �m�[�h�ǉ�
	 * @param previous �}����̃m�[�h�̈ʒu
	 * @param datas    �ǉ�����f�[�^
	 * @return         �ǉ������m�[�h�̈ʒu
	 */
	iterator addNode(const const_iterator& nodePos, const ScoreData& datas) {
		//�V�K�m�[�h�Ƀf�[�^����
		Node* current = new Node{ nullptr, nullptr, datas };

		//�������X�g����ł������ꍇ
		if (listSize == 0) {
			//�擪�Ɩ����A�h���X��current����
			head = tail = current;
		}
		//����previous�������̃m�[�h�������ꍇ		
		else if (nodePos.node == nullptr) {
			//current��ǉ����A�����A�h���X��current����
			current->prevNode = tail;
			tail->nextNode = current;
			tail = current;
		}
		
		//�����}���悪�擪�ł������ꍇ
		else if (nodePos.node == head) {
			//current��擪�ɂ��Ahead�̈ʒu�����
			current->nextNode = head;
			head->prevNode = current;
			head = current;
		}
		//�����ǂ���ł��Ȃ��r������̑}���������ꍇ
		else {
			//previous�̃m�[�h��current�̑O��̃A�h���X���ĕҐ�
			Node* prev = nodePos.node->prevNode;
			current->prevNode = prev;
			current->nextNode = nodePos.node;
			prev->nextNode = current;

			//���̃m�[�h�̑O�v�f�A�h���X��current����
			nodePos.node->prevNode = current;
		}

		//���X�g�T�C�Y���Ǘ�����ϐ���+1
		++listSize;

		return iterator(current);
	}


	/**
	 * @brief          �m�[�h�폜
	 * @param previous �폜����m�[�h�̈ʒu
	 * @return         ���̃m�[�h�̈ʒu
	 */
	iterator deleteNode(const iterator& nodePos) {
		Node* current = nodePos.node;

		//������Ȃ������ꍇ�́Anullptr��return
		if (!current) return iterator(nullptr);

		Node* next = current->nextNode;

		//�m�[�h�̑O��̃|�C���^���ĕҐ�
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

		//current���폜���A���X�g�T�C�Y�����炷
		delete current;
		--listSize;
		return iterator(next);
	}

	/**
	 * @brief ���X�g���̗v�f�擪����S����
	 */
	void clear() {
		Node* current = head;
		//current��nullptr�ɂȂ�܂Ń��[�v���A�m�[�h���폜
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
	 * @brief �t�@�C���ǂݍ���
	 * @param �t�@�C���p�X
	 */
	void inputData(std::string filePath) {
		//�܂��̓��X�g�̓��e���N���A
		clear();

		//�t�@�C���p�X����t�@�C�����J��
		std::ifstream file(filePath.c_str());

		//�����������t�@�C�����J���Ȃ������ꍇ�Areturn
		if (!file.is_open()) {
			return;
		}

		std::string line = {};

		//��s����List�ɒǉ�
		while (std::getline(file, line)) {
			if (line.empty()) continue;  //�����󂾂����ꍇ�A���s


			//������ƃf�[�^�̏���
			ScoreData data = {};
			//�ŏ��̃^�u�L�[���͂̈ʒu������
			std::size_t tab = line.find('\t');

			//�^�u�����o�����ꍇ�A����܂ł̕�������X�R�A�ɂ��A
			if (tab != std::string::npos) {
				std::string s_score = line.substr(0, tab);

				//�c����v���C���[����
				std::string name = line.substr(tab + 1);

				//�����̕����񂾂����ꍇ�Aint�ɕϊ�
				try {
					data.score = std::stoi(s_score);
				}
				catch (...) {
					// ���l�łȂ��ꍇ�̓X�L�b�v
					continue;
				}
				data.userName = name;
			}
			else {
				//�^�u���Ȃ������ꍇ�A�X�R�A��0�ɂ��A���O�̂ݑ��
				data.score = 0;
				data.userName = line;
			}

			// �����Ƀm�[�h��ǉ�
			addNode(const_iterator(nullptr), data);
		}

		//�t�@�C�������
		file.close();
	}

	/**
	 * @brief �ǂݍ��񂾓��e�̏o��
	 * @return ������(string)
	 */
	std::string outputData() {
		std::string outputLine;
		Node* current = head;

		//current��nullptr�ɂȂ�܂Ń��[�v
		while (current != nullptr) {
			//�������ǉ����A���s
			outputLine += std::to_string(current->data.score);  //�X�R�A�𕶎���ɕϊ����A
			outputLine += '\t';   //�^�u��ǉ�
			outputLine += current->data.userName;  //���O��ǉ���
			outputLine += '\n';   //���s

			//���̃m�[�h��
			current = current->nextNode;
		}

		//�������Ԃ�
		return outputLine;
	}

	/**
	 * @brief �f�t�H���g�R���X�g���N�^�B
	 */
	DoublyLinkedList() = default;

	/**
	 * @brief �f�X�g���N�^�Ń��X�g�̓��e���S����
	 */
	~DoublyLinkedList() {
		clear(); 
	}

	/**
	 * @brief �R�s�[�R���X�g���N�^���폜�B
	 */
	DoublyLinkedList(const DoublyLinkedList&) = delete;

	/**
	 * @brief �R�s�[������Z�q���폜�B
	 */
	DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;
};