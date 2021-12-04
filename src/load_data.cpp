#include <iostream>
#include <string>
#include <filesystem>

#include "../include/load_data.hpp"

// namespace fs = std::filesystem;

// Dataloader constructor
Dataloader::Dataloader(std::string name, std::string path) 
{ 
    // Constructor sets name and path to images
    // it also sets up the data structure that can
    // be queried at any time to get the next image
    dataset_name = name;
    dataset_path = path;

    // Create image vector
    create_feature_vectors();
}

void Dataloader::create_feature_vectors() {
    // Delete this after our two images test
    int counter = 0;
    bool plot = false;

    cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
    
    for (const auto & entry : std::filesystem::directory_iterator(dataset_path)) 
    {
        // --- Delete this after our two images test ---
        if (counter > 1) {
            break;
        }
        // --- Delete this after our two images test ---
        
        const cv::Mat image = cv::imread(entry.path(), 0); //Load as grayscale

        std::vector<cv::KeyPoint> keypoints;
        detector->detect(image, keypoints);

        // Add results to image and save.
        if (plot)
        {
            std::string build_name = create_image_title(counter);
            save_image_with_features(image, keypoints, build_name);
        }
        // --- Delete this after our two images test ---
        counter++;
    }
}

// Helper Functions
void Dataloader::save_image_with_features(cv::Mat image_in, std::vector<cv::KeyPoint> key_points, std::string save_path)
{
    cv::Mat output;
    cv::drawKeypoints(image_in, key_points, output);
    cv::imwrite(save_path, output);
    return;
}

std::string Dataloader::create_image_title(int counter)
{
    std::string build_name = "../temp_images/";
    build_name += std::to_string(counter);
    build_name += ".jpg";
    return build_name;
}

// Dataloader getter functions
std::string Dataloader::get_dataset_name() { return dataset_name; }
std::string Dataloader::get_dataset_path() { return dataset_path; }