#include "DoublyLinkedList.h"
#include <iostream>


void main(void) {
	DoublyLinkedList list = {};
	list.inputData("Scores.txt");

	std::string output = list.outputData();	
	std::cout << output;	
}