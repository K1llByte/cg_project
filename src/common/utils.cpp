#include "common/utils.hpp"


std::vector<std::string_view> str_utils::split(const std::string& cmd, const char separator)
{
	uint num_sparator = 0;
	for(uint i = 0 ; i < cmd.length() ; ++i)
		if(cmd[i] != separator && (i == 0 || cmd[i-1] == separator))
		{
			++num_sparator;
		}

	std::vector<std::string_view> res(num_sparator);
	uint idx = 0;
	uint begin = 0;
	uint end = 0;
	for(uint i = 0 ; i < cmd.length() ; ++i)
	{
		if(cmd[i] == separator || cmd[i] == '\n')
		{
			if(end != begin)
			{ 
				std::string_view tmp(cmd.c_str(),end);
				tmp.remove_prefix(begin);
				res[idx++] = tmp;
			}
			begin = ++end;
		}
		else
		{
			++end;
		}
	}
	std::string_view tmp(cmd.c_str(),end);
	tmp.remove_prefix(begin);

	if(!tmp.empty())
		res[idx] = tmp;
	return res;
}