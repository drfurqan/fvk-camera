/*********************************************************************************
created:	2017/07/27   05:39AM
filename: 	camera_program_1.cpp
file base:	camera_program_1
file ext:	cpp
author:		Furqan Ullah (Post-doc, Ph.D.)
website:    http://real3d.pk
CopyRight:	All Rights Reserved

purpose:	Simplest program to run a camera device. It uses OpenCV window to show
the grabbed image frames.
First method to set a function to show the grabbed frames.

/**********************************************************************************
*	Fast Visualization Kit (FVK)
*	Copyright (C) 2017 REAL3D
*
* This file and its content is protected by a software license.
* You should have received a copy of this license with this file.
* If not, please contact Dr. Furqan Ullah immediately:
**********************************************************************************/

#include <fvk/camera/fvkCamera.h>
#include <fvk/camera/fvkImageProcessing.h>

using namespace R3D;

int main()
{
	// create an object for the camera device
	// first argument is the id of the opened video capturing device (i.e. a camera index).
	// If there is a single camera connected, just pass 0.
	// In case of multiple cameras, try to pass 1 or 2 or 3, so on...
	// second argument is the desired camera frame width and height.
	// resolution Size(-1, -1) will do the auto-selection for the frame's width and height.
	fvkCamera cam(0, cv::Size(640, 480));

	// try to connect the camera device and initialize it.
	if (cam.connect())
	{
		// start the camera threads (frame grabbing + processing thread).
		if (cam.start())
		{
			std::cout << "camera has started!" << std::endl;
		}
	}

	// create a window using OpenCV.
	cv::namedWindow("FVK Camera");

	// create a function that will be calling in the continuous running camera thread.
	// _frame is the grabbed frame.
	std::function<void(cv::Mat&)> f = [&](cv::Mat& _frame)
	{
		auto m = _frame.clone();			// always create a clone to process the frame.
		cv::cvtColor(m, m, CV_BGR2GRAY);	// just doing the simple image processing that converts to gray-scale image.
		cv::imshow("FVK Camera", m);		// show the grabbed _frame.
		_frame = m;
	};

	// set the above create function in the camera.
	cam.setFrameViewerSlot(f);

	// fvkCamera class also gives some basic image processing filters.
	// Bilateral filter denoising the grabbed image with the kernel size 9.
	cam.imageProcessing().setDenoisingMethod(fvkImageProcessing::DenoisingMethod::Bilateral);
	cam.imageProcessing().setDenoisingLevel(9);

	// OpenCV event loop that will prevent to exit the main loop.
	cv::waitKey(0);

	return 1;
}