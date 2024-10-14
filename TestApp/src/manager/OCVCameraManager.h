#pragma once
#include "pgpch.h"

#include <Pagoda.h>
#include <opencv2/opencv.hpp>

class OCVCameraManager : public Pagoda::Base::AbstractManager {
public:
    OCVCameraManager();
    ~OCVCameraManager();

    virtual void Init() override;

    cv::Mat GetFrame();

    virtual void BeforeUpdate() override;
    virtual void OnUpdate() override;

    virtual void ShutDown() override;

private:
    void UpdateCameraFrame();

    Pagoda::Base::ReentrantLock m_lock;
    std::thread m_tframeUpdate;

    cv::Mat m_frame;

    std::unique_ptr<cv::VideoCapture> m_cap;
};
