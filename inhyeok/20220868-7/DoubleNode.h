//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "RAIISharedPointer.h"
#include "Student.h"


template<typename Data>
struct SingleNode {
    Data data;
    RAIISharedPointer<SingleNode> next = nullptr;

    SingleNode(Data data) : data(std::move(data)) {
    }
};

template<typename Data>
struct DoubleNode {
    Data data;
    RAIISharedPointer<DoubleNode> prev = nullptr;
    RAIISharedPointer<DoubleNode> next = nullptr;

    DoubleNode(Data data) : data(std::move(data)) {
    }
};

