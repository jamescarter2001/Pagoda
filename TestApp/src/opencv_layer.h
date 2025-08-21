#pragma once

#include <Pagoda.h>

#include <opencv2/opencv.hpp>

#include "manager/OCVCameraManager.h"

class OpenCVLayer : public Pagoda::Universe::Layer {
    public:
	    OpenCVLayer();
        virtual ~OpenCVLayer();

        virtual void OnEvent(Pagoda::Universe::ApplicationContext& ctx, Pagoda::Base::Event& e) const override;
        virtual void OnUpdate(Pagoda::Universe::ApplicationContext& ctx) override;
        virtual void OnAttach(Pagoda::Universe::ApplicationContext& ctx) override;

    private:
        std::unique_ptr<OCVCameraManager> m_cameraManager;

        bool m_grayscale = false;
};
