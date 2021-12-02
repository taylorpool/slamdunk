#include <iostream>
#include <string>

#include "../include/load_data.hpp"

// Dataloader constructor
Dataloader::Dataloader(std::string foobar) { dataset_name = foobar; }

// Dataloader setter functions
void Dataloader::set_dataset_name(std::string dataset_name_in)
{
    dataset_name = dataset_name_in;
}
void Dataloader::set_dataset_path(std::string path_in)
{
    dataset_path = path_in;
}

// Dataloader getter functions
std::string Dataloader::get_dataset_name() { return dataset_name; }
std::string Dataloader::get_dataset_path() { return dataset_path; }