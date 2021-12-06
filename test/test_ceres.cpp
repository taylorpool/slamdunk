#include "test_ceres.hpp"

#include <gtest/gtest.h>
#include <ceres/ceres.h>

#include <iostream>

using namespace test_ceres;

int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);

    double initial_x = 5.0;
    double x = initial_x;
    ceres::Problem problem;

    ceres::CostFunction* cost_function = 
        new ceres::AutoDiffCostFunction<CostFunctor,1,1>(new CostFunctor);
    problem.AddResidualBlock(cost_function, nullptr, &x);

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    std::cout << summary.BriefReport() << std::endl;
    std::cout << "x: " << initial_x;
    std::cout << " -> " << x << std::endl;


    double x1 = 3.0;
    double x2 = -1.0;
    double x3 = 0.0;
    double x4 = 1.0;

    ceres::Problem powell_problem;

    powell_problem.AddResidualBlock(
        new ceres::AutoDiffCostFunction<F1,1,1,1>(new F1), nullptr, &x1, &x2
    );
    powell_problem.AddResidualBlock(
        new ceres::AutoDiffCostFunction<F2,1,1,1>(new F2), nullptr, &x3, &x4
    );
    powell_problem.AddResidualBlock(
        new ceres::AutoDiffCostFunction<F3,1,1,1>(new F3), nullptr, &x2, &x3
    );
    powell_problem.AddResidualBlock(
        new ceres::AutoDiffCostFunction<F4,1,1,1>(new F4), nullptr, &x1, &x4
    );

    ceres::Solve(options, &powell_problem, &summary);

    std::cout << summary.BriefReport() << std::endl;
    std::cout << "x1: " << x1 << " x2: " << x2 << " x3: " << x3 << " x4: " << x4 << std::endl;


    // Curve Fitting Problem!

    double true_m = 1.0;
    double true_c = -10.0;

    std::vector<double> X(100);
    std::vector<double> y(100);

    for(int index = 0; index < 100; ++index)
    {
        X[index] = index*0.1;
        double noise = static_cast<double>(rand())/(RAND_MAX) - 0.5;
        y[index] = exp(X[index]*true_m+true_c) + noise;
    }

    double m = 0.0;
    double c = 0.0;

    ceres::Problem curve_problem;


    for(int index = 0; index < y.size(); ++index)
    {
        curve_problem.AddResidualBlock(
            new ceres::AutoDiffCostFunction<ExponentialResidual,1,1,1>(
                new ExponentialResidual(X[index], y[index])), new ceres::CauchyLoss(0.5), &m, &c
        );
    }

    ceres::Solve(options, &curve_problem, &summary);

    std::cout << summary.BriefReport() << std::endl;
    std::cout << "m: " << m << " c: " << c << std::endl;
}