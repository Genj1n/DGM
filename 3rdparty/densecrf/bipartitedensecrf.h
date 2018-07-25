#pragma once

#include "densecrf.h"

// A dense CRF in a bipartite graph
class BipartiteDenseCRF {
protected:
	// Two dense CRF's that are connected by a set of completely connected edges (in a bipartite graph)
	DenseCRF * dense_crfs_[2];

	// Number of variables and labels
	int N_[2], M_;

	// All bipartite pairwise potentials (all others are stored in each dense_crfs respectively)
	std::vector<PairwisePotential*> pairwise_[2];

	// Don't copy this object, bad stuff will happen
	BipartiteDenseCRF(BipartiteDenseCRF & o) {}

	// Run inference and return the pointer to the result
	void runInference(int n_iterations, float ** prob, float relax);


public:
	// Create a dense CRF model of size N with M labels
	BipartiteDenseCRF(int N1, int N2, int M);
	~BipartiteDenseCRF();

	// Add  a pairwise potential defined over some feature space
	// The potential will have the form:    w*exp(-0.5*|f_i - f_j|^2)
	// The kernel shape should be captured by transforming the
	// features before passing them into this function
	void addPairwiseEnergy(const float * features1, const float * features2, int D, float w = 1.0f, const SemiMetricFunction * function = NULL);

	// Add your own favorite pairwise potential (ownwership will be transfered to this class)
	void addPairwiseEnergy(PairwisePotential* potential12, PairwisePotential* potential21);

	// Run inference and return the probabilities
	void inference(int n_iterations, float* result1, float * result2, float relax = 1);

	// Run MAP inference and return the map for each pixel
	void map(int n_iterations, short int* result1, short int* result2, float relax = 1);

	// Access the two CRF's directly
	DenseCRF& getCRF(int i);
	const DenseCRF& getCRF(int i) const;

	// Step by step inference
	void startInference();
	void stepInference(float relax = 1.0);
	void currentMap(short * result);
};