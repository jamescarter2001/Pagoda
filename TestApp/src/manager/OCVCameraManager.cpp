#include "OCVCameraManager.h"

OCVCameraManager::OCVCameraManager() {

}

OCVCameraManager::~OCVCameraManager() {

}

void OCVCameraManager::Init() {
    m_cap = std::make_unique<cv::VideoCapture>(0);

    // Check if the camera opened successfully
    if (!m_cap->isOpened()) {
        PG_ERROR("Could not open camera!");
    }

    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

    m_tframeUpdate = std::thread([this]() { while (m_cap->isOpened()) UpdateCameraFrame(); });
}

cv::Mat OCVCameraManager::GetFrame() {
    Pagoda::Base::ScopedLock l(m_lock);
    return m_frame;
}

void OCVCameraManager::UpdateCameraFrame() {
    cv::Mat f;
    *m_cap >> f;

    {
        Pagoda::Base::ScopedLock l(m_lock);
        m_frame = f;
    }
}

void OCVCameraManager::BeforeUpdate() {

}

void OCVCameraManager::OnUpdate() {

}

void OCVCameraManager::ShutDown() {

}