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
  // number of images to use from dataset dir
  int num_images;

  
  /*
    Stores each images feature vector depth, so there
    is a depth value for each feature for each image
  */
  std::vector<std::vector<double>> feature_depth;

  /* 
    private member variable that is a vector
    of vectors. The inner vectors hold Keypoints
    for an image that has been through an opencv
    feature detection method
  */
  std::vector<std::vector<cv::KeyPoint>> feature_vec;
  /*
    private member var of type cv::Mat, it is
    a vector of descriptors that are used in
    the feature matching process
  */
  std::vector<std::shared_ptr<cv::Mat>> descr_vec;
  std::vector<std::shared_ptr<cv::Mat>> temp_imgs;

  /*
    Vector of good matches between two images
    a good match means that their exists two
    features in the descriptors of the images
    that have a distance lower than a threshold
    when this occurs it is a good match and that
    descriptor is stored in this vector member var
  */
  std::vector<std::vector<cv::DMatch>> all_good_matches;

public:
  // Constructor
  Dataloader(std::string name_in, std::string path_in, int num_images_in);

  // Creates the image vector as Mat opencv datatypes
  void create_feature_vectors();
  // Matches images based on features and descriptors
  void match_images();

  // Helper Functions
  /*
    Get all files from dataset directory and sort
    them in alphabetical order
  */
  std::vector<std::string> get_sorted_files();

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

  void create_dataset();

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

  std::string find_depth_filename(std::string file_in);
  int find_depth_in_image(std::string depth_name, int x, int y);

  // ^^ similar to above but save images after matching
  void save_matched_images(std::vector<cv::DMatch> good_matches, int i, bool plot);

  // Check if we want to plot, see above for 
  // @param details
  void plot_logic(bool plot, int counter, cv::Mat image_in, std::vector<cv::KeyPoint> key_points);
  // Getters
  std::string get_dataset_name();
  std::string get_dataset_path();
  std::vector<std::vector<cv::KeyPoint>> get_feature_vec();
  std::vector<std::shared_ptr<cv::Mat>> get_descr_vec();
  std::vector<std::vector<cv::DMatch>> get_good_matches();
  std::vector<std::vector<double>> get_depth_vector();
};