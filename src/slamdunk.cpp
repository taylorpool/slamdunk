#include <iostream>
#include <string>
#include "../include/load_data.hpp"
//#include "ceres/ceres.h"

//using ceres::Solve;     // I put this here because I think Ceres is working on
                        // my machine, no errors are thrown and Ceres is installed
                        // locally, maybe we just build locally and work on factor graph stuff...?

int main()
{
    std::cout << "--- SLAM-DUNK ---" << std::endl;


    /* -------- Params -------- */
    std::string dataset_name_in = "xyz_test_dataset";
    std::string dataset_path_in = "../datasets/rgbd_dataset_freiburg1_xyz/rgb";
    int num_images_to_use = 1;  // tested on 5 images and no errors were thrown

    // note that the saving of images is currently turned off
    // note that if you want to save the key point and mathing images,
    // then you'll need to add a folder in the base directory called
    // temp_images/ that's where the images are being saved

    /*  
        ******************* IMPORTANT INFORMATION *******************

        Instaniate Dataloader object:
        Member variables include: dataset_name, dataset_path, num_images
        We create four important vectors full of different opencv objects:
        - feature_vec: is a vector of key point vectors. This is the result
        of the ORB feature detection from opencv
        - descr_vec: is a vector of the descriptors of the aforementioned 
        feature vectors, descriptors are just a fast way to compare two images
        and their resulting feature vectors
        - temp_imags: is a vector of images in the format of cv::Mat shared
        pointers. They are optional, they must be there if you want to draw 
        the matched images.
        - all_good_matches: this is a vector of vectors of type DMatch,
        this stores all the good matches between two images and we can 
        then reference into the key point vectors using indices in the 
        DMatch object to find which of those key points are good matches,
        we most likely will need to do one more step here and create a 
        vector or structure that will only contain the good matches between
        each image and that will most likely be the key piece from my work
        so far here in the Dataloader class to feed to the Ceres piece.
        Call or text me if this doesn't make sense. 

        ******************* IMPORTANT INFORMATION *******************
    */
    Dataloader dataloader(dataset_name_in, dataset_path_in, num_images_to_use);

    std::cout << "Size of feature vector: " << dataloader.get_feature_vec().size() << std::endl;
    std::cout << "Size of descr vector: " << dataloader.get_descr_vec().size() << std::endl;
    std::cout << "Dataset name: " << dataloader.get_dataset_name() << std::endl;
    std::cout << "Dataset path: " << dataloader.get_dataset_path() << std::endl;


    // LOOPS THROUGH ALL 2D POINTS IN EACH FEATURE VECTOR
    // std::vector<std::vector<cv::KeyPoint>> all_feat_vecs = dataloader.get_feature_vec();
    // for (int i=0; i < all_feat_vecs.size(); i++)
    // {   
    //     std::vector<cv::KeyPoint> curr_key = all_feat_vecs[i];
    //     for (int j=0; j<curr_key.size(); j++)
    //     {
    //         std::cout << "j: " << std::to_string(j) << "\tval: " << curr_key[j].pt << std::endl;  // accesses all the points
    //     }
    //     std::cout << "i: " << std::to_string(i) << std::endl;
    // }


    // LOOPS THROUGH ALL 2D POINTS IN EACH DESCRIPTOR VECTOR
    // std::vector<std::shared_ptr<cv::Mat>> desc_vec = dataloader.get_descr_vec();
    // for (int i=0; i < desc_vec.size(); i++)
    // {
    //     cv::Mat mat_descr = *desc_vec[i];
    //     std::cout << "i: " << std::to_string(i) << "\t mat_descr[i]: " << mat_descr.size() << std::endl;
    // }


    // GET ALL COORDINATES
    // std::vector<cv::KeyPoint::pt> kp_vec;

    // LOOPS THROUGH ALL 2D POINTS IN EACH GOOD MATCH VECTOR
    // std::vector<std::vector<cv::DMatch>> all_matches = dataloader.get_good_matches();
    // std::cout << "Size of all_good_matches vector: " << all_matches.size() << std::endl;

    dataloader.create_dataset();
}