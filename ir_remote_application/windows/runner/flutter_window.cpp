#include "flutter_window.h"

#include <optional>

#include "flutter/generated_plugin_registrant.h"

#include <flutter/binary_messenger.h>
#include <flutter/standard_method_codec.h>
#include <flutter/method_channel.h>
#include <flutter/method_result_functions.h>
#include <string>

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

FlutterWindow::FlutterWindow(const flutter::DartProject &project)
    : project_(project) {}

FlutterWindow::~FlutterWindow() {}

void callKeyboardKey(int key)
{
  keybd_event((BYTE)key, 0, 1, 0);
}
float GetSystemVolume()
{
  HRESULT hr;

  // -------------------------
  CoInitialize(NULL);
  IMMDeviceEnumerator *deviceEnumerator = NULL;
  hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
  IMMDevice *defaultDevice = NULL;

  hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
  deviceEnumerator->Release();
  deviceEnumerator = NULL;

  IAudioEndpointVolume *endpointVolume = NULL;
  hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
  defaultDevice->Release();
  defaultDevice = NULL;

  float currentVolume = 0;

  // Current volume as a scalar
  hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);

  endpointVolume->Release();
  CoUninitialize();

  return currentVolume;
}
void SetSystemVolume(double newVolume)
{
  HRESULT hr;

  // -------------------------
  CoInitialize(NULL);
  IMMDeviceEnumerator *deviceEnumerator = NULL;
  hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
  IMMDevice *defaultDevice = NULL;

  hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
  deviceEnumerator->Release();
  deviceEnumerator = NULL;

  IAudioEndpointVolume *endpointVolume = NULL;
  hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
  defaultDevice->Release();
  defaultDevice = NULL;

  hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);

  endpointVolume->Release();

  CoUninitialize();
}

void initMethodChannel(flutter::FlutterEngine *flutter_instance)
{
  // name your channel
  const static std::string channel_name("native_channel");

  auto channel =
      std::make_unique<flutter::MethodChannel<>>(
          flutter_instance->messenger(), channel_name,
          &flutter::StandardMethodCodec::GetInstance());

  channel->SetMethodCallHandler(
      [](const flutter::MethodCall<> &call,
         std::unique_ptr<flutter::MethodResult<>> result)
      {
        // cheack method name called from dart

        if (call.method_name()._Starts_with("virtualKeyboard"))
        {
          std::string keyName = call.method_name().substr(16);

          callKeyboardKey(stoi(keyName));
          result->Success(keyName);
        }
        else if (call.method_name()._Starts_with("terminal"))
        {
          std::string cmd = call.method_name().substr(9);

          result->Success(std::system(cmd.c_str()));
        }
        else if (call.method_name()._Starts_with("volume"))
        {
          std::string volume = call.method_name().substr(7);
          // waveOutSetVolume(NULL, (int)(atoi(volume.c_str()) * 655.35));
          SetSystemVolume(((float)atoi(volume.c_str())) / 100);
          result->Success("volume set");
        }
        else
        {
          result->NotImplemented();
        }
      });
}

bool FlutterWindow::OnCreate()
{
  if (!Win32Window::OnCreate())
  {
    return false;
  }

  RECT frame = GetClientArea();

  // The size here must match the window dimensions to avoid unnecessary surface
  // creation / destruction in the startup path.
  flutter_controller_ = std::make_unique<flutter::FlutterViewController>(
      frame.right - frame.left, frame.bottom - frame.top, project_);
  // Ensure that basic setup of the controller was successful.
  if (!flutter_controller_->engine() || !flutter_controller_->view())
  {
    return false;
  }
  RegisterPlugins(flutter_controller_->engine());
  SetChildContent(flutter_controller_->view()->GetNativeWindow());

  // initialize method channel here
  initMethodChannel(flutter_controller_->engine());

  // other codes
  return true;
}

void FlutterWindow::OnDestroy()
{
  if (flutter_controller_)
  {
    flutter_controller_ = nullptr;
  }

  Win32Window::OnDestroy();
}

LRESULT
FlutterWindow::MessageHandler(HWND hwnd, UINT const message,
                              WPARAM const wparam,
                              LPARAM const lparam) noexcept
{
  // Give Flutter, including plugins, an opportunity to handle window messages.
  if (flutter_controller_)
  {
    std::optional<LRESULT> result =
        flutter_controller_->HandleTopLevelWindowProc(hwnd, message, wparam,
                                                      lparam);
    if (result)
    {
      return *result;
    }
  }

  switch (message)
  {
  case WM_FONTCHANGE:
    flutter_controller_->engine()->ReloadSystemFonts();
    break;
  }

  return Win32Window::MessageHandler(hwnd, message, wparam, lparam);
}
