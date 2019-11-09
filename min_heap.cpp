#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <utility>
#include <vector>

struct Node
{
	std::string value;
	long long int key;
	Node(long long int, std::string);
};

Node::Node(long long int set_key, std::string set_value) :key{ set_key }, value{ set_value } {}

class MinHeap 
{
	std::vector<Node> array;
	std::unordered_map<long long int, long long int> map;
	void heapify(long long int i);
	void up(long long int index);

public:
	void add(long long int K, std::string &&V);
	void set(long long int K, std::string &&V);
	std::tuple<long long int, long long int, std::string> min();
	std::tuple<long long int, long long int, std::string> max();
	std::tuple<bool, long long int, std::string> search(long long int K);
	void Delete (long long int K);
	std::pair<long long int, std::string> extract();
	std::string print();
};

void MinHeap::heapify(long long int i)
{
	long long int left = 2 * i + 1;
	long long int right = 2 * i + 2;
	long long int tiny;
	if (left < array.size() && array[left].key < array[i].key)
		tiny = left;
	else tiny = i;
	if (right < array.size() && array[right].key < array[tiny].key) 
		tiny = right;
	if (tiny != i)
	{
		std::swap(array[i], array[tiny]);
		map.erase(map.find(array[i].key));
		map.erase(map.find(array[tiny].key));
		map.insert(std::make_pair(array[i].key, i));
		map.insert(std::make_pair(array[tiny].key, tiny));
		heapify(tiny);
	}
}

void MinHeap::up(long long int index)
{
	long long int parent_index = (index - 1) / 2;
	while ((index > 0) && array[index].key < array[parent_index].key)
	{
		std::swap(array[index], array[parent_index]);
		map.erase(map.find(array[parent_index].key));
		map.erase(map.find(array[index].key));
		map.insert(std::make_pair(array[index].key, index));
		map.insert(std::make_pair(array[parent_index].key, parent_index));
		index = parent_index;
		parent_index = (parent_index - 1) / 2;
	}
}

void MinHeap::add(long long int K, std::string&& V)
{
	if (std::get<0>(search(K))) throw std::logic_error("double add");
	Node new_node(K, V);
	long long int i = array.size();
	array.push_back(new_node);
	long long int index_parent = (i - 1) / 2;

	while (i >= 1 && array[index_parent].key >= K) 
	{
		array[i] = array[index_parent];
		map.erase(map.find(array[index_parent].key));
		map.insert(std::make_pair(array[i].key, i));
		i = index_parent;
		index_parent = (i - 1) / 2;
	}
	array[i] = new_node;
	map.insert(std::make_pair(new_node.key, i));
}

void MinHeap::set(long long int K, std::string&& V)
{
	std::tuple<bool, long long int, std::string> vertex = search(K);
	if (std::get<0>(vertex) == false) throw std::logic_error("heap have not vertex");
	array[std::get<1>(vertex)].value = V;
}

std::tuple<long long int, long long int, std::string> MinHeap::min()
{
	if (array.size() == 0) throw std::logic_error("heap is empty");
	return std::make_tuple(array[0].key,0 , array[0].value);
}

std::tuple<long long int, long long int, std::string> MinHeap::max()
{
	if (array.size() == 0) throw std::logic_error("heap is empty");
	Node node = *std::max_element(array.begin(), array.end(), [](Node  i, Node  j) {return i.key < j.key; });
	return std::make_tuple(node.key, (map[node.key]), node.value);
}

std::tuple<bool, long long int, std::string> MinHeap::search(long long int K)
{
	auto iterator = map.find(K);
	if (iterator != map.end()) 
	{
		return std::make_tuple(true, (*iterator).second, array[(*iterator).second].value);
	}
	else return std::make_tuple(false, 0, "");
}

void MinHeap::Delete(long long int K)
{
	auto iterator = map.find(K);
	if (iterator == map.end())throw std::logic_error("heap have not vertex");
	long long int index = (*iterator).second;
	if (index == array.size() - 1) 
	{
		map.erase(map.find(K));
		array.pop_back();
		return;
	}
	std::swap(array[index], array[array.size() - 1]);
	map.erase(map.find(array[index].key));
	map.erase(map.find(array[array.size() - 1].key));
	map.insert(std::make_pair(array[index].key, index));
	array.pop_back();
	if (array[index].key < array[(index - 1) / 2].key) 
		up(index);
	else heapify(index);
}

std::pair<long long int, std::string> MinHeap::extract()
{
	if (array.size() == 0) throw std::logic_error("heap is empty");
	Node min = array[0];
	std::pair<long long int, std::string> extr_vertex = std::make_pair(min.key, min.value);
	Delete(array[0].key);
	return extr_vertex;
}

std::string MinHeap::print()
{
	if (array.size() == 0) return "_";
	size_t counter_h = 1;
	std::string out = '[' + std::to_string(array[0].key) + ' ' + array[0].value + ']';
	if (array.size() > 1) out += '\n';
	for (size_t i = 1; i < array.size(); i++)
	{
		out+= '[' + std::to_string(array[i].key) + ' ' + array[i].value + ' ' + std::to_string(array[(i-1)/2].key) + ']';
		if ((((i + 1) & (i + 2)) == 0) && i < array.size() - 1)
		{
			out += '\n';
			counter_h++;
		}
		else if ((((i + 1) & (i + 2)) != 0)) out += ' ';
		if (i == array.size() - 1) 
		{
			while (((i + 1) & (i + 2)) != 0)
			{
				out += '_';
				++i;
				if (((i + 1) & (i + 2)) != 0) 
					out += ' ';
			}
		}
	}
	return out;
}

int main()
{
	std::string temp_1, temp_2, temp_3, output = "";
	MinHeap My_heap;

	std::getline(std::cin, temp_1, '\0');
	std::istringstream line_stream(temp_1);
	while (line_stream >> temp_1)
	{
		try
		{
			temp_2.clear(), temp_3.clear();
			if (temp_1 == "print" && line_stream.peek() == '\n')
			{
				output += My_heap.print() + '\n';
			}
			else if (temp_1 == "extract" && line_stream.peek() == '\n')
			{
				std::pair<long long int, std::string> pair_ex = My_heap.extract();
				output += std::to_string(pair_ex.first) + ' ' + pair_ex.second + '\n';
			}
			else if (temp_1 == "min" && line_stream.peek() == '\n')
			{
				std::tuple<long long int, long long int, std::string> tuple_min= My_heap.min();
				output += std::to_string(std::get<0>(tuple_min)) + ' ' + std::to_string(std::get<1>(tuple_min)) + ' '
					+ std::get<2>(tuple_min) + '\n';
			}
			else if (temp_1 == "max" && line_stream.peek() == '\n')
			{
				std::tuple<long long int, long long int, std::string> tuple_max = My_heap.max();
				output += std::to_string(std::get<0>(tuple_max)) + ' ' + std::to_string(std::get<1>(tuple_max)) + ' '
					+ std::get<2>(tuple_max) + '\n';
			}
			else if (temp_1 == "search" && line_stream.peek() != '\n' && line_stream >> temp_2 && line_stream.peek() == '\n')
			{
				long long search_ = std::stoll(temp_2);
				std::tuple<long long int, long long int, std::string> tuple_search = My_heap.search(search_);
				if (!std::get<0>(tuple_search)) output += "0\n";
				else output += std::to_string(std::get<0>(tuple_search)) + ' ' + std::to_string(std::get<1>(tuple_search)) + ' '
					+ std::get<2>(tuple_search) + '\n';
			}
			else if (temp_1 == "delete" && line_stream.peek() != '\n' && line_stream >> temp_2 && line_stream.peek() == '\n')
			{

				long long delete_ = std::stoll(temp_2);
				My_heap.Delete(delete_);
			}
			else if (temp_1 == "set" && line_stream.peek() != '\n' && line_stream >> temp_2 && line_stream.peek() != '\n' &&
				line_stream >> temp_3 && line_stream.peek() == '\n')
			{
				long long set_ = std::stoll(temp_2);
				My_heap.set(set_,std::move(temp_3));
			}
			else if (temp_1 == "add" && line_stream.peek() != '\n' && line_stream >> temp_2 && line_stream.peek() != '\n' &&
				line_stream >> temp_3 && line_stream.peek() == '\n')
			{
				long long add_ = std::stoll(temp_2);
				My_heap.add(add_, std::move(temp_3));
			}
			else
			{
				line_stream.ignore(256, '\n');
				output += "error\n";
			}
		}
		catch (...)
		{
			output += "error\n";
			line_stream.ignore(256, '\n');
		}
	}
	if (output != "") output.pop_back();
	std::cout << output;
	return 0;
}
