#include "fd_interpolate.h"

void fd_interpolate(
  const int nx,
  const int ny,
  const int nz,
  const double h,
  const Eigen::RowVector3d & corner,
  const Eigen::MatrixXd & P,
  SparseMatrixType& W)
{

    int i, j, k;
    double x_d, y_d, z_d;

    auto gridIdx= [&](int i, int j, int k) { return i + nx*(j + k * ny); };

    for (int idx = 0; idx < P.rows(); idx++) {

        //  Use trilinear interpolation(https://en.wikipedia.org/wiki/Trilinear_interpolation) to represent
        //  point 'S' as linear combination of the eight corners of a length 'h' grid element.
        //
        //       t               g
        //        +-----s11-----+
        //       /|     /      /|
        //      / |    s''    / |
        //     /  |   /|     /  |
        //  e +-----s10-----+ f |
        //    |   |    S    |   |
        //    | d +---s01-- | --+ c
        //    |  /    /     |  /
        //    | /    s'     | /
        //    |/    /       |/
        //    +----s00------+
        //    a            b
        //
        //   s00 = (1 - x_d) * a + x_d * b
        //   s01 = (1 - x_d) * d + x_d * c
        //   s'  = (1 - y_d) * s00 + y_d * s01
        //       = (1 - x_d) * (1 - y_d) * a + x_d * (1 - y_d) * b + (1 - x_d) * y_d * d + x_d * y_d * c
        //
        //   s10 = (1 - x_d) * e + x_d * f
        //   s11 = (1 - x_d) * t + x_d * g
        //   s'' = (1 - y_d) * s10 + y_d * s11
        //       = (1 - x_d) * (1 - y_d) * e + x_d * (1 - y_d) * f + (1 - x_d) * y_d * t + x_d * y_d * g
        //
        //   S = (1 - z_d) * s' + z_d * s'' =>
        //
        //   S = (1 - x_d) * (1 - y_d) * (1 - z_d) * a
        //     + x_d       * (1 - y_d) * (1 - z_d) * b
        //     + (1 - x_d) * y_d       * (1 - z_d) * d
        //     + x_d       * y_d       * (1 - z_d) * c
        //     + (1 - x_d) * (1 - y_d) * z_d       * e
        //     + x_d       * (1 - y_d) * z_d       * f
        //     + (1 - x_d) * y_d       * z_d       * t
        //     + x_d       * y_d       * z_d       * g
        // --------------------------------------------------

        // determine to which voxel `p` belongs
        Eigen::VectorXd p = P.row(idx);
        i = floor((p.x() - corner.x()) / h);
        j = floor((p.y() - corner.y()) / h);
        k = floor((p.z() - corner.z()) / h);

        auto a = corner + h * Eigen::RowVector3d(i, j, k);
        x_d = (p.x() - a.x()) / h;
        y_d = (p.y() - a.y()) / h;
        z_d = (p.z() - a.z()) / h;

        W.insert(idx, gridIdx(i, j, k))             = (1 - x_d) * (1 - y_d) * (1 - z_d); // a
        W.insert(idx, gridIdx(i + 1, j, k))         = x_d * (1 - y_d) * (1 - z_d);       // b
        W.insert(idx, gridIdx(i + 1, j + 1, k))     = x_d * y_d * (1 - z_d);             // c
        W.insert(idx, gridIdx(i, j + 1, k))         = (1 - x_d) * y_d * (1 - z_d);       // d
        W.insert(idx, gridIdx(i, j, k + 1))         = (1 - x_d) * (1 - y_d) * z_d;       // e
        W.insert(idx, gridIdx(i + 1, j, k + 1))     = x_d * (1 - y_d) * z_d;             // f
        W.insert(idx, gridIdx(i + 1, j + 1, k + 1)) = x_d * y_d * z_d;                   // g
        W.insert(idx, gridIdx(i, j + 1, k + 1))     = (1 - x_d) * y_d * z_d;             // t
    }

}
