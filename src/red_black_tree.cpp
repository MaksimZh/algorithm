#include <iostream>
#include <iomanip>
#include <cassert>
#include <memory>
#include <queue>

#include "vectors.hpp"

template <typename T>
class BinaryTree {
private:
    struct Node {
        Node* parent = nullptr;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        T value;
    };

public:
    enum Position { self, left, right };

    class Cursor {
    public:
        Cursor() = default;
        bool operator==(const Cursor&) const = default;
        Cursor(Node* node, Position position)
            : _node(node), _position(position)
        {
            switch(position) {
            case Position::self:
                return;
            case Position::left:
                assert(node != nullptr);
                assert(node->left == nullptr);
                return;
            case Position::right:
                assert(node != nullptr);
                assert(node->right == nullptr);
                return;
            }
        }

        Node* node() {
            return _node;
        }

        Position position() {
            return _position;
        }

        bool isNode() {
            return _position == Position::self && _node != nullptr;
        }

        bool isLeft() {
            switch(_position) {
            case Position::self: {
                Cursor p = parent();
                return p.isNode() && p._node->left.get() == _node;
            }
            case Position::left:
                return true;
            case Position::right:
                return false;
            default:
                return false;
            }
        }

        bool isRight() {
            switch(_position) {
            case Position::self: {
                Cursor p = parent();
                return p.isNode() && p._node->right.get() == _node;
            }                
            case Position::left:
                return false;
            case Position::right:
                return true;
            default:
                return false;
            }
        }

        Cursor parent() {
            if(!isNode()) {
                return Cursor(_node, Position::self);
            }
            return Cursor(_node->parent, Position::self);
        }

        Cursor left() {
            assert(isNode());
            if (_node->left != nullptr) {
                return Cursor(_node->left.get(), Position::self);
            }
            return Cursor(_node, Position::left);
        }

        Cursor right() {
            assert(isNode());
            if (_node->right != nullptr) {
                return Cursor(_node->right.get(), Position::self);
            }
            return Cursor(_node, Position::right);
        }

        Cursor sibling() {
            Cursor p = parent();
            assert(p.isNode());
            if (isLeft()) {
                return p.right();
            }
            return p.left();
        }

        Cursor uncle() {
            return parent().sibling();
        }

        T& value() {
            assert(isNode());
            return _node->value;
        }
    
    private:
        Node* _node = nullptr;
        Position _position = Position::self;
    };

    size_t size() {
        return numOfNodes;
    }

    Cursor root() {
        return Cursor(rootNode.get(), Position::self);
    }

    Cursor createNode(Cursor c) {
        assert(!c.isNode());
        ++numOfNodes;
        switch(c.position()) {
        case Position::self:
            assert(rootNode == nullptr);
            rootNode = std::make_unique<Node>();
            return root();
        case Position::left:
            assert(c.node() != nullptr);
            c.node()->left = std::make_unique<Node>();
            c.node()->left->parent = c.node();
            return Cursor(c.node()->left.get(), Position::self);
        case Position::right:
            assert(c.node() != nullptr);
            c.node()->right = std::make_unique<Node>();
            c.node()->right->parent = c.node();
            return Cursor(c.node()->right.get(), Position::self);
        default:
            return Cursor();
        }
    }

    Cursor rotateLeft(Cursor c) {
        Node* node = c.node();
        Node* parent = node->parent;
        std::unique_ptr<Node> right = std::move(node->right);
        assert(right);
        right->parent = nullptr;
        node->right = std::move(right->left);
        if (node->right) {
            node->right->parent = node;
        }
        if (!parent) {
            right->left = std::move(rootNode);
            right->left->parent = right.get();
            rootNode = std::move(right);
            return root();
        }
        if (c.isLeft()) {
            right->left = std::move(parent->left);
            right->left->parent = right.get();
            parent->left = std::move(right);
            parent->left->parent = parent;
            return Cursor(parent->left.get(), Position::self);
        }
        right->left = std::move(parent->right);
        right->left->parent = right.get();
        parent->right = std::move(right);
        parent->right->parent = parent;
        return Cursor(parent->right.get(), Position::self);
    }

    Cursor rotateRight(Cursor c) {
        Node* node = c.node();
        Node* parent = node->parent;
        std::unique_ptr<Node> left = std::move(node->left);
        assert(left);
        left->parent = nullptr;
        node->left = std::move(left->right);
        if (node->left) {
            node->left->parent = node;
        }
        if (!parent) {
            left->right = std::move(rootNode);
            left->right->parent = left.get();
            rootNode = std::move(left);
            return root();
        }
        if (c.isRight()) {
            left->right = std::move(parent->right);
            left->right->parent = left.get();
            parent->right = std::move(left);
            parent->right->parent = parent;
            return Cursor(parent->right.get(), Position::self);
        }
        left->right = std::move(parent->left);
        left->right->parent = left.get();
        parent->left = std::move(left);
        parent->left->parent = parent;
        return Cursor(parent->left.get(), Position::self);
    }

private:
    std::unique_ptr<Node> rootNode;
    size_t numOfNodes = 0;
};


template <typename T>
class SortedBinaryTree {
public:
    using Cursor = BinaryTree<T>::Cursor;
    
    size_t size() {
        return tree.size();
    }

    Cursor root() {
        return tree.root();
    }

    template <typename K>
    Cursor find(const K& key) {
        return findFrom(tree.root(), key);
    }

    Cursor newNode(Cursor c) {
        assert(!c.isNode());
        return tree.createNode(c);
    }

    Cursor rotateLeft(Cursor c) {
        return tree.rotateLeft(c);
    }

    Cursor rotateRight(Cursor c) {
        return tree.rotateRight(c);
    }

private:
    BinaryTree<T> tree;

    template <typename K>
    Cursor findFrom(Cursor c, const K& key) {
        if (!c.isNode()) {
            return c;
        }
        if (c.value() == key) {
            return c;
        }
        if (c.value() < key) {
            return findFrom(c.right(), key);
        }
        return findFrom(c.left(), key);
    }
};


template <typename T>
class RedBlackTree {
private:
    struct ColorValue {
        bool red;
        T value;

        bool operator==(const T& key) const {
            return value == key;
        }

        bool operator<(const T& key) const {
            return value < key;
        }
    };

    using Cursor = BinaryTree<ColorValue>::Cursor;

public:
    size_t size() {
        return tree.size();
    }

    void insert(T&& value) {
        Cursor c = tree.find(value);
        if (c.isNode()) {
            c.value().value = std::move(value);
            return;
        }
        c = tree.newNode(c);
        c.value() = ColorValue(true, std::move(value));
        fixRed(c);
    }

    void print() {
        std::queue<Cursor> items;
        items.push(tree.root());
        int counter = 1;
        int delta = 0;
        while (!items.empty()) {
            Cursor c = items.front();
            items.pop();
            if (!c.isNode()) {
                std::cout << "  X   ";
            } else {
                if (c.value().red) {
                    std::cout << "(" << std::setw(3) << c.value().value <<") ";
                } else {
                    std::cout << " " << std::setw(3) << c.value().value << "  ";
                }
                items.push(c.left());
                items.push(c.right());
                delta += 2;
            }
            if (--counter == 0) {
                counter = delta;
                delta = 0;
                std::cout << std::endl;
            }
        }
    }

private:
    SortedBinaryTree<ColorValue> tree;

    void fixRed(Cursor c) {
        assert(c.value().red);
        if (c == tree.root()) {
            c.value().red = false;
            return;
        }
        Cursor parent = c.parent();
        if (!parent.value().red) {
            return;
        }
        Cursor uncle = c.uncle();
        if (uncle.isNode() && uncle.value().red) {
            parent.value().red = false;
            uncle.value().red = false;
            Cursor grandParent = parent.parent();
            grandParent.value().red = true;
            fixRed(grandParent);
            return;
        }
        if (c.isRight() && parent.isLeft()) {
            parent = tree.rotateLeft(parent);
            fixRed(parent.left());
            return;
        }
        if (c.isLeft() && parent.isRight()) {
            parent = tree.rotateRight(parent);
            fixRed(parent.right());
            return;
        }
        if (c.isLeft() && parent.isLeft()) {
            Cursor grandParent = parent.parent();
            grandParent.value().red = true;
            parent.value().red = false;
            tree.rotateRight(grandParent);
            return;
        }
        if (c.isRight() && parent.isRight()) {
            Cursor grandParent = parent.parent();
            grandParent.value().red = true;
            parent.value().red = false;
            tree.rotateLeft(grandParent);
            return;
        }
    }
};


int main() {
    RedBlackTree<int> tree;
    IntVec values = randomVector(50);
    tree.print();
    for (auto v: values) {
        std::cout << "----------------------------------------" << std::endl;
        tree.insert(std::move(v));
        tree.print();
    }
    return 0;
}
