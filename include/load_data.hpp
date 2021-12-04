#pragma once
#include <string>
#include <vector>

#include "opencv2/opencv.hpp"
#include <opencv2/features2d.hpp>

class Dataloader
{
public:
  std::string dataset_name;
  std::string dataset_path;

  std::vector<std::vector<cv::KeyPoint>> feature_vec;

  // Constructor
  Dataloader(std::string name_in, std::string path_in);

  // Creates the image vector as Mat opencv datatypes
  void create_feature_vectors();

  // Helper Functions
  std::string create_image_title(int counter);
  void save_image_with_features(cv::Mat image_in, std::vector<cv::KeyPoint> key_points, std::string save_path);

  // Getters
  std::string get_dataset_name();
  std::string get_dataset_path();
  std::vector<std::vector<cv::KeyPoint>> get_feature_vec();
};