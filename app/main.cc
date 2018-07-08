#include <exception>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <world_builder/assert.h>
#include <world_builder/utilities.h>
#include <world_builder/world.h>

namespace po = boost::program_options;
using namespace WorldBuilder::Utilities;

int main(int argc, char** argv)
{
	/**
	 * First parse the command line options
	 */
	std::string wb_file;
	std::string data_file;

	unsigned int dim = 3;
	unsigned int compositions = 0;

	try {
	    po::options_description desc("Allowed options");
	    desc.add_options()
	        ("help", "produce help message")
			("dim", po::value<unsigned int>(), "dimension of the data file.")
			("compositions", po::value<unsigned int>(), "compositions in the data output.")
			("files", po::value<std::vector<std::string> >(), "list of files, starting with the World Builder "
     			                                              "file and data file(s) after it.");

	    po::positional_options_description p;
	    p.add("files", -1);

	    po::variables_map vm;
	    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	    po::notify(vm);

	    if (vm.count("help")) {
	    	std::cout << std::endl << "TODO: Write description how to use this." << std::endl << std::endl;
	    	std::cout << desc << "\n";
	        return 0;
	      }

	    if(vm.count("dim"))
	    {
	    	dim = vm["dim"].as<unsigned int>();
	    }

	    if(vm.count("compositions"))
	    {
	    	compositions = vm["compositions"].as<unsigned int>();
	    }

	    if(!vm.count("files"))
	    {
	    	std::cout << "Error: There where no files passed to the World Builder, use --help for more " << std::endl
	    			  << "information on how  to use the World Builder app." << std::endl;
	    	return 0;
	    }

	    std::vector<std::string> file_names = vm["files"].as<std::vector<std::string> >();

		if(file_names.size() < 2)
		{
			std::cout << "Error:  The World Builder app requires at least two files, a World Builder file " << std::endl
					  << "and a data file to convert." << std::endl;
			return 0;
		}

	    wb_file = file_names[0];
	    // Todo: Is it useful to check whether the string is empty?


	    data_file = file_names[1];
	    // Todo: Is it useful to check whether the string is empty?

	}
	    catch(std::exception& e) {
	    	std::cerr << "error: " << e.what() << "\n";
	        return 1;
	    }
	    catch(...) {
	    	std::cerr << "Exception of unknown type!\n";
	    return 1;
	    }

	    /**
	     * Try to start the world builder
	     */
	    std::unique_ptr<WorldBuilder::World> world;
	    try{
	       world = std::make_unique<WorldBuilder::World>(wb_file);
	    }
	    catch(std::exception& e) {
	    	std::cerr << "Could not start the World builder, error: " << e.what() << "\n";
	        return 1;
	    }
	    catch(...) {
	    	std::cerr << "Exception of unknown type!\n";
	    return 1;
	    }


	    /**
	     * Read the data from the data files
	     */
	    // if config file is available, parse it
	  /*  if(config_file != "")
	    {
	    	// Get world builder file and check wether it exists
	    	WBAssertThrow(access( config_file.c_str(), F_OK ) != -1,
	    			"Could not find the provided convig file at the specified location: " + config_file);


	    	// Now read in the world builder file into a file stream and
	    	// put it into a boost property tree.
	    	//std::ifstream json_input_stream(config_file.c_str());
	    	ptree tree;
	    	tree.read_json(config_file, tree);

	    	if(boost::optional<unsigned int> value = tree.get_optional<unsigned int>("dim"))
	    			dim = value.get();

	    	if(boost::optional<unsigned int> value = tree.get_optional<unsigned int>("compositions"))
	    			compositions = value.get();

	    }*/
	    std::string line;
	    std::ifstream data_stream(data_file);

	    // move the data into a vector of strings
	    std::vector<std::vector<std::string> > data;
	    std::string temp;

	    while (std::getline(data_stream, temp)) {
	        std::istringstream buffer(temp);
	        std::vector<std::string> line((std::istream_iterator<std::string>(buffer)),
	                                 std::istream_iterator<std::string>());

	        // remove the comma's in case it is a comma separated file.
	        // TODO: make it split for comma's and/or spaces
	        for(unsigned int i = 0; i < line.size(); ++i)
	          line[i].erase(std::remove(line[i].begin(), line[i].end(), ','), line[i].end());

	        data.push_back(line);
	    }

	    // Read config from data if pressent
	    for(unsigned int i = 0; i < data.size(); ++i)
	    {
	    	if(data[i][0] == "#" && data[i][1] == "dim" && data[i][2] == "=" && data[i][2] == "=")
	    	{
	    		dim = string_to_unsigned_int(data[i][3]);
	    	}

	    	if(data[i][0] == "#" && data[i][1] == "compositions" && data[i][2] == "=")
	    		compositions = string_to_unsigned_int(data[i][3]);

	    }

	    // set the header
	    std::cout << "# x ";

	    if(dim == 2)
	    	std::cout << "z ";
	    else
	    	std::cout << "y z ";

	    std::cout << "d T ";

	    for(unsigned int c = 0; c < compositions; ++c)
	    	std::cout << "c" << c << " ";

	    std::cout <<std::endl;

	    // set the values
	    switch(dim)
	    {
	    case 2:
		    for(unsigned int i = 0; i < data.size(); ++i)
		    	if(data[i][0] != "#")
		    		{
		    		WBAssertThrow(data[i].size() == dim + 2, "The file needs to contain dim + 2 entries, but contains " << data[i].size() << " entries "
		    				                                 " on line " << i+1 << " of the data file.  Dim is " << dim << ".");

		    			std::array<double,2> coords = {string_to_double(data[i][0]),
		    					                       string_to_double(data[i][1])};
		    			std::cout << data[i][0] << " " << data[i][1] << " " << data[i][2] << " " << data[i][3] << " ";
		    			std::cout << world->temperature(coords, string_to_double(data[i][2]), string_to_double(data[i][3]))  << " ";

		    			for(unsigned int c = 0; c < compositions; ++c)
		    			{
		    				std::cout << world->composition(coords, string_to_double(data[i][2]), c)  << " ";
		    			}
		    			std::cout << std::endl;

		    		}
		    break;
	    case 3:
		    for(unsigned int i = 0; i < data.size(); ++i)
		    	if(data[i][0] != "#")
		    		{
	    			    WBAssertThrow(data[i].size() == dim + 2, "The file needs to contain dim + 2 entries, but contains " << data[i].size() << " entries "
		    				                                      " on line " << i+1 << " of the data file. Dim is " << dim << ".");
		    			std::array<double,3> coords = {string_to_double(data[i][0]),
		    					                       string_to_double(data[i][1]),
								                       string_to_double(data[i][3])};

		    			std::cout << data[i][0] << " " << data[i][1] << " " << data[i][2] << " " << data[i][3] << " " << data[i][4] << " ";
		    			std::cout << world->temperature(coords, string_to_double(data[i][3]), string_to_double(data[i][4]))  << " ";

		    			for(unsigned int c = 0; c < compositions; ++c)
		    			{
		    				std::cout << world->composition(coords, string_to_double(data[i][3]), c)  << " ";
		    			}
		    			std::cout << std::endl;

		    		}
		    break;
	    default:
	                std::cout << "The World Builder can only be run in 2d and 3d but a different space dimension " << std::endl;
	                		     "is given.";
	                		     return 0;
	    }

	return 0;
}
