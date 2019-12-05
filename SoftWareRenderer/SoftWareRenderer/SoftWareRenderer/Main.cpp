
#include<Windows.h>
#include"Matrix4x4.h"
#include"Vector3D.h"
#include"CubeMeshData.h"
#include"MathTools.h"
#include"Light.h"
#include"Camera.h"
#include"Enum.h"

#include<gdiplus.h>
#include <string>
#pragma comment(lib, "gdiplus.lib")  

#define WINDOW_TITLE   L"MyRaster"
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define PI 3.1415926
// required system components
static HDC screen_hdc;
static HDC hCompatibleDC; 
static HBITMAP hCompatibleBitmap;
static HBITMAP hOldBitmap;	  
static BITMAPINFO binfo;

Gdiplus::Bitmap* texture;
int textureWidth;
int textureHeight;

float zBuff[SCREEN_HEIGHT][SCREEN_WIDTH];//Saving the z information of the pixel for depth testing
CubeMeshData mesh;//data of a cube
Light light;
Camera camera;
Color ambient;//Global ambient light color
float rot = 0;//Cube rotation
float rotationSpeed = 0.03f;//cube rotation speed
//
RenderMode currentMode;
LightMode lightMode;
TextureFilterMode textureFilterMode;
BYTE *textureBuffer;//Saving the texture RGB data
BYTE *backBuffer;  //saving color information of backbuff, every three bytes is a pixel
//
DWORD nowTime = 0;
DWORD intervalTime = 0;


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void Init(HWND hwnd);//Initialize components
void LoadTexture(wstring TexureFilename);//Load texture from a file

void SetMVTransform(Matrix4x4 m, Matrix4x4 v, Vertex &vertex);//transform the vertex from model space to view space
void SetProjectionTransform(Matrix4x4 p, Vertex &vertex);//Project vertices into clipping space
bool Clip(Vertex v);
void TransformToScreen(Vertex &v);//transform the vertex to Screen space
bool BackFaceCulling(Vertex p1, Vertex p2, Vertex p3);
void Draw(Matrix4x4 m, Matrix4x4 v, Matrix4x4 p);//Draw the scene to the backbuff
void DrawTriangle(Vertex p1, Vertex p2, Vertex p3, Matrix4x4 m, Matrix4x4 v, Matrix4x4 p);
void TriangleRasterization(Vertex p1, Vertex p2, Vertex p3);//Rasterize triangles
void DrawTriangleTop(Vertex p1, Vertex p2, Vertex p3);//Draw a flat-topped triangle
void DrawTriangleBottom(Vertex p1, Vertex p2, Vertex p3);//Draw a flat-bottomed triangle
void ScanlineFill(Vertex left, Vertex right, int yIndex);//Fills triangles by using scan line filling algorithm
Gdiplus::Color TransFormToGdiColor(Color color);//Convert our custom color to the color used for GDI painting
void BresenhamDrawLine(Vertex p1, Vertex p2);///Draw a line by using breasenham algorithm
void Lighting(Matrix4x4 m, Vector3D worldEyePositon, Vertex &v);//vertex lighting process
Color ReadTexture(int uIndex, int vIndex, Gdiplus::Color *color);
void ClearBackBuffer(byte r, byte g, byte b);
void SetBackBuff(int uIndex, int vIndex, Gdiplus::Color color);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	WNDCLASSEX wndclass = { 0 };
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = L"SoftWareRenderer";
	if (!RegisterClassEx(&wndclass))
		return -1;
	HWND hwnd;

	hwnd = CreateWindow(L"SoftWareRenderer", WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
	Init(hwnd);

	MoveWindow(hwnd, 250, 80, SCREEN_WIDTH, SCREEN_HEIGHT, true);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);


	nowTime = GetTickCount();
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DWORD deltaTime = GetTickCount() - nowTime;
			nowTime = GetTickCount();
			intervalTime += deltaTime;
			if (intervalTime > 0.03)//keep 30fps
			{
				intervalTime = 0;
				//clear zbuff and backbuff
				ZeroMemory(&zBuff, sizeof(zBuff));
				ClearBackBuffer(100, 100, 100);

				//generate m£¬v£¬p matrix
				rot += rotationSpeed;
				Matrix4x4 m = MathTools::GetRotateX(rot) * MathTools::GetRotateY(rot) * MathTools::GetTranslate(0, 0, 10);
				Matrix4x4 v = MathTools::GetView(camera.pos, camera.lookAt, camera.up);
				Matrix4x4 p = MathTools::GetProjection(camera.fov, camera.aspect, camera.zn, camera.zf);
				//
				Draw(m, v, p);
				//
				SetDIBits(screen_hdc, hCompatibleBitmap, 0, SCREEN_HEIGHT, backBuffer, (BITMAPINFO*)&binfo, DIB_RGB_COLORS);
				BitBlt(screen_hdc, -1, -1, SCREEN_WIDTH, SCREEN_HEIGHT, hCompatibleDC, 0, 0, SRCCOPY);
			}
			
		}
	}
	UnregisterClass(L"SoftWareRenderer", wndclass.hInstance);
	return 0;
}

void ClearBackBuffer(byte r, byte g, byte b)
{
	for (int i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; ++i)
	{

		backBuffer[i * 3 + 0] = r;
		backBuffer[i * 3 + 1] = g;
		backBuffer[i * 3 + 2] = b;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:

		ValidateRect(hwnd, NULL);
		break;

	case WM_KEYDOWN:
		if (unsigned int(wParam) == 'Q')
		{//press Q to switch RenderMode
			if (currentMode == RenderMode::Textured)
			{
				currentMode = RenderMode::VertexColor;
			}
			else if (currentMode == RenderMode::VertexColor)
			{
				currentMode = RenderMode::Wireframe;
			}
			else
			{
				currentMode = RenderMode::Textured;
			}
		}

		if (unsigned int(wParam) == 'W')
		{//press W to switch LightMode
			if (lightMode == LightMode::On)
			{
				lightMode = LightMode::Off;
			}
			else
			{
				lightMode = LightMode::On;
			}
		}

		if (unsigned int(wParam) == 'E')
		{//press W to switch TextureFilterMode
			if (textureFilterMode == TextureFilterMode::Bilinear)
			{
				textureFilterMode = TextureFilterMode::point;
			}
			else
			{
				textureFilterMode = TextureFilterMode::Bilinear;
			}
		}

		//press Up or Down to adjust rotation speed
		if (wParam == VK_UP)
		{
			rotationSpeed += 0.005f;
		}
		else if (wParam == VK_DOWN)
		{
			rotationSpeed -= 0.005f;
			if (rotationSpeed < 0)
			{
				rotationSpeed = 0;
			}
		}

		if (wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
		break;
	

	case WM_KEYUP:
		
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;

	default: return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

void Init(HWND hwnd)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	AdjustWindowRect(&rect, GetWindowLong(hwnd, GWL_STYLE), 0);

	ZeroMemory(&binfo, sizeof(BITMAPINFO));
	binfo.bmiHeader.biBitCount = 24;      // 24bits per pixel 
	binfo.bmiHeader.biCompression = BI_RGB;
	binfo.bmiHeader.biHeight = -SCREEN_HEIGHT;
	binfo.bmiHeader.biPlanes = 1;
	binfo.bmiHeader.biSizeImage = 0;
	binfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	binfo.bmiHeader.biWidth = SCREEN_WIDTH;

	screen_hdc = GetDC(hwnd);

	hCompatibleDC = CreateCompatibleDC(screen_hdc);
	hCompatibleBitmap = CreateCompatibleBitmap(screen_hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
	hOldBitmap = (HBITMAP)SelectObject(hCompatibleDC, hCompatibleBitmap);

	//Loads the Texture data from the Texture folder
	LoadTexture(wstring(L"Texture/MyTexture.jpg"));
	backBuffer = new byte[SCREEN_WIDTH*SCREEN_HEIGHT* 24 / 8];

	currentMode = RenderMode::Textured;
	lightMode = LightMode::On;
	textureFilterMode = TextureFilterMode::Bilinear;
	//
	ambient = Color(255, 255, 255);
	//
	light = Light(Vector3D(0, 0, 0), Color(255, 255, 255));
	//
	camera = Camera(Vector3D(0, 0, 0, 1), Vector3D(0, 0, 1, 1), Vector3D(0, 1, 0, 0), (float)PI / 4, SCREEN_WIDTH / SCREEN_HEIGHT, 1, 500);

	
}

void LoadTexture(wstring TexureFilename)
{
	Gdiplus::GdiplusStartupInput gdiplusstartupinput;
	Gdiplus::Color color;
	ULONG_PTR gdiplustoken;

	Gdiplus::GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);

	//loading texture
	texture = new Gdiplus::Bitmap(TexureFilename.c_str());
	textureWidth = texture->GetWidth();
	textureHeight = texture->GetHeight();
	

	//Create a texture buff
	textureBuffer = new BYTE[textureWidth*textureHeight * 3];

	for (int i = 0; i < textureHeight; ++i)
	{
		for (int j = 0; j <textureWidth; ++j)
		{
			texture->GetPixel(j, i, &color);
			//Writes the pixel colors to the texture buff, noting that the order of colors is RGB
			textureBuffer[i * textureWidth * 3 + (j + 1) * 3 - 1] = color.GetR();
			textureBuffer[i * textureWidth * 3 + (j + 1) * 3 - 2] = color.GetG();
			textureBuffer[i * textureWidth * 3 + (j + 1) * 3 - 3] = color.GetB();
		}
	}

	Gdiplus::GdiplusShutdown(gdiplustoken);

}



void SetMVTransform(Matrix4x4 m, Matrix4x4 v, Vertex &vertex)
{
	vertex.point = vertex.point * m * v;
}


void SetProjectionTransform(Matrix4x4 p, Vertex &vertex)
{
	vertex.point = vertex.point * p;
	//we have transformed the vertex into clipping space, the original z information of the vertex are saved in vertex.point.w(it depends on the projection matrix we used, autually we use the projection matrix to save z in w)

	//OnePerZ saves 1/z for convenience of calculation of Interpolation
	vertex.onePerZ = 1 / vertex.point.w;

	//Multiply u,v,vcolor, and lightingcolor by 1/w for perspective correction later
	vertex.u *= vertex.onePerZ;
	vertex.v *= vertex.onePerZ;
	//
	vertex.vcolor = vertex.vcolor * vertex.onePerZ;
	//
	vertex.lightingColor = vertex.lightingColor * vertex.onePerZ;
}

bool Clip(Vertex v)
{//whether to clip this vertex
	if (v.point.x >= -v.point.w && v.point.x <= v.point.w &&
		v.point.y >= -v.point.w && v.point.y <= v.point.w &&
		v.point.z >= 0 && v.point.z <= v.point.w)
	{
		return true;
	}
	return false;
}
void TransformToScreen(Vertex &v)
{
	if (v.point.w != 0)
	{
		//do perspective division£¬ transform point into CVV
		v.point.x *= 1 / v.point.w;
		v.point.y *= 1 / v.point.w;
		v.point.z *= 1 / v.point.w;
		v.point.w = 1;
		//cvv to screen
		v.point.x = (v.point.x + 1) * 0.5f * SCREEN_WIDTH;
		v.point.y = (1 - v.point.y) * 0.5f * SCREEN_HEIGHT;
	}
}
bool BackFaceCulling(Vertex p1, Vertex p2, Vertex p3)
{
	if (currentMode == RenderMode::Wireframe)
	{
		return true;
	}
	else
	{
		Vector3D v1 = p2.point - p1.point;
		Vector3D v2 = p3.point - p2.point;
		Vector3D normal = Vector3D::Cross(v1, v2);
		//Because it's in view space, the camera position is (0,0,0).
		Vector3D viewDir = p1.point -  Vector3D(0, 0, 0);
		if (Vector3D::Dot(normal, viewDir) > 0)
		{
			return true;
		}
		return false;
	}
}
void Draw(Matrix4x4 m, Matrix4x4 v, Matrix4x4 p)
{
	for (unsigned i = 0; i + 2 < mesh.verts.size(); i += 3)
	{
		DrawTriangle(mesh.verts[i], mesh.verts[i + 1], mesh.verts[i + 2], m, v, p);
	}
}
void DrawTriangle(Vertex p1, Vertex p2, Vertex p3, Matrix4x4 m, Matrix4x4 v, Matrix4x4 p)
{
	//--------------------The geometric phase---------------------------//

	if (lightMode == LightMode::On)
	{//Vertex lighting
		Lighting(m, camera.pos,  p1);
		Lighting(m, camera.pos,  p2);
		Lighting(m, camera.pos,  p3);
	}

	//transform to view space
	SetMVTransform(m, v,  p1);
	SetMVTransform(m, v,  p2);
	SetMVTransform(m, v,  p3);

	//back culling in view space
	if (BackFaceCulling(p1, p2, p3) == false)
	{
		return;
	}

	//Transform to the homogeneous clipping space
	SetProjectionTransform(p,  p1);
	SetProjectionTransform(p,  p2);
	SetProjectionTransform(p,  p3);

	if (Clip(p1) == false || Clip(p2) == false || Clip(p3) == false)
	{
		return;
	}
	//Transform to screen coordinates
	TransformToScreen( p1);
	TransformToScreen( p2);
	TransformToScreen( p3);

	//--------------------Raster phase---------------------------//

	if (currentMode == RenderMode::Wireframe)
	{
		BresenhamDrawLine(p1, p2);
		BresenhamDrawLine(p2, p3);
		BresenhamDrawLine(p3, p1);
	}
	else
	{
		TriangleRasterization(p1, p2, p3);
	}
}
void TriangleRasterization(Vertex p1, Vertex p2, Vertex p3)
{
	if (p1.point.y == p2.point.y)
	{
		if (p1.point.y < p3.point.y)
		{
			DrawTriangleTop(p1, p2, p3);
		}
		else
		{
			DrawTriangleBottom(p3, p1, p2);
		}
	}
	else if (p1.point.y == p3.point.y)
	{
		if (p1.point.y < p2.point.y)
		{
			DrawTriangleTop(p1, p3, p2);
		}
		else
		{
			DrawTriangleBottom(p2, p1, p3);
		}
	}
	else if (p2.point.y == p3.point.y)
	{
		if (p2.point.y < p1.point.y)
		{
			DrawTriangleTop(p2, p3, p1);
		}
		else
		{
			DrawTriangleBottom(p1, p2, p3);
		}
	}
	else
	{//split triangles
		Vertex top;

		Vertex bottom;
		Vertex middle;
		if (p1.point.y > p2.point.y && p2.point.y > p3.point.y)
		{
			top = p3;
			middle = p2;
			bottom = p1;
		}
		else if (p3.point.y > p2.point.y && p2.point.y > p1.point.y)
		{
			top = p1;
			middle = p2;
			bottom = p3;
		}
		else if (p2.point.y > p1.point.y && p1.point.y > p3.point.y)
		{
			top = p3;
			middle = p1;
			bottom = p2;
		}
		else if (p3.point.y > p1.point.y && p1.point.y > p2.point.y)
		{
			top = p2;
			middle = p1;
			bottom = p3;
		}
		else if (p1.point.y > p3.point.y && p3.point.y > p2.point.y)
		{
			top = p2;
			middle = p3;
			bottom = p1;
		}
		else if (p2.point.y > p3.point.y && p3.point.y > p1.point.y)
		{
			top = p1;
			middle = p3;
			bottom = p2;
		}
		else
		{
			//three points are in the same line
			return;
		}
		///figure out  middle point x
		float middlex = (middle.point.y - top.point.y) * (bottom.point.x - top.point.x) / (bottom.point.y - top.point.y) + top.point.x;
		float dy = middle.point.y - top.point.y;
		float t = dy / (bottom.point.y - top.point.y);
		//figure out left and right point
		Vertex newMiddle;
		newMiddle.point.x = middlex;
		newMiddle.point.y = middle.point.y;
		MathTools::ScreenSpaceLerpVertex(newMiddle, top, bottom, t);

		DrawTriangleBottom(top, newMiddle, middle);
		DrawTriangleTop(newMiddle, middle, bottom);
	}
}
void DrawTriangleTop(Vertex p1, Vertex p2, Vertex p3)
{
	for (float y = p1.point.y; y <= p3.point.y; y += 0.5)
	{
		int yIndex = (int)(round(y));
		if (yIndex >= 0 && yIndex < SCREEN_HEIGHT)
		{
			float xl = (y - p1.point.y) * (p3.point.x - p1.point.x) / (p3.point.y - p1.point.y) + p1.point.x;
			float xr = (y - p2.point.y) * (p3.point.x - p2.point.x) / (p3.point.y - p2.point.y) + p2.point.x;

			float dy = y - p1.point.y;
			float t = dy / (p3.point.y - p1.point.y);
			//figure out left point and right point
			Vertex new1;
			new1.point.x = xl;
			new1.point.y = y;
			MathTools::ScreenSpaceLerpVertex(new1, p1, p3, t);
			//
			Vertex new2;
			new2.point.x = xr;
			new2.point.y = y;
			MathTools::ScreenSpaceLerpVertex(new2, p2, p3, t);
			//fills  triangle
			if (new1.point.x < new2.point.x)
			{
				ScanlineFill(new1, new2, yIndex);
			}
			else
			{
				ScanlineFill(new2, new1, yIndex);
			}
		}
	}
}
void DrawTriangleBottom(Vertex p1, Vertex p2, Vertex p3)
{
	for (float y = p1.point.y; y <= p2.point.y; y += 0.5)
	{
		int yIndex = (int)round(y);
		if (yIndex >= 0 && yIndex < SCREEN_HEIGHT)
		{
			float xl = (y - p1.point.y) * (p2.point.x - p1.point.x) / (p2.point.y - p1.point.y) + p1.point.x;
			float xr = (y - p1.point.y) * (p3.point.x - p1.point.x) / (p3.point.y - p1.point.y) + p1.point.x;

			float dy = y - p1.point.y;
			float t = dy / (p2.point.y - p1.point.y);

			Vertex new1;
			new1.point.x = xl;
			new1.point.y = y;
			MathTools::ScreenSpaceLerpVertex(new1, p1, p2, t);
			//
			Vertex new2;
			new2.point.x = xr;
			new2.point.y = y;
			MathTools::ScreenSpaceLerpVertex(new2, p1, p3, t);

			if (new1.point.x < new2.point.x)
			{
				ScanlineFill(new1, new2, yIndex);
			}
			else
			{
				ScanlineFill(new2, new1, yIndex);
			}
		}
	}
}
void ScanlineFill(Vertex left, Vertex right, int yIndex)
{
	float dx = right.point.x - left.point.x;
	for (float x = left.point.x; x <= right.point.x; x += 0.5)
	{   
		int xIndex = (int)(x);
		if (xIndex >= 0 && xIndex < SCREEN_WIDTH)
		{
			float lerpFactor = 0;
			if (dx != 0)
			{
				lerpFactor = (x - left.point.x) / dx;
			}
			
			float onePreZ = MathTools::Lerp(left.onePerZ, right.onePerZ, lerpFactor);//interpolates 1/z linearly for perspective correction later
			if (onePreZ >= zBuff[yIndex][xIndex])//Using 1/z for depth testing
			{
				
				zBuff[yIndex][xIndex] = onePreZ;

				float w = 1 / onePreZ;

				// uv perspective correction 
				float u = MathTools::Lerp(left.u, right.u, lerpFactor) * w * (textureWidth - 1);
				float v = MathTools::Lerp(left.v, right.v, lerpFactor) * w * (textureHeight - 1);
				
				Color texColor;
				Gdiplus::Color textureColor;
				Color finalColor;

				if (RenderMode::Textured == currentMode)
				{
					if (textureFilterMode == TextureFilterMode::point)
					{//Point sampling

						int uIndex = (int)round(u);
						int vIndex = (int)round(v);
						uIndex = MathTools::Range(uIndex, 0, textureWidth - 1);
						vIndex = MathTools::Range(vIndex, 0, textureHeight - 1);
						//The uv coordinate system we used is Direct3D style
						texColor = ReadTexture(uIndex, vIndex, &textureColor);

					}
					else if (textureFilterMode == TextureFilterMode::Bilinear)
					{//Bilinear sampling
						float uIndex = (float)floor(u);
						float vIndex = (float)floor(v);
						float du = u - uIndex;
						float dv = v - vIndex;

						Color texcolor1 = ReadTexture((int)uIndex, (int)vIndex, &textureColor) * (1 - du) * (1 - dv);
						Color texcolor2 = ReadTexture((int)uIndex + 1, (int)vIndex, &textureColor) * du * (1 - dv);
						Color texcolor3 = ReadTexture((int)uIndex, (int)vIndex + 1, &textureColor) * (1 - du) * dv;
						Color texcolor4 = ReadTexture((int)uIndex + 1, (int)vIndex + 1, &textureColor) * du * dv;
						texColor = texcolor1 + texcolor2 + texcolor3 + texcolor4;
					}
					if (lightMode == LightMode::On)
					{
						Color lightColor = MathTools::Lerp(left.lightingColor, right.lightingColor, lerpFactor) * w;
						finalColor = texColor * lightColor;
					}
					else
					{
						finalColor = texColor;
					}
				}
				else if (RenderMode::VertexColor == currentMode)
				{
					Color vertColor = MathTools::Lerp(left.vcolor, right.vcolor, lerpFactor) * w;
					if (lightMode == LightMode::On)
					{
						Color lightColor = MathTools::Lerp(left.lightingColor, right.lightingColor, lerpFactor) * w;
						finalColor = vertColor * lightColor;
					}
					else
					{
						finalColor = vertColor;
					}
				}
				
				SetBackBuff(xIndex, yIndex, TransFormToGdiColor(finalColor));
				
			}
		}
	}
}


void BresenhamDrawLine(Vertex p1, Vertex p2)
{
	int x = (int)round(p1.point.x);
	int y = (int)round(p1.point.y);
	int dx = (int)round(p2.point.x - p1.point.x);
	int dy = (int)round(p2.point.y - p1.point.y);
	int stepx = 1;
	int stepy = 1;

	if (dx >= 0)
	{
		stepx = 1;
	}
	else
	{
		stepx = -1;
		dx = abs(dx);
	}

	if (dy >= 0)
	{
		stepy = 1;
	}
	else
	{
		stepy = -1;
		dy = abs(dy);
	}

	int dx2 = 2 * dx;
	int dy2 = 2 * dy;

	if (dx > dy)
	{
		int error = dy2 - dx;
		for (int i = 0; i <= dx; i++)
		{
			SetBackBuff(x, y, Gdiplus::Color::White);
			if (error >= 0)
			{
				error -= dx2;
				y += stepy;
			}
			error += dy2;
			x += stepx;

		}
	}
	else
	{
		int error = dx2 - dy;
		for (int i = 0; i <= dy; i++)
		{
			SetBackBuff(x, y, Gdiplus::Color::White);
			if (error >= 0)
			{
				error -= dy2;
				x += stepx;
			}
			error += dx2;
			y += stepy;

		}
	}
}
void Lighting(Matrix4x4 m, Vector3D worldEyePositon, Vertex &v)
{
	Vector3D worldPoint = v.point * m;//position in World space 
	Matrix4x4 it = m.Inverse();
	it.Transpose();
	Vector3D normal = v.normal * it;//The model space normals are multiplied by the transposition of inverse of the world matrix to get the world space normals
	normal.Normalize();
	Color emissiveColor = mesh.material.emissive;
	Color ambientColor = ambient * mesh.material.ka;

	Vector3D inLightDir = light.worldPosition - worldPoint;
	inLightDir.Normalize();
	float diffuse = Vector3D::Dot(normal, inLightDir);
	if (diffuse < 0)
	{
		v.lightingColor = emissiveColor + ambientColor;
	}
	else
	{
		Color diffuseColor = mesh.material.diffuse * diffuse * light.lightColor;
		//
		Vector3D inViewDir = worldEyePositon - worldPoint;
		inViewDir.Normalize();
		Vector3D h = inViewDir + inLightDir;
		h.Normalize();
		float specular = 0;
		if (diffuse != 0)
		{
			specular = pow(MathTools::Range(Vector3D::Dot(h, normal), (float)0, (float)1), mesh.material.shininess);
		}
		Color specularColor = mesh.material.specular * specular * light.lightColor;
		v.lightingColor = emissiveColor + ambientColor + diffuseColor + specularColor;
	}
		

	//
	
}
Color ReadTexture(int uIndex, int vIndex, Gdiplus::Color *color)
{
	int u = MathTools::Range(uIndex, 0, textureWidth - 1);
	int v = MathTools::Range(vIndex, 0, textureHeight- 1);

	byte r = textureBuffer[v* textureWidth * 3 + (u + 1) * 3 - 3];
	byte g = textureBuffer[v* textureWidth * 3 + (u + 1) * 3 - 2];
	byte b = textureBuffer[v * textureWidth * 3 + (u + 1) * 3 - 1];

	return Color(r , g , b );

}


Gdiplus::Color TransFormToGdiColor(Color color)
{
	byte r = color.GetR();
	byte g = color.GetG();
	byte b = color.GetB();
	return Gdiplus::Color(r, g, b);
};

void SetBackBuff(int uIndex, int vIndex, Gdiplus::Color color)
{
	backBuffer[vIndex * SCREEN_WIDTH * 3 + (uIndex + 1) * 3 - 3] = color.GetR();
	backBuffer[vIndex *  SCREEN_WIDTH * 3 + (uIndex + 1) * 3 - 2] = color.GetG();
	backBuffer[vIndex *  SCREEN_WIDTH * 3 + (uIndex + 1) * 3 - 1] = color.GetB();
}


