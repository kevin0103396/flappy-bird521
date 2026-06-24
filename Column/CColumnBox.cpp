#include "CColumnBox.h"
#include"../GameConfig/GameConfig.h"

void ColumnBox_Init(ColumnBox* p)
{
    List_Init(&p->m_ColLst);
}

void ColumnBox_ShowAll(ColumnBox* p)
{
    ListNode* node = p->m_ColLst.head;
    while (node) {
        if (node->data) {
            Column* col = static_cast<Column*>(node->data);
            Column_Show(col);
        }
        node = node->next;
    }
}

void ColumnBox_MoveAll(ColumnBox* p, int step)
{
    ListNode* prev = nullptr;
    ListNode* node = p->m_ColLst.head;
    while (node) {
        Column* col = static_cast<Column*>(node->data);
        if (col) {
            Column_Move(col, step);
            if (col->m_x < -IMG_COLUMN_WIDTH) {
                ListNode* toDel = node;
                node = node->next;
                if (prev) prev->next = node;
                else p->m_ColLst.head = node;
                if (toDel == p->m_ColLst.tail) p->m_ColLst.tail = prev;
                delete col;
                delete toDel;
                p->m_ColLst.size--;
                continue;
            }
        }
        prev = node;
        node = node->next;
    }
}

void ColumnBox_Clear(ColumnBox* p)
{
    /* Free all Column data first */
    ListNode* node = p->m_ColLst.head;
    while (node) {
        if (node->data) {
            delete static_cast<Column*>(node->data);
        }
        node = node->next;
    }
    List_Clear(&p->m_ColLst);
}
