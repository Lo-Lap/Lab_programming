#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

template <typename ObjectType, typename ...Args>
class Wrapper
{
private:
	ObjectType* Class; //save class object
	int(ObjectType::* func_method)(Args...);  //save the transferred class method


	std::unordered_map <std::string, int> map_args; //unordered_map is used so that there is no automatic sorting by key
	//here we save the dictionary of arguments
	// when you initialize a dictionary, we determine the order of arguments, 
	// such as entered when creating Wrapper object

	std::vector<int> values_of_args; //the arguments are stored in the correct order (according to the key order of the map_args dictionary) 
	//then func_method will be applied to the values of this vector


	//function to fill the value vector from the dictionary
	void vector_filling()
	{
		for (auto& elem_args : map_args)
		{
			values_of_args.push_back(elem_args.second);
		}
	}

	// function to decompress vector into Args argument list and call func_method function 
	template<std::size_t... S>
	int calculation_value(std::index_sequence<S...>)
	{
		//values_of_args[S]... unzips vector in Args
		//(Class->*(func_method) call method passed class
		return (Class->*(func_method))(values_of_args[S]...);
	};

public:

	Wrapper(ObjectType* my_class, int(ObjectType::* Function)(Args...), std::unordered_map <std::string, int> arg_map_from)
	{
		Class = my_class;
		func_method = Function;
		map_args = arg_map_from;

	};

	//link method function with dictionary arguments
	std::function<int(std::unordered_map<std::string, int>)> func_calculation = [this](std::unordered_map<std::string, int> args_map_to_cal)
		{
			try
			{
				if (values_of_args.empty() == false) values_of_args.clear(); //if the function is called for the same class object, 
				//the values of the argument that were not necessary for the current calculations could remain in the vector

				if (args_map_to_cal.size() != map_args.size())
				{
					throw "Incorrect number of arguments to pass";
				}

				for (auto& elem_args : args_map_to_cal)
				{
					if (map_args.find(elem_args.first) == map_args.end())
						//did not find such a key in the dictionary initialized when creating the Wrapper object
					{
						throw "Invalid argument name";
					}

					map_args[elem_args.first] = elem_args.second; //fill the argument dictionary initialized when creating the Wrapper object
				}

				vector_filling();

				//std::make_index_sequence<sizeof ...(Args)>() is needed to create an index sequence 
				//elements of the argument value vector to transform the vector into a list of function arguments in the called function
				return calculation_value(std::make_index_sequence<sizeof ...(Args)>());
			}
			catch (const char* error_message)
			{
				std::cout << error_message << std::endl;
			}

			return -1;
		};

};