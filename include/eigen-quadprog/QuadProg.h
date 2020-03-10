// This file is part of eigen-quadprog.
//
// eigen-quadprog is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// eigen-quadprog is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with eigen-quadprog.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

// Eigen
#include <Eigen/Core>
#include <Eigen/Sparse>

namespace Eigen {

extern "C" int qpgen1_(double* dmat, double* dvec, const int* fddmat, const int* n, double* sol, double* crval, double* amat,
                       const int* iamat, double* bvec, const int* fdamat, const int* q, const int* meq, int* iact, int* nact, int* iter,
                       double* work, const int* ierr);

extern "C" int qpgen2_(double* dmat, double* dvec, const int* fddmat, const int* n, double* sol, double* crval, double* amat, double* bvec,
                       const int* fdamat, const int* q, const int* meq, int* iact, int* nact, int* iter, double* work, const int* ierr);

/** Common method for Quadprog solver classes.
 *
 * \f{align}{
 *   \underset{{x} \in \mathbb{R}^n}{\text{minimize}} & \ \frac{1}{2}{x^TQx} + {c^Tx} \nonumber \\
 *   \text{subject to} & \ A_{eq} x = b_{eq} \\
 *   & \ A_{ineq} x \leq b_{ineq}
 * \f}
 *
 */
class QuadProgCommon {
 public:
  /** Default constructor.
   *
   */
  QuadProgCommon();

  /** Two-dimensional iteration vector.
   *
   * First component gives the number of "main" iterations, the second one
   * says how many constraints were deleted after they became active.
   *
   */
  const VectorXi& iter() const;

  /** Failure code, 0 is success.
   *
   */
  int fail() const;

  /** Solution found by the solver.
   *
   */
  const VectorXd& result() const;

  /** Set problem dimensions.
   *
   * \param nrvar Dimension \f$n\f$ of optimization vector \f$x \in
   * \mathbb{R}^n\f$.
   *
   * \param nreq Number of equality constraints, i.e. number of lines of
   * \f$A_{eq}\f$ and \f$b_{eq}\f$.
   *
   * \param nrineq Number of inequality constraints, i.e. number of lines of
   * \f$A_{ineq}\f$ and \f$b_{ineq}\f$.
   *
   */
  void problem(int nrvar, int nreq, int nrineq);

 protected:
  void fillQCBf(int nreq, int nrineq, const Ref<const MatrixXd>& Q, const Ref<const VectorXd>& C, const Ref<const VectorXd>& Beq,
                const Ref<const VectorXd>& Bineq, bool isDecomp);

 protected:
  MatrixXd Q_;    /**< Cost function matrix */
  VectorXd C_;    /**< Cost function vector */
  VectorXd B_;    /**< Inequality constraint vector. \note ENTRIES CORRESPONDING TO EQUALITY CONSTRAINTS MAY HAVE CHANGED SIGNS ON EXIT */
  VectorXd X_;    /**< Solution of the quadratic program */
  int fail_;      /**< Failure code, 0 is success */
  VectorXi iact_; /**< Vector of constraints that are active in solution */
  VectorXi iter_; /**< 2x1 vector, first component gives the number of "main"
                    iterations, the second one says how many constraints were
                    deleted after they became active */
  VectorXd work_; /**< Working space vector with length at least
                    \f$2n+r(r+5)/2+2q+1\f$ where \f$r=\min(n,q)\f$ */
};

/** Dense quadratic program.
 *
 * \f{align}{
 *   \underset{{x} \in \mathbb{R}^n}{\text{minimize}} & \ \frac{1}{2}{x^TQx} + {c^Tx} \nonumber \\
 *   \text{subject to} & \ A_{eq} x = b_{eq} \\
 *   & \ A_{ineq} x \leq b_{ineq}
 * \f}
 *
 */
class QuadProgDense : public QuadProgCommon {
 public:
  QuadProgDense();
  QuadProgDense(int nrvar, int nreq, int nrineq);

  /** Set problem dimensions.
   *
   * \param nrvar Dimension \f$n\f$ of optimization vector \f$x \in
   * \mathbb{R}^n\f$.
   *
   * \param nreq Number of equality constraints, i.e. number of lines of
   * \f$A_{eq}\f$ and \f$b_{eq}\f$.
   *
   * \param nrineq Number of inequality constraints, i.e. number of lines of
   * \f$A_{ineq}\f$ and \f$b_{ineq}\f$.
   *
   */
  void problem(int nrvar, int nreq, int nrineq);

  /** Solve quadratic program.
   *
   * \param Q Cost function matrix.
   *
   * \param C Cost function vector.
   *
   * \param Aeq Matrix of equality constraints \f$A_{eq} x = b_{eq}\f$.
   *
   * \param Beq Vector of equality constraints \f$A_{eq} x = b_{eq}\f$.
   *
   * \param Aineq Matrix of equality constraints \f$A_{ineq} x \leq b_{ineq}\f$.
   *
   * \param Bineq Vector of equality constraints \f$A_{ineq} x \leq b_{ineq}\f$.
   *
   * \param isDecomp If false (default), Q is not decomposed. If true, the
   * solver will assume that Q is already decomposed into \f$Q = R^T R\f$ and
   * it is given \f$R^{-1}\f$.
   *
   * \return success True if the solver found a solution.
   *
   */
  bool solve(const Ref<const MatrixXd>& Q, const Ref<const VectorXd>& C, const Ref<const MatrixXd>& Aeq, const Ref<const VectorXd>& Beq,
             const Ref<const MatrixXd>& Aineq, const Ref<const VectorXd>& Bineq, bool isDecomp = false);

 private:
  MatrixXd A_; /**< Inequality constraint matrix. \note ENTRIES CORRESPONDING TO EQUALITY CONSTRAINTS MAY HAVE CHANGED SIGNES ON EXIT */
};

/** Sparse quadratic program.
 *
 * \f{align}{
 *   \underset{{x} \in \mathbb{R}^n}{\text{minimize}} & \ \frac{1}{2}{x^TQx} + {c^Tx} \nonumber \\
 *   \text{subject to} & \ A_{eq} x = b_{eq} \\
 *   & \ A_{ineq} x \leq b_{ineq}
 * \f}
 *
 */
class QuadProgSparse : public QuadProgCommon {
 public:
  /** Default constructor.
   *
   */
  QuadProgSparse();

  /** Constructor with problem dimensions.
   *
   * \param nrvar Dimension \f$n\f$ of optimization vector \f$x \in
   * \mathbb{R}^n\f$.
   *
   * \param nreq Number of equality constraints, i.e. number of lines of
   * \f$A_{eq}\f$ and \f$b_{eq}\f$.
   *
   * \param nrineq Number of inequality constraints, i.e. number of lines of
   * \f$A_{ineq}\f$ and \f$b_{ineq}\f$.
   *
   */
  QuadProgSparse(int nrvar, int nreq, int nrineq);

  /** Set problem dimensions.
   *
   * \param nrvar Dimension \f$n\f$ of optimization vector \f$x \in
   * \mathbb{R}^n\f$.
   *
   * \param nreq Number of equality constraints, i.e. number of lines of
   * \f$A_{eq}\f$ and \f$b_{eq}\f$.
   *
   * \param nrineq Number of inequality constraints, i.e. number of lines of
   * \f$A_{ineq}\f$ and \f$b_{ineq}\f$.
   *
   */
  void problem(int nrvar, int nreq, int nrineq);

  /** Solve quadratic program.
   *
   * \param Q Cost function matrix.
   *
   * \param C Cost function vector.
   *
   * \param Aeq Matrix of equality constraints \f$A_{eq} x = b_{eq}\f$.
   *
   * \param Beq Vector of equality constraints \f$A_{eq} x = b_{eq}\f$.
   *
   * \param Aineq Matrix of equality constraints \f$A_{ineq} x \leq b_{ineq}\f$.
   *
   * \param Bineq Vector of equality constraints \f$A_{ineq} x \leq b_{ineq}\f$.
   *
   * \param isDecomp If false (default), Q is not decomposed. If true, the
   * solver will assume that Q is already decomposed into \f$Q = R^T R\f$ and
   * it is given \f$R^{-1}\f$.
   *
   * \return success True if the solver found a solution.
   *
   */
  bool solve(const Ref<const MatrixXd>& Q, const Ref<const VectorXd>& C, const SparseMatrix<double>& Aeq, const Ref<const VectorXd>& Beq,
             const SparseMatrix<double>& Aineq, const Ref<const VectorXd>& Bineq, bool isDecomp = false);

 private:
  MatrixXd A_;  /**< Inequality constraint matrix. \note ENTRIES CORRESPONDING TO EQUALITY CONSTRAINTS MAY HAVE CHANGED SIGNS ON EXIT */
  MatrixXi iA_; /**< Inequality constraint matrix: these two matrices store
                  the matrix A in compact form. the format is:
                  \f[
                    A = [A1 A2]
                  \f]
         - iA_(1,i) is the number of non-zero elements in column i of A
         - iA_(k,i) for k>=2, is equal to j if the (k-1)-th non-zero element in column i of A is A(i,j)
         - A_(k,i) for k>=1, is equal to the k-th non-zero element
                    in column i of A. */
};

}  // namespace Eigen
