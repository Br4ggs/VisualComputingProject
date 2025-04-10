# Visual Computing Project: Interactive CGS Surface Editor

## Overview

This project is an interactive tool for modeling and fast rendering of implicit surfaces. The system supports both primitive surfaces (defined via Signed Distance Functions, such as spheres, cubes, planes, and cylinders) and compound surfaces built using Constructive Solid Geometry (CSG) operations. Real-time rendering is provided via several backends (CPU-based sphere tracing, GPU-based sphere tracing, and marching cubes) with an intuitive UI built using ImGui.

## Getting Started

### Installation

Unzip the provided archive to your desired directory. The precompiled executable is included in the archive, so no repository cloning is necessary.

### Running the Program

To run the program, simply execute the provided binary:

```bash
./VisualComputingProject
```
## Usage Instructions
When the program launches, an OpenGL window appears along with an interactive ImGui interface containing the following panels:

### Controls
- Movement: Use the W, A, S, and D keys to move the camera.
- Zoom: Scroll the mouse wheel to zoom in or out.
- Rotation: Hold the right mouse button and drag to rotate the camera.
- Input Factors: Adjust numerical parameters (e.g., scroll and movement factors) in the Controls panel.

### Rendering
Rendering Backend: A dropdown menu in the Rendering panel lets you choose between:

**Sphere Tracing (CPU)**: Executes sphere tracing on the CPU.

**Marching Cubes**: Generates a polygonal mesh from the implicit surface.

**Sphere Tracing (GPU)**: Utilizes GPU-based sphere tracing.

#####  Backend Settings: Depending on the selected backend, adjust specific parameters:

For **Sphere Tracing (CPU/GPU)**: Configure maximum steps, maximum distance, epsilon, background color, and fog creep.

For **Marching Cubes**: Set grid size and grid spacing.

### Camera
Camera Settings: Modify the camera's position, look-at point, and field of view using the Camera panel.

### Lighting
Lighting Controls: Adjust lighting properties, including position, specular color, ambient color, specular strength, and shininess.

### Geometry
Editing Primitives: View and edit the shapes currently in the scene. Editable attributes include:
- RGB Color.
- Position (XYZ).
- Rotation (XYZ).
- Scale (XYZ).


Adding Elements: Use the "Add Primitive" and "Add Operator" buttons to include new shapes or CSG operations in your scene.

## Additional Information
### Demo Video: A demo video is included with the final deliverables.


### Slides: The final set of presentation slides is provided alongside the project.


### Source Code: The complete source code is supplied in the archive.
