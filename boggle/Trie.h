#include <string>
#include <unordered_map>

class Node
{
private:
	char _letter;
	bool _marked;
	std::unordered_map<char, Node*> _children;

public:
	Node() : _letter(' '), _marked(false) {}
	Node(const char &letter) : _letter(letter), _marked(false) {}

	char getLetter() { return _letter; }
	void setLetter(const char &letter) { _letter = letter; }
	bool isMarked() { return _marked; }
	void mark() { _marked = true; }

	Node* find(const char &letter)
	{
		if (_children.count(letter) > 0)
		{
			return _children[letter];
		}
		return nullptr;
	}
	void append(const char &letter)
	{
		Node* newNode = new Node(letter);
		const std::pair<char, Node*> newChild(letter, newNode);
		_children.insert(newChild);
	}


};

class Trie
{
public:
	Trie() : _root(new Node()) {}

	void addWord(const std::string &word);
	bool isValidPrefix(const std::string &word);
	bool hasWord(const std::string &word);

private:
	Node* _root;
};