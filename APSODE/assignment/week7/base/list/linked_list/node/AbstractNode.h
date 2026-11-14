#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H
#include <optional>

template<typename T, typename N>
class AbstractNode {
    T* m_item;

    public:
        std::optional<T> get() const {
            if (m_item == nullptr) {
                return std::optional<T>{};
            }

            return std::optional<T>(this->m_item);
        }

        void set(const T* item) {
            this->m_item = item;
        }

        virtual N* next() const = 0;

        virtual N* set_next(const N* next_node) = 0;

        virtual bool has_next() = 0;


    protected:
        AbstractNode() : m_item(nullptr) {}

        AbstractNode(const T* item) : m_item(item) {}

        ~AbstractNode() = default;
};

#endif //ABSTRACTNODE_H
