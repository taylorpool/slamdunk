#include <iostream>
#include <string>
#include "../include/load_data.hpp"

int main()
{
    std::cout << "--- SLAM-DUNK ---" << std::endl;

    // probably want a better place to put these but for now
    // Params
    std::string dataset_name_in = "xyz_test_dataset";
    std::string dataset_path_in = "../datasets/rgbd_dataset_freiburg1_xyz/rgb";
    
    Dataloader dataloader(dataset_name_in, dataset_path_in);

    std::cout << "Dataset name: " << dataloader.get_dataset_name() << std::endl;
    std::cout << "Dataset path: " << dataloader.get_dataset_path() << std::endl;
}