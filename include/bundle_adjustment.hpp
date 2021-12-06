#pragma once
#include <ceres/ceres.h>
#include <ceres/rotation.h>

#include <iostream>
#include <string>

namespace slamdunk
{

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
}