#pragma once

#include "edgepotential.h"
#include "permutohedral.h"


// ********************** CPottsPotential **********************
class CEdgePotentialPotts : public CEdgePotential
{
public:
	CEdgePotentialPotts(const Mat &features, float w = 1.0f, const SemiMetricFunction *pFunction = NULL, bool per_pixel_normalization = true);
	virtual ~CEdgePotentialPotts(void) {}

	void apply(const Mat &src, Mat &dst, Mat &temp = Mat()) const;


	// TODO: try private
	// TODO: try without members
protected:
	float                           m_w;
	std::unique_ptr<CPermutohedral> m_pLattice;
	Mat                             m_norm;
    const SemiMetricFunction      * m_pFunction;
};

