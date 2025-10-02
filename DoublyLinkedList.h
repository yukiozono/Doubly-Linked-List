#pragma once
#include <string>
#include <fstream>
/**
 * @class DoublyLinkedList
 * @brief �w�肵���t�@�C������荞�߂�o�������X�g
 *
 * @note ���͂Əo�͂�std::string�̂�
 * txt�t�@�C�����w�肵�A�ǂݍ��ނ��ƂŁA
 * �ēx�����ɏo�͂ł��܂�
 */
class DoublyLinkedList {
public:

	struct ScoreData {
		int            score = 0;
		std::string    userName = {};
	};

	struct Node {
		Node* prevNode = nullptr;    //��O�̃m�[�h�̃|�C���^
		Node* nextNode = nullptr;    //���̃m�[�h�̃|�C���^
		ScoreData data = {};
	};


	Node* head = nullptr;  //�擪�A�h���X
	Node* tail = nullptr;  //�����A�h���X
	unsigned short listSize = 0;  //���݂̃��X�g�̃T�C�Y

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
			pushback(line);
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
			outputLine += current->data;
			outputLine += '\n';

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

private:
	
	
	/**
	 * @brief �m�[�h�ǉ�
	 * @param previous �}����̃m�[�h
	 * @param current  �}������m�[�h
	 */
	void addNode(Node* previous, Node* current) {
		
		//�������X�g����ł������ꍇ
		if (listSize == 0) {
			//�擪�Ɩ����A�h���X��current����
			head = current;
			tail = current;
			current->prevNode = current->nextNode = nullptr;
		}
		//����previous�̎��̃m�[�h�������̃m�[�h�ł͂Ȃ������ꍇ(�r������̑}���̏ꍇ)
		else if (previous != tail) {
			//previous�̃m�[�h��current�̑O��̃A�h���X���ĕҐ�
			current->nextNode = previous->nextNode;
			previous->nextNode = current;
			current->prevNode = previous;
			
			//���̃m�[�h�̑O�v�f�A�h���X��current����
			current->nextNode->prevNode = current;
		}
		//����previous�������̃m�[�h�������ꍇ
		else if (previous == tail) {
			//current��ǉ����A�����A�h���X��current����
			previous->nextNode = current;
			current->prevNode = previous;
			current->nextNode = nullptr;
			tail = current;
		}		

		//���X�g�T�C�Y���Ǘ�����ϐ���+1
		listSize++;
	}

	/**
	 * @brief ���X�g�ɗv�f�ǉ�
	 * @param newLine �ǉ����镶����
	 */
	void pushback(const std::string& newLine) {
		Node* node = new Node;
		node->data = newLine;
		addNode(tail, node);
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
};