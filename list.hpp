#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T *data;
        node *prev;
        node *next;

        node() : data(nullptr), prev(nullptr), next(nullptr) {}
        node(const T &value, node *p = nullptr, node *n = nullptr)
            : data(new T(value)), prev(p), next(n) {}
        ~node() {
            delete data;
        }
    };

protected:
    /**
     * add data members for linked list as protected members
     */
    node *head;
    node *tail;
    size_t list_size;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        if (!cur) return nullptr;

        cur->next = pos;
        cur->prev = pos->prev;

        if (pos->prev) {
            pos->prev->next = cur;
        } else {
            head = cur;
        }

        pos->prev = cur;
        list_size++;
        return cur;
    }
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        if (!pos) return nullptr;

        if (pos->prev) {
            pos->prev->next = pos->next;
        } else {
            head = pos->next;
        }

        if (pos->next) {
            pos->next->prev = pos->prev;
        } else {
            tail = pos->prev;
        }

        list_size--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        node *ptr;
        list *container;

    public:
        iterator(node *p = nullptr, list *c = nullptr) : ptr(p), container(c) {}
        iterator(const iterator &other) : ptr(other.ptr), container(other.container) {}

        /**
         * iter++
         */
        iterator operator++(int) {
            iterator tmp = *this;
            if (!ptr) throw invalid_iterator();
            ptr = ptr->next;
            return tmp;
        }
        /**
         * ++iter
         */
        iterator & operator++() {
            if (!ptr) throw invalid_iterator();
            ptr = ptr->next;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            iterator tmp = *this;
            if (!ptr) {
                // If this is end iterator, move to last element
                if (container && container->tail) {
                    ptr = container->tail;
                } else {
                    throw invalid_iterator();
                }
            } else {
                ptr = ptr->prev;
            }
            return tmp;
        }
        /**
         * --iter
         */
        iterator & operator--() {
            if (!ptr) {
                // If this is end iterator, move to last element
                if (container && container->tail) {
                    ptr = container->tail;
                } else {
                    throw invalid_iterator();
                }
            } else {
                ptr = ptr->prev;
            }
            return *this;
        }
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (!ptr || !ptr->data) throw invalid_iterator();
            return *ptr->data;
        }
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (!ptr || !ptr->data) throw invalid_iterator();
            return ptr->data;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const;
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const;

        friend class const_iterator;
        friend class list;
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        const node *ptr;
        const list *container;

    public:
        const_iterator(const node *p = nullptr, const list *c = nullptr) : ptr(p), container(c) {}
        const_iterator(const const_iterator &other) : ptr(other.ptr), container(other.container) {}
        const_iterator(const iterator &other) : ptr(other.ptr), container(other.container) {}

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            if (!ptr) throw invalid_iterator();
            ptr = ptr->next;
            return tmp;
        }
        const_iterator & operator++() {
            if (!ptr) throw invalid_iterator();
            ptr = ptr->next;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            if (!ptr) {
                // If this is end iterator, move to last element
                if (container && container->tail) {
                    ptr = container->tail;
                } else {
                    throw invalid_iterator();
                }
            } else {
                ptr = ptr->prev;
            }
            return tmp;
        }
        const_iterator & operator--() {
            if (!ptr) {
                // If this is end iterator, move to last element
                if (container && container->tail) {
                    ptr = container->tail;
                } else {
                    throw invalid_iterator();
                }
            } else {
                ptr = ptr->prev;
            }
            return *this;
        }
        const T & operator *() const {
            if (!ptr || !ptr->data) throw invalid_iterator();
            return *ptr->data;
        }
        const T * operator ->() const {
            if (!ptr || !ptr->data) throw invalid_iterator();
            return ptr->data;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class iterator;
    };

    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() : head(nullptr), tail(nullptr), list_size(0) {}

    list(const list &other) : head(nullptr), tail(nullptr), list_size(0) {
        try {
            for (node *cur = other.head; cur; cur = cur->next) {
                push_back(*cur->data);
            }
        } catch (...) {
            clear();
            throw;
        }
    }
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
    }
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) return *this;

        clear();

        try {
            for (node *cur = other.head; cur; cur = cur->next) {
                push_back(*cur->data);
            }
        } catch (...) {
            clear();
            throw;
        }

        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) throw container_is_empty();
        return *head->data;
    }
    const T & back() const {
        if (empty()) throw container_is_empty();
        return *tail->data;
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head, this);
    }
    const_iterator cbegin() const {
        return const_iterator(head, this);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(nullptr, this);
    }
    const_iterator cend() const {
        return const_iterator(nullptr, this);
    }
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return list_size == 0;
    }
    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return list_size;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        while (head) {
            node *tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = nullptr;
        list_size = 0;
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.container != this) throw invalid_iterator();

        node *new_node = new node(value);
        if (!new_node) throw runtime_error();

        if (pos.ptr) {
            insert(pos.ptr, new_node);
            return iterator(new_node, this);
        } else {
            if (tail) {
                tail->next = new_node;
                new_node->prev = tail;
                tail = new_node;
            } else {
                head = tail = new_node;
            }
            list_size++;
            return iterator(new_node, this);
        }
    }
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (pos.container != this) throw invalid_iterator();
        if (!pos.ptr) throw invalid_iterator();

        node *next_node = pos.ptr->next;
        erase(pos.ptr);
        delete pos.ptr;

        if (next_node) {
            return iterator(next_node, this);
        } else {
            return end();
        }
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        node *new_node = new node(value);
        if (!new_node) throw runtime_error();

        if (tail) {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        } else {
            head = tail = new_node;
        }
        list_size++;
    }
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) throw container_is_empty();

        node *old_tail = tail;
        tail = tail->prev;
        if (tail) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete old_tail;
        list_size--;
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        node *new_node = new node(value);
        if (!new_node) throw runtime_error();

        if (head) {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        } else {
            head = tail = new_node;
        }
        list_size++;
    }
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) throw container_is_empty();

        node *old_head = head;
        head = head->next;
        if (head) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete old_head;
        list_size--;
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (list_size <= 1) return;

        // Use bubble sort with swap to avoid default construction
        bool swapped;
        do {
            swapped = false;
            node *cur = head;
            while (cur && cur->next) {
                if (*cur->next->data < *cur->data) {
                    // Swap the data pointers
                    std::swap(cur->data, cur->next->data);
                    swapped = true;
                }
                cur = cur->next;
            }
        } while (swapped);
    }
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) return;
        if (other.empty()) return;
        if (empty()) {
            std::swap(head, other.head);
            std::swap(tail, other.tail);
            std::swap(list_size, other.list_size);
            return;
        }

        node *cur1 = head;
        node *cur2 = other.head;
        node *new_head = nullptr;
        node *new_tail = nullptr;
        node *prev = nullptr;

        while (cur1 && cur2) {
            if (*cur2->data < *cur1->data) {
                if (!new_head) {
                    new_head = cur2;
                }
                cur2->prev = prev;
                if (prev) {
                    prev->next = cur2;
                }
                prev = cur2;
                cur2 = cur2->next;
            } else {
                if (!new_head) {
                    new_head = cur1;
                }
                cur1->prev = prev;
                if (prev) {
                    prev->next = cur1;
                }
                prev = cur1;
                cur1 = cur1->next;
            }
        }

        if (cur1) {
            if (prev) {
                prev->next = cur1;
            }
            cur1->prev = prev;
            new_tail = tail;
        } else {
            if (prev) {
                prev->next = cur2;
            }
            if (cur2) {
                cur2->prev = prev;
            }
            new_tail = other.tail;
        }

        head = new_head;
        tail = new_tail;
        list_size += other.list_size;

        other.head = nullptr;
        other.tail = nullptr;
        other.list_size = 0;
    }
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (list_size <= 1) return;

        node *cur = head;
        while (cur) {
            std::swap(cur->prev, cur->next);
            cur = cur->prev;
        }

        std::swap(head, tail);
    }
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (list_size <= 1) return;

        node *cur = head;
        while (cur && cur->next) {
            if (*cur->data == *cur->next->data) {
                node *to_delete = cur->next;
                cur->next = to_delete->next;
                if (to_delete->next) {
                    to_delete->next->prev = cur;
                } else {
                    tail = cur;
                }
                delete to_delete;
                list_size--;
            } else {
                cur = cur->next;
            }
        }
    }
};

// Iterator comparison operators
template<typename T>
bool list<T>::iterator::operator==(const const_iterator &rhs) const {
    return ptr == rhs.ptr;
}

template<typename T>
bool list<T>::iterator::operator!=(const const_iterator &rhs) const {
    return ptr != rhs.ptr;
}

}

#endif //SJTU_LIST_HPP
