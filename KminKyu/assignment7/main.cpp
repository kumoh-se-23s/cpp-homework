#include <iostream>

#include "DoubleNode.h"
#include "NestedDoubleNode.h"
#include "SingleNode.h"
#include "SinglyLinkedList.h"

using namespace std;

int main(int argc, char* argv[]) {
    int temp1 = 4;
    DoubleNode<int> temp2 = DoubleNode(temp1);
    SingleNode<DoubleNode<int>> *temp3 = new SingleNode(temp2);
    NestedSingleNode<int> temp = NestedSingleNode(temp2);
    SinglyLinkedList<DoubleNode<int>> temp4 = SinglyLinkedList<DoubleNode<int>>();
    temp4.addFirst(temp3);
    temp4.add(temp3);
    std::cout << temp.getUnravelItem() << endl;
    std::cout << temp4.getSize() << endl ;
    DoubleNode<int> findtt = temp3->getItem();
    std::cout << temp4.find(findtt) << endl;
    return 0;
}
