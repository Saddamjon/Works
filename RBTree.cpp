#include <iostream>
#include <string> 

enum Color { RED, BLACK };

struct Node {
	int data;
	Color color;
	Node* left;
	Node* right;
	Node* parent;

	// Конструктор для узла
	Node(int val) : data(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
	Node* root;

	// Вспомогательные приватные методы
	void rotateLeft(Node* node);
	void rotateRight(Node* node);
	void fixInsertion(Node* node);
	Node* searchHelper(Node* node, int val);
	Node* minimum(Node* node);
	void deletenodeHelper(Node* node, int key);
	void transplant(Node* u, Node* v);
public:
	// Конструктор
	RedBlackTree() : root(nullptr) {}

	// Публичные методы
	void insert(int val);
	void print();
	Node* search (int val);
};

// Вспомогательные методы для удаления

Node* RedBlackTree::minimum(Node* node) {
	while (node->left != nullptr) {
		node = node->left;
	}
	return node;
}

void RedBlackTree::transplant(Node* u, Node* v) {
	if (u->parent == nullptr) {
		root = v;
	}
	else if (u == u->parent->left) {
		u->parent->left = v;
	}
	else {
		u->parent->right = v;
	}
	if (v != nullptr) {
		v->parent = u->parent;
	}
}

void RedBlackTree::deleteNodeHelper(Node* node, int key) {
	Node* z = nullptr;
	Node* x, *y;
	while (node != nullptr) {
		if (node->data == key) {
			z = node;
		}
		if (node->data <= key) {
			node = node->right;
		}
		else {
			node = node->left;
		}
	}

	if (z == nullptr) {
		std::cout << "Couldn't find key in the tree" << std::endl;
		return;
	}

	y = z;
	Color y_original_color = y->color;
	if (z->left == nullptr) {
		x = z->right;
		transplant(z, z->right);
	}
	else if (z->right == nullptr) {
		x = z->left;
		transplant(z, z->left);
	}
	else {
		y = minimum(z->right);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z) {
			if (x != nullptr) {
				x->parent = y;
			}
		}
		else {
			transplant(y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		transplant(z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	delete z;
	if (y_original_color == BLACK) {
		fixDeletion(x);
	}
}

// Метод удаления узла
void RedBlackTree::remove(int val) {
	deleteNodeHelper(root, val);
}

// Метод исправления после удаления
void RedBlackTree::fixDeletion(Node* x) {
	Node* s;
	while (x != root && (x == nullptr || x->color == BLACK)) {
		if (x == x->parent->left) {
			s = x->parent->right;
			if (s->color == RED) {
				s->color = BLACK;
				x->parent->color = RED;
				rotateLeft(x->parent);
				s = x->parent->right;
			}

			if ((s->left == nullptr || s->left->color == BLACK) &&
				(s->right == nullptr || s->right->color == BLACK)) {
				s->color = RED;
				x = x->parent;
			}
			else {
				if (s->right == nullptr || s->right->color == BLACK) {
					s->left->color = BLACK;
					s->color = RED;
					rotateRight(s);
					s = x->parent->right;
				}

				s->color = x->parent->color;
				x->parent->color = BLACK;
				if (s->right != nullptr) s->right->color = BLACK;
				rotateLeft(x->parent);
				x = root;
			}
		}
		else {
			s = x->parent->left;
			if (s->color == RED) {
				s->color = BLACK;
				x->parent->color = RED;
				rotateRight(x->parent);
				s = x->parent->left;
			}

				if ((s->left == nullptr || s->left->color == BLACK) &&
					(s->right == nullptr || s->right->color == BLACK)) {
					s->color = RED;
					x = x->parent;
				}
				else {
					if (s->left == nullptr || s->left->color == BLACK) {
						s->right->color = BLACK;
						s->color = RED;
						rotateLeft(s);
						s = x->parent->left;
					}

					s->color = x->parent->color;
					x->parent->color = BLACK;
					if (s->left != nullptr) s->left->color = BLACK;
					rotateRight(x->parent);
					x = root;
				}
		}
	}
	if (x != nullptr) x->color = BLACK;
}

// Реализация метода поиска
Node* RedBlackTree::search(int val) {
	return searchHelper(root, val);
}

Node* RedBlackTree::searchHelper(Node* node, int val) {
	// Если узел пустой или значение найдено
	if (node == nullptr || node->data == val)
		return node;

	// Если значение меньше текущего узла, ищем в левом поддереве
	if (val < node->data)
		return searchHelper(node->left, val);

	// Если значение больше текущего узла, ищем в правом поддереве
	return searchHelper(node->right, val);
}

// Реализация метода печати
void RedBlackTree::print() {
	printHelper(root, "", true);
}

void RedBlackTree::printHelper(Node* root, std::string indent, bool last) {
	if (root != nullptr) {
		std::cout << indent;
		if (last) {
			std::cout << "R----";
			indent += "   ";
		}
		else {
			std::cout << "L----";
			indent += "|  ";
		}

		std::string sColor = root->color == RED ? "RED" : "BLACK";
		std::cout << root->data << " (" << sColor << ")" << std::endl;

		printHelper(root->left, indent, false);
		printHelper(root->right, indent, true);
	}
}

Node* RedBlackTree::search(int val) {
	return searchHelper(root, val);
}
Node* RedBlackTree::searchHelper(Node* node, int val)
{
	if (node == nullptr || node->data == val)
		return node;
	if (val < node->data)
		return searchHelper(node->left, val);
	return searchHelper(node->right, val);
}
// Методы поворота
void RedBlackTree::rotateLeft(Node* node) {
	Node* rightChild = node->right;  /// Сохраняем правого потомка узла
	node->right = rightChild->left;  /// Переносим левого потомка правого потомка на место правого потомка

	if (rightChild->left != nullptr) {
		rightChild->left->parent = node;  /// Обновляем родителя левого потомка правого потомка
	}

	rightChild->parent = node->parent;  /// Обновляем родителя правого потомка

	if (node->parent == nullptr) {
		root = rightChild;  /// Если узел был корневым, обновляем корень
	}
	else if (node == node->parent->left) {
		node->parent->left = rightChild;  /// Обновляем левого потомка родителя узла
	}
	else {
		node->parent->right = rightChild;  /// Обновляем правого потомка родителя узла
	}

	rightChild->left = node;  /// Переносим узел влево от правого потомка
	node->parent = rightChild;  /// Обновляем родителя узла
}


void RedBlackTree::rotateRight(Node* node) {
	Node* leftChild = node->left;
	/// Сохраняем левого потомка узла node
	node->left = leftChild->right;
	/// Переносим правого потомка левого потомка в качестве нового левого потомка узла node
	if (leftChild->right != nullptr) {
		leftChild->right->parent = node;
	}
	/// Обновляем родителя правого потомка левого потомка, если он существует

	leftChild->parent = node->parent;
	/// Обновляем родителя левого потомка как родителя узла node
	if (node->parent == nullptr) {
		root = leftChild;
	}
	/// Если узел node является корнем, обновляем корень на левого потомка
	else if (node == node->parent->right) {
		node->parent->right = leftChild;
	}
	/// Если узел node является правым потомком своего родителя, обновляем правого потомка родителя
	else {
		node->parent->left = leftChild;
	}
	/// В противном случае обновляем левого потомка родителя

	leftChild->right = node;
	/// Устанавливаем узел node в качестве правого потомка левого потомка
	node->parent = leftChild;
	/// Обновляем родителя узла node как левый потомок

}


/// Метод для исправления возможного нарушения правил
void RedBlackTree::fixInsertion(Node* node) {
	while (node != root && node->parent->color == RED) {/// Проверяем, что текущий узел не является корнем и его родительский узел красный
		if (node->parent == node->parent->parent->left) {/// Если родитель текущего узла является левым потомком своего дедушки
			Node* uncle = node->parent->parent->right;
			if (uncle != nullptr && uncle->color == RED) {/// Если дядя текущего узла существует и является красным
				node->parent->color = BLACK;
				uncle->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}
			else {
				if (node == node->parent->right) {/// Если текущий узел является правым потомком своего родителя
					node = node->parent;
					rotateLeft(node);
				}
				/// Выполняем повороты и перекраску
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				rotateRight(node->parent->parent);
			}
		}
		else {
			/// Если родитель текущего узла является правым потомком своего дедушки
			Node* uncle = node->parent->parent->left;
			if (uncle != nullptr && uncle->color == RED) {
				/// Если дядя текущего узла существует и является красным
				node->parent->color = BLACK;
				uncle->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}
			else {
				if (node == node->parent->left) {/// Если текущий узел является левым потомком своего родителя
					node = node->parent;
					rotateRight(node);
				}
				/// Выполняем повороты и перекраску
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				rotateLeft(node->parent->parent);
			}
		}
	}
	root->color = BLACK;
}


// Метод для вставки узла в дерево
void RedBlackTree::insert(int val) {
	Node* newNode = new Node(val);
	Node* parent = nullptr;
	Node* current = root;

	// Находим место для вставки
	while (current != nullptr) {
		parent = current;
		if (newNode->data < current->data) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	// Вставляем узел
	newNode->parent = parent;
	if (parent == nullptr) {
		root = newNode;
	}
	else if (newNode->data < parent->data) {
		parent->left = newNode;
	}
	else {
		parent->right = newNode;
	}

	// Проверяем и исправляем возможное нарушение правил
	fixInsertion(newNode);
}

// Метод для вывода дерева на экран
void printHelper(Node* root, std::string indent, bool last) {
	if (root != nullptr) {
		std::cout << indent;
		if (last) {
			std::cout << "R----";
			indent += "   ";
		}
		else {
			std::cout << "L----";
			indent += "|  ";
		}

		std::string sColor = root->color == RED ? "RED" : "BLACK";
		std::cout << root->data << " (" << sColor << ")" << std::endl;

		printHelper(root->left, indent, false);
		printHelper(root->right, indent, true);
	}
}

// Публичный метод для вызова вывода дерева
void RedBlackTree::print() {
	printHelper(root, "", true);
}


int main() {
	RedBlackTree tree;
	tree.insert(10);
	tree.insert(20);
	tree.insert(30);
	tree.insert(15);
	tree.insert(25);
	tree.insert(1);
	tree.insert(2);
	tree.insert(3);
	tree.insert(4);
	tree.insert(6);
	tree.insert(7);
	tree.insert(8);
	tree.insert(9);
	tree.insert(24);
	tree.print();

	Node* found = tree.search(15);
	if (found != nullptr) {
		std::cout << "Node found: " << found->data << std::endl;
	}
	else {
		std::cout << "Node not found" << std::endl;
	}

	return 0;
}