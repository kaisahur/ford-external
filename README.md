## ford-external

A lightweight external Roblox cheat written in C++.
DirectComposition overlay · Low latency · Simple codebase
<br> </div>
## About

ford-external is a small external Roblox project focused on keeping things simple, clean, and fast.
The overlay renders through a standard DXGI swapchain, but instead of relying on DWM’s layered‑window composition, it uses DirectComposition. This provides a modern GPU‑accelerated composition path, reducing latency and avoiding the traditional external‑overlay bottlenecks associated with DWM.



## Requirements

- Windows 10/11

- Visual Studio 2022+

- Windows SDK
