#pragma once
#include <utility>
#include "Wrapper.h"


class Engine
{
private:
	//unordered_map is used so that there is no automatic sorting by key
	std::unordered_map<std::string, std::function<int(std::unordered_map<std::string, int>)>> regist_commands;

public:

	template <typename ObjectType, typename ...Args>
	void register_command(Wrapper<ObjectType, Args...>* wrapper, std::string command_name) //command logging feature
	{
		try
		{
			if (regist_commands.find(command_name) != regist_commands.end()) //command has already been registered
			{
				throw "The command has already been registered";
			}

			regist_commands[command_name] = (*wrapper).func_calculation;  //add lambda function from wrapper to command dictionary
		}
		catch (const char* error_message)
		{
			std::cout << error_message << std::endl;
		}
	}

	int execute(std::string command_name, std::unordered_map <std::string, int> arg_map)
	{
		try
		{
			if (regist_commands.find(command_name) == regist_commands.end()) //command not found in registered
			{
				throw "Called command not registered";
			}

			std::function<int(std::unordered_map<std::string, int>)> func = regist_commands[command_name];
			return func(std::move(arg_map)); //return the value counted by the registered function
		}
		catch (const char* error_message)
		{
			std::cout << error_message << std::endl;
		}

		return -1;

	}

};
