#pragma once

#include <functional>
#include <string>
#include <map>
#include <vector>

template <typename ObjectType, typename ...Args>
class Wrapper
{
private:
	ObjectType* Class;
	int(ObjectType::*func_method)(Args...);

	std::function<int(std::map<std::string, int>)> func_calculation;


	// function to decompress vector into Args argument list and call func_method function 
	template<std::size_t... S>
	int calculation_value(std::index_sequence<S...>) {
		//values_of_args[S]... unzips vector in Args
		//(Class->*func_method) call method passed class
		return (Class->*func_method)(values_of_args[S]...);  
	}

public:
	
	std::map <std::string, int> map_args;
	std::vector<int> values_of_args;

	Wrapper(ObjectType* my_class, int(ObjectType::* Function)(Args...), std::map <std::string, int> arg_map_from)
	{
		Class = my_class;
		func_method = Function;
		map_args = arg_map_from;

		//link method function with dictionary arguments
		func_calculation = [this](std::map<std::string, int> args_map_to_cal)
			{
				if (args_map_to_cal.size() != this->map_args.size())
				{
					throw std::exception();
				}

				for (auto& elem_args : args_map_to_cal)
				{
					if (this->map_args.find(elem_args.first) == this->map_args.end()) //не нашли такой аргумент в изначальном
					{
						throw std::exception();
					}

					this->values_of_args.push_back(elem_args.second);
				}

				//std::make_index_sequence<sizeof ...(Args)>() is needed to create an index sequence 
				//elements of the argument value vector to transform the vector into a list of function arguments in the called function
				return calculation_value(std::make_index_sequence<sizeof ...(Args)>());
			};
	};
};