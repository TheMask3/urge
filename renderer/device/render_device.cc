// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "renderer/device/render_device.h"

#include "base/debug/logging.h"

namespace renderer {

namespace {

void DeviceLostErrorHandler(const wgpu::Device&,
                            wgpu::DeviceLostReason reason,
                            wgpu::StringView message) {
  std::string reason_name;
  switch (reason) {
    default:
    case wgpu::DeviceLostReason::Unknown:
      reason_name = "Unknown";
      break;
    case wgpu::DeviceLostReason::Destroyed:
      reason_name = "Destroyed";
      break;
    case wgpu::DeviceLostReason::InstanceDropped:
      reason_name = "InstanceDropped";
      break;
    case wgpu::DeviceLostReason::FailedCreation:
      reason_name = "FailedCreation";
      break;
  }

  LOG(INFO) << "[Renderer] Device lost - Error: " << reason_name << ": "
            << std::string_view(message);
}

void UncaptureErrorHandler(const wgpu::Device&,
                           wgpu::ErrorType type,
                           wgpu::StringView message) {
  const char* errorTypeName = "";
  switch (type) {
    case wgpu::ErrorType::Validation:
      errorTypeName = "Validation";
      break;
    case wgpu::ErrorType::OutOfMemory:
      errorTypeName = "Out of memory";
      break;
    case wgpu::ErrorType::Unknown:
      errorTypeName = "Unknown";
      break;
    case wgpu::ErrorType::DeviceLost:
      errorTypeName = "Device lost";
      break;
    default:
      break;
  }

  LOG(INFO) << "[Renderer] " << errorTypeName
            << " error: " << std::string_view(message);
}

}  // namespace

std::unique_ptr<RenderDevice> RenderDevice::Create(
    base::WeakPtr<ui::Widget> window_target,
    wgpu::BackendType required_backend) {
  wgpu::InstanceDescriptor instance_desc;
  instance_desc.features.timedWaitAnyEnable = true;
  wgpu::Instance instance = wgpu::CreateInstance(&instance_desc);
  if (!instance)
    return nullptr;

  wgpu::RequestAdapterOptions options;
  options.compatibilityMode = true;

  wgpu::Adapter adapter;
  instance.WaitAny(
      instance.RequestAdapter(
          &options, wgpu::CallbackMode::WaitAnyOnly,
          [&](wgpu::RequestAdapterStatus status, wgpu::Adapter result_adapter,
              wgpu::StringView message) {
            if (status != wgpu::RequestAdapterStatus::Success) {
              LOG(INFO) << "[Renderer] Failed to get an adapter: "
                        << std::string_view(message);
              return;
            }

            adapter = std::move(result_adapter);
          }),
      UINT64_MAX);

  if (!adapter)
    return nullptr;

  wgpu::AdapterInfo info;
  adapter.GetInfo(&info);
  LOG(INFO) << "[Renderer] Renderer Device: " << std::string_view(info.device);
  LOG(INFO) << "[Renderer] Description: " << std::string_view(info.description);

  wgpu::DeviceDescriptor device_desc = {};
  device_desc.SetUncapturedErrorCallback(UncaptureErrorHandler);
  device_desc.SetDeviceLostCallback(wgpu::CallbackMode::AllowSpontaneous,
                                    DeviceLostErrorHandler);

  wgpu::Device device;
  instance.WaitAny(
      adapter.RequestDevice(
          &device_desc, wgpu::CallbackMode::WaitAnyOnly,
          [&](wgpu::RequestDeviceStatus status, wgpu::Device result_device,
              wgpu::StringView message) {
            if (status != wgpu::RequestDeviceStatus::Success) {
              LOG(INFO) << "[Renderer] Failed to get an device: "
                        << std::string_view(message);
              return;
            }

            device = std::move(result_device);
          }),
      UINT64_MAX);

  if (!device)
    return nullptr;

  SDL_PropertiesID win_prop =
      SDL_GetWindowProperties(window_target->AsSDLWindow());

  wgpu::SurfaceDescriptor surf_desc;
  surf_desc.label = nullptr;

#if defined(OS_WIN)
  wgpu::SurfaceSourceWindowsHWND winHWND;
  winHWND.hinstance = ::GetModuleHandle(nullptr);
  winHWND.hwnd = SDL_GetPointerProperty(
      win_prop, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
  surf_desc.nextInChain = &winHWND;
#else
#error unsupport operation system platform
#endif

  wgpu::Surface surface = instance.CreateSurface(&surf_desc);
  if (!surface)
    return nullptr;

  wgpu::SurfaceCapabilities capabilities;
  surface.GetCapabilities(adapter, &capabilities);

  wgpu::SurfaceConfiguration config;
  config.device = device;
  config.format = capabilities.formats[0];
  config.width = window_target->GetSize().x;
  config.height = window_target->GetSize().y;
  surface.Configure(&config);

  return std::unique_ptr<RenderDevice>(
      new RenderDevice(window_target, instance, adapter, device, surface));
}

RenderDevice::RenderDevice(base::WeakPtr<ui::Widget> window,
                           const wgpu::Instance& instance,
                           const wgpu::Adapter& adapter,
                           const wgpu::Device& device,
                           const wgpu::Surface& surface)
    : window_(window),
      instance_(instance),
      adapter_(adapter),
      device_(device),
      surface_(surface) {}

}  // namespace renderer
