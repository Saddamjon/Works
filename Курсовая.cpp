#include <iostream>
#include <fstream>
#include <string>

using namespace std;

template <class T>
class Stack
{
private:
	struct Node
	{
		T data;
		Node* next;
	};

	Node* top;

public:
	Stack()
	{
		top = NULL;
	}

	void push(const T& value)
	{
		Node* newNode = new Node;
		newNode->data = value;
		newNode->next = top;
		top = newNode;
	}

	T pop()
	{
		if (top)
		{
			T value = top->data;
			Node* temp = top;
			top = top->next;
			delete temp;
			return value;
		}
	}

	bool isEmpty()
	{
		if (top == NULL)
			return true;
		return false;
	}

	~Stack()
	{
		while (top)
			pop();
	}
};

class MorseTree
{
private:
	struct Node
	{
		char symbol;
		Node* left;
		Node* right;
	};

	Node* root;

	void addNode(char symbol, const string& code) {
		Node* curr = root;
		for (char x : code)
		{
			if (x == '.')
			{
				if (!curr->left)
				{
					curr->left = new Node{ '.', NULL, NULL };
				}
				curr = curr->left;
			}
			else if (x == '-')
			{
				if (!curr->right)
				{
					curr->right = new Node{ '-', NULL, NULL };
				}
				curr = curr->right;
			}
		}

		curr->symbol = symbol;
	}
	void deleteTree(Node* node) {
		if (node->left) deleteTree(node->left);
		if (node->right) deleteTree(node->right);
		delete node;
	}

public:
	MorseTree() {
		root = new Node{ '-', NULL, NULL };
		addNode('A', ".-");
		addNode('B', "-...");
		addNode('C', "-.-.");
		addNode('D', "-..");
		addNode('E', ".");
		addNode('F', "..-.");
		addNode('G', "--.");
		addNode('H', "....");
		addNode('I', "..");
		addNode('J', ".---");
		addNode('K', "-.-");
		addNode('L', ".-..");
		addNode('M', "--");
		addNode('N', "-.");
		addNode('O', "---");
		addNode('P', ".--.");
		addNode('Q', "--.-");
		addNode('R', ".-.");
		addNode('S', "...");
		addNode('T', "-");
		addNode('U', "..-");
		addNode('V', "...-");
		addNode('W', ".--");
		addNode('X', "-..-");
		addNode('Y', "-.--");
		addNode('Z', "--..");
	}

	template <class T>
	void getCode(Node* root, Stack<T>& s, char symbol, char c, ofstream& out)
	{
		if (root == NULL) return;

		s.push(c);
		if (root->symbol == symbol)
		{
			string str = "";
			while (!s.isEmpty())
				str = s.pop() + str;

			out << str << " ";
			return;
		}
		getCode(root->left, s, symbol, '.', out);
		getCode(root->right, s, symbol, '-', out);
		s.pop();
	}
	void getMorseCode(char c, ofstream& out)
	{
		Stack<char> stack;
		Node* curr = root;

		getCode(curr, stack, c, ' ', out);
	}

	char getSymbolFromMorseCode(const string& code)
	{
		Node* curr = root;

		for (char i : code)
		{
			if (i == '.') curr = curr->left;
			else if (i == '-') curr = curr->right;
		}

		return curr->symbol;
	}

	/*Node* get() { return root; }
	void printTree(Node* root)
	{
		if (root == NULL) return;
		cout << root->symbol << " ";
		printTree(root->left);
		printTree(root->right);

	}*/
	~MorseTree() {
		deleteTree(root);
	}
};



int main()
{
	MorseTree morseTree;/*
	morseTree.printTree(morseTree.get());*/





	ifstream in1("input.txt");
	ofstream out1("encoded.txt");

	string word;
	while (in1 >> word)
		for (char i : word)
			morseTree.getMorseCode(i, out1);

	in1.close();
	out1.close();





	ifstream in2("encoded.txt");
	ofstream out2("decoded.txt");

	string code;
	while (in2 >> code)
		out2 << morseTree.getSymbolFromMorseCode(code) << " ";

	in2.close();
	out2.close();
}