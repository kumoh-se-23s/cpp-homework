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

        virtual N* next() const = 0;

        virtual N* set_next(N *next_node) = 0;

        [[nodiscard]] virtual bool has_next() const = 0;

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
