#pragma once
#ifndef fvkCameraProcessingThread_h__
#define fvkCameraProcessingThread_h__

/*********************************************************************************
created:	2016/01/10   01:37AM
modified:	2017/02/09   11:48PM
filename: 	fvkCameraProcessingThread.h
file base:	fvkCameraProcessingThread
file ext:	h
author:		Furqan Ullah (Post-doc, Ph.D.)
website:    http://real3d.pk
CopyRight:	All Rights Reserved

purpose:	Class to create a thread for processing of the grabbed camera frame.
This thread is synchronized with the camera thread using a semaphore. 

/**********************************************************************************
*	Fast Visualization Kit (FVK)
*	Copyright (C) 2017 REAL3D
*
* This file and its content is protected by a software license.
* You should have received a copy of this license with this file.
* If not, please contact Dr. Furqan Ullah immediately:
**********************************************************************************/

#include "fvkImageProcessing.h"
#include "fvkSemaphoreBuffer.h"
#include "fvkVideoWriter.h"
#include "fvkThread.h"

namespace R3D
{

class fvkCameraAbstract;

class FVK_EXPORT fvkCameraProcessingThread : public fvkThread
{
public:
	// Description:
	// Default constructor.
	// _buffer is the semaphore to synchronizer the processing thread with this thread.
	// _frameobserver is the parent class of Camera that will override the processFrame function.
	// _device is the id of the opened video capturing device (i.e. a camera index).
	// If there is a single camera connected, just pass 0.
	// In case of multiple cameras, try to pass 1 or 2 or 3, so on...
	// If _width and _height is specified, then this will become the camera frame resolution.
	fvkCameraProcessingThread(fvkSemaphoreBuffer<cv::Mat>* _buffer, fvkCameraAbstract* _frameobserver, int _device_id = 0);
	// Description:
	// Default destructor to stop the thread as well as recorder, and delete the data.
	~fvkCameraProcessingThread();

	// Description:
	// Overridden function to process the camera frame.
	void run() override;

	// Description:
	// Overridden function to get the current grabbed frame.
	cv::Mat getFrame();

	// Description:
	// Function that trigger the saveFrameToDisk function that saves the current image frame
	// to the specified output location setFrameOutputLocation("D:\\frame.jpg").
	// Note: The folder should have writable permission, such as Pictures/Videos/Documents folder.
	// Example:
	// p->setFrameOutputLocation("D:\\frame.jpg");
	// p->saveFrameOnClick();
	void saveFrameOnClick();

	// Description:
	// Function to set the saved frame file path.
	// The path folder should have writable permission, such as Pictures/Videos/Documents folder.
	// Example:
	// p->setFrameOutputLocation("D:\\frame.jpg");
	void setFrameOutputLocation(const std::string& _path_with_filename) { m_filelocation = _path_with_filename; }
	// Description:
	// Function to get the saved frame file path.
	auto getFrameOutputLocation() const { return m_filelocation; }

	// Description:
	// Function to set the camera device id.
	//  _device is the id of the opened video capturing device (i.e. a camera index).
	//  If there is a single camera connected, just pass 0.
	//  In case of multiple cameras, try to pass 1 or 2 or 3, so on...
	void setDeviceIndex(int _index) { m_device_index = _index; }
	// Description:
	// Function to get the camera device id.
	auto getDeviceIndex() const { return m_device_index; }

	// Description:
	// Set emit function to display the captured frame.
	// The display function should be capable of handling multi-threading updating.
	void setFrameViewerSlot(const std::function<void(cv::Mat&)>& _f) { emit_display_frame = _f; }

	// Description:
	// Set emit function to get the average frames per second of this thread
	// as well as the total number of frames that has been processed/passed.
	// The input function should be capable of handling multi-threading updating.
	void setFrameStatisticsSlot(const std::function<void(const fvkAverageFpsStats&)>& _f) { emit_stats = _f; }

	// Description:
	// Function to get a reference to video writer.
	auto& writer() { return m_vr; }

	// Description:
	// Function to get a reference to image processing.
	auto& imageProcessing() { return m_ip; }

	// Description:
	// Function to set a pointer to semaphore buffer which does synchronization between capturing and processing threads.
	void setSemaphoreBuffer(fvkSemaphoreBuffer<cv::Mat>* _p) { p_buffer = _p; }
	// Description:
	// Function to get a pointer to semaphore buffer which does synchronization between capturing and processing threads.
	auto getSemaphoreBuffer() const { return p_buffer; }

protected:
	// Description:
	// Function that saves the current frame to disk (file path must be specified by setSavedFile("")).
	auto saveFrameToDisk(const cv::Mat& _frame) -> bool;

	std::mutex m_savemutex;
	std::mutex m_processing_mutex;
	fvkSemaphoreBuffer<cv::Mat> *p_buffer;
	fvkCameraAbstract* p_frameobserver;
	std::function<void(cv::Mat&)> emit_display_frame;

	std::function<void(const fvkAverageFpsStats&)> emit_stats;

	fvkImageProcessing m_ip;
	fvkVideoWriter m_vr;

	int m_device_index;
	cv::Mat m_frame;
	cv::Rect m_rect;
	std::string m_filelocation;
	int m_savedframes;
	bool m_iscapture;
};

}

#endif // fvkCameraProcessingThread_h__