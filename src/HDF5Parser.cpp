/**
 * \file HDF5Parser.cpp
 * Code to parse hdf5 file containing neural network information
 *
 * \brief Parser for hdf5 neural network file:
 * This code is meant to declare a neural network (NeuralNetwork.h) 
 * based on the hdf5 file that was output by the training code
 *
 * \author: $Author: Sarah Aguasvivas Manzano $
 *
 * \version $Version: 0.1 $
 *
 * \date $Date: 2019/02/18 $
 *
 * Contact: saag5228@colorado.edu
 *
 * Created on: Thu Feb 07 2019
 * 
 * */
#ifdef _HDF5PARSER_C__ 
#define _HDF4PARSER_C__


#else
#include <iostream>
#endif
#include <string>
#include "datastructures/tensor.h"
#include <vector>

#ifndef H5_NO_NAMESPACE
#ifndef H5_NO_STD
    using std::cout;
    using std::cerr;
    using std::endl;
#endif  // H5_NO_STD
#endif

#include "H5Cpp.h"
#include "Parser.h"
#ifndef H5_NO_NAMESPACE
using namespace H5;

#endif

// Callback function:
extern "C" herr_t weights_callback(hid_t loc_id, const char *name, const H5L_info_t * linfo, void *opdata);
extern "C" herr_t network_callback(hid_t loc_id, const char *name, const H5L_info_t * linfo, void *opdata);

int HDF5Parser::parse()
{
  const H5std_string FILE_NAME( this->file_name );
  try
   {
      Exception::dontPrint();
        
      H5File file = H5File( FILE_NAME, H5F_ACC_RDWR );
      
      // Interested in model_weights only:
      Group group = Group( file.openGroup( "model_weights" ));
      Group root= Group(file.openGroup("/")); 
                  
        // Parsing overall neural network structure:
      cout<< endl<<"Parsing neural newtork structure: "<<endl;
      herr_t rat= H5Literate(group.getId(), H5_INDEX_NAME, H5_ITER_INC, NULL, network_callback, NULL);
      cerr<<endl;

      // Parsing specific weights:
      cerr << endl << "Parsing weights:" << endl;
      herr_t idx=  H5Lvisit(group.getId(), H5_INDEX_NAME, H5_ITER_INC,  weights_callback, NULL);
      cerr << endl;

      // Reading neural network configuration file:
      hid_t attr, dataspace;
      herr_t status;
      cout<< "Reading Attributes: "<<endl;
      attr = H5Aopen(root.getId(), "model_config", H5P_DEFAULT); 
      char* rdata = new char[100];
      status = H5Aread(attr, H5Tcopy(H5T_C_S1), &rdata);
      cout<< (char)rdata[0]<<endl;
      delete [] rdata;
    return 0;//

   }  // end of try block

   // catch failure caused by the H5File operations
   catch( FileIException error )
   {
      //error.printError();
      return -1;
   }

   // catch failure caused by the DataSet operations
   catch( DataSetIException error )
   {
      //error.printError();
      return -1;
   }

   // catch failure caused by the DataSpace operations
   catch( DataSpaceIException error )
   {
      //error.printError();
      return -1;
   }

   // catch failure caused by the Attribute operations
   catch( AttributeIException error )
   {
      //error.printError();
      return -1;
   }
return 0; 
}

herr_t
network_callback(hid_t loc_id, const char *name, const H5L_info_t *linfo, void *opdata)
{
    cout<< "Layer: ";
    cout<< name << endl;
    // Declare NN node + use builderMap to attach layer.      
    return 0;
}

herr_t
weights_callback(hid_t loc_id, const char *name, const H5L_info_t * linfo, void *opdata)
{

    hid_t group;
    hid_t status;
    hid_t attribute;
    H5O_info_t infobuf;
    
    group= H5Gopen2(loc_id, name, H5P_DEFAULT); // here group is actually a dset
    status = H5Oget_info_by_name(loc_id, name, &infobuf, H5P_DEFAULT);

    switch(infobuf.type){
        case H5O_TYPE_GROUP:{
            cout << "Group : " << name << endl;
            break;
                            }
        case H5O_TYPE_DATASET:{

            cout<< "Dataset: " << name;
            hid_t datatype, dataspace, cclass, order, size, rank; 
            hid_t dset = H5Dopen2(loc_id, name, H5P_DEFAULT);
            datatype= H5Dget_type(dset);
            cclass = H5Tget_class(datatype); 
            size= H5Tget_size(datatype);
            dataspace = H5Dget_space(dset);
            rank= H5Sget_simple_extent_ndims(dataspace); 
            hsize_t dims[rank];
            H5Sget_simple_extent_dims(dataspace, dims, NULL);
            std::vector<unsigned int> tensor_dims;
            
            cout<<endl<< "Dimensions of the dataset:" <<endl;
            for (int i=0; i<rank; i++) {
                cout<< dims[i] << "  ";            
                tensor_dims.push_back((unsigned int)dims[i]);
            }
            cout<<endl;
             
            // TODO: handle the data type specific to the output type, 
            // not only <double>
           
            //Tensor<float>* T(tensor_dims);
            float *rbuf;
            herr_t ret;
            int flat=1;
            for (int i=0; i<rank; i++){
                flat*= dims[i];
            }
            rbuf= new float [flat];
            ret = H5Dread(dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, rbuf); 
            cout<<endl;
            
            for (int i=0; i<flat; i++) {
                cout<< rbuf[i] << "  ";
            }  

           /* switch(rank){
                case 1:
                    {
                        for (int i=0; i<dims[0]; i++){
                            (*T)(i) = rbuf[i];
                            cout<< (*T)(i) <<endl;
                        }

                    }

                default:
                    break;

            }*/
            //link weights and biases to layer object
            cout<<endl;
              
            ret= H5Dclose(dset); 
            
            //no toki:
            break;
                              }
        case H5O_TYPE_NAMED_DATATYPE:{
            cout<< "DataType: " << name <<endl;
            break;
                }
        default:{
            cout<< "Unknown"<<endl;
            
                }       
    }
             
    H5Gclose(group);

    return 0;
 }