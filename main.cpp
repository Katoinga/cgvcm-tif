
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <memory>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <istream>
#include <sstream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

using namespace std;

class OBJFile
{
    public:
        using Vertices = vector < float > ;
        using texturePositions = vector < float > ;
        using normalPositions = vector < float > ;
        using Indices = vector < unsigned int > ;
        Vertices verticesRead;
        texturePositions texturesRead;
        normalPositions normalPositionRead;
        Indices fillRead;
        const Vertices& GetVertices() const
        {
            return verticesRead;
        }
        const texturePositions& GettexturePositions() const
        {
            return texturesRead;
        }
        const normalPositions& GetnormalPositions() const
        {
            return normalPositionRead;
        }
        const Indices& GetIndices() const
        {
            return fillRead;
        }
        //clase para abrir archivo obj encontrando los vertices linea por linea
        //lo realiza completando las matrices meidante va obteniendo la cadena
        // de cada uno
        //v x x x
        //vt x
        //vn
        // f
        OBJFile(const std::string& filename)
        {
            std::ifstream fileStream{ filename, std::ios_base::in };
            if (fileStream.is_open())
            {
                while (!fileStream.eof())
                {
                    std::string line;
                    std::getline(fileStream, line);
                    std::stringstream lStream{line };
                    std::string fillPositionSymbol;
                    lStream >> fillPositionSymbol;
                    //lee vertices
                    if (fillPositionSymbol == "v")
                    {
                        float vertexPosition{};
                        for (unsigned int i = 0; i < 3; ++i)
                        {
                            lStream >> vertexPosition;
                            //guarda lo leido
                            verticesRead.emplace_back(vertexPosition);
                        }
                    }
                    //lee las coord de la textura
                    else if (fillPositionSymbol == "vt")
                    {
                        float texturePosition{};
                        for (unsigned int i = 0; i < 2; ++i)
                        {
                            lStream >> texturePosition;
                            //guarda lo leido
                            texturesRead.emplace_back(texturePosition);
                        }
                    }
                    //lee la normalPosition del vertice
                    else if (fillPositionSymbol == "vn")
                    {
                        float normalPosition{};
                        for (unsigned int i = 0; i < 3; ++i)
                        {
                            lStream >> normalPosition;
                            //guarda lo leido
                            normalPositionRead.emplace_back(normalPosition);
                        }
                    }
                    //lee el filler del mesh
                    else if (fillPositionSymbol == "f")
                    {
                        char nline;
                        unsigned short fillPosition{};
                        for (unsigned int i = 0; i < 3; ++i)
                        {
                            for (unsigned int j = 0; j < 3; ++j)
                            {
                                lStream >> fillPosition;
                                //guarda lo leido
                                fillRead.emplace_back(fillPosition);
                                if (j < 2)
                                {

                                    lStream >> nline;
                                }
                            }
                        }
                    }
                }
            }
        }

};

//Autor > Emilio Valdivia
void WriteFrame(unsigned fillPosition,std::vector<glm::vec3> vertices){
    //solo guardo los 150 primeros frames por mi disco xd
        // solo estoy guardando vertices porque para un obj, los f que serian el contenido que se
    // entrelaza se calculan mediante metodos que aun no hemos visto
    //como texturas y otras cosas.
        if(fillPosition <150){
            std::string fileName  = "/home/emilio/lab2fin/frames/frame_"+std::to_string(fillPosition)+".obj";

            std::ofstream frame (fileName);

            if (!frame.is_open())
            {
                std::cerr<<"Error opening "<<fileName <<std::endl;
                return;
            }

            //headers
            frame <<"# Blender3D v249 OBJ File: untitled.blend"<<std::endl;
            frame<<"www.blender3d.org"<<std::endl;
            frame<<"mtllib cube.mtl"<<std::endl;

            //solamente escribe vetices, porque vt vn no realizan el trazo de las aristas
            //f es para dibujar la textura y este se bugea debido a que esta cayendo el objeto
            //por este motivo solo decidi guardar los vertices
            //estuve buscando como dibujar aristas en obj por files
            //no encontre informacion a tiempo :(

            for (int i=0;i<vertices.size();i++){

                frame<<"v " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z<<std::endl;

            }

            frame<<"s off"<< std::endl;

        }




}
int main()
{
    const char* vertexShaderSrc =
            "#version 400\n"
            "in vec4 position;"
            "void main()"
            "{"
                 "gl_Position = position;"
            "}";


    const char* fragmentShaderSrc =
            "#version 400\n"
            "out vec4 color;"
            "void main()"
            "{"
                 "color = vec4(0.6, 0.5, 0.0, 1.0);"
            "}";



    if(glfwInit() == 0)
    {
        std::cerr<<"GLFW failed\n";
        return -1;
    }
    GLFWwindow* window;
    window = glfwCreateWindow(800, 800, "Hello OpenGL", nullptr, nullptr);

    if(window == nullptr)
    {
        std::cerr<<"GLFW failed\n";
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Iniciar la librería GLEW
    if(glewInit() != GLEW_OK)
    {
        std::cerr<<"GLEW failed\n";
        // glfwTerminate(): Liberar memoria
        glfwTerminate();
        return -1;

    }
    std::vector<glm::vec3> velocities(3, glm::vec3(0,0,0));

    std::vector<float> mass = {1, 2, 0.5};

    glm::vec3 gravity(0, -0.98, 0);

    float h = 0.0002;
    float time=0;
    int count2=0;

    int cont= 0;
    // OBJ
    //cambiar el path segun el file que se quiera leer

    //OBJFile obj("/home/emilio/lab2fin/gm.obj");
    //OBJFile obj("/home/emilio/lab2fin/attic.obj");
    OBJFile obj("/home/emilio/lab2fin/cyber.obj");
    //OBJFile obj("/home/emilio/lab2fin/bunny.obj");

    std::vector<float> vector = obj.GetVertices();
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> edges = obj.GetIndices();


    for(unsigned int i=0; i<vector.size(); ++i){
        vertices.push_back(glm::vec3(float(vector[i]/6),float(vector[i+1]/6),float(vector[i+2]/6)));
        i = i+2;
    }

    GLuint vertexbuffer;

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(float)  , &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, nullptr);



    GLuint vertexShader  = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertexShader);


    GLuint fragmentShader  = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fragmentShader);


    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);



    // Mantiene la ventana abierta, mientras el usuario no cierre la ventana
    while(!glfwWindowShouldClose(window))
    {


        glClearColor(0.0, 0.0, 0.2, 1.0);

        glClear(GL_COLOR_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //Tomando del codigo visto en clase para cada vertice, en este caso quiero
        //que se mueva todo el objeto junto con la gravedad para abajo
        //poreso solo calculo una velocidad y se la aplico a todos los vertices

        velocities[0]  = velocities[0] + h*(gravity/mass[1]);
        //euler simple gravedad hacia abajo
        /*
        for (int i=0;i<vertices.size();i++){
            vertices[i] = vertices[i] + velocities[0]*(time/2);
        }*/
        // euler punto medio gravedad en diagonal hacia adelante y abajo
        vec3 velocity;
        float k = 1.0f;
        float damping = 0.1f;
        float dt = 0.01f;
        /*
         for (int i=0;i<vertices.size();i++){
            auto halfpos = vertices[i] + velocities[0]*(time/2);
            vertices[i] = vertices[i] + velocities[0]+time*halfpos;

        }*/
         //spring and dampers
        if (count2 < mass.size()){
        count2++;
         for (int i=0;i<vertices.size();i++){

                 vec3 gravity = vec3(0, -0.98, 0)*dt;
                 vec3 spring = gravity*(vec3(0,0,0))*k*dt;
                 vec3 dampingForce = velocity*damping;

                 //Calculate velocity
                 vec3 acceleration = (gravity+spring-dampingForce)/mass[1];
                 velocity += acceleration;
                 vertices[i] += velocity;


         }
        }
        else if (count2 >=mass.size()){
            count2=0;
            for (int i=0;i<vertices.size();i++){

                    vec3 gravity = vec3(0, -0.98, 0)*dt;
                    vec3 spring = gravity*(vec3(0,0,0))*k*dt;
                    vec3 dampingForce = velocity*damping;

                    //Calculate velocity
                    vec3 acceleration = (gravity+spring-dampingForce)/mass[1];
                    velocity += acceleration;
                    vertices[i] -= velocity;


            }
        }




        time+=h;
        cont++;
        //guardo frame
        WriteFrame(cont,vertices);





        glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(float)  , &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, nullptr);

        //aca uso los vertices para dibujar sale perfecto pero no son realmente la conexion de aristas
        //pero realmente es la que mas funciona me funciono probando mas de 20 obj
        //descomentar la linea de debajo y comentar drawelements para visualizar
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        //aca uso las aristas, se entrelazan las aristas, como aun no muevo camara, es complicado ver algunos obj
        //pero aveces no funciona del todo correcto se bugea con euler y un vertice nunca cae
        //quise seguir probando pero el disco empezo a fallar asi que lo deje asi funciona pero no del todo

        //glDrawElements(GL_TRIANGLES,vector.size(),GL_UNSIGNED_INT, &edges[1]);


        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glDeleteBuffers(1, &vertexbuffer);
    glfwTerminate();

    return 0;
}


