// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일

#include "pch.h"
#include <vtkAutoInit.h>
#define vtkRenderingCore_AUTOINIT \
4(vtkRenderingOpenGL2, vtkInteractionStyle, vtkRenderingFreeType, vtkRenderingContextOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)

#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkConeSource.h>
// 미리 컴파일된 헤더를 사용하는 경우 컴파일이 성공하려면 이 소스 파일이 필요합니다.
