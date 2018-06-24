#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <Windows.h>
#include <MMSystem.h>

GLUquadric *sun;

GLfloat ctrlpoints[4][3] = {
	{ -4.0, -4.0, 0.0 },{ -2.0, 4.0, 0.0 },
{ 2.0, -4.0, 0.0 },{ 4.0, 4.0, 0.0 } };

static int moveA = 0, moveB = 0;
static int pushA = 0, pushB = 0;// 더 세개 밀기
static int meetA = 0, meetB = 0; // 만났을때 1
int RotFlag = 0; // 공전시작
int NightFlag = 0;

static int flagA = 0, flagB = 0;
// 잘못 누를 시 딜레이?
static double x, y;
static int Time = 0;
static int Pause = 0;

GLuint MyTextureObject[1];
typedef union PixelInfo
{
	std::uint32_t Colour;
	struct
	{
		std::uint8_t B, G, R, A;
	};
} *PPixelInfo;


class BMP
{
private:
	std::uint32_t width, height;
	std::uint16_t BitsPerPixel;
	std::vector<std::uint8_t> Pixels;

public:
	BMP(const char* FilePath);
	std::vector<std::uint8_t> GetPixels() const { return this->Pixels; }
	std::uint32_t GetWidth() const { return this->width; }
	std::uint32_t GetHeight() const { return this->height; }
	bool HasAlphaChannel() { return BitsPerPixel == 32; }
};

BMP::BMP(const char* FilePath)
{
	std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
	if (!hFile.is_open()) throw std::invalid_argument("Error: File Not Found.");

	hFile.seekg(0, std::ios::end);
	std::size_t Length = hFile.tellg();
	hFile.seekg(0, std::ios::beg);
	std::vector<std::uint8_t> FileInfo(Length);
	hFile.read(reinterpret_cast<char*>(FileInfo.data()), 54);

	if (FileInfo[0] != 'B' && FileInfo[1] != 'M')
	{
		hFile.close();
		throw std::invalid_argument("Error: Invalid File Format. Bitmap Required.");
	}

	if (FileInfo[28] != 24 && FileInfo[28] != 32)
	{
		hFile.close();
		throw std::invalid_argument("Error: Invalid File Format. 24 or 32 bit Image Required.");
	}

	BitsPerPixel = FileInfo[28];
	width = FileInfo[18] + (FileInfo[19] << 8);
	height = FileInfo[22] + (FileInfo[23] << 8);
	std::uint32_t PixelsOffset = FileInfo[10] + (FileInfo[11] << 8);
	std::uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
	Pixels.resize(size);

	hFile.seekg(PixelsOffset, std::ios::beg);
	hFile.read(reinterpret_cast<char*>(Pixels.data()), size);
	hFile.close();
}


void init(void)//Spece ad46
{

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

void MyDisplay() {


	GLfloat lightSource_diffuse[] = { 1.0, 1.0, 1.0};
	GLfloat lightSource_emission[] = { 1.0, 1.0, 1.0};
	GLfloat mat_specular[] = { 2.0, 2.0, 2.0 };
	GLfloat mat_shininess[] = { 5.0 };
	GLfloat light_position[] = { 0.0, 0.0, 0.0 };
	GLfloat light_ambient[] = { 1.0,1.0,1.0 };//주변광

	glShadeModel(GL_SMOOTH);

	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightSource_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.8);




	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();






	


	/////////텍스쳐 매핑

	glClearColor(0.0, 0.0, 0.0, 0.0);

	if (NightFlag == 1) {
		glClearColor(0.3,0.3, 0.3, 0.0); //부딫힐때의 이펙트
		NightFlag = 0;
	}
	

	BMP info = BMP("C:/Users/안종환/source/repos/ComputerGraphics/ComputerGraphics/ssun.bmp");//태양 이미지 같이 제출
	glGenTextures(1, &MyTextureObject[0]);

	glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.GetWidth(), info.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, info.GetPixels().data());

	// Wrapping
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// Filtering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glClear(GL_COLOR_BUFFER_BIT);

	
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 0.0);
	
	if (Time >= 300)
		RotFlag = 1;

	if(RotFlag == 1 )
		glRotatef(Time,0.0, 0.0, 1.0);// 박진감



	glutSolidSphere(0.4, 30, 16);// 태양

	


	
	

	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT0);


	
	



	glPushMatrix();


	x = -1.2 + 0.2 + moveA / 10.0;
	if (y - x <= 0.3 && y - x >= -0.3) {// 만날 때 밀어냄
		
		NightFlag = 1;
		meetA = 1;
		meetB = 1;
		moveA = moveA - 2;
	}


	if (x <= -1.4)// 승자가 정해질 시
		Pause = 1;



	glTranslatef(x, 0.0, 0.0);
	
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glutSolidSphere(0.15, 30, 8);// 왼쪽 구

	glPopMatrix();

	glPushMatrix();
	y = 0.8 + 0.2 - moveB / 10.0;

	if (y - x <= 0.3 && y - x >= -0.3) {
		NightFlag = 1;
		meetA = 1;
		meetB = 1;
		moveB = moveB - 2;
	}


	if (y >= 1.3)// 승자가 정해질 시
		Pause = 1;



	glDisable(GL_TEXTURE_2D);
	info = BMP("C:/Users/안종환/source/repos/ComputerGraphics/ComputerGraphics/apple.bmp");//apple
	glGenTextures(1, &MyTextureObject[0]);

	glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.GetWidth(), info.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, info.GetPixels().data());

	// Wrapping
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// Filtering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);

	glTranslatef(y, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(0.15, 30, 8);// 오른쪽 구
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glPopMatrix();
	glPopMatrix();


	glutSwapBuffers();
}




void MyKeyboard(unsigned char key, int x, int y) {
	if (Pause == 1) {}

	else {
		switch (key) {
		case 'd':case 'D':


			if (RotFlag == 1) {// 모드 변경시 변경되는 키
				if (flagB == 1) {
					moveB = moveB + 1;
					flagB = 0;
					glutPostRedisplay();
				}
			}




			else if (flagA == 1) {// 원래 키
				moveA = moveA + 1;
				flagA = 0;


				glutPostRedisplay();
			}






			break;


		case 'a':case 'A':


			if (RotFlag == 1) {//변경되는 키
				if (flagB == 0) {
					moveB = moveB + 1;
					flagB = 1;
					glutPostRedisplay();
				}
			}




			else if (flagA == 0) {
				moveA = moveA + 1;
				flagA = 1;
				glutPostRedisplay();
			}
			break;

		case 's':case 'S':


			if (RotFlag == 1) {
				if (meetB == 1) {
					moveA = moveA - 3;
					meetB = 0;
				}
			}
			break;



		case '4':
			if (RotFlag == 1) {// 모드 변경시 변경되는 키

				if (flagA == 1) {
					moveA = moveA + 1;
					flagA = 0;


					glutPostRedisplay();
				}
			}


			else if (flagB == 1) {
				moveB = moveB + 1;
				flagB = 0;
				glutPostRedisplay();
			}
			break;


		case '6':

			if (RotFlag == 1) {// 모드 변경시 변경되는 키
				if (flagA == 0) {
					moveA = moveA + 1;
					flagA = 1;
					glutPostRedisplay();
				}
			}



			else if (flagB == 0) {
				moveB = moveB + 1;
				flagB = 1;
				glutPostRedisplay();
			}
			break;


		case '5':

			if (RotFlag == 1) {
				if (meetB == 1) {
					moveB = moveB - 3;
					meetA = 0;
				}
			}



			break;


		case 'R':

			moveA = moveB = 0;
			flagA = flagB = 0;
			glutPostRedisplay();

			break;


		default:
			break;
		}
	}
}

void MyTimer(int Value) {

	if (Pause == 1) {}

	else {
		Time = (Time + 5) % 360;
	}


	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);

}



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("AhnJongHee");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);
	glutTimerFunc(40, MyTimer, 1);
	glutMainLoop();
	return 0;
}