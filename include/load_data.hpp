#pragma once
#include <string>
#include <vector>

#include "opencv2/opencv.hpp"

class Dataloader
{
public:
  std::string dataset_name;
  std::string dataset_path;

  std::vector<cv::Mat> image_vec;


  Dataloader(std::string name_in, std::string path_in);

  // Getters
  std::string get_dataset_name();
  std::string get_dataset_path();
};