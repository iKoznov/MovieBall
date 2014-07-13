#include "engine/HelloIOS.hpp"
#include "MovieBallSFML.hpp"
#include <thread>
#include <chrono>
#include <fstream>

#include <boost/thread/thread.hpp>
#include <boost/format.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp>

void getSettings(const sf::Window &window)
{
	sf::ContextSettings settings = window.getSettings();
	
	std::cout << boost::format("depth bits:%s") % settings.depthBits << std::endl;
	std::cout << boost::format("stencil bits:%s") % settings.stencilBits << std::endl;
	std::cout << boost::format("antialiasing level:%s") % settings.antialiasingLevel << std::endl;
	std::cout << boost::format("version:%s.%s") % settings.majorVersion % settings.minorVersion << std::endl;
}

int glm1() {
	glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
	glm::mat4 Model = glm::mat4(1.0f);
	Model[3] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 Transformed = Model * Position;
	std::cout << boost::format("%s * %s = %s") % glm::to_string(Model) % glm::to_string(Position) % glm::to_string(Transformed) << std::endl;
	return 0;
}

int glm2() {
	glm::vec4 Position = glm::vec4(glm:: vec3(0.0f), 1.0f);
	glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f));
	glm::vec4 Transformed = Model * Position;
	return 0;
}

void shaderLog(unsigned int shader)
{
	int infologLen   = 0;
	int charsWritten = 0;
	char *infoLog;
	
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
	
	if(infologLen > 1)
	{
		infoLog = new char[infologLen];
		if(infoLog == NULL)
		{
			std::cerr<<"ERROR: Could not allocate InfoLog buffer\n";
			exit(1);
		}
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
		std::cerr << "SHADER: " << infoLog << "\n\n";
		delete[] infoLog;
	}
}

GLuint compileShader(std::string path, const GLenum shaderType)
{
	std::ifstream t(path);
	std::stringstream buffer;
	buffer << t.rdbuf();
	const std::string str = buffer.str();
	
	GLint length = str.length() + 1;
	GLchar* source = new GLchar[length];
	std::strcpy ( source, str.c_str() );
    
    GLuint shaderHandle = glCreateShader(shaderType);
    
    glShaderSource( shaderHandle, 1, &source, &length );
	free(source);
    
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
	shaderLog(shaderHandle);
    if (compileSuccess == GL_TRUE) {
		std::cout << boost::format("SHADER %s compiled") % path << std::endl;
	}
    
    return shaderHandle;
}

int main(int argc, char *argv[])
{
	hello();
	
#if 0
	glm1();
	glm2();
	
	{
		sf::Clock clock1;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::cout << "std::sleep " << clock1.restart().asMilliseconds() << std::endl;
	}
	
	{
		//waits 2 seconds
		sf::Clock clock2;
		boost::this_thread::sleep( boost::posix_time::seconds(1) );
		boost::this_thread::sleep( boost::posix_time::milliseconds(1000) );
		std::cout << "boost::sleep " << clock2.restart().asMilliseconds() << std::endl;
	}
	
	Game game;
	game.run();
#endif
	
	// create the window
	sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32,0,0,3,1));
	getSettings(window);
	
	// load resources, initialize the OpenGL states, ...
	glClearColor(1, 1, 1, 1);
	
	GLuint vertexShader = compileShader(MB_SHADERS"SimpleVertex.glsl", GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader(MB_SHADERS"SimpleFragment.glsl", GL_FRAGMENT_SHADER);
	
	GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
	
	GLint linkSuccess;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        perror(messages);
    } else {
		std::cout << "SHADER programm linked" << std::endl;
	}
	
	glUseProgram(programHandle);
	
	// run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
		
        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT);
		
        // draw...
		glBegin(GL_TRIANGLES);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, 0, 0);
		glEnd();
		
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }
	
    // release resources...
	
	return 0;
}
