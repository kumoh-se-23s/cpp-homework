#include <iostream>

#include "DoubleNode.h"
#include "NestedSingleNode.h"
#include "SingleNode.h"
#include "SinglyLinkedList.h"

using namespace std;

int main() {
    int temp1 = 4;
    DoubleNode<int> temp2 = DoubleNode(temp1);
    SingleNode<int> temp3 = SingleNode(temp1);
    NestedSingleNode<int> temp = NestedSingleNode(temp2);
    SinglyLinkedList<Node<int>> temp4 = SinglyLinkedList<Node<int>>();
    temp4.add(temp.getItem());
    std::cout << temp.getUnravelItem() << endl;
    std::cout << temp4.getSize() << endl ;
    std::cout << temp4.find(temp)->getItem().getUnravelItem() << endl;
    return 0;
}
