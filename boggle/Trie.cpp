#include "stdafx.h"

void Trie::addWord(const std::string &word)
{
	Node* current = _root;

	if (word.length() == 0)
	{
		return;
	}

	for (int i = 0; i < word.length(); ++i)
	{
		Node* child = current->find(word[i]);
		if (child)
		{
			current = child;
		}
		else
		{
			current->append(word[i]);
			current = current->find(word[i]);
		}
	}

	current->mark();
}

bool Trie::isValidPrefix(const std::string &word)
{
	Node* current = _root;

	for (int i = 0; i < word.length(); ++i)
	{
		Node* temp = current->find(word[i]);
		if (!temp)
		{
			return false;
		}

		current = temp;
	}

	return true;
}

bool Trie::hasWord(const std::string &word)
{
	Node* current = _root;

	for (int i = 0; i < word.length(); ++i)
	{
		Node* temp = current->find(word[i]);
		if (!temp)
		{
			return false;
		}

		current = temp;
	}

	if (current->isMarked())
	{
		return true;
	}

	return false;
}