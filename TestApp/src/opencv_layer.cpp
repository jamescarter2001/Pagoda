#include "opencv_layer.h"

#include "imgui.h"

OpenCVLayer::OpenCVLayer() : Layer("OpenCVLayer") {

}

OpenCVLayer::~OpenCVLayer() {
}

void OpenCVLayer::OnAttach(Pagoda::Universe::ApplicationContext& ctx) {

	m_cap = std::make_unique<cv::VideoCapture>(0);

	// Check if the camera opened successfully
        if (!m_cap->isOpened()) {
            std::cerr << "Error: Could not open camera." << std::endl;
        }

        cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);
}

void OpenCVLayer::OnEvent(Pagoda::Universe::ApplicationContext& ctx, Pagoda::Base::Event& e) const {

}

void OpenCVLayer::OnUpdate(Pagoda::Universe::ApplicationContext& ctx) {
    ImGuiIO& io = ImGui::GetIO();

    PG_ASSERT(ImGui::GetCurrentContext() != NULL, "ImGui context not loaded!");

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");  // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");           // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f

        if (ImGui::Button("Button"))  // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Checkbox("Grayscale", &m_grayscale);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    cv::Mat frame, grayFrame;

    // Capture frame-by-frame
    *m_cap >> frame;

    // Check if the frame is empty
    if (frame.empty()) {
        std::cerr << "Error: Blank frame grabbed" << std::endl;
    }

    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

    // Display the frame
    cv::imshow("Webcam", m_grayscale ? grayFrame : frame);

}