#include <iostream>
#include <string>

#include "../include/load_data.hpp"

// Dataloader constructor
Dataloader::Dataloader(std::string name, std::string path) 
{ 
    // Constructor sets name and path to images
    // it also sets up the data structure that can
    // be queried at any time to get the next image
    dataset_name = name;
    dataset_path = path; 

    
}

// Dataloader getter functions
std::string Dataloader::get_dataset_name() { return dataset_name; }
std::string Dataloader::get_dataset_path() { return dataset_path; }