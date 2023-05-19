#include <iostream>
#include <cassert>
using namespace std;

enum Color { RED, BLACK };

struct Node {
    int data;
    bool color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int data) {
        this->data = data;
        left = right = parent = NULL;
        this->color = RED;
    }
};

class RedBlackTree {
private:
    Node* root;

    void rotate_left(Node* n) {
        Node* nnew = n->right;
        Node* p = n->parent;
        assert(nnew != NULL); 

        n->right = nnew->left;
        if (n->right != NULL) {
            n->right->parent = n;
        }

        nnew->left = n;
        n->parent = nnew;

        if (p != NULL) {
            if (n == p->left) {
                p->left = nnew;
            } else if (n == p->right) {
                p->right = nnew;
            }
        } else {
            root = nnew;
        }
        nnew->parent = p;
    }

    void rotate_right(Node* n) {
        Node* nnew = n->left;
        Node* p = n->parent;
        assert(nnew != NULL); 

        n->left = nnew->right;
        if (n->left != NULL) {
            n->left->parent = n;
        }

        nnew->right = n;
        n->parent = nnew;

        if (p != NULL) {
            if (n == p->left) {
                p->left = nnew;
            } else if (n == p->right) {
                p->right = nnew;
            }
        } else {
            root = nnew;
        }
        nnew->parent = p;
    }

    void insert_case1(Node* n);
    void insert_case2(Node* n);
    void insert_case3(Node* n);
    void insert_case4(Node* n);
    void insert_case5(Node* n);

    void delete_one_child(Node* n);
    void replace_node(Node* n, Node* child);
    void delete_case1(Node* n);
    void delete_case2(Node* n);
    void delete_case3(Node* n);
    void delete_case4(Node* n);
    void delete_case5(Node* n);
    void delete_case6(Node* n);

    Node* search(Node* root, int data);
    void inorder(Node* root);

public:
    RedBlackTree() {
        root = NULL;
    }
 Node* BSTInsert(Node* root, Node* pt);
    void insert(int data);
    void remove(int data);
    void printInOrder();
};

void RedBlackTree::insert(int data) {
    Node* node = new Node(data);
    root = BSTInsert(root, node);
    insert_case1(node);
}

void RedBlackTree::remove(int data) {
    Node* to_delete = search(root, data);
    if (to_delete != NULL) {
        delete_one_child(to_delete);
    }
}

void RedBlackTree::printInOrder() {
    inorder(root);
    cout << endl;
}

Node* RedBlackTree::BSTInsert(Node* root, Node* pt) {
    if (root == NULL) {
        return pt;
    }

    if (pt->data < root->data) {
        root->left = BSTInsert(root->left, pt);
        root->left->parent = root;
    } else if (pt->data > root->data) {
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }

    return root;
}

void RedBlackTree::insert_case1(Node* n) {
    if (n->parent == NULL) {
        n->color = BLACK;
    } else {
        insert_case2(n);
    }
}

void RedBlackTree::insert_case2(Node* n) {
    if (n->parent->color == BLACK) {
        return;
    } else {
        insert_case3(n);
    }
}

void RedBlackTree::insert_case3(Node* n) {
    Node* u = n->parent->parent->left;
    if (u != NULL && u->color == RED) {
        n->parent->color = BLACK;
        u->color = BLACK;
        n->parent->parent->color = RED;
        insert_case1(n->parent->parent);
    } else {
        insert_case4(n);
    }
}

void RedBlackTree::insert_case4(Node* n) {
    if (n == n->parent->right && n->parent == n->parent->parent->left) {
        rotate_left(n->parent);
        n = n->left;
    } else if (n == n->parent->left && n->parent == n->parent->parent->right) {
        rotate_right(n->parent);
        n = n->right;
    }
    insert_case5(n);
}

void RedBlackTree::insert_case5(Node* n) {
    n->parent->color = BLACK;
    n->parent->parent->color = RED;
    if (n == n->parent->left && n->parent == n->parent->parent->left) {
        rotate_right(n->parent->parent);
    } else {
        rotate_left(n->parent->parent);
    }
}

void RedBlackTree::delete_one_child(Node* n) {
    Node* child = (n->right != NULL) ? n->right : n->left;
    replace_node(n, child);

    if (n->color == BLACK) {
        if (child != NULL && child->color == RED) {
            child->color = BLACK;
        } else {
            delete_case1(child);
        }
    }
    delete n;
}

void RedBlackTree::replace_node(Node* n, Node* child) {
    if (n->parent == NULL) {
        root = child;
    } else {
        if (n == n->parent->left) {
            n->parent->left = child;
        } else {
            n->parent->right = child;
        }
    }
    if (child != NULL) {
        child->parent = n->parent;
    }
}

void RedBlackTree::delete_case1(Node* n) {
    if (n->parent != NULL) {
        delete_case2(n);
    }
}

void RedBlackTree::delete_case2(Node* n) {
    Node* s = (n == n->parent->left) ? n->parent->right : n->parent->left;
    if (s != NULL && s->color == RED) {
        n->parent->color = RED;
        s->color = BLACK;
        if (n == n->parent->left) {
            rotate_left(n->parent);
        } else {
            rotate_right(n->parent);
        }
    }
    delete_case3(n);
}

void RedBlackTree::delete_case3(Node* n) {
    Node* s = (n == n->parent->left) ? n->parent->right : n->parent->left;
    if (n->parent->color == BLACK && s != NULL && s->color == BLACK &&
        (s->left == NULL || s->left->color == BLACK) &&
        (s->right == NULL || s->right->color == BLACK)) {
        s->color = RED;
        delete_case1(n->parent);
    } else {
        delete_case4(n);
    }
}

void RedBlackTree::delete_case4(Node* n) {
    Node* s = (n == n->parent->left) ? n->parent->right : n->parent->left;
    if (n->parent->color == RED && s != NULL && s->color == BLACK &&
        (s->left == NULL || s->left->color == BLACK) &&
        (s->right == NULL || s->right->color == BLACK)) {
        s->color = RED;
        n->parent->color = BLACK;
    } else {
        delete_case5(n);
    }
}

void RedBlackTree::delete_case5(Node* n) {
    Node* s = (n == n->parent->left) ? n->parent->right : n->parent->left;
    if (s != NULL && s->color == BLACK) {
        if (n == n->parent->left && s->right != NULL && s->right->color == RED &&
            (s->left == NULL || s->left->color == BLACK)) {
            s->color = RED;
            s->right->color = BLACK;
            rotate_left(n->parent);
        } else if (n == n->parent->right && s->left != NULL && s->left->color == RED &&
                   (s->right == NULL || s->right->color == BLACK)) {
            s->color = RED;
            s->left->color = BLACK;
            rotate_right(n->parent);
        }
    }
    delete_case6(n);
}

void RedBlackTree::delete_case6(Node* n) {
    Node* s = (n == n->parent->left) ? n->parent->right : n->parent->left;
    s->color = n->parent->color;
    n->parent->color = BLACK;

    if (n == n->parent->left) {
        if (s->right != NULL) {
            s->right->color = BLACK;
        }
        rotate_left(n->parent);
    } else {
        if (s->left != NULL) {
            s->left->color = BLACK;
        }
        rotate_right(n->parent);
    }
}

Node* RedBlackTree::search(Node* root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }

    if (data < root->data) {
        return search(root->left, data);
    }

    return search(root->right, data);
}

void RedBlackTree::inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        cout << root->data << " ";
        inorder(root->right);
    }
}

int main() {
    RedBlackTree tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);
    tree.insert(25);
    tree.insert(5);

    tree.printInOrder();
    tree.remove(15);
    tree.printInOrder();

    return 0;
}
