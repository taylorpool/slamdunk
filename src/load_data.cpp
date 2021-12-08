#include <iostream>
#include <map>
#include <string>
#include <filesystem>
#include <sstream>
#include <ostream>
#include <istream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>

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
    /*
        # depth maps
        # file: 'rgbd_dataset_freiburg1_xyz.bag'
        # timestamp filename
        1305031102.160407 depth/1305031102.160407.png
        1305031102.194330 depth/1305031102.194330.png
    */

    // loops through each image individually // note that for
    // all these push_backs order must be maintained
    int file_vec_size = all_sorted_files.size();
    for (int i=0; i < file_vec_size; i++)
    {   
        // std::cout << all_sorted_files[i] << std::endl;
        if (counter > num_images) { break; }

        std::string rgb_image_name = all_sorted_files[i];

        // find the filename and extension from path
        int index = rgb_image_name.find_last_of("/\\");
        std::string base_filename = rgb_image_name.substr(index+1);

        // remove file extension
        size_t lastindex = base_filename.find_last_of("."); 
        std::string raw_name = base_filename.substr(0, lastindex); 
        std::cout << "raw_name: " << raw_name << std::endl;

        // GET CAMERA POSES
        std::vector<double> curr_cam_poses = grab_camera_poses(raw_name);
        camera_poses.push_back(curr_cam_poses);


        // GET DEPTH
        std::string depth_filename = find_depth_filename(raw_name);
        std::cout << "Depth filename found: " << depth_filename << std::endl << std::endl;
        if (depth_filename == "NO FILE") { throw std::invalid_argument( "*** NO DEPTH FILE FOUND ***" ); }

        const cv::Mat image = cv::imread(all_sorted_files[i], 0); //Load as grayscale
        std::vector<cv::KeyPoint> keypoints;
        cv::Mat desc;

        detector->detect(image, keypoints);
        descriptor->compute(image, keypoints, desc);

        std::vector<double> img_depths;
        for (int j=0; j < keypoints.size(); j++)
        {
            int x = keypoints[j].pt.x;
            int y = keypoints[j].pt.y;
            int depth = find_depth_in_image(depth_filename, x, y);
            img_depths.push_back(depth); // add key feature depth to vec

        }
        feature_depth.push_back(img_depths); // add depth vec to all depths vec


        //cv::Point2f first_point = keypoints[0].pt;
        //std::cout << "\nKey point xy: " << first_point << std::endl;

        feature_vec.push_back( keypoints );  // add keypoints to keypoint vec
        descr_vec.push_back(std::make_shared<cv::Mat>(desc)); // add descr to vec
        temp_imgs.push_back(std::make_shared<cv::Mat>(image)); // used for drawing

        plot_logic(plot, counter, image, keypoints);
        counter++;
    }
}

// Helper Functions
int Dataloader::find_depth_in_image(std::string depth_name, int x, int y)
{
    std::string full_path = "../datasets/rgbd_dataset_freiburg1_xyz/" + depth_name; // creates full path
    const cv::Mat image = cv::imread(full_path, 0); //Load as grayscale
    int depth = (int)image.at<uchar>(x, y);
    return depth;
}


std::string Dataloader::find_depth_filename(std::string file_name_in)
{
    std::string depth_file = "../datasets/rgbd_dataset_freiburg1_xyz/depth.txt";
    std::ifstream file(depth_file);
    std::string string_in;
    //std::cout << "file name in: " << file_name_in << std::endl;

    double file_name_in_double = std::stod(file_name_in); // converts incoming file name to double

    double closest_num = std::numeric_limits<double>::infinity();
    std::string closest_name = "NO FILE";
    
    while (std::getline(file, string_in)) {
        //std::cout << "\nFile name in the find depth filename function: " << file_name_in << std::endl;
	    std::istringstream iss(string_in);
        bool record_next = false;
	    for (std::string s; iss >> s; )
        {
            if (record_next) { closest_name = s; }
            else
            {
                double time_stamp = std::stod(s); //  converts string to double
                double diff = std::abs(( file_name_in_double - time_stamp ));
                if (diff < closest_num)
                {
                    closest_num = diff; // allows us to update closest timestamp to input file
                    record_next = true; // allows us to record the closest file name
                }
            }
            if (!record_next) { break; } // allows us to move on if the timestapmp we just looked at isn't close
            
        }
    }
    return closest_name;
}

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

void Dataloader::create_dataset()
{
    using Match = std::vector<int>;
    std::map<int, std::vector<Match>> match_map;

    int obs_counter = 0;
    int point_counter = 0;
    for (int i=0; i < all_good_matches.size(); i++)
    {
        std::vector<cv::DMatch> curr_match = all_good_matches[i];    
        //std::cout << "Size of match object: " << curr_match.size() << std::endl;
        // If the key does not yet exist in our map
        if(i == 0)
        {
            if(match_map.count(i) == 0)
            {
                match_map[i] = std::vector<Match>();
                std::cout << "Creating empty vector" << std::endl;
            }
            // For zeroeth camera, we add all matched feature points
            // between cameras 0 and 1 and increment point counter with
            // every matched feature
            for (int j=0; j<curr_match.size(); j++)
            {
                int curr_kf_idx = curr_match[j].trainIdx;
                int next_kf_idx = curr_match[j].queryIdx;
                match_map[i].push_back(Match({curr_kf_idx, next_kf_idx, point_counter}));
                ++obs_counter;
                ++point_counter;
            }
            // Pretend like we iterated to camera 2
            ++i;
        }
        // Everything but zeroeth camera
        // If we have not seen this camera before
        if(match_map.count(i) == 0)
        {
            match_map[i] = std::vector<Match>();
            std::cout << "Creating empty vector" << std::endl;
        }
        int last_key = i-1;
        auto last_vector = match_map[last_key];
        // Iterates over all key point features of
        // the two images of interest
        for (int j = 0; j < curr_match.size(); j++)
        {
            int curr_kf_idx = curr_match[j].trainIdx;
            int next_kf_idx = curr_match[j].queryIdx;

            int point_number = -1;
            for(int k = 0; k < last_vector.size(); ++k)
            {
                // Set to correctly consolidate duplicate points
                // between camera matches
                // int point_number = -1;
                int parent_query_kf = last_vector[k][1];
                // Check if current feature match has already
                // been found
                if(parent_query_kf == curr_kf_idx)
                { 
                    point_number = last_vector[k][2];
                    std::cout << "parent == cur " << std::to_string(curr_kf_idx) << " " << std::to_string(next_kf_idx) << " " << std::to_string(point_number) << std::endl;
                    match_map[i].push_back(Match({
                        curr_kf_idx, next_kf_idx, point_number}));
                    ++obs_counter;
                    break; // *** I THINK THIS IS OK BUT NOT SURE! IT WOULD MEAN THAT VECTOR OF MATCH OBJECTS HAS UNIQUE KEY FEATURES
                }
                // Check if there is a new point according
                // to our given feature indices
            }
            if(point_number == -1)
            {
                std::cout << "Point counter in point_number == -1: " << std::to_string(point_counter) << std::endl;
                match_map[i].push_back(Match({
                    curr_kf_idx, next_kf_idx, point_counter}));
                ++obs_counter;
                ++point_counter;
            }
        }
        // Iterate through the last vector
    }
    std::cout << "Match map size: " << match_map.size() << std::endl;
    std::cout << "Camera pose size: " << camera_poses.size() << std::endl;


    // OPEN FILE TO WRITE TO
    std::ofstream fout("../datasets/out_feature_points.txt");
    fout << camera_poses.size() << " " << point_counter << " " << obs_counter << "\n";


    std::vector<std::vector<double>> XYZ;
    // Iterate over all map keys and values
    // std::map<int, std::vector<Match>> match_map;
    // Match = std::vector<int>;
    for(std::map<int,std::vector<Match>>::iterator iter = match_map.begin(); iter != match_map.end(); ++iter)
    {
        int img_idx =  iter->first; // int
        std::vector<Match> match_point_vec = iter->second; // vector of vectors of ints which are curr_kf_idx, next_kf_idx, point_number
        std::cout << "\nCurrent Image index: " << std::to_string(img_idx) << std::endl;
        std::cout << "All x, y, p in curr image: " << std::endl;
        for (int jk=0; jk<match_point_vec.size(); jk++)
        {
            int key_feature_idx_1 = match_point_vec[jk][0];
            // int key_feature_idx_2 = match_point_vec[jk][1];

            std::cout << "\nkey point idx 1: " <<  key_feature_idx_1 << std::endl;
            // std::cout << "key point idx 2: " <<  key_feature_idx_2 << std::endl;

            int associated_point = match_point_vec[jk][2];
            // std::cout << key_feature_idx_1 << " " << key_feature_idx_2 << " " << associated_point << std::endl;

            std::vector<cv::KeyPoint> feature_vec_left = feature_vec[img_idx];
            std::vector<double> depth_vec = feature_depth[img_idx];

            std::cout << "feature_vec_left size: " <<  feature_vec_left.size() << std::endl;
            std::cout << "depth_vec size: " <<  depth_vec.size() << std::endl;

            cv::KeyPoint key_point = feature_vec_left[key_feature_idx_1];

            double k1_x = key_point.pt.x;
            double k1_y = key_point.pt.y;
            int depth = depth_vec[key_feature_idx_1];

            // Write to file, observations:
            fout << img_idx << " " << associated_point << " " << k1_x << " " << k1_y << "\n";
            
            std::cout << "Key point one x: " << k1_x << " Key point one y: " << k1_y << std::endl;
            std::cout << "Key point depth: " << depth << std::endl;
            
            double factor = 5000;
            double fx = 525.0; 
            double fy = 525.0;
            double cx = 319.5;
            double cy = 239.5;
            
            // in terms of the camera frame
            double Z = depth / factor;
            double X = (k1_x - cx) * Z / fx;
            double Y = (k1_y - cy) * Z / fy;
            std::cout << "X: " << std::to_string(X) << " Y: " << std::to_string(Y) << " Z: " << std::to_string(Z) << std::endl;

            // store XYZ in camera frame
            std::vector<double> temp_holder_vector;
            temp_holder_vector.push_back(X);
            temp_holder_vector.push_back(Y);
            temp_holder_vector.push_back(Z);
            XYZ.push_back(temp_holder_vector);
        }
    }

    // write camera poses to file
    // cam params
    double fx = 525.0;
    double fy = 525.0;
    double cx = 319.5;
    double cy = 239.5;
    double d0 = 0.2624;
    double d1 = -0.9531;
    double d2 = -0.0054;
    double d3 = 0.0026;
    double d4 = 1.1633;
    for (int jk=0; jk<camera_poses.size(); jk++)
    {
        //pose vector is of form tx ty tz qx qy qz qw
        std::vector<double> pose_vector = camera_poses[jk];
        for (int kl=0; kl<pose_vector.size(); kl++)
        {
            // tx ty tz qx qy qz qw fx fy cx cy d0 d1 d2 d3 d4
            fout << pose_vector[kl] << "\n";
        }
        fout << fx << "\n";
        fout << fy << "\n";
        fout << cx << "\n";
        fout << cy << "\n";
        fout << d0 << "\n";
        fout << d1 << "\n";
        fout << d2 << "\n";
        fout << d3 << "\n";
        fout << d4 << "\n";

    }

    for (int xyz=0; xyz<XYZ.size(); xyz++)
    {
        fout << XYZ[xyz][0] << "\n" <<  XYZ[xyz][1] << "\n" << XYZ[xyz][2] << "\n"; 
    }
    fout.close();

}

std::vector<double> Dataloader::grab_camera_poses(std::string file_name_in)
{
    /*
        # ground truth trajectory
        # file: 'rgbd_dataset_freiburg1_xyz.bag'
        # timestamp tx ty tz qx qy qz qw
    */
    std::string ground_truth_path = "../datasets/rgbd_dataset_freiburg1_xyz/groundtruth.txt";
    std::ifstream file(ground_truth_path);
    std::string string_in;

    double file_name_in_double = std::stod(file_name_in); // converts incoming file name to double

    double closest_num = std::numeric_limits<double>::infinity();
    std::vector<double> trans_and_quats;
    
    while (std::getline(file, string_in)) {
	    std::istringstream iss(string_in);
        bool record_next = false;
	    for (std::string s; iss >> s; )
        {
            if (record_next) // if new closest line from groundtruth file
            { 
                trans_and_quats.push_back( std::stod(s) ); // add the double value to return vector
            }
            else
            {
                double time_stamp = std::stod(s); //  converts string to double, this is only ever timestamp
                double diff = std::abs(( file_name_in_double - time_stamp ));
                if (diff < closest_num)
                {
                    closest_num = diff; // allows us to update closest timestamp to input file
                    record_next = true; // allows us to record the closest file name
                    trans_and_quats.clear(); // clears the vector of the last closest stuff to make room for the new stuff
                }
            }
            if (!record_next) { break; } // allows us to move on if the timestapmp we just looked at isn't close
            
        }
    }
    // std::cout << "\nCurrent results for trans quats: " << std::endl;
    // for (int i=0; i<trans_and_quats.size(); i++)
    // {
    //     std::cout << std::to_string(trans_and_quats[i]) << std::endl;
    // }
    return trans_and_quats;
}



// Dataloader getter functions
std::string Dataloader::get_dataset_name() { return dataset_name; }
std::string Dataloader::get_dataset_path() { return dataset_path; }
std::vector<std::vector<cv::KeyPoint>> Dataloader::get_feature_vec() { return feature_vec; }
std::vector<std::shared_ptr<cv::Mat>> Dataloader::get_descr_vec() { return descr_vec; }
std::vector<std::vector<cv::DMatch>> Dataloader::get_good_matches() { return all_good_matches; }
std::vector<std::vector<double>> Dataloader::get_depth_vector() { return feature_depth; }

