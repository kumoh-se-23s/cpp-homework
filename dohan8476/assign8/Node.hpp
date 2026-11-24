#pragma once

template <typename T>
class Node{
    protected:
        T* data;

    public:
        Node(T* data = nullptr) : data(data) {}
        virtual ~Node() { delete data; }

        //얕은 복사가 이루어져 같은곳을 가르키는데 delete는 각각 수행해서
        //복사를 원천에 차단? 한다는 의미로 사용됨
        //double free 방지용
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        T* getData() const { return data; }
        void setData(T* data) { this->data = data; }
};