#include <ceres/ceres.h>
#include <ceres/rotation.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


struct ReprojectionError
{
    ReprojectionError(double observed_x, double observed_y)
        : observed_x_(observed_x), observed_y_(observed_y) {}

    template <typename T>
    bool operator()(const T* const camera, const T* const point, T* residuals) const
    {
        T p[3];
        ceres::AngleAxisRotatePoint(camera, point, p);
        p[0] += camera[3];
        p[1] += camera[4];
        p[2] += camera[5];

        T xp = -p[0]/p[2];
        T yp = -p[1]/p[2];

        const T& l1 = camera[7];
        const T& l2 = camera[8];
        T r2 = xp*xp + yp*yp;
        T distortion = 1.0 + r2*(l1 + l2*r2);

        const T& focal = camera[6];
        T predicted_x = focal*distortion*xp;
        T predicted_y = focal*distortion*yp;

        residuals[0] = predicted_x - static_cast<T>(observed_x_);
        residuals[1] = predicted_y - static_cast<T>(observed_y_);

        return true;
    }

    static ceres::CostFunction* Create(const double observed_x, const double observed_y)
    {
        return (new ceres::AutoDiffCostFunction<ReprojectionError,2,9,3>(
            new ReprojectionError(observed_x, observed_y)
        ));
    }

    private:
        double observed_x_;
        double observed_y_;
};

struct Observation
{
    Observation(int cam_index=0, int point_index=0, double x=0.0, double y=0.0)
    : camera_index(cam_index), point_index(point_index), x(x), y(x) {}

    std::string toString() const
    {
        std::stringstream ss;
        ss << "Camera Index: " << camera_index << std::endl;
        ss << "Point Index: " << point_index << std::endl;
        ss << "X: " << x << std::endl;
        ss << "Y: " << y << std::endl;
        return ss.str(); 
    }

    int camera_index;
    int point_index;
    double x;
    double y;
};

enum Camera
{
    rotate0,
    rotate1,
    rotate2,
    translate0,
    translate1,
    translate2,
    focal,
    radial0,
    radial1
};

enum Point
{
    x,
    y,
    z
};

int main(int argc, char** argv)
{
    int num_cameras;
    int num_points;
    int num_observations;

    std::ifstream fin;
    fin.open("/Users/taylorpool/slamdunk/test/problem-1723-156502-pre.txt");
    std::cout<<"Got here"<<std::endl;

    fin >> num_cameras;
    fin >> num_points;
    fin >> num_observations;

    Observation observations[num_observations];
    double cameras[num_cameras][9];
    double points[num_points][3];
    std::cout<<"Got here"<<std::endl;

    for(int obs_index = 0; obs_index < num_observations; ++obs_index)
    {
        fin >> observations[obs_index].camera_index;
        fin >> observations[obs_index].point_index;
        fin >> observations[obs_index].x;
        fin >> observations[obs_index].y;
        std::cout << observations[obs_index].toString() << std::endl;
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

    ceres::Problem problem;
    for(int obs_index = 0; obs_index < num_observations; ++obs_index)
    {    
        auto obs = observations[obs_index];
        ceres::CostFunction* cost_function = ReprojectionError::Create(
            obs.x, obs.y);
        problem.AddResidualBlock(cost_function, nullptr, cameras[obs.camera_index], points[obs.point_index]);
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_SCHUR;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;

    ceres::Solve(options, &problem, &summary);
    std::cout << summary.FullReport() << std::endl;

    std::ofstream fout("/Users/taylorpool/slamdunk/test/solved-1723-156502.txt");
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
}