#include <iostream>
#include <string>
#include "../include/load_data.hpp"
#include "./load_data.cpp"

int main()
{
    std::cout << "--- SLAM-DUNK ---" << std::endl;

    // probably want a better place to put these but for now
    std::string dataset_name_in = "xyz_test_dataset";
    std::string dataset_path_in = "../datasets/rgbd_dataset_freiburg1_xyz";
    
    Dataloader dataloader("barfoo");
    // Dataloader::Dataloader dataloader = Dataloader::Dataloader();
    // dataloader.set_dataset_name("random");
    // dataloader.set_dataset_path(dataset_path_in);

    

    std::cout << "Dataset name: " << dataloader.dataset_name << std::endl;
    std::cout << "Dataset path: " << std::endl;//dataloader.dataset_path << std::endl;
}