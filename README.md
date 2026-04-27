# Orion Engine

Orion Engine is a C++ OpenGL game engine with a minimal but functional **Entity Component System** and a **Scene System**.
Please follow the steps below to build **Orion Engine**

---

## Prerequisites

- [Microsoft Visual Studio 2022](https://visualstudio.microsoft.com/downloads/)
- Dear ImGui
- GLFW & GLAD
- GLM

---

## Clone the Repository 

- Open PowerShell or Command Prompt and go into the directory you wish to clone the repository in.
- Clone the repository
	```
		git clone https://github.com/BishopPokhrel/OrionEngine.git
	```

---

## Building Orion Engine

- Open `.sln` file in the repository and right click -> Properties.
- Verify the following properties:
	```
		General -> C++ Language Standard: ISO C++20 Standard (/std:c++20)
	```
	```
		C/C++ -> General -> Additional Include Directories: $(ProjectDir)\Source
	```
- Build the project

---

**If you have all the required libraries and have set the properties correctly, the project should build successfully!**