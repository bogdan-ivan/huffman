#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>

int arraytoint(int arr[], int n)
{
	std::stringstream ss;

	for (int i = 0; i < n; i++)
	{
		ss << arr[i];
	}

	int result;
	ss >> result;
	return result;
}

int CautareBinaraRecursivaPosition(int array[], int element, int start, int end)
{
	if (start > end)
	{
		return 0;
	}
	int mijloc = (start + end) / 2;
	if (array[mijloc] == element)
	{
		return mijloc;
	}
	if (array[mijloc] > element)
	{
		return CautareBinaraRecursivaPosition(array, element, start, mijloc - 1);
	}
	else
	{
		return CautareBinaraRecursivaPosition(array, element, mijloc + 1, end);
	}
}

struct node
{
	char caracter = '\0';
	int frecventa;
	node *left = nullptr;
	node *right = nullptr;
	std::string codfix;
	int codvar = 0;
};

node* newNode(int frecventa)
{
	node* node = new struct node;
	node->frecventa = frecventa;
	return node;
}

node* newParentNode(node *child1, node *child2)
{
	node* node = new struct node;
	node->frecventa = child1->frecventa + child2->frecventa;
	node->left = child1;
	node->right = child2;
	return node;
}

void freadTable(std::vector<node> &vec, std::fstream &file)
{
	char c;
	int index = 0;
	for (index;; index++)
	{
		file >> c;
		if (isdigit(c))
		{
			break;
		}
	}
	vec.resize(index);
	file.seekg(0, file.beg);
	for (int i = 0; i < vec.size(); i++)
	{
		file >> vec[i].caracter;
	}
	for (int i = 0; i < vec.size(); i++)
	{
		file >> vec[i].frecventa;
	}
	for (int i = 0; i < vec.size(); i++)
	{
		file >> vec[i].codfix;
	}
}

void printVectorChar(std::vector<node> &vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		std::cout << vec[i].caracter << " ";
	}
	std::cout << std::endl;
}

void printVectorFreq(std::vector<node> &vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		std::cout << vec[i].frecventa << " ";
	}
	std::cout << std::endl;
}

bool comparator(node a, node b)
{
	return a.frecventa > b.frecventa;
}

void reInsert(std::vector<node> &leafs, node* parent)
{
	node temp;
	temp.caracter = parent->caracter;
	temp.frecventa = parent->frecventa;
	temp.left = parent->left;
	temp.right = parent->right;
	temp.codfix = parent->codfix;
	temp.codvar = parent->codvar;

	int i = 0;
	for (i; i < leafs.size(); i++)
	{
		if (leafs[i].frecventa < temp.frecventa)
		{
			break;
		}
	}

	leafs.emplace(leafs.begin() + i, temp);
}

node* createTree(std::vector<node> &leafs)
{
	while (leafs.size() >= 2)
	{
		node* child1 = newNode(0);
		child1->caracter = leafs.back().caracter;
		child1->frecventa = leafs.back().frecventa;
		child1->left = leafs.back().left;
		child1->right = leafs.back().right;
		child1->codfix = leafs.back().codfix;
		leafs.pop_back();
		node* child2 = newNode(0);
		child2->caracter = leafs.back().caracter;
		child2->frecventa = leafs.back().frecventa;
		child2->left = leafs.back().left;
		child2->right = leafs.back().right;
		child2->codfix = leafs.back().codfix;
		leafs.pop_back();
		node* p1 = newParentNode(child1, child2);
		reInsert(leafs, p1);
	}
	return &leafs[0];
}

bool isLeaf(node* node)
{
	if ((node->left == nullptr)&&(node->right == nullptr))
	{
		return true;
	}
	return false;
}

void printArray(int arr[], int n)
{
	for (int i = 0; i < n; ++i)
	{
		std::cout << arr[i];
	}
	std::cout << std::endl;
}

void printCodes(node* root, int array[], int top, std::fstream &file, std::vector<node> &leafs)

{
	if (root->left)
	{
		array[top] = 0;
		printCodes(root->left, array, top + 1, file, leafs);
	}

	if (root->right) 
	{
		array[top] = 1;
		printCodes(root->right, array, top + 1, file, leafs);
	}

	if (isLeaf(root)) 
	{
		std::cout << root->codfix << " => ";
		//std::cout << root->caracter << " = ";
		//printArray(array, top);
		root->codvar = arraytoint(array, top);
		std::cout << root->caracter << " = " << root->codvar << std::endl;
	    file << root->codvar << " ";
		reInsert(leafs, root);
	}
}

std::string codificare(std::vector<node> leafs)
{
	std::string message;
	std::string code;
	std::cout << "Introduceti mesajul: ";
	std::getline(std::cin, message);

	for (int i = 0; i < message.length(); i++)
	{
		for (int j = 0; j < leafs.size(); j++)
		{
			if (message[i] == leafs[j].caracter)
			{
				code += leafs[j].codvar;
			}
		}
	}
	return code;
}

void decodificare()
{

}

std::string codeToChar(std::vector<node> leafs, int lungime)
{
	std::string code;
	std::string tempcode;
	std::string message;
	std::cout << "Introduceti codul: ";
	std::getline(std::cin, code);
	int k = 0;
	for (int i = 0; i < code.length()/lungime; i++)
	{
		for (int j = 0; j < lungime; j++)
		{
			tempcode += code[k];
			k++;
		}
		for (int m = 0; m < leafs.size(); m++)
		{
			if (tempcode == leafs[m].codfix)
			{
				message += leafs[m].caracter;
				break;
			}
		}
		tempcode = "";
	}
	return message;
}

std::string charToCode(std::vector<node> leafs)
{
	std::string message;
	std::string code;
	std::cout << "Introduceti mesajul: ";
	std::getline(std::cin, message);

	for (int i = 0; i < message.length(); i++)
	{
		for (int j = 0; j < leafs.size(); j++)
		{
			if (message[i] == leafs[j].caracter)
			{
				code += leafs[j].codfix;
			}
		}
	}
	return code;
}

void main()
{
	int lungimeFixa = 0;
	int codes[10];

	std::fstream file;
	file.open("tabel.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	std::vector<node> leafs;
	freadTable(leafs, file);
	lungimeFixa = leafs[0].codfix.length();
	printVectorChar(leafs);
	printVectorFreq(leafs);
	std::sort(leafs.begin(), leafs.end(), comparator);
	printVectorChar(leafs);
	printVectorFreq(leafs);


	node* root = createTree(leafs);

	/*printVectorChar(leafs);
	printVectorFreq(leafs);*/
	
	/*std::cout << root->left->frecventa << "   " << root->right->frecventa << std::endl;
	std::cout << "    " << root->right->left->frecventa << " " << root->right->right->frecventa << std::endl;
	std::cout << "" << root->right->left->left->frecventa << " " << root->right->left->right->frecventa << " " << root->right->right->left->frecventa << " " << root->right->right->right->frecventa << std::endl;
	std::cout << "     " << root->right->right->left->left->frecventa << " " << root->right->right->left->right->frecventa << std::endl;
	*/
	//std::cout << std::boolalpha << isLeaf(root->right->right->right) << std::endl;

	printCodes(root, codes, 0, file, leafs);
	file.put('x'); // ?
	file.close();

	printVectorChar(leafs);
	printVectorFreq(leafs);


#if 0

	std::string test = charToCode(leafs);
	std::cout << test << std::endl;
	std::string test2 = codeToChar(leafs, lungimeFixa);
	std::cout << test2 << std::endl;
	std::string test3 = codificare(leafs);
	std::cout << test3 << std::endl;

#endif 
}