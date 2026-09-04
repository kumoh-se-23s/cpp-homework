//
// Created by apsode on 26. 9. 3..
//

#ifndef CPP_HOMEWORK_COLLECTION_H
#define CPP_HOMEWORK_COLLECTION_H

namespace collection {
    template <typename E> class Collection {
    private:
        int capacity;
    public:
        Collection();
        Collection(int manual_size);
        bool is_empty();
        int size();
    };

} // collection

#endif //CPP_HOMEWORK_COLLECTION_H
