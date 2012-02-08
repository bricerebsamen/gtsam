/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file testSerializationInference.cpp
 * @brief 
 * @author Richard Roberts
 * @date Feb 7, 2012
 */

#define GTSAM_MAGIC_KEY

#include <gtsam/inference/IndexConditional.h>
#include <gtsam/inference/SymbolicFactorGraph.h>
#include <gtsam/inference/BayesTree.h>
#include <gtsam/nonlinear/Ordering.h>

#include <gtsam/base/serializationTestHelpers.h>
#include <CppUnitLite/TestHarness.h>

using namespace std;
using namespace gtsam;
using namespace gtsam::serializationTestHelpers;

/* ************************************************************************* */
TEST (Serialization, symbolic_graph) {
  Ordering o; o += "x1","l1","x2";
  // construct expected symbolic graph
  SymbolicFactorGraph sfg;
  sfg.push_factor(o["x1"]);
  sfg.push_factor(o["x1"],o["x2"]);
  sfg.push_factor(o["x1"],o["l1"]);
  sfg.push_factor(o["l1"],o["x2"]);

  EXPECT(equalsObj(sfg));
  EXPECT(equalsXML(sfg));
}

/* ************************************************************************* */
TEST (Serialization, symbolic_bn) {
  Ordering o; o += "x2","l1","x1";

  IndexConditional::shared_ptr x2(new IndexConditional(o["x2"], o["l1"], o["x1"]));
  IndexConditional::shared_ptr l1(new IndexConditional(o["l1"], o["x1"]));
  IndexConditional::shared_ptr x1(new IndexConditional(o["x1"]));

  SymbolicBayesNet sbn;
  sbn.push_back(x2);
  sbn.push_back(l1);
  sbn.push_back(x1);

  EXPECT(equalsObj(sbn));
  EXPECT(equalsXML(sbn));
}

/* ************************************************************************* */
TEST (Serialization, symbolic_bayes_tree ) {
  typedef BayesTree<IndexConditional> SymbolicBayesTree;
  static const Index _X_=0, _T_=1, _S_=2, _E_=3, _L_=4, _B_=5;
  IndexConditional::shared_ptr
  B(new IndexConditional(_B_)),
  L(new IndexConditional(_L_, _B_)),
  E(new IndexConditional(_E_, _L_, _B_)),
  S(new IndexConditional(_S_, _L_, _B_)),
  T(new IndexConditional(_T_, _E_, _L_)),
  X(new IndexConditional(_X_, _E_));

  // Bayes Tree for Asia example
  SymbolicBayesTree bayesTree;
  SymbolicBayesTree::insert(bayesTree, B);
  SymbolicBayesTree::insert(bayesTree, L);
  SymbolicBayesTree::insert(bayesTree, E);
  SymbolicBayesTree::insert(bayesTree, S);
  SymbolicBayesTree::insert(bayesTree, T);
  SymbolicBayesTree::insert(bayesTree, X);

  EXPECT(equalsObj(bayesTree));
  EXPECT(equalsXML(bayesTree));
}

/* ************************************************************************* */
int main() { TestResult tr; return TestRegistry::runAllTests(tr); }
/* ************************************************************************* */
