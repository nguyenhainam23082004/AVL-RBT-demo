#include <iostream>
#include <algorithm>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    int height;
};

int getHeight(Node* root) {
    if (root == NULL)
        return 0;
    return 1 + max(getHeight(root->left), getHeight(root->right));
}

Node* rightRotate(Node* root) {
    Node* x = root->left;

    root->left = x->right;
    x->right = root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));

    return x;
}

Node* leftRotate(Node* root) {
    Node* y = root->right;

    root->right = y->left;
    y->left = root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));

    return y;
}

Node* leftLeftRotate(Node* root) {
    Node* new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    new_root->height = max(getHeight(new_root->left), getHeight(new_root->right)) + 1;
    return new_root;
}

Node* rightRightRotate(Node* root) {
    Node* new_root = root->right;
    root->right = new_root->left;
    new_root->left = root;
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    new_root->height = max(getHeight(new_root->left), getHeight(new_root->right)) + 1;
    return new_root;
}

Node* rightLeftRotate(Node* root) {
    root->right = leftLeftRotate(root->right);
    return rightRightRotate(root);
}

Node* leftRightRotate(Node* root) {
    root->left = rightRightRotate(root->left);
    return leftLeftRotate(root);
}

int getBalanceFactor(Node* node) {
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

Node* createNode(int data) {
    Node* node = new Node();
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

Node* insertNode(Node* node, int data) {
    if (node == NULL)
        return createNode(data);

    if (data < node->data)
        node->left = insertNode(node->left, data);
    else if (data > node->data)
        node->right = insertNode(node->right, data);
    else
        return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = getBalanceFactor(node);

    if (balance > 1 && data < node->left->data)
        return leftLeftRotate(node);

    if (balance > 1 && data > node->left->data) {
        node->left = leftRotate(node->left);
        return leftLeftRotate(node);
    }

    if (balance < -1 && data > node->right->data)
        return rightRightRotate(node);

    if (balance < -1 && data < node->right->data) {
        node->right = rightRotate(node->right);
        return rightRightRotate(node);
    }

    return node;
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, int data) {
    if (root == NULL)
        return root;

    if (data < root->data)
        root->left = deleteNode(root->left, data);
    else if (data > root->data)
        root->right = deleteNode(root->right, data);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            delete temp;
        } else {
            Node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getBalanceFactor(root);

    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return leftLeftRotate(root);

    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return leftLeftRotate(root);
    }

    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return rightRightRotate(root);

    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return rightRightRotate(root);
    }

    return root;
}

Node* searchNode(Node* root, int data) {
    if (root == NULL || root->data == data)
        return root;

    if (root->data < data)
        return searchNode(root->right, data);

    return searchNode(root->left, data);
}

void inorderTraversal(Node* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        cout << root->data << " ";
        inorderTraversal(root->right);
    }
}

int main() {
    Node* root = NULL;
    root = insertNode(root, 10);
    root = insertNode(root, 20);
    root = insertNode(root, 30);
    root = insertNode(root, 40);
    root = insertNode(root, 50);
    root = insertNode(root, 25);

    inorderTraversal(root);
    cout << endl;

    root = deleteNode(root, 30);

    inorderTraversal(root);
    cout << endl;

    Node* searchResult = searchNode(root, 40);
    if (searchResult != NULL)
        cout << "Node found." << endl;
    else
        cout << "Node not found." << endl;

    return 0;
}
