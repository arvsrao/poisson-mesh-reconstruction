#include "poisson_surface_reconstruction.h"
#include "fd_grad.h"
#include "fd_interpolate.h"
#include <igl/copyleft/marching_cubes.h>
#include <algorithm>

void poisson_surface_reconstruction(
    const Eigen::MatrixXd & P,
    const Eigen::MatrixXd & N,
    Eigen::MatrixXd & V,
    Eigen::MatrixXi & F)
{
  ////////////////////////////////////////////////////////////////////////////
  // Construct FD grid, CONGRATULATIONS! You get this for free!
  ////////////////////////////////////////////////////////////////////////////
  // number of input points
  const int n = P.rows();
  // Grid dimensions
  int nx, ny, nz;
  // Maximum extent (side length of bounding box) of points
  double max_extent =
    (P.colwise().maxCoeff()-P.colwise().minCoeff()).maxCoeff();
  // padding: number of cells beyond bounding box of input points
  const double pad = 8;
  // choose grid spacing (h) so that shortest side gets 30+2*pad samples
  double h  = max_extent/double(30+2*pad);
  // Place bottom-left-front corner of grid at minimum of points minus padding
  Eigen::RowVector3d corner = P.colwise().minCoeff().array()-pad*h;
  // Grid dimensions should be at least 3 
  nx = std::max((P.col(0).maxCoeff()-P.col(0).minCoeff()+(2.*pad)*h)/h,3.);
  ny = std::max((P.col(1).maxCoeff()-P.col(1).minCoeff()+(2.*pad)*h)/h,3.);
  nz = std::max((P.col(2).maxCoeff()-P.col(2).minCoeff()+(2.*pad)*h)/h,3.);
  // Compute positions of grid nodes
  Eigen::MatrixXd x(nx*ny*nz, 3);
  for(int i = 0; i < nx; i++) 
  {
    for(int j = 0; j < ny; j++)
    {
      for(int k = 0; k < nz; k++)
      {
         // Convert subscript to index
         const auto ind = i + nx*(j + k * ny);
         x.row(ind) = corner + h*Eigen::RowVector3d(i,j,k);
      }
    }
  }
  Eigen::VectorXd g = Eigen::VectorXd::Zero(nx*ny*nz);

  /// NOTE: The provided FD grid is a regular grid not an adaptive grid.
    SparseMatrixType G((nx-1)*ny*nz + nx*(ny-1)*nz + nx*ny*(nz-1), nx*ny*nz),
          W(n, nx*ny*nz),
          Wx(n, (nx-1)*ny*nz),
          Wy(n, nx*(ny-1)*nz),
          Wz(n, nx*ny*(nz-1));

  fd_grad(nx, ny, nz, h, G);
  fd_interpolate(nx, ny, nz, h,corner, P, W);
  fd_interpolate(nx-1, ny, nz, h,corner + h*Eigen::RowVector3d(0.5,0,0), P, Wx);
  fd_interpolate(nx, ny-1, nz, h,corner + h*Eigen::RowVector3d(0,0.5,0), P, Wy);
  fd_interpolate(nx, ny, nz-1, h,corner + h*Eigen::RowVector3d(0,0,0.5), P, Wz);

  Eigen::MatrixXd vx, vy, vz;
  vx = Wx.transpose() * N.col(0);
  vy = Wy.transpose() * N.col(1);
  vz = Wz.transpose() * N.col(2);

  Eigen::MatrixXd v((nx-1)*ny*nz + nx*(ny-1)*nz + nx*ny*(nz-1), 1);
  v << vx, vy, vz;

  // use conjugate gradient to solve for g, where Ag = b and A = G^T * G and b = G^T * v
  Eigen::BiCGSTAB<SparseMatrixType> solver(G.transpose() * G);
  g = solver.solve(G.transpose() * v);

  // compute a "good" iso-value a suggested by As suggested in [Kazhdan et al. 2006].
  double sigma = (W * g).sum() / (double) n;
  g -= sigma * Eigen::VectorXd::Ones(nx*ny*nz);

  ////////////////////////////////////////////////////////////////////////////
  // Run black box algorithm to compute mesh from implicit function: this
  // function always extracts g=0, so "pre-shift" your g values by -sigma
  ////////////////////////////////////////////////////////////////////////////
  igl::copyleft::marching_cubes(g, x, nx, ny, nz, V, F);
}
