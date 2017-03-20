//============================================================================
// Name        : juliaset.cpp
// Author      : anematode
// Version     : 1.0.0
// Copyright   : N/A
// Description : Generation of a Julia set through iterative application of a function onto the complex numbers.
//============================================================================

#include <iostream>
#include <fstream>
#include <complex>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

/** Customizable variables **/

const int w = 2048;						// Width, in pixels, of the output image
const int h = 2048;						// Height, in pixels, of the output image

const float re_min = -2;				// Real value of the left edge of the image
const float re_max = 2;					// Real value of the right edge of the image
const float im_min = -2;				// Imaginary value of the bottom edge of the image
const float im_max = 2;					// Imaginary value of the top edge of the image

const int iterations = 255;				// Number of iterations required to assume boundedness under iteration
const int escaperadius = 2;				// Critical radius determining non-boundedness under iteration

// Filename to be saved to (.pgm format). Please modify for the program to work.
string filename = "/Users/anematode/Desktop/juliaset.pgm";

complex<double> c(0.3,0.575);

int main() {
	clock_t begin = clock();								// Begin clock

	const float colordecrement = 250.0/iterations;			// Change in pixel darkness after each iteration
	const int radiussquared = escaperadius * escaperadius;

	ofstream image;											// Image output stream
	image.open(filename,ios_base::app);						// Open output stream

	image << "P2\n# Fractal image\n" <<
			to_string(w) << ' ' << to_string(h)				// Write header data
			<< "\n255\n";

	float xdelta = (re_max-re_min)/w;						// Difference between consecutive pixels in a row
	float ydelta = (im_max-im_min)/h;						// Difference between consecutive pixels in a column

	int row = 0;											// Current row number

	for (double y = im_min; y < im_max; y += ydelta) {		// For all imaginary positions y...

		row += 1;											// Increment row number
		if (row % 100 == 0) {								// Status report every 100 rows
			cout << "Completed row #" << row << ". "
					<< to_string(100*(y-im_min)/(im_max-im_min))
					<< "\% done with image." << endl;
		}

		for (double x = re_min; x < re_max; x += xdelta) {	// For all real positions x...
			complex<double> z(x,y);							// Construct a complex number z = x + yi

			complex<double> k;
			k = z;											// Construct a complex number k = z

			float n = 255.0;								// Stores grayscale value of current pixel (255 = white, 0 = black)

			while (z.real()*z.real() + z.imag()*z.imag() < radiussquared
					&& n >= 5) {							// While complex number is within the escape radius and insufficient iterations have passed...

				/** The following assignment of z is the central iterative function, which continually assigns z = f(z).
				 * Since the function is f(z) = z*z + c, where c is a predetermined complex number, this program
				 * will produce a Julia set.
				 */

				z = z*z+c;

				n -= colordecrement;						// Decrement n by the needed grayscale value
			}
			image << to_string((int) n) << endl;			// Write pixel to file
		}
		image << '\n';										// End this row
	}

	clock_t end = clock();									// End clock
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	cout << "Computed " << w*h << " pixels in " << elapsed_secs << " seconds (CPU time)!" << endl;

	image.close();											// Close the image
}
