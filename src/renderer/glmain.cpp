#include "renderer/renderer.hpp"
#include "renderer/scene_loader.hpp"

#include <string>
#include <cassert>
#include <memory>


using bar = std::array<std::unique_ptr<uint>,8>;
class foo
{
	private:
	bar val;

	public:
	foo(bar& a)
		: val(std::move(a)) {}
};


int main(int argc, char** argv) 
{
	// bar tmp;
	// tmp[0] =  std::make_unique<uint>(1);

	// foo a_foo(tmp);

	/* std::array<std::unique_ptr<cg::color>,8> tmp2 = std::move(tmp);
	
	tmp[1] = std::make_unique<cg::color>(0.2,0.2,0.2,1.0);

	for(uint i = 0 ; i < tmp.size() ; ++i)
	{
		if(tmp[i] == nullptr)
			std::cout << "(null)" << "\n";
		else
			std::cout << (*tmp[i])[0] << "," << (*tmp[i])[1] << "," << (*tmp[i])[2] << "," << (*tmp[i])[3]  << "\n";
	} */

	if(argc > 2)
	{
		std::cerr << "Error: Too many arguments\n";
		return EXIT_FAILURE;
	}
 
	
	cg::renderer::init(&argc, argv);
	
	//using cg::renderer::main_scene;  
	auto tmp_scene = cg::xml_to_scene((argc < 2) ? "scene.xml" : argv[1]);
	if(!tmp_scene)
	{
		std::cerr << "Error: File '" + ((argc < 2) ? "scene.xml" : std::string(argv[1])) + "' could not be loaded\n";
		return EXIT_FAILURE;
	}
	
	cg::scene main_scene = tmp_scene.value();
	cg::scene::debug = false;
	
	//main_scene.set_camera(cg::camera(5,5,5));
	
	cg::renderer::set_scene(&main_scene);
	
	cg::renderer::run();
 
	return EXIT_SUCCESS;
}