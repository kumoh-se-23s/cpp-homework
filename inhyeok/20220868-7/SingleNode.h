//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "RAIISharedPointer.h"


template<typename Data>
struct SingleNode {
    Data data;
    RAIISharedPointer<SingleNode> next = nullptr;

    SingleNode(Data data) : data(std::move(data)) {
    }
};

