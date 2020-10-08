#include "generator/gen.hpp"

#define DEBUG_TOOLS
#include "common/utils.hpp"
#include "generator/gen_parser.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>


int main(int argc, char** argv)
{
    if(argc < 2)
    {
		std::cerr << "Error: No arguments provided\n";
		exit(1);
    }

    std::vector<std::string_view> gen_command(argc-2);
	for(int i = 1 ; i < argc-1 ; ++i)
	{
		gen_command[i-1] = std::string_view(argv[i]);
	}

    //std::vector<cg::vec3> p_mesh;

    std::string model_file = "models/";

    try
    {
        
    }
    catch(const std::invalid_argument& e)
    {
        std::cerr << e.what() << "[file_name]" << '\n';
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    cg::vnt_vectors p_mesh = gen_by_strviews(gen_command);

    model_file.append(argv[argc-1]);

    cg::vertices_to_file(p_mesh,model_file);
 
    std::cout << "Primitive Successfully Generated\n";
    
    return 0;
}