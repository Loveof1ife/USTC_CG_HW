#include "RBF_warper.h"

namespace USTC_CG
{
void RBFWarper::update_transforms()
{
    if (n_ == 0)
        return;
    if (n_ == 1)
    {
        b_ = Eigen::Vector2f(
            (end_points_[0].x - start_points_[0].x),
            (end_points_[0].y - start_points_[0].y));
        A_ = A_ = Eigen::Matrix2f::Identity();
        alpha_ = Eigen::MatrixXf::Zero(1, 2);
        transforms_dirty_ = false;
        return;
    }
    else if (n_ == 2)
    {
        // Calculate translation + uniform scaling + rotation
        Eigen::Vector2f v_start(
            (start_points_[1].x - start_points_[0].x),
            start_points_[1].y - start_points_[0].y);
        Eigen::Vector2f v_end(
            (end_points_[1].x - end_points_[0].x),
            end_points_[1].y - end_points_[0].y);

        float scale = v_end.norm() / v_start.norm();
        float angle =
            atan2(v_end.y(), v_end.x()) - atan2(v_start.y(), v_start.x());

        A_ << scale * cos(angle), -scale * sin(angle), scale * sin(angle),
            scale * cos(angle);

        b_ = Eigen::Vector2f(end_points_[0].x, end_points_[0].y) -
             A_ * Eigen::Vector2f(start_points_[0].x, start_points_[0].y);
        alpha_ = Eigen::MatrixXf::Zero(2, 2);
        transforms_dirty_ = false;
        return;
    }

    // step1 : calculate supported radius -- the distance between each
    // control points
    r_.resize(n_);
    for (int i = 0; i < n_; ++i)
    {
        float min_dist = std::numeric_limits<float>::max();
        for (int j = 0; j < n_; ++j)
        {
            if (i == j)
                continue;

            float d = distance(start_points_[i], start_points_[j]);
            if (d < min_dist)
            {
                min_dist = d;
                r_[i] = min_dist;
            }
        }
    }
    // step2 : calculate linear system
    Eigen::MatrixXf G(n_, n_);

    for (int i = 0; i < n_; ++i)  // row
    {
        for (int j = 0; j < n_; ++j)  // col
        {
            float d = distance(start_points_[i], start_points_[j]);
            G(i, j) = 1.0f / (d * d + r_[i] * r_[i]);
        }
    }

    Eigen::MatrixXf C(n_, 3);

    for (int i = 0; i < n_; ++i)
    {
        C(i, 0) = start_points_[i].x;
        C(i, 1) = start_points_[i].y;
        C(i, 2) = 1.0f;
    }

    Eigen::MatrixXf Q(n_, 2);

    for (int i = 0; i < n_; ++i)
    {
        Q(i, 0) = end_points_[i].x;
        Q(i, 1) = end_points_[i].y;
    }

    Eigen::MatrixXf A(n_ + 3, n_ + 3);
    A.block(0, 0, n_, n_) = G;
    A.block(0, n_, n_, 3) = C;
    A.block(n_, 0, 3, n_) = C.transpose();
    A.block(n_, n_, 3, 3) = Eigen::MatrixXf::Zero(3, 3);

    Eigen::MatrixXf B(n_ + 3, 2);
    B.block(0, 0, n_, 2) = Q;
    B.block(n_, 0, 3, 2) = Eigen::MatrixXf::Zero(3, 2);

    Eigen::JacobiSVD<Eigen::MatrixXf> svd(
        A, Eigen::ComputeThinU | Eigen::ComputeThinV);

    Eigen::MatrixXf X = svd.solve(B);

    alpha_ = X.block(0, 0, n_, 2);

    Eigen::MatrixXf beta = X.block(n_, 0, 3, 2);

    A_ << beta(0, 0), beta(1, 0), beta(0, 1), beta(1, 1);
    b_ << beta(2, 0), beta(2, 1);

    transforms_dirty_ = false;
}

std::pair<int, int> RBFWarper::rbf_warping(int x, int y, int width, int height)
{
    Eigen::Vector2f p(x, y);
    Eigen::Vector2f result = A_ * p + b_;

    // 计算径向基部分
    for (size_t i = 0; i < n_; ++i)
    {
        Eigen::Vector2f pi(start_points_[i].x, start_points_[i].y);
        float d = (p - pi).norm();
        float g = 1.0f / (d * d + r_[i] * r_[i]);
        result += alpha_.row(i).transpose() * g;
    }

    // 边界约束
    int new_x = static_cast<int>(result.x());
    int new_y = static_cast<int>(result.y());
    return { new_x, new_y };
}

float RBFWarper::distance(const ImVec2& p1, const ImVec2& p2) const
{
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

}  // namespace USTC_CG