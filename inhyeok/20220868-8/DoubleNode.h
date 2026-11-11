//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "RAIIUniquePointer.h"
#include "RAIISharedPointer.h"


template<typename Data>
struct DoubleNode {
    RAIIUniquePointer<Data> data;
    RAIISharedPointer<DoubleNode> prev = nullptr;
    RAIISharedPointer<DoubleNode> next = nullptr;
    explicit DoubleNode(RAIIUniquePointer<Data> data) : data(std::move(data)) {
    }
};

