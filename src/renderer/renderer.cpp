#include "renderer/renderer.hpp"


#	include <GL/glew.h>
#ifdef __APPLE__
#	include <GLUT/glut.h>
#else
#	include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <array>
#include <memory>
#include <filesystem>

#include "common/utils.hpp"

#include "renderer/model.hpp"
#include "renderer/scene.hpp"
#include "renderer/color.hpp"
#include "renderer/scene_loader.hpp"

#include <iomanip>
#include <sstream>

namespace cg
{

    namespace renderer
    {
		struct renderer_data
		{
			int width;
			int height;
		};
		
        // ============== Namespace Vars ============== // 

        cg::scene* main_scene;

		renderer_data rd = { .width = 1720, .height = 880 };

        bool control_up = false;
        bool control_down = false;
        bool control_right = false;
        bool control_left = false;

		bool control_look_forward = false;
        bool control_look_back = false;
        bool control_look_right = false;
        bool control_look_left = false;

        bool control_time_plus = false;
		bool control_time_minus = false;

        float control_angle_y = 0;
        float control_angle_z = 0;

        const float SPEEDO = 0.05;
		const float ZOOM_SPEED = 1.2;
		const float TIME_SPEED = 0.05;
		const float MOVE_LOOK_SPEED = 0.1;

		bool zoom_in = false;
		bool zoom_out = false;

        // ============================================ //

        void update(const int)
        {
			// TODO: Rename variable name ... not so precise

			// Camera movement update 
        	using namespace coordinates;
        	cg::vec3 c_pos_original = main_scene->get_camera().get_position();
			cg::vec3 c_dir = main_scene->get_camera().get_direction();

			cg::vec3 c_pos = spherical::to_spherical({ c_pos_original[0] - c_dir[0] , c_pos_original[1] - c_dir[1] , c_pos_original[2] - c_dir[2]});

        	const float speed_inc_vertical = SPEEDO*control_right - SPEEDO*control_left;
        	const float speed_inc_horizontal = SPEEDO*control_down - SPEEDO*control_up;
			const float speed_inc_zoom = ZOOM_SPEED*zoom_out - ZOOM_SPEED*zoom_in;

			c_pos[0] += speed_inc_zoom;
        	c_pos[1] += speed_inc_vertical;
        	c_pos[2] += speed_inc_horizontal;

			if(c_pos[0] <= 0)
			{
				c_pos[0] -= speed_inc_zoom;
			}
        	if(c_pos[2] >= M_PI || c_pos[2] <= 0)
			{
				c_pos[2] -= speed_inc_horizontal;
			}

			// Camera look position update

			const cg::vec3 unit_forward = cg::normalize(cg::vec3{ c_dir[0] - c_pos_original[0] , 0 , c_dir[2] - c_pos_original[2] });
			const cg::vec2 forward {  unit_forward[0]*MOVE_LOOK_SPEED*control_look_forward ,  unit_forward[2]*MOVE_LOOK_SPEED*control_look_forward }; 
			const cg::vec2 back    { -unit_forward[0]*MOVE_LOOK_SPEED*control_look_back    , -unit_forward[2]*MOVE_LOOK_SPEED*control_look_back    };
			const cg::vec2 left    {  unit_forward[2]*MOVE_LOOK_SPEED*control_look_left    , -unit_forward[0]*MOVE_LOOK_SPEED*control_look_left    };
			const cg::vec2 right   { -unit_forward[2]*MOVE_LOOK_SPEED*control_look_right   ,  unit_forward[0]*MOVE_LOOK_SPEED*control_look_right   };

			const float move_x = forward[0] + back[0] + left[0] + right[0];
			const float move_z = forward[1] + back[1] + left[1] + right[1];


			c_pos = spherical::to_cartesian(c_pos);

			main_scene->get_camera().set_position( { c_pos[0] + move_x + c_dir[0], c_pos[1] , c_pos[2] + move_z + c_dir[2] }); // TODO: Change reference directly
			main_scene->get_camera().set_direction({ c_dir[0] + move_x           , c_dir[1] , c_dir[2] + move_z            }); // TODO: Change reference directly


			/* Time multiplier update */
			main_scene->relative_time_multiplier += TIME_SPEED*control_time_plus - TIME_SPEED*control_time_minus;

        	glutPostRedisplay();
        	glutTimerFunc(1,update,0);
        }


        void change_size(int w, int h) 
        {
        	// Prevent a divide by zero, when window is too short
        	// (you cant make a window with zero width).
        	if(h == 0)
        		h = 1;

        	// compute window's aspect ratio 
        	float ratio = w * 1.0 / h;

        	// Set the projection matrix as current
        	glMatrixMode(GL_PROJECTION);
        	// Load Identity Matrix
        	glLoadIdentity();

        	// Set the viewport to be the entire window
            glViewport(0, 0, w, h);

        	// Set perspective
        	gluPerspective(45.f ,ratio, 1.0f ,1000.0f);

        	// return to the model view matrix mode
        	glMatrixMode(GL_MODELVIEW);
        }


        void keyboard_down(unsigned char key, int, int ) 
        {
        	switch (key)
        	{
        	case ' ':
        		//render_mode = (render_mode == GL_FILL) ? GL_LINE : GL_FILL;
				cg::scene::debug = !cg::scene::debug;
        		break;
			case 'E':
			case 'e':
				zoom_in = true;
				break;
			case 'Q':
			case 'q':
				zoom_out = true;
				break;
			case '-':
				control_time_minus = true;
				break;
			case '+':
				control_time_plus = true;
				break;

			// Change Camera look position
			case 'D':
			case 'd':
        		control_look_right = true;
        		break;
        	case 'A':
			case 'a':
        		control_look_left = true;
        		break;
        	case 'W':
			case 'w':
        		control_look_forward = true;
        		break;
        	case 'S':
			case 's':
        		control_look_back = true;
        		break;

        	default:
        		break;
        	}

        	glutPostRedisplay();
        }


		void keyboard_up(unsigned char key, int, int ) 
        {
        	switch (key)
        	{
			case 'E':
			case 'e':
				zoom_in = false;
				break;
			case 'Q':
			case 'q':
				zoom_out = false;
				break;
			case '-':
				control_time_minus = false;
				break;
			case '+':
				control_time_plus = false;
				break;

			// Change Camera look position
			case 'D':
			case 'd':
        		control_look_right = false;
        		break;
        	case 'A':
			case 'a':
        		control_look_left = false;
        		break;
        	case 'W':
			case 'w':
        		control_look_forward = false;
        		break;
        	case 'S':
			case 's':
        		control_look_back = false;
        		break;

        	default:
        		break;
        	}

        	glutPostRedisplay();
        }


        void special_keyboard_down(int key, int , int ) 
        {
        	switch (key)
        	{
				// Change Camera spherical position
        		case GLUT_KEY_RIGHT:
        			control_right = true;
        			break;
        		case GLUT_KEY_LEFT:
        			control_left = true;
        			break;
        		case GLUT_KEY_UP:
        			control_up = true;
        			break;
        		case GLUT_KEY_DOWN:
        			control_down = true;
        			break;
        		default:
        			break;
        	}
        	glutPostRedisplay();
        }


        void special_keyboard_up(int key, int , int ) 
        {
        	switch (key)
        	{
				// Change Camera spherical position
        		case GLUT_KEY_RIGHT:
        			control_right = false;
        			break;
        		case GLUT_KEY_LEFT:
        			control_left = false;
        			break;
        		case GLUT_KEY_UP:
        			control_up = false;
        			break;
        		case GLUT_KEY_DOWN:
        			control_down = false;
        			break;
        		default:
        			break;
        	}
        	glutPostRedisplay();
        }


		void render_text(const std::string& text, const float width, const float height, const float pos_x=0, const float pos_y=0)
		{
			// Push Identity and save PROJECTION / MODELVIEW matrices
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
		    glDisable(GL_LIGHTING);

			gluOrtho2D(0, width, height, 0);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			//void* font = GLUT_BITMAP_8_BY_13;

		    //float textw = glutBitmapLength(font, (unsigned char*) text.c_str());
			glRasterPos2d(pos_x, pos_y); // width/2 - textw/2 // 24
			// Ignorar profundidade
			glDisable(GL_DEPTH_TEST);


			for (const char& c : text)
		        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);

		    // Restore PROJECTION / MODELVIEW matrices
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
		}


        void render(void)
        {
        	// clear buffers
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        	glLoadIdentity();

        	main_scene->compose();

        	glPolygonMode(GL_FRONT, (cg::scene::debug) ? GL_LINE : GL_FILL);


			static uint frame = 0;
			static int timet = 0;
			static int timebase = 0;
			static float fps = 0;
			

			if(cg::scene::debug)
			{
				frame++;
				timet=glutGet(GLUT_ELAPSED_TIME); 
				if (timet - timebase > 1000) { 
					fps = frame*1000.0/(timet-timebase); 
					timebase = timet; 
					frame = 0; 
				}

				std::stringstream ss1;
				ss1 << "FPS: " << std::fixed << std::setprecision(1) << fps;
				glColor3f(1,0,1);
				render_text(ss1.str(),rd.width,rd.height,10,24);
                std::stringstream ss2;
                ss2 << "Time Multiplier: " << std::fixed << std::setprecision(2) << main_scene->relative_time_multiplier;
                render_text(ss2.str(),rd.width,rd.height,10,37);

			}
			
			
        	// End of frame
        	glutSwapBuffers();
        }

		void lightning_init(void)
		{
			//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
			glEnable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
		}

        // ============================================ //
        
        void init(int* argc, char** argv)
        {
            // init GLUT and the window
 	        glutInit(argc, argv);
	        glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
			
	        glutInitWindowPosition(100,80);
	        glutInitWindowSize(rd.width,rd.height);
	        glutCreateWindow("Rendering Engine");
			//glutFullScreen();
	        glutTimerFunc(1,update,0);
    
            // Required callback registry 
        	glutDisplayFunc(render);
        	glutReshapeFunc(change_size);
    
            // Callback registration for keyboard processing
        	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
        	glutKeyboardFunc(keyboard_down);
			glutKeyboardUpFunc(keyboard_up);
        	glutSpecialFunc(special_keyboard_down);
        	glutSpecialUpFunc(special_keyboard_up);

			lightning_init();
			//glShadeModel(GL_FLAT); // this is here just for testing/exploring
			glewInit();


            //  OpenGL settings
        	glEnable(GL_DEPTH_TEST);
        	glEnable(GL_CULL_FACE);
        	glEnable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);

    		glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			
        	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	        glBlendEquation(GL_FUNC_ADD);
    
        }

        void run()
        {
            // enter GLUT's main cycle
	        glutMainLoop();
			delete main_scene;
		}

        void set_scene(cg::scene* s)
        {
            main_scene = s;
        }

        // ============================================ //         
    }
}