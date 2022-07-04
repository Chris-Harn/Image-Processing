#include "Application.h"

Application::Application() : AppRunning(true) {

}

bool Application::Initialization( unsigned window_width, unsigned int window_height, const char *title ) {
    return true;
}

void Application::ProcessInput() {

}

void Application::Update() {
    AppRunning = false;
}

void Application::Render() {

}

void Application::CleanUp() {

}

bool Application::AppStillOpen() {
    return AppRunning;
}