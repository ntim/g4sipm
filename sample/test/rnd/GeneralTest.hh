/*
 * RandGeneralTest.hh
 *
 * @date Sep 9, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef GENERALTEST_HH
#define GENERALTEST_HH

#include <gtest/gtest.h>

#include <map>
#include <cmath>

#include "rnd/General.hh"

class GeneralTest: public testing::Test {
protected:
	std::vector<double> pdf, x;

	virtual void SetUp() {
		pdf.push_back(0);
		pdf.push_back(1);
		pdf.push_back(2);
		pdf.push_back(1);
		pdf.push_back(0);
		x.push_back(0);
		x.push_back(1);
		x.push_back(2);
		x.push_back(3);
		x.push_back(4);
	}
};

TEST_F(GeneralTest, shouldCreate) {
	rnd::General rand(x, pdf);
	rand.print();
}

TEST_F(GeneralTest, create) {
	rnd::General rand("resources/led-waveform-12ns.txt");
	rand.print();
}

TEST_F(GeneralTest, shoot) {
	std::map<double, unsigned int> h;
	rnd::General rand(x, pdf);
	for (unsigned int i = 0; i < 100000; i++) {
		double v = rand.shoot();
		ASSERT_GE(v, x.front());
		ASSERT_LT(v, x.back());
		unsigned int bin = floor(v);
		if (!h.count(bin)) {
			h[bin] = 0.0;
		}
		h[bin] += 1;
	}
	for (std::map<double, unsigned int>::iterator it = h.begin(); it != h.end(); it++) {
		std::cout << "GeneralTest::shoot(): (x, #) = (" << it->first << ", " << it->second << ")" << std::endl;
	}
	ASSERT_NEAR(h[1] + h[3], h[2], 0.03 * h[2]);
}

TEST_F(GeneralTest, createAndShoot) {
	rnd::General rand("resources/led-waveform-12ns.txt");
	rand.print();
	std::map<double, unsigned int> h;
	for (unsigned int i = 0; i < 100000; i++) {
		double v = rand.shoot();
		double bin = floor(v);
		if (!h.count(bin)) {
			h[bin] = 0.0;
		}
		h[bin] += 1;
	}
	for (std::map<double, unsigned int>::iterator it = h.begin(); it != h.end(); it++) {
		std::cout << "GeneralTest::shoot(): (x, #) = (" << it->first << ", " << it->second << ")" << std::endl;
	}
}

#endif /* GENERALTEST_HH */
