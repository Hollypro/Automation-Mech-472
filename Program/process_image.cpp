#include <iostream>
#include <cmath>
#include <conio.h>
#include "timer.h"
#include "global_variables.h"
#include "image_transfer3.h"
#include "vision.h";

using namespace std;

// input rgb image B-G-R-A
extern image rgb_in, rgb_copy;

// sample time of current incoming image
extern double t_sample;

int process_image()
// this function is called continuously from the call back function
{
//	find_centroid(grey)
//	find_edge(rgb)
	
	return 0;
}

int rgb_detection(image rgb_in, image &rgb_out)
// detects RGB and grey everything else out
{
	//input error checks
	if (rgb_in.width != rgb_out.width || rgb_in.height != rgb_out.height)
	{
		cout << "ERROR: rgb_detection--image sizes incompatible\n";
		return 1;
	}
	if (rgb_in.type != RGB_IMAGE || rgb_out.type != RGB_IMAGE)
	{
		cout << "ERROR: rgb_detection--image input type invalid\n";
		return 1;
	}

	ibyte *p,*p2;
	ibyte R, G, B;
	int k;

	// process image
	p = rgb_in.pdata; // get image pointer
	p2 = rgb_out.pdata;
	for (k = 0; k < 640 * 480; k++)  // access each pixel k
	{
		//  B G R
		B = *p;
		G = *(p + 1);
		R = *(p + 2);
		
		if ((B > 100) && (R < 100) && (G < 100)) 
		{
			R = 0;
			G = 0;
			B = 255;
		}
		else if ((B < 100) && (R > 100) && (G < 100)) 
		{
			R = 255;
			G = 0;
			B = 0;
		}
		else if ((B < 100) && (R < 100) && (G > 100)) 
		{
			R = 0;
			G = 255;
			B = 0;
		}
		else 
		{
			R = 0.587 * G;
			G *= 0.587;
			B = G;
		}

		//input into rgb_out (leave rgb_in)
		*p2 = B;
		*(p2 + 1) = G;
		*(p2 + 2) = R;

		// go to next pixel
		p += 3;
		p2 += 3;
	}

	return 0;
}

int sobel(image grey_in, image &mag, image &theta)
// grey input used if want theta
{
	//error checks
	if (grey_in.height != mag.height || grey_in.height != theta.height || grey_in.width != mag.width || grey_in.width != theta.width)
	{
		cout << "ERROR: sobel--image size incompatible\n";
		return 1;
	}
	if (grey_in.type != GREY_IMAGE || grey_in.type != mag.type || grey_in.type != theta.type)
	{
		cout << "ERROR: sobel--image input type invalid\n";
		return 1;
	}

	ibyte *p, *p_mag, *p_theta, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;	// p7 p8 p9
	i2byte width, height;														// p4 p5 p6
	i4byte size, i, j;															// p1 p2 p3
	int sx, sy, M;
	double ang;
	const double pi = 3.14159;
	image temp;

	width = grey_in.width;
	height = grey_in.height;

	temp.height = height;
	temp.width = width;
	temp.type = GREY_IMAGE;
	allocate_image(temp);
	copy(grey_in, temp);

	p = temp.pdata + width + 1;
	p_mag = mag.pdata + width + 1;
	p_theta = theta.pdata + width + 1;

	p1 = p - width - 1;
	p2 = p - width;
	p3 = p - width + 1;
	p4 = p - 1;
	p5 = p;
	p6 = p + 1;
	p7 = p + width - 1;
	p8 = p + width;
	p9 = p + width + 1;

	size = (i4byte)width*(height - 2) - 2;

	for (int i = 0; i < size; i++)
	{
		sx = -1 * (*p7) + 0 * (*p8) + 1 * (*p9)
			- 2 * (*p4) + 0 * (*p5) + 2 * (*p6)
			- 1 * (*p1) + 0 * (*p2) + 1 * (*p3);

		sy = 1 * (*p7) + 2 * (*p8) + 1 * (*p9)
			+ 0 * (*p4) + 0 * (*p5) + 0 * (*p6)
			- 1 * (*p1) - 2 * (*p2) - 1 * (*p3);

		M = abs(sx) + abs(sy); //easy approximation of sqrt(sx^2 +sy^2)
		if (M > 255) M = 255; //check for overflow

		ang = atan2((double)sy, (double)sx); //rad -pi to pi
		ang = ang * 180.0 / pi; //deg 180 to 180
		ang = ang + 180.0; //deg 0 to 360
		ang = ang * 255.0 / 360.0 + 0.01; // pixel 0 - 255;

		if (M < 75) ang = 0;

		*p_mag = M;
		*p_theta = (int)ang;

		p1++; p2++; p3++; p4++; p5++; p6++; p7++; p8++;
		p9++; p++; p_theta++; p_mag++;
	}

	p_mag = mag.pdata;
	p_theta = theta.pdata;
	size = (i4byte)height*width - 1;

	for (int i = 1; i < width; i++)
	{
		p_mag[i] = p_mag[i + width]; //bottom
		p_mag[size - i] = p_mag[size - width - i]; //top
		p_theta[i] = p_theta[i + width];
		p_theta[size - i] = p_theta[size - width - i];
	}

	for (int i = 0; i < height; i++)
	{
		p_mag[i*width] = p_mag[i*width + 1]; //left
		p_mag[i*width + width - 1] = p_mag[i*width + width - 2]; //right
		p_theta[i*width] = p_theta[i*width + 1];
		p_theta[i*width + width - 1] = p_theta[i*width + width - 2];
	}

	free_image(temp);
	return 0;
}

int sobel(image grey_in, image &mag)
// grey input, used if ONLY want mag
{
	//error checks
	if (grey_in.height != mag.height || grey_in.width != mag.width)
	{
		cout << "ERROR: sobel--image size incompatible\n";
		return 1;
	}
	if (grey_in.type != GREY_IMAGE || grey_in.type != mag.type)
	{
		cout << "ERROR: sobel--image input type invalid\n";
		return 1;
	}

	ibyte *p, *p_mag, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;	// p7 p8 p9
	i2byte width, height;											// p4 p5 p6
	i4byte size, i, j;												// p1 p2 p3
	int sx, sy, M;
	const double pi = 3.14159;
	image temp;

	width = grey_in.width;
	height = grey_in.height;

	temp.height = height;
	temp.width = width;
	temp.type = GREY_IMAGE;
	allocate_image(temp);
	copy(grey_in, temp);

	p = temp.pdata + width + 1;
	p_mag = mag.pdata + width + 1;

	p1 = p - width - 1;
	p2 = p - width;
	p3 = p - width + 1;
	p4 = p - 1;
	p5 = p;
	p6 = p + 1;
	p7 = p + width - 1;
	p8 = p + width;
	p9 = p + width + 1;

	size = (i4byte)width*(height - 2) - 2;

	for (int i = 0; i < size; i++)
	{
		sx = -1 * (*p7) + 0 * (*p8) + 1 * (*p9)
			- 2 * (*p4) + 0 * (*p5) + 2 * (*p6)
			- 1 * (*p1) + 0 * (*p2) + 1 * (*p3);

		sy = 1 * (*p7) + 2 * (*p8) + 1 * (*p9)
			+ 0 * (*p4) + 0 * (*p5) + 0 * (*p6)
			- 1 * (*p1) - 2 * (*p2) - 1 * (*p3);

		M = abs(sx) + abs(sy); //easy approximation of sqrt(sx^2 +sy^2)
		if (M > 255) M = 255; //check for overflow

		*p_mag = M;

		p1++; p2++; p3++; p4++; p5++; p6++; p7++; p8++;
		p9++; p++; p_mag++;
	}

	p_mag = mag.pdata;
	size = (i4byte)height*width - 1;

	for (int i = 1; i < width; i++)
	{
		p_mag[i] = p_mag[i + width]; //bottom
		p_mag[size - i] = p_mag[size - width - i]; //top
	}

	for (int i = 0; i < height; i++)
	{
		p_mag[i*width] = p_mag[i*width + 1]; //left
		p_mag[i*width + width - 1] = p_mag[i*width + width - 2]; //right
	}


	free_image(temp);
	return 0;
}

int sobel(image &image_in)
// rgb or grey in, changes to greyscale mag sobel image
{
	//error checks
	if (image_in.type == LABEL_IMAGE)
	{
		cout << "ERROR: sobel--image input type invalid/n";
		return 1;
	}

	ibyte *p, *p_mag, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;	// p7 p8 p9
	i2byte width, height;											// p4 p5 p6
	i4byte size, i, j;												// p1 p2 p3
	int sx, sy, M;
	const double pi = 3.14159;
	image temp,mag;

	width = image_in.width;
	height = image_in.height;

	temp.height = height;
	temp.width = width;
	temp.type = GREY_IMAGE;
	allocate_image(temp);
	copy(image_in, temp); //ensures sobel done in greyscale
	scale(temp, temp); //ensures 0-255 (important for sobel quality)

	mag.height = height;
	mag.width = width;
	mag.type = GREY_IMAGE;
	allocate_image(mag);
	copy(temp, mag);

	p = temp.pdata + width + 1;
	p_mag = mag.pdata + width + 1;

	p1 = p - width - 1;
	p2 = p - width;
	p3 = p - width + 1;
	p4 = p - 1;
	p5 = p;
	p6 = p + 1;
	p7 = p + width - 1;
	p8 = p + width;
	p9 = p + width + 1;

	size = (i4byte)width*(height - 2) - 2;

	for (int i = 0; i < size; i++)
	{
		sx = -1 * (*p7) + 0 * (*p8) + 1 * (*p9)
			- 2 * (*p4) + 0 * (*p5) + 2 * (*p6)
			- 1 * (*p1) + 0 * (*p2) + 1 * (*p3);

		sy = 1 * (*p7) + 2 * (*p8) + 1 * (*p9)
			+ 0 * (*p4) + 0 * (*p5) + 0 * (*p6)
			- 1 * (*p1) - 2 * (*p2) - 1 * (*p3);

		M = abs(sx) + abs(sy); //easy approximation of sqrt(sx^2 +sy^2)
		if (M > 255) M = 255; //check for overflow

		*p_mag = M;

		p1++; p2++; p3++; p4++; p5++; p6++; p7++; p8++;
		p9++; p++; p_mag++;
	}

	p_mag = mag.pdata;
	size = (i4byte)height*width - 1;

	for (int i = 1; i < width; i++)
	{
		p_mag[i] = p_mag[i + width]; //bottom
		p_mag[size - i] = p_mag[size - width - i]; //top
	}

	for (int i = 0; i < height; i++)
	{
		p_mag[i*width] = p_mag[i*width + 1]; //left
		p_mag[i*width + width - 1] = p_mag[i*width + width - 2]; //right
	}

	copy(mag, image_in); //returns back to original format

	free_image(temp);
	free_image(mag);
	
	return 0;
}

int find_centroid(camera &cam)
//takes grey image, filters and finds centroids
//puts centroid positions into array
//need to calibrate filters and threshold value depending on lighting
{
	// error checks
	if (cam.grey.type != GREY_IMAGE)
	{
		cout << "ERROR:find_centroid--invalid image type\n";
		return 1;
	}
	int nlabels = 0;
	int thresh = 147;
	int i = 0;

	image grey;
	grey.height = height;
	grey.width = width;
	grey.type = GREY_IMAGE;
	allocate_image(grey);

	lowpass_filter(cam.grey, cam.grey);
	lowpass_filter(cam.grey, cam.grey);
	scale(cam.grey, cam.grey);
	/*		while (ch != 't') //useful for calibrating threshold value (thresh)
	{
	if (ch == 'p') thresh += 10;
	if (ch == 'l') thresh -= 10;
	cout << "thresh = " << thresh << "\n";
	threshold(cam.grey, grey, thresh);
	copy(grey, rgb_nozzle);
	view_rgb_image(rgb_nozzle);
	ch = getch();
	}
	copy(grey, cam.grey);
	*/
	threshold(cam.grey, cam.grey, thresh);
	for (i = 0; i < 2; i++)	//calibrate the 2
	{
		copy(cam.grey, grey);
		dialate(grey, cam.grey);
	}
	for (i = 0; i < 2; i++) //calibrate the 2
	{
		copy(cam.grey, grey);
		erode(grey, cam.grey);
	}
	invert(cam.grey, cam.grey);
	label_image(cam.grey, cam.label, cam.nlabels);

	if (cam.nlabels > 900)
	{
		cout << "ERROR:find_centroid--nlabels too large\n";
		return 1;
	}

	for (i = 1; i <= cam.nlabels; i++)
	{
		centroid(cam.grey, cam.label, i, cam.ic[i], cam.jc[i]);
//		cout << ic[i] << "\t" << jc[i] << "\t" << i << "\n"; //for debugging
		draw_point(cam.grey, cam.ic[i], cam.jc[i], 125);
	}

	copy(cam.grey, cam.rgb); //need rgb for view

	free_image(grey);
	return 0;
}

int find_edge(camera &cam)
//takes rgb image and runs through sobel and then creates binary image
{
	//erro checks
	if (rgb_in.type != RGB_IMAGE)
	{
		cout << "ERROR:find_edge--invalid image type\n";
			return 1;
	}

	sobel(cam.rgb); //edge detection
	copy(cam.rgb, cam.grey); //need grey for threshold
	threshold(cam.grey, cam.grey, 250); //removes noise
	copy(cam.grey, cam.rgb); //need rgb for view

	return 0;
}

int get_image(camera &cam, char ch[])
//gets camera image from either specified file or live feed
{
	if (ch == "live") acquire_image(cam.rgb, cam.num); //default ch
	else load_rgb_image(ch, cam.rgb);

	copy(cam.rgb, cam.grey);

	return 0;
}

int trace_object(camera cam,int obj)
{
	//error checks
	if (obj<1 || obj>cam.nlabels)
	{
		cout << "ERROR:trace_object--obj not defined\n";
		return 1;
	}

	ibyte *plab, *pgrey;
	i4byte size;
	double ei, ej;
	int edge;

	copy(cam.rgb, cam.grey);

	size = height*width;

	plab = cam.label.pdata;
	pgrey = cam.grey.pdata;

	for (int i = 0; i < size; i++)
	{
		if (*plab == obj && *pgrey == 255) //if object looking for and if white (edge)
		{
			ei = i%cam.grey.width;
			ej = i - ei*cam.grey.width;
			//TODO: send ei,ej g-code to printer
		}
		plab++; pgrey++;
	}

	return 0;
}

