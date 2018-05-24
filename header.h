#ifndef LISTOFTREES_HEADER_H
#define LISTOFTREES_HEADER_H

#include <fstream>

using namespace std;

ifstream Fin("input.txt");
ofstream Fout("output.txt");

struct Node {
    int key = 0;     // значение узла
    Node *left = nullptr;  // указатель на левое поддерево
    Node *right = nullptr; // указатель на правое поддерево
    Node *parent = nullptr;
    int height = 0;  // высота поддерева дерева
    Node *prev = nullptr;  //предыдущий в очереди в списке
    Node *next = nullptr;  //следующий элемент в списке
    Node *que = nullptr;   //указатель на элемент с таким же ключом

};

class SList {
public:
    Node *head = nullptr;
    Node *first = nullptr;
    Node *last;
    int length;

    void listInsert(int pos, int key) {
        if (first == nullptr) {//если список пуст(случай работы при добавлении первого элемента)
            first = new Node;
            last = first;
            head = first;
            first->key = key;
            first->height = 1;
            length = 1;
        } else {
            length++;
            if (pos >= length) {//если попытка вставить элемент на позицию, которой нет в списке
                Node *tmp = new Node;
                tmp->prev = last;
                last->next = tmp;
                last = tmp;
                tmp->height = 1;
                tmp->key = key;
                insertNode(tmp);
            } else {
                Node *tmp = first;
                for (int i = 0; i < pos - 1; ++i) {
                    tmp = tmp->next;
                }
                Node *tmp1 = new Node;
                tmp1->key = key;
                if (tmp != nullptr && tmp->next != nullptr)tmp1->next = tmp->next;
                tmp1->prev = tmp;
                if (tmp != nullptr && tmp->next != nullptr)tmp->next->prev = tmp1;
                if (tmp != nullptr)tmp->next = tmp1;
                tmp1->height = 1;
                insertNode(tmp1);

            }
        }
    };

    void listDelete(int pos) {
        if (length == 1) {
            length = 0;
            delete first;
        } else if (pos >= length) {
            Node *tmp = last;
            deleteNode(tmp);
            length--;
        } else {
            Node *tmp = first;
            for (int i = 0; i <= pos - 1; ++i) {
                tmp = tmp->next;
            }
            if (pos == 0)
            {
                first = first->next;
            }
            deleteNode(tmp);
            length--;
        }
    }

    void listEdit(int pos, int key) {
        this->listDelete(pos);
        this->listInsert(pos, key);
    };

    int listMin() {
        Node *tmp = head;
        while (tmp->left != nullptr) {
            tmp = tmp->left;
        }
        return tmp->key;
    };

    int listMax() {
        Node *tmp = head;
        while (tmp->right != nullptr) {
            tmp = tmp->right;
        }
        return tmp->key;
    };

    void listReverse() {
        Node *tmp = first;
        Node *tmp1 = first->next;
        swap(first, last);
        tmp->next = nullptr;
        do {
            tmp->prev = tmp1;
            tmp1 = tmp1->next;
            tmp1->prev->next = tmp;
            tmp = tmp1->prev;

        } while (tmp1->next != nullptr);
        tmp1->next = tmp;
        tmp->prev = tmp1;
        tmp1->prev = nullptr;
    };

    int listSearchfirst(int key) {
        Node *tmp = first;
        int i = 0;

        while (tmp->key != key) {
            i++;
            tmp = tmp->next;
        }
        return i;
    }

    int listSearchall(int key) {
        Node *tmp = head;
        while (((tmp->key > key) ? tmp->left != nullptr : tmp->right != nullptr) && (tmp->key != key)) {
            (tmp->key > key) ? tmp = tmp->left : tmp = tmp->right;
        }
        if (tmp->key == key) {

        }
    };

    void listPrint() {
        Node *result = first;

        while (result != nullptr)
        {
            Fout << result->key << " ";
            result = result->next;
        }
        Fout << '\n';
    }

private:

    int height(const Node *const node) const {
        return node == nullptr ? 0 : node->height;
    }

    void leftRotate(Node *&node) {
        Node *result = node->right;
        Node *tmp = result->left;

        // Изменяем потомков
        result->parent = node->parent;
        result->left = node;
        node->parent = result;
        node->right = tmp;
        if (tmp != nullptr)tmp->parent = node;
        //  Изменяем высоту
        node->height = std::max(height(node->left), height(node->right)) + 1;
        result->height = std::max(height(result->left), height(result->right)) + 1;

        // Присваивание нового корня
        node = result;
    }

    void rightRotate(Node *&node) {
        Node *result = node->left;
        Node *tmp = result->right;

        // Изменяем потомков
        result->parent = node->parent;
        result->right = node;
        node->parent = result;
        node->left = tmp;
        if (tmp != nullptr)tmp->parent = node;
        //  Изменяем высоту
        node->height = std::max(height(node->left), height(node->right)) + 1;
        result->height = std::max(height(result->left), height(result->right)) + 1;

        // Присваивание нового корня
        node = result;
    }

    int getBalance(Node *node) {
        return node == nullptr ? 0 : height(node->left) - height(node->right);
    }

    void swap(Node *&a, Node *&b) {
        Node *c;
        c = a;
        a = b;
        b = c;
    }

    void insertNode(Node *node) {
        Node *tmp = head;
        while (((tmp->key > node->key) ? tmp->left != nullptr : tmp->right != nullptr) && (tmp->key != node->key)) {
            (tmp->key > node->key) ? tmp = tmp->left : tmp = tmp->right;
        }

        if (tmp->key == node->key) {//если элемент уже есть в дереве
            Node *temp = tmp->que;
            tmp->que = node;
            node->que = temp;
        } else {
            (tmp->key > node->key) ? tmp->left = node : tmp->right = node;
            node->parent = tmp;
            int i = 2;
            while (tmp != nullptr and tmp->height <= i++) {
                tmp->height = i;
                int balance = getBalance(tmp);
                if (balance > 1 && getBalance(tmp->left) >= 0) {
                    rightRotate(tmp);
                } else if (balance > 1 && getBalance(tmp->left) < 0) {
                    leftRotate(tmp->left);
                    rightRotate(tmp);
                } else if (balance < -1 && getBalance(tmp->right) <= 0) {
                    leftRotate(tmp);
                } else if (balance < -1 && getBalance(tmp->right) > 0) {
                    rightRotate(tmp->right);
                    leftRotate(tmp);
                }
                if (tmp->parent == nullptr) head = tmp;
                else (tmp->parent->key > tmp->key) ? tmp->parent->left = tmp : tmp->parent->right = tmp;
                tmp = tmp->parent;
            }

        }
        return;
    }
    void del(Node *node){/////удаление очереди
        if(node->prev != nullptr) node->prev->next = node->next;
        if(node->next != nullptr) node->next->prev = node->prev;
    };

    void deleteNode(Node *node) {
        Node *tmp = head;
        while (((tmp->key > node->key) ? tmp->left != nullptr : tmp->right != nullptr) && (tmp->key != node->key)) {
            (tmp->key > node->key) ? tmp = tmp->left : tmp = tmp->right;
        }
        if (tmp->que != nullptr) {
            if (tmp == node) {
                tmp->que->parent = tmp->parent;
                tmp->que->right = tmp->right;
                tmp->que->left = tmp->left;
                if (tmp->key < tmp->parent->key) tmp->parent->left = tmp->que; else tmp->parent->right = tmp->que;
                if (tmp->que->right != nullptr)tmp->que->right->parent = tmp->que;
                if (tmp->que->left != nullptr)tmp->que->left->parent = tmp->que;
                tmp->que->height = tmp->height;
                del(tmp);
                delete (tmp);
                return;
            } else {
                while (tmp->que != node) {
                    tmp = tmp->que;
                }
                tmp->que = tmp->que->que;
                del(node);
                delete (node);
                return;
            }
        } else {//если элемент с таким ключом в списке один

            if (tmp->right ==
                nullptr) {/////если нет правого поддерева то по свойству авлдерева это либо лист либо у левого поддерева высота 1
                if (tmp->left != nullptr) {
                    tmp->left->parent = tmp->parent;
                    (tmp->parent->key > tmp->key) ? tmp->parent->left = tmp->left : tmp->parent->right = tmp->left;
                } else {
                    if (tmp->parent->left == nullptr) tmp->parent->right = nullptr;
                    else if (tmp->parent->right == nullptr) tmp->parent->left = nullptr;
                    else (tmp->parent->key > tmp->key) ? tmp->parent->left = nullptr : tmp->parent->right = nullptr;
                }
                Node *tmp1 = tmp;
                tmp = tmp->parent;
                del(node);
                delete tmp1;
            } else {//////случай когда есть правое поддерево
                tmp = tmp->right;
                while (tmp->left != nullptr) tmp = tmp->left;
                Node tmp1 = *tmp;  ////////// тут ошибка в замене, нужно менять не ноды и очереди а полностью все указатели
                tmp->que = node->que;
                tmp->key = node->key;
                node->que = tmp1.que;
                node->key = tmp1.key;
                swap(node, tmp);

                if (node->right != nullptr) {
                    node->parent->right = node->right;
                    node->right->parent = node->parent;

                } else {
                    node->parent->right = nullptr;
                }

                del(node);
                delete (node);
            }


            while (tmp != nullptr) {
                tmp->height = std::max(height(tmp->left), height(tmp->right)) + 1;
                int balance = getBalance(tmp);
                if (balance > 1 && getBalance(tmp->left) >= 0) {
                    rightRotate(tmp);
                } else if (balance > 1 && getBalance(tmp->left) < 0) {
                    leftRotate(tmp->left);
                    rightRotate(tmp);
                } else if (balance < -1 && getBalance(tmp->right) <= 0) {
                    leftRotate(tmp);
                } else if (balance < -1 && getBalance(tmp->right) > 0) {
                    rightRotate(tmp->right);
                    leftRotate(tmp);
                }
                if (tmp->parent == nullptr) head = tmp;
                else (tmp->parent->key > tmp->key) ? tmp->parent->left = tmp : tmp->parent->right = tmp;
                tmp = tmp->parent;
            }

        }
    }
};

#endif //LISTOFTREES_HEADER_H
