#include <iostream>
#include <string>
#include <filesystem>

#include "../include/load_data.hpp"

// namespace fs = std::filesystem;

// Dataloader constructor
Dataloader::Dataloader(std::string name, std::string path, int in_num_images) 
{ 
    // Constructor sets name and path to images
    // it also sets up the data structure that can
    // be queried at any time to get the next image
    dataset_name = name;
    dataset_path = path;
    // number of images we will use from the dataset directory
    num_images = in_num_images;

    // Creates and populates feature point and descriptor vectors
    create_feature_vectors();
    // Image matching based on feature point and descriptor vectors ^
    match_images();
}

void Dataloader::match_images() 
{
    bool plot = false;
    int norm = 2;                       // currently set to l2 norm
    const float ratio_thresh = 0.7f;    // used for checking for good matches
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE);

    int vec_size = descr_vec.size();
    for (int i=0; i < vec_size; i++)
    {
        if (i == (vec_size - 1)) { break; } // break if we are at end of images
        
        // Create descriptor vars for the two images of current focus
        cv::Mat descr_1 = *descr_vec[i]; 
        cv::Mat descr_2 = *descr_vec[i+1];

        // Create vector of match obejcts and do knn matching with some norm
        std::vector<std::vector<cv::DMatch> > knn_matches;
        matcher->knnMatch( descr_1, descr_2, knn_matches, norm ); 

        // -- Filter matches using the Lowe's ratio test
        std::vector<cv::DMatch> good_matches;
        int k_size = knn_matches.size();
        for (size_t j = 0; j < k_size; j++)
        {
            if (knn_matches[j][0].distance < ratio_thresh * knn_matches[j][1].distance)
            {
                good_matches.push_back(knn_matches[j][0]);
            } // end if
        } // end innner for

        all_good_matches.push_back(good_matches);  // store the good matches in vec
        save_matched_images(good_matches, i, plot);
    
    } // end outer for
}

void Dataloader::create_feature_vectors() 
{
    int counter = 0;
    bool plot = false;
    cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
    cv::Ptr<cv::DescriptorExtractor> descriptor = cv::ORB::create();
    
    std::vector<std::string> all_sorted_files = get_sorted_files();

    int file_vec_size = all_sorted_files.size();
    for (int i=0; i < file_vec_size; i++)
    {   
        // std::cout << all_sorted_files[i] << std::endl;
        if (counter > num_images) { break; }
        
        const cv::Mat image = cv::imread(all_sorted_files[i], 0); //Load as grayscale
        std::vector<cv::KeyPoint> keypoints;
        cv::Mat desc;

        detector->detect(image, keypoints);
        descriptor->compute(image, keypoints, desc);

        feature_vec.push_back( keypoints );  // add keypoints to keypoint vec
        descr_vec.push_back(std::make_shared<cv::Mat>(desc)); // add descr to vec
        temp_imgs.push_back(std::make_shared<cv::Mat>(image)); // used for drawing

        plot_logic(plot, counter, image, keypoints);
        counter++;
    }
}

// Helper Functions
std::vector<std::string> Dataloader::get_sorted_files() 
{
    std::vector<std::string> all_files;
    for (const auto & entry : std::filesystem::directory_iterator(dataset_path)) 
    {   
        all_files.push_back( entry.path() );
    }
    // Sort the vector of strings alphabetically
    sort(all_files.begin(), all_files.end());
    return all_files;
}

void Dataloader::save_matched_images(std::vector<cv::DMatch> good_matches, int i, bool plot)
{
    if (plot)
    {
        //-- Draw matches
        cv::Mat img_1 = *temp_imgs[i];
        cv::Mat img_2 = *temp_imgs[i+1];
        std::vector<cv::KeyPoint> key_1 = feature_vec[i];
        std::vector<cv::KeyPoint> key_2 = feature_vec[i+1];

        cv::Mat img_matches;
        cv::drawMatches( img_1, key_1, img_2, key_2, good_matches, img_matches, cv::Scalar::all(-1),
                    cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

        std::string tmp_name = "../temp_images/" + std::to_string(i) + "_and_" 
                                + std::to_string((i+1)) + "_matched.jpg";
        cv::imwrite(tmp_name, img_matches);
    }
    return;
}

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

void Dataloader::plot_logic(bool plot, int counter, cv::Mat image_in, std::vector<cv::KeyPoint> key_points)
{
    // Add results to image and save.
    if (plot)
    {
        std::string build_name = create_image_title(counter);
        save_image_with_features(image_in, key_points, build_name);
    }
    return;
}

// Dataloader getter functions
std::string Dataloader::get_dataset_name() { return dataset_name; }
std::string Dataloader::get_dataset_path() { return dataset_path; }
std::vector<std::vector<cv::KeyPoint>> Dataloader::get_feature_vec() { return feature_vec; }
std::vector<std::shared_ptr<cv::Mat>> Dataloader::get_descr_vec() { return descr_vec; }



