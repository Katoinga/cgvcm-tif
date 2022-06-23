
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
#include "glm/ext.hpp"
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
//Autor > Sergio Soncco
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
void drawWalls()
{

glColor3f(1,0,0);
glPushMatrix();
//glRotatef(0,0,0,1);
//glScalef(2,1,2);
glBegin(GL_QUADS);
/* Floor */
glVertex3f(-1,-1,-1);
glVertex3f(-1,-1,1);
glVertex3f(1,-1,1);
glVertex3f(1,-1,-1);

glEnd();

glPopMatrix();

}
int main()
{

    //Vertex shader para solo rotación
   /* const char* vertexShaderSrc =
            "#version 400\n"
            "in vec3 vp;"
            "uniform mat4 mvMatrix;" // matriz de model-vista
            "uniform mat4 projMatrix;" //matriz de proyeccion
            "void main(){"
                  "gl_Position = projMatrix * mvMatrix * vec4(vp, 1.0);"
            "}";*/



    //Fragment shader solo para rotacion
    /*
       const char* fragmentShaderSrc =
               "#version 400\n"
               "out vec4 frag_color;"
               "uniform mat4 mvMatrix;" // matriz de model-vista
               "uniform mat4 projMatrix;" //matriz de proyeccion
               "void main(){"
                     "frag_color = vec4(0.5, 0.5, 0.5, 0.5);"
               "}";*/


    //Vertex shader para rotación y iluminacion
    const char* vertexShaderSrc =
            "#version 400\n"
            "uniform mat4 projMat;" //projection
            "uniform mat4 mvMat;" // model-view

            "layout (location=0) in vec3 vertPos;"
            "layout (location=1) in vec3 vertNormal;"

            "out vec4 varyingColor;"

            "struct PositionalLight"
            "{ vec4 ambient;"
            "vec4 diffuse;"
            "vec4 specular;"
            "vec3 position;"
            "};"

            "struct Material"
            "{ vec4 ambient;"
            "vec4 diffuse;"
            "vec4 specular;"
            "float shininess;"
            "};"


            "uniform vec4 globalAmbient;"
            "uniform PositionalLight light;"
            "uniform Material material;"
            "uniform mat4 norm_matrix;" // for transforming normals

            "void main()"
            "{"

            "vec4 P = mvMat * vec4(vertPos,1.0);"

            "vec3 N = normalize((norm_matrix * vec4(vertNormal,1.0)).xyz);"
            "vec3 L = normalize(light.position - P.xyz);"
            "vec3 V = normalize(-P.xyz);"
            "vec3 R = reflect(-L,N);"

            "vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;"
            "vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(N,L), 0.0);"
            "vec3 specular= material.specular.xyz * light.specular.xyz * pow(max(dot(R,V), 0.0f), material.shininess);"
            "varyingColor = vec4((ambient + diffuse + specular), 1.0);"
            "gl_Position = projMat*mvMat*vec4(vertPos, 1.0);"

            "}";


    //Fragment shader para rotacion y iluminacion
    const char* fragmentShaderSrc =
            "#version 400\n"
            "out vec4 color;"
            "uniform mat4 mvMatrix;" // matriz de model-vista
            "uniform mat4 projMatrix;" //matriz de proyeccion
            "in vec4 varyingColor;"
            "void main()"
            "{"
            "color = varyingColor;"
            "}";





    if(glfwInit() == 0)
    {
        std::cerr<<"GLFW failed\n";
        return -1;
    }
    GLFWwindow* window;
    window = glfwCreateWindow(700, 700, "Hello OpenGL", nullptr, nullptr);

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

    std::vector<float> mass = {1, 0.05, 0.5};

    glm::vec3 gravity(0, -0.98, 0);

    float h = 0.0002;
    float time=0;
    int count2=0;
    int count3=0;

    int cont= 0;
    // OBJ
    //cambiar el path segun el file que se quiera leer

    //OBJFile obj("../lab2fin/cyber.obj");
    //OBJFile obj("/home/emilio/lab2fin/attic.obj");
    OBJFile obj("../cgvcm-tif/cyber.obj");
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



    //Valores de giro de la cámara
    float cameraX = 2.f;
    float cameraY = 0.f;
    float cameraZ = 7.f;

    glm::mat4 mMat, vMat, mvMat, pMat;
    float angle=0;

    //projection

    int width, height;

    //camera block

    glfwGetFramebufferSize(window, &width, &height);
    float aspect =static_cast<float>(width)/static_cast<float>(height);
    glm::mat4 projMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projMat");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMat));

    //light


    // material de oro
    //material
    float matAmbSilver[4] = {0.24725f,  0.1995f,   0.0745f, 1}; //RGB-A
    float matDifSilver[4] = {0.75164f,  0.60648f,  0.22648f, 1};
    float matSpeSilver[4] = {0.628281f, 0.555802f, 0.366065f, 1};
    float matShiSilver = 51.2f;

    //light source
    // white light properties
    float globalAmbient[4] = {0.7f, 0.7f, 0.7f, 1.0f };

    float lightAmbient[4]  = {0.0f, 0.0f, 0.0f, 1.0f };
    float lightDiffuse[4]  = {1.0f, 1.0f, 1.0f, 1.0f };
    float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };


    glm::vec3 currentLightPos, lightPosV; // light position as Vector3f, in both model and view space
    float lightPos[3];


    //camera block
    // Mantiene la ventana abierta, mientras el usuario no cierre la ventana
    while(!glfwWindowShouldClose(window))
    {


        glClearColor(0.0, 0.0, 0.2, 1.0);

        glClear(GL_COLOR_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //Tomando del codigo visto en clase para cada vertice, en este caso quiero
        //que se mueva todo el objeto junto con la gravedad para abajo
        //poreso solo calculo una velocidad y se la aplico a todos los vertices

        //bloqueo camara luz

        GLuint mvLoc2 = glGetUniformLocation(shaderProgram, "mvMat");
        //luz algo
        //movimiento luz

        glm::mat4 viewMat = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -7.f)); //cam/>for all objects (scene)
        auto modelMatPy = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -2.f, 0.f)); //obj->pyramid
        auto mvMat = viewMat*modelMatPy;
        glUniformMatrix4fv(mvLoc2, 1, GL_FALSE, glm::value_ptr(mvMat));
        //luces
        GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;
        globalAmbLoc = glGetUniformLocation(shaderProgram, "globalAmbient");
        ambLoc = glGetUniformLocation(shaderProgram, "light.ambient");
        diffLoc = glGetUniformLocation(shaderProgram, "light.diffuse");
        specLoc = glGetUniformLocation(shaderProgram, "light.specular");
        posLoc = glGetUniformLocation(shaderProgram, "light.position");
        mAmbLoc = glGetUniformLocation(shaderProgram, "material.ambient");
        mDiffLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
        mSpecLoc = glGetUniformLocation(shaderProgram, "material.specular");
        mShiLoc = glGetUniformLocation(shaderProgram, "material.shininess");
        GLuint nLoc = glGetUniformLocation(shaderProgram, "norm_matrix");

        currentLightPos = glm::vec3(5.0f, 5.0f, 2.0f);
        lightPosV = glm::vec3(viewMat * glm::vec4(currentLightPos, 1.0));
        lightPos[0] = lightPosV.x;
        lightPos[1] = lightPosV.y;
        lightPos[2] = lightPosV.z;
        glm::mat4 invTrMat = glm::transpose(glm::inverse(mvMat)); //model-view for normals
        glProgramUniform4fv(shaderProgram, globalAmbLoc, 1, globalAmbient);
        glProgramUniform4fv(shaderProgram, ambLoc, 1, lightAmbient);
        glProgramUniform4fv(shaderProgram, diffLoc, 1, lightDiffuse);
        glProgramUniform4fv(shaderProgram, specLoc, 1, lightSpecular);
        glProgramUniform3fv(shaderProgram, posLoc, 1, lightPos);
        glProgramUniform4fv(shaderProgram, mAmbLoc, 1, matAmbSilver);
        glProgramUniform4fv(shaderProgram, mDiffLoc, 1, matDifSilver);
        glProgramUniform4fv(shaderProgram, mSpecLoc, 1, matSpeSilver);
        glProgramUniform1f(shaderProgram, mShiLoc, matShiSilver);
        glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
//bloqueo camara luz
        /*

        //gravedades
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
        if (count2 < 6){
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
        else if (count2 >=6){
            count3++;

            for (int i=0;i<vertices.size();i++){

                    vec3 gravity = vec3(0, -0.98, 0)*dt;
                    vec3 spring = gravity*(vec3(0,0,0))*k*dt;
                    vec3 dampingForce = velocity*damping;

                    //Calculate velocity
                    vec3 acceleration = (gravity+spring-dampingForce)/mass[1];
                    velocity += acceleration;
                    vertices[i] -= velocity;
            }
            if (count3>=6){
                count2=0;
            }
        }
    //springs and dampers
        time+=h;
        cont++;
        //guardo frame
        //WriteFrame(cont,vertices);


        //Rotate camera

        //Obtener las matrices
        GLuint mvLoc    = glGetUniformLocation(shaderProgram, "mvMat");
        GLuint projLoc  = glGetUniformLocation(shaderProgram, "projMat");


        //Obtener la perspectiva actual
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = static_cast<float>(width)/static_cast<float>(height);
        pMat =  glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

        //Obtener matriz modelo-vista
        vMat = glm::translate(glm::mat4(1.f), glm::vec3(-cameraX, -cameraY, -cameraZ));

        //Rotar cámara
        glm::mat4 vRot  = glm::rotate(glm::mat4(1.f) , angle , glm::vec3(0, -1,0));
        vMat =  vMat*vRot;

        glm::mat4 mOrg = glm::translate(glm::mat4(1.f), glm::vec3(1.0f, -1.0f, 1.0f)); //offset
        glm::mat4 mBack = glm::translate(glm::mat4(1.f), glm::vec3(-1.0f, 1.0f, -1.0f)); //offset

        glm::mat4 mRot  = mBack*glm::rotate(mOrg, angle , glm::vec3(1  , 0, 0 ));

        //aumentar el angulo y volver a 0 si se llega a 360
        angle+=0.05;
        if(angle>360) angle=0;

        mMat = glm::translate(glm::mat4(1.f), glm::vec3(0.0, -2.0, 0.0))*mRot; //offset

        mvMat = vMat * mMat;

        glUniformMatrix4fv(mvLoc, 1, GL_FALSE,  glm::value_ptr(mvMat));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE,  glm::value_ptr(pMat));
        //Rotate camera




        glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(float)  , &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        //aca uso los vertices para dibujar sale perfecto pero no son realmente la conexion de aristas
        //pero realmente es la que mas funciona me funciono probando mas de 20 obj
        //descomentar la linea de debajo y comentar drawelements para visualizar
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        //aca uso las aristas, se entrelazan las aristas, como aun no muevo camara, es complicado ver algunos obj
        //pero aveces no funciona del todo correcto se bugea con euler y un vertice nunca cae
        //quise seguir probando pero el disco empezo a fallar asi que lo deje asi funciona pero no del todo

        //glDrawElements(GL_TRIANGLES,vector.size(),GL_UNSIGNED_INT, &edges[1]);

        drawWalls();
        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glDeleteBuffers(1, &vertexbuffer);
    glfwTerminate();

    return 0;
}


