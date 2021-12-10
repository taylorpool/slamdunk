#pragma once
#include <ceres/ceres.h>
#include <ceres/rotation.h>

#include <iostream>
#include <string>

namespace slamdunk
{


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

struct ReprojectionError
{
    ReprojectionError(double observed_x, double observed_y)
        : observed_x_(observed_x), observed_y_(observed_y) {}

    template <typename T>
    bool operator()(const T* const camera, const T* const point, T* residuals) const
    {
        T p[3];
        ceres::AngleAxisRotatePoint(camera, point, p);
        p[Point::x] += camera[Camera::translate0];
        p[Point::y] += camera[Camera::translate1];
        p[Point::z] += camera[Camera::translate2];

        T xp = -p[Point::x]/p[Point::z];
        T yp = -p[Point::y]/p[Point::z];

        const T& l1 = camera[Camera::radial0];
        const T& l2 = camera[Camera::radial1];
        T r2 = xp*xp + yp*yp;
        T distortion = 1.0 + r2*(l1 + l2*r2);

        const T& focal = camera[Camera::focal];
        T predicted_x = focal*distortion*xp;
        T predicted_y = focal*distortion*yp;

        residuals[Point::x] = predicted_x - static_cast<T>(observed_x_);
        residuals[Point::y] = predicted_y - static_cast<T>(observed_y_);

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

enum TumCamera
{
    TranslateX,
    TranslateY,
    TranslateZ,
    QuaternionX,
    QuaternionY,
    QuaternionZ,
    QuaternionW,
    FocalX,
    FocalY,
    CenterX,
    CenterY,
    Distortion0,
    Distortion1,
    Distortion2,
    Distortion3,
    Distortion4
};

struct TumReprojectionError
{
    TumReprojectionError(double observed_x, double observed_y, 
        double focal_x, double focal_y, double center_x, double center_y)
    : observed_x_(observed_x), observed_y_(observed_y),
        focal_x_(focal_x), focal_y_(focal_y),
        center_x_(center_x), center_y_(center_y) {}

    template <typename T>
    bool operator()(const T* const camera, const T* const point, T* residuals) const
    {
        T p[3];
        T quat[4] = {
            -camera[TumCamera::QuaternionX],
            -camera[TumCamera::QuaternionY],
            -camera[TumCamera::QuaternionZ],
            camera[TumCamera::QuaternionW]};

        ceres::QuaternionRotatePoint(quat, point, p);
        p[0] -= camera[TumCamera::TranslateX];
        p[1] -= camera[TumCamera::TranslateY];
        p[2] -= camera[TumCamera::TranslateZ];

        T xp = static_cast<T>(focal_x_)*point[Point::x]/point[Point::z] + static_cast<T>(center_x_);
        T yp = static_cast<T>(focal_y_)*point[Point::y]/point[Point::z] + static_cast<T>(center_y_);

        residuals[0] = xp - observed_x_;
        residuals[1] = yp - observed_y_;
        return true;
    }

    static ceres::CostFunction* Create(const double observed_x, const double observed_y, const double camera[])
    {
        return (new ceres::AutoDiffCostFunction<TumReprojectionError,2,7,3>(
            new TumReprojectionError(observed_x, observed_y, 
                camera[TumCamera::FocalX], camera[TumCamera::FocalY],
                camera[TumCamera::CenterX], camera[TumCamera::CenterY])
        ));
    }

    private:
        double observed_x_;
        double observed_y_;
        double focal_x_;
        double focal_y_;
        double center_x_;
        double center_y_;
        double distortion[4];
};

template <int NumCameras, int NumPoints, int NumObservations, int CameraDim>
struct BundleAdjustmentProblem
{
    double cameras[NumCameras][CameraDim];
    double points[NumPoints][3];
    Observation observations[NumObservations];
};
}