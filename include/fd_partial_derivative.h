#ifndef FD_PARTIAL_DERIVATIVE_H
#define FD_PARTIAL_DERIVATIVE_H
#include <Eigen/Sparse>
// Construct a partial derivative matrix for a finite-difference grid in a
// given direction. Derivative are computed using first-order differences onto
// a staggered grid
//
// Inputs:
//   nx  number of grid steps along the x-direction
//   ny  number of grid steps along the y-direction
//   nz  number of grid steps along the z-direction
//   h  grid step size
//   dir  index indicating direction: 0-->x, 1-->y, 2-->z
// Outputs:
//   D  m by nx*ny*nz sparse partial derivative matrix, where:
//     m = (nx-1)*ny*nz  if dir = 0
//     m = nx*(ny-1)*nz  if dir = 1
//     m = nx*ny*(nz-1)  otherwise (if dir = 2)
//
// See also: fd_partial_derivative.h
//
//       *--------o-------*--------o-------*
//       |        |       |        |       |
//       *--------o-------*--------o-------*
//       |        |       |        |       |
//       *--------o-------*--------o-------*
//
//   Consider a 3 x 3 example, where values of g: {0,1,2} x {0,1,2} => ℤ lie on the grid points (*).
//   The derivative maps g into the smaller scattered grid space. Furthermore, the derivative
//   values are taken between the grid nodes (at 'o'). In this small example, the derivative (in x, column-wise)
//   grid is 2 x 3 and the corresponding transformation is a 6 x 9 matrix.
//
//       ┌                         ┐  ┌         ┐
//       | -1  1 0  0  0 0  0  0 0 |  | g_(0,0) |
//       |  0 -1 1  0  0 0  0  0 0 |  | g_(1,0) |
//       |  0  0 0 -1  1 0  0  0 0 |  | g_(2,0) |
//       |  0  0 0  0 -1 1  0  0 0 |  | g_(0,1) |
//       |  0  0 0  0  0 0 -1  1 0 |  | g_(1,1) |
//       |  0  0 0  0  0 0  0 -1 1 |  | g_(2,1) |
//       └                         ┘  | g_(0,2) |
//                                    | g_(1,2) |
//                                    | g_(2,2) |
//                                    └         ┘

using SparseMatrixType = Eigen::SparseMatrix<double, Eigen::RowMajor>;

void fd_partial_derivative(
  const int nx,
  const int ny,
  const int nz,
  const double h,
  const int dir,
  SparseMatrixType& D);
#endif
