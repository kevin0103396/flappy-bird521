#ifndef CLIST_H
#define CLIST_H

#include <new>

struct ListNode {
    void* data;
    ListNode* next;
};

struct List {
    ListNode* head;
    ListNode* tail;
    int size;
};

inline void List_Init(List* lst) noexcept
{
    lst->head = nullptr;
    lst->tail = nullptr;
    lst->size = 0;
}

inline void List_PushBack(List* lst, void* data) noexcept
{
    ListNode* node = new (std::nothrow) ListNode;
    if (!node) return;
    node->data = data;
    node->next = nullptr;
    if (lst->tail) {
        lst->tail->next = node;
    } else {
        lst->head = node;
    }
    lst->tail = node;
    lst->size++;
}

/* Frees list nodes only, does NOT free data pointers */
inline void List_Clear(List* lst) noexcept
{
    ListNode* cur = lst->head;
    while (cur) {
        ListNode* next = cur->next;
        delete cur;
        cur = next;
    }
    lst->head = nullptr;
    lst->tail = nullptr;
    lst->size = 0;
}

#endif
