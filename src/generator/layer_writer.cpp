/*      LayerWriter.cpp
 *
 * Sarah Aguasvivas Manzano
 *
 */

#include "generator/layer_writer.h"
#include <string>
#include <map>

LayerWriter* LayerWriter::make_writer(Layer* layer, std::string init_string)
{
  if(Conv1D* ptr = dynamic_cast<Conv1D*>(layer))
    return new Conv1DGenerator(ptr, init_string);

  else if(Conv2D* ptr = dynamic_cast<Conv2D*>(layer))
    return new Conv2DGenerator(ptr, init_string);

  else if(Dense* ptr = dynamic_cast<Dense*>(layer))
    return new DenseGenerator(ptr, init_string);

  else if(Flatten* ptr = dynamic_cast<Flatten*>(layer))
    return new FlattenGenerator(ptr, init_string);

  else if(MaxPooling1D* ptr = dynamic_cast<MaxPooling1D*>(layer))
    return new MaxPooling1DGenerator(ptr, init_string);

  else if(MaxPooling2D* ptr = dynamic_cast<MaxPooling2D*>(layer))
    return new MaxPooling2DGenerator(ptr, init_string);

  else
    return NULL;
}

void Conv1DGenerator::build_map(std::string prev_id){

    mapping[KERNEL_SIZE] = std::to_string((int)layer->kernel_size[0]);
    mapping[STRIDE_SIZE] = std::to_string(layer->strides);
    mapping[INPUT_SIZE] =

    mapping[INPUT_CHANNELS] = 
    mapping[OUTPUT_CHANNELS] = std::to_string((int)layer->filters);

    mapping[WEIGHT_NAME]= layer->w->identifier;
    mapping[BIAS_NAME]= layer->b->identifier;
}
std::string Conv1DGenerator::write_init()
{
  //Take init string and replace delimiters.
	std::string delim;
	size_t start = 0;
	size_t end = 0;

	start = init_template.find_first_of("<%",start);
	end = init_template.find_first_of(">",start);

	//Replace all delimiters in the string.
	while(start != std::string::npos)
	{
		delim = init_template.substr(start+1,end-start-2);

		//Do stuff with the delimiter.
		delim = mapping[delim];

		//Replace the delimiter.
		init_template.replace(start,end-start,delim);


		//Reset start and end positions.
		start = init_template.find_first_of("<%",end);
		end = init_template.find_first_of(">", start);
	}
	init_template += '\n';

  return init_template;
}

void Conv2DGenerator::build_map(std::string prev_id){

}
std::string Conv2DGenerator::write_init()
{

}

void DenseGenerator::build_map(std::string prev_id){

    mapping[INPUT_SIZE] =
    mapping[OUTPUT_SIZE] = layer->units;

    mapping[WEIGHT_NAME] = layer->w->identifier;
    mapping[BIAS_NAME] = layer->b->identifier;
}
std::string DenseGenerator::write_init()
{
  //Take init string and replace delimiters.
	std::string delim;
	size_t start = 0;
	size_t end = 0;

	start = init_template.find_first_of("<%",start);
	end = init_template.find_first_of(">",start);

	//Replace all delimiters in the string.
	while(start != std::string::npos)
	{
		delim = init_template.substr(start+1,end-start-2);

		//Do stuff with the delimiter.
		delim = mapping[delim];

		//Replace the delimiter.
		init_template.replace(start,end-start,delim);


		//Reset start and end positions.
		start = init_template.find_first_of("<%",end);
		end = init_template.find_first_of(">", start);
	}
	init_template += '\n';

  return init_template;
}

void FlattenGenerator::build_map(std::string prev_id){
   mapping[LAYER_ID] = layer->identifier;
   //mapping[INPUT_SIZE_0] =
   //mapping[INPUT_SIZE_1] =
   mapping[PREVIOUS_LAYER_ID] = prev_id;
}
std::string FlattenGenerator::write_init()
{

}

void MaxPooling1DGenerator::build_map(std::string prev_id){


}
std::string MaxPooling1DGenerator::write_init()
{

}

void MaxPooling2DGenerator::build_map(std::string prev_id){


}
std::string MaxPooling2DGenerator::write_init()
{

}
