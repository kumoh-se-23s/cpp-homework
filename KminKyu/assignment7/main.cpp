#include <iostream>

#include "DoubleNode.h"
#include "NestedSingleNode.h"
#include "SingleNode.h"

int main() {
    int temp1 = 1;
    DoubleNode<int> temp2 = DoubleNode(temp1);
    SingleNode<int> temp3 = SingleNode(temp1);
    NestedSingleNode<int> temp = NestedSingleNode(temp2);
    std::cout << temp.getUnravelItem();
    return 0;
}
