#pragma once

#include <Pagoda.h>

#include <opencv2/opencv.hpp>

class OpenCVLayer : public Pagoda::Universe::Layer {
    public:
	    OpenCVLayer();
        virtual ~OpenCVLayer();

        virtual void OnEvent(Pagoda::Universe::ApplicationContext& ctx, Pagoda::Base::Event& e) const override;
        virtual void OnUpdate(Pagoda::Universe::ApplicationContext& ctx) override;
        virtual void OnAttach(Pagoda::Universe::ApplicationContext& ctx) override;

    private:
        Pagoda::Base::ReentrantLock m_lock;
        std::thread m_tframeUpdate;

        cv::Mat m_frame;

        void UpdateCameraFrame();
        std::unique_ptr<cv::VideoCapture> m_cap;

        bool m_grayscale = false;
};
