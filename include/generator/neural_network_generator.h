#include <string>
#include <map>


#ifndef __NEURAL_NETWORK_GENERATOR_H__
#define __NEURAL_NETWORK_GENERATOR_H__

class NNGenerator
{
    private:
    	//Delimiters
    	static std::string INIT;
    	static std::string FWD;
    
    	//Paths to header and source file for neural_network.
        std::string header_template_path;
		std::string source_template_path;
		
		//Template material to be extracted from template edited and dumped.
		std::string header;
		std::string source;
		
		//Maps containing forward and inititalize call strings.
		//std::map<std::string, std::string> init_calls;
		//std::map<std::string, std::string> fwd_calls;
    
    	//Loads the templates using the paths into the header and source.
        void loadTemplates();
        
    public:
    
        NNGenerator(std::string, std::string);
        ~NNGenerator();
        
        //void addMaps(std::map<std::string, std::string>, std::map<std::string, std::string>);
        
        void addLayer_Header();
        void addLayer_Init();
        void addLayer_Fwd();
        void dump();

};

#endif