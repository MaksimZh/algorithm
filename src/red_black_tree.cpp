#include <iostream>
#include <cassert>
#include <memory>


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
        Cursor(Node* node, Position position)
            : node(node), position(position)
        {
            switch(position) {
            case Position::self:
                return;
            case Position::left:
                assert(node != nullptr);
                assert(node->left == nullptr);
                return;
            case Position::right:;
                assert(node != nullptr);
                assert(node->right == nullptr);
                return;
            }
        }

        bool isNode() {
            return position == Position::self && node != nullptr;
        }

        Cursor parent() {
            assert(isNode);
            return Cursor(node->parent.get(), Position::self);
        }

        Cursor left() {
            assert(isNode);
            if (node->left != nullptr) {
                return Cursor(node->left.get(), Position::self);
            }
            return Cursor(node.get(), Position::left);
        }

        Cursor right() {
            assert(isNode);
            if (node->right != nullptr) {
                return Cursor(node->right.get(), Position::self);
            }
            return Cursor(node.get(), Position::right);
        }

        T& value() {
            assert(isNode());
            return node->value;
        }
    
    private:
        Node* node = nullptr;
        Position position = Position::self;

        friend class BinaryTree;
    };

    size_t size() {
        return numOfNodes;
    }

    Cursor root() {
        return Cursor(rootNode.get(), Position::self);
    }

    void createNode(Cursor c) {
        assert(!c.isNode());
        ++numOfNodes;
        switch(c.position) {
        case Position::self:
            assert(rootNode == nullptr);
            rootNode = std::make_unique<Node>();
            return;
        case Position::left:
            assert(c.node != nullptr);
            c.node->left = std::make_unique<Node>();
            c.node->left->parent = c.node;
            return;
        case Position::right:;
            assert(c.node != nullptr);
            c.node->right = std::make_unique<Node>();
            c.node->right->parent = c.node;
            return;
        }
    }

private:
    std::unique_ptr<Node> rootNode;
    size_t numOfNodes = 0;
};


template <typename T>
class RedBlackTree {
private:
    struct ColorValue {
        bool red;
        T value;
    };
public:
    size_t size() {
        return tree.size();
    }

    BinaryTree<ColorValue>::Cursor root() {
        return tree.root();
    }

    void insert(T&& value) {}

private:
    BinaryTree<ColorValue> tree;
};


int main() {
    RedBlackTree<int> tree;
    tree.insert(100);
    std::cout << tree.root().value().value << std::endl;
    return 0;
}
