/**
 * Copyright 2011, 2012 Jonatan Olofsson
 *
 * This file is part of C++ GL Framework (CPGL).
 *
 * CPGL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CPGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CPGL.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include "types.hpp"
#include "Window.hpp"

namespace CPGL {
    namespace glut {
        using namespace core;
        void idle();
        void display();
        void reshape(int w, int h);
        void mouse(int button, int state, int x, int y);
        void motion(int x, int y);
        void passivemotion(int x,int y);
        void keyboard(unsigned char key, int x, int y);


        typedef std::map<int, window_t> glutmap;
        glutmap windows;

        void set_window_title(const int wn, const std::string name) {
            assert(wn);
            glutSetWindow(wn);
            glutSetWindowTitle(name.c_str());
        }

        void redisplay() {
            glutPostRedisplay();
        }

        window_t create_window(const YAML::Node& c) {
            glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
            glutInitWindowPosition( c["dimensions"]["x"].as<int>(0), c["dimensions"]["y"].as<int>(0) );
            glutInitWindowSize( c["dimensions"]["width"].as<int>(800), c["dimensions"]["height"].as<int>(600) );

            int id = glutCreateWindow(c["name"].as<std::string>("").c_str());
            glutIdleFunc(glut::idle);
            glutDisplayFunc(glut::display);
            glutReshapeFunc(glut::reshape);
            glutMotionFunc(glut::motion);
            glutMouseFunc(glut::mouse);
            glutPassiveMotionFunc(glut::passivemotion);
            glutKeyboardFunc(glut::keyboard);
            glutShowWindow();

            window_t win(new Window(id, c));
            windows.insert(glutmap::value_type(id, win));
            return win;
        }

        void OnTimer(int value)
        {
            glutPostRedisplay();
            glutTimerFunc(20, &OnTimer, value);
        }

        void run(int& argc, char* argv[], void(*fn)(window_handle_callback_t), window_handle_callback_t wincb) {
            glutInit(&argc, argv);
            fn(wincb);
            glutTimerFunc(20, &OnTimer, 0);
            glutMainLoop();
        }

        boost::thread glut_thread;
        void init(int& argc, char* argv[], void(*fn)(window_handle_callback_t), window_handle_callback_t wincb) {
            glut_thread = boost::thread(boost::bind(run, argc, argv, fn, wincb));
        }

        void wait() {
            glut_thread.join();
        }

        void idle() { usleep(100); }
        void display() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //~ std::cout << "Display..."<< std::endl;
            int window = glutGetWindow();
            if(window == 0) return;

            glutmap::iterator w = windows.find(window);
            if(w == windows.end()) return;
            w->second->DRAW();
            glutSwapBuffers();//glutPostRedisplay();
        }
        void reshape(int w, int h) {
            //~ std::cout << "reshape" << std::endl;
            int window = glutGetWindow();
            if(window == 0) return;

            glutmap::iterator win = windows.find(window);
            if(win == windows.end()) return;
            win->second->RESHAPE(w,h);
        }
        void mouse(int button, int state, int x, int y) {
            int window = glutGetWindow();
            if(window == 0) return;

            glutmap::iterator w = windows.find(window);
            if(w == windows.end()) return;
            w->second->MOUSE(button,state,x,y);
        }
        void motion(int x, int y) {
            int window = glutGetWindow();
            if(window == 0) return;

            glutmap::iterator w = windows.find(window);
            if(w == windows.end()) return;
            w->second->MOTION(x,y);
        }
        void passivemotion(int x,int y) {
            int window = glutGetWindow();
            if(window == 0) return;

            glutmap::iterator w = windows.find(window);
            if(w == windows.end()) return;
            w->second->PASSIVEMOTION(x,y);
        }
        void keyboard(unsigned char key, int x, int y) {
            int window = glutGetWindow();
            if(window == 0) return;

            glutmap::iterator w = windows.find(window);
            if(w == windows.end()) return;
            w->second->KEYBOARD(key,x,y);
        }
    }
}
