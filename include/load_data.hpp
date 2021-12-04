#pragma once
#include <string>
#include <vector>

#include "opencv2/opencv.hpp"
#include <opencv2/features2d.hpp>

class Dataloader
{
private:
  // name of the dataset we want to use
  std::string dataset_name;
  // relative path to build/ to get to dataset folder
  std::string dataset_path;

  /* 
    public member variable that is a vector
    of vectors. The inner vectors hold Keypoints
    for an image that has been through an opencv
    feature detection method
  */
  std::vector<std::vector<cv::KeyPoint>> feature_vec;
public:
  // Constructor
  Dataloader(std::string name_in, std::string path_in);

  // Creates the image vector as Mat opencv datatypes
  void create_feature_vectors();

  // Helper Functions

  /*
    @param counter, this is a variable that 
    helps us name the image we are going to 
    save to a file, it counts the images we've
    gone through
    @return a string that is the path to a 
    directory called ../temp_images/ where images
    can be stored
  */
  std::string create_image_title(int counter);

  /*
    @param image_in, a specific loaded image of type cv::Mat
    @param keypoints is vector of vectors of keypoints of the 
    corresponding image being passed in i.e. image_in
    @param save_path, path to which we save the final image
    
    Purpose of this function is to save the original
    image with the features that were detected using
    the cv feature detection on that image to see
    how well it did
  */
  void save_image_with_features(cv::Mat image_in, std::vector<cv::KeyPoint> key_points, std::string save_path);

  // Check if we want to plot, see above for 
  // @param details
  void plot_logic(bool plot, int counter, cv::Mat image_in, std::vector<cv::KeyPoint> key_points);
  // Getters
  std::string get_dataset_name();
  std::string get_dataset_path();
  std::vector<std::vector<cv::KeyPoint>> get_feature_vec();
};