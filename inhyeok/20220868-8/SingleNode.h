//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "RAIIUniquePointer.h"
#include "RAIISharedPointer.h"


template<typename Data>
struct SingleNode {
    RAIIUniquePointer<Data> data;
    RAIISharedPointer<SingleNode> next = nullptr;

    explicit SingleNode(RAIIUniquePointer<Data> data) : data(std::move(data)) {
    }
};

