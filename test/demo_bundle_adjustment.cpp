#include "bundle_adjustment.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using Camera = slamdunk::Camera;
using Point = slamdunk::Point;
using Observation = slamdunk::Observation;

int main(int argc, char** argv)
{
    int num_cameras;
    int num_points;
    int num_observations;

    std::string read_file = argv[1];

    std::ifstream fin;
    fin.open(read_file);

    fin >> num_cameras;
    fin >> num_points;
    fin >> num_observations;

    Observation observations[num_observations];
    double cameras[num_cameras][9];
    double points[num_points][3];

    std::cout << "Loading..." << std::endl;

    for(int obs_index = 0; obs_index < num_observations; ++obs_index)
    {
        fin >> observations[obs_index].camera_index;
        fin >> observations[obs_index].point_index;
        fin >> observations[obs_index].x;
        fin >> observations[obs_index].y;
    }

    for(int camera_index = 0; camera_index < num_cameras; ++camera_index)
    {
        fin >> cameras[camera_index][Camera::rotate0];
        fin >> cameras[camera_index][Camera::rotate1];
        fin >> cameras[camera_index][Camera::rotate2];
        fin >> cameras[camera_index][Camera::translate0];
        fin >> cameras[camera_index][Camera::translate1];
        fin >> cameras[camera_index][Camera::translate2];
        fin >> cameras[camera_index][Camera::focal];
        fin >> cameras[camera_index][Camera::radial0];
        fin >> cameras[camera_index][Camera::radial1];
    }

    for(int point_index = 0; point_index < num_points; ++point_index)
    {
        fin >> points[point_index][Point::x];
        fin >> points[point_index][Point::y];
        fin >> points[point_index][Point::z];
    }

    fin.close();

    std::cout << "Loaded" << std::endl;

    ceres::Problem problem;
    for(int obs_index = 0; obs_index < num_observations; ++obs_index)
    {    
        auto obs = observations[obs_index];
        ceres::CostFunction* cost_function = slamdunk::ReprojectionError::Create(
            obs.x, obs.y);
        problem.AddResidualBlock(
            cost_function, nullptr, cameras[obs.camera_index], points[obs.point_index]);
    }

    std::cout << "Created Problem" << std::endl;

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_SCHUR;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;

    std::cout << "Solving..." << std::endl;
    ceres::Solve(options, &problem, &summary);
    std::cout << summary.FullReport() << std::endl;

    std::cout << "Saving to file..." << std::endl;
    std::string output_file = read_file+"-solved.txt";
    std::ofstream fout(output_file);
    fout << num_points << "\t" << num_cameras << std::endl;
    for(int point_index = 0; point_index < num_points; ++point_index)
    {
        fout << points[point_index][Point::x] << "\t";
        fout << points[point_index][Point::y] << "\t";
        fout << points[point_index][Point::z] << std::endl;
    }

    for(int camera_index = 0; camera_index < num_cameras; ++camera_index)
    {
        fout << cameras[camera_index][Camera::translate0] << "\t";
        fout << cameras[camera_index][Camera::translate1] << "\t";
        fout << cameras[camera_index][Camera::translate2] << std::endl;
    }

    fout.close();
    std::cout << "Wrote to file: " << output_file << std::endl;
}