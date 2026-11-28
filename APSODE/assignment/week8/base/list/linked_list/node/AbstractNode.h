#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

template<typename T, typename N>
class AbstractNode {
    T m_item;

    public:
        T& get() {
            return this->m_item;
        }

        const T& get() const {
            return this->m_item;
        }

        void set(const T &item) {
            this->m_item = item;
        }

    protected:
        AbstractNode()
            : m_item() {
        }

        explicit AbstractNode(const T &item)
            : m_item(item) {
        }

        virtual ~AbstractNode() = default;
};

#endif //ABSTRACTNODE_H
