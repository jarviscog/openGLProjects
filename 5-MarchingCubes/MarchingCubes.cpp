// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <iomanip>


// Include GLFW and GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow* window;


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>
#include "TriTable.hpp"
//using namespace glm;

// Make definitions for marching cubes
#define BOTTOM_LEFT_FRONT  1
#define BOTTOM_RIGHT_FRONT 2
#define BOTTOM_RIGHT_BACK 4
#define BOTTOM_LEFT_BACK  8

#define TOP_LEFT_FRONT     16
#define TOP_RIGHT_FRONT    32
#define TOP_RIGHT_BACK    64
#define TOP_LEFT_BACK     128


//////////////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////////////

// Variables for camera
float last_mouse_x_position = 0;
float last_mouse_y_position = 0;
float x_angle = 0;
float y_angle = 45;
float radius = 11.18033f;
float eye_x, eye_y, eye_z;

// Variables for mesh
std::vector<GLfloat> mesh_verts;
std::vector<unsigned int> mesh_indices;
std::vector<GLfloat> mesh_normals;

bool multithread = true;
std::mutex m;

int marching_x = 0;
int marching_y = 0;
int marching_z = 0;

// Open a window and create its OpenGL context
float screen_width = 1400;
float screen_height = 900;


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }



    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }


    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

//////////////////////////////////////////////////////////////////////////////
// Helper Objects
//////////////////////////////////////////////////////////////////////////////

class Axes {

    glm::vec3 origin;
    glm::vec3 extents;

    glm::vec3 xcol = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 ycol = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 zcol = glm::vec3(0.0f, 0.0f, 1.0f);

public:

    Axes(glm::vec3 orig, glm::vec3 ex) : origin(orig), extents(ex) {}

    void draw() {

        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();


        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glColor3f(xcol.x, xcol.y, xcol.z);
        glVertex3f(origin.x, origin.y, origin.z);
        glVertex3f(origin.x + extents.x, origin.y, origin.z);

        glVertex3f(origin.x + extents.x, origin.y, origin.z);
        glVertex3f(origin.x + extents.x, origin.y, origin.z+0.1);
        glVertex3f(origin.x + extents.x, origin.y, origin.z);
        glVertex3f(origin.x + extents.x, origin.y, origin.z-0.1);

        glColor3f(ycol.x, ycol.y, ycol.z);
        glVertex3f(origin.x, origin.y, origin.z);
        glVertex3f(origin.x, origin.y + extents.y, origin.z);

        glVertex3f(origin.x, origin.y + extents.y, origin.z);
        glVertex3f(origin.x, origin.y + extents.y, origin.z+0.1);
        glVertex3f(origin.x, origin.y + extents.y, origin.z);
        glVertex3f(origin.x, origin.y + extents.y, origin.z-0.1);

        glColor3f(zcol.x, zcol.y, zcol.z);
        glVertex3f(origin.x, origin.y, origin.z);
        glVertex3f(origin.x, origin.y, origin.z + extents.z);

        glVertex3f(origin.x, origin.y, origin.z + extents.z);
        glVertex3f(origin.x+0.1, origin.y, origin.z + extents.z);

        glVertex3f(origin.x, origin.y, origin.z + extents.z);
        glVertex3f(origin.x-0.1, origin.y, origin.z + extents.z);
        glEnd();


        glPopMatrix();
    }

};

class Mesh{

private:

    std::string image_path;
    std::string ply_path;

    std::vector<float> verts;
    std::vector<unsigned int> indices;
    std::vector<float> normals;

    // For shaders
    GLuint ProgramID;
    GLuint MatrixID;


    GLuint MVPID;
    GLuint MID;
    GLuint VID;
    GLuint LightPosID;
    GLuint colorID;
    GLuint alphaID;

public:

    ~Mesh(){
        glDeleteProgram(ProgramID);
    }

    Mesh(std::vector<float> in_verts, std::vector<unsigned int> in_indices, std::vector<float> in_normals){

        verts = in_verts;
        indices = in_indices;
        normals = in_normals;

//        ProgramID = LoadShaders( "PhongTexture.vertexshader", "PhongTexture.fragmentshader" );
        ProgramID = LoadShaders( "DiffuseShader.vertexshader", "DiffuseShader.fragmentshader" );

        // Get a handle for our "MVP" uniform
        MVPID = glGetUniformLocation(ProgramID, "MVP");
        MID = glGetUniformLocation(ProgramID, "M");
        VID = glGetUniformLocation(ProgramID, "V");
        LightPosID = glGetUniformLocation(ProgramID, "LightPosition_worldspace");
        colorID = glGetUniformLocation(ProgramID, "modelcolor");
        alphaID = glGetUniformLocation(ProgramID, "alpha");

        glUseProgram(ProgramID);
        glm::mat4 M(1.0f);
        glUniformMatrix4fv(MID, 1, GL_FALSE, &M[0][0]); //model matrix always identity.

    }

    void draw(glm::vec3 lightPos, glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec4 color, float alpha) const {

        glm::mat4 MVP = P*V*M;
        glUseProgram(ProgramID);

//        Generate uniforms
        glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(VID, 1, GL_FALSE, &V[0][0]);
        glUniform3f(LightPosID, lightPos.x, lightPos.y, lightPos.z);
        glUniform4fv(colorID, 1, &color[0]);
        glUniform1f(alphaID, alpha);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                verts.data()            // array buffer offset
        );

        // 2nd attribute buffer : normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                3,                                // size
                GL_FLOAT,                         // type
                GL_TRUE,                         // normalized?
                0,                                // stride
                normals.data()            // array buffer offset
        );

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        glUseProgram(0);
    }

};

void draw_lines(float min[3], float max[3]){


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glLineWidth(5.0f);
    glBegin(GL_LINES);

    glVertex3f(min[0], min[1], min[2]);
    glVertex3f(max[0], min[1], min[2]);

    glVertex3f(min[0], min[1], min[2]);
    glVertex3f(min[0], max[1], min[2]);

    glVertex3f(min[0], min[1], min[2]);
    glVertex3f(min[0], min[1], max[2]);


    glVertex3f(min[0], max[1], min[2]);
    glVertex3f(max[0], max[1], min[2]);

    glVertex3f(max[0], min[1], min[2]);
    glVertex3f(max[0], max[1], min[2]);

    glVertex3f(max[0], min[1], min[2]);
    glVertex3f(max[0], min[1], max[2]);

    glVertex3f(min[0], min[1], max[2]);
    glVertex3f(max[0], min[1], max[2]);

    glVertex3f(min[0], min[1], max[2]);
    glVertex3f(min[0], max[1], max[2]);

    glVertex3f(min[0], max[1], min[2]);
    glVertex3f(min[0], max[1], max[2]);


    glVertex3f(min[0], max[1], max[2]);
    glVertex3f(max[0], max[1], max[2]);

    glVertex3f(max[0], min[1], max[2]);
    glVertex3f(max[0], max[1], max[2]);

    glVertex3f(max[0], max[1], min[2]);
    glVertex3f(max[0], max[1], max[2]);


    glEnd();


    glPopMatrix();
    glDisable(GL_BLEND);

}

//////////////////////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////////////////////
static void cursor_position_callback(GLFWwindow* callback_window, double xpos, double ypos)
{
//    std::cout << glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) << std::endl;
//    If the mouse is not clicked, don't move the camera
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        return;
    }
    glfwGetCursorPos(window, &xpos, &ypos);

    float delta_x;
    float delta_y;

    if(last_mouse_x_position != 0){
        delta_x = last_mouse_x_position - xpos;

    }else{
        delta_x = xpos;
    }
    if(last_mouse_y_position != 0){
        delta_y = last_mouse_y_position - ypos;
    }else{
        delta_y = ypos;
    }

//    Disregard big jumpy movements
    if(delta_x > 60){
        delta_x = 0;
    }
    if(delta_x < -60){
        delta_x = 0;
    }
    if(delta_y > 60){
        delta_y = 0;
    }
    if(delta_y < -60){
        delta_y = 0;
    }

    x_angle += delta_x/3;
    y_angle += delta_y/3;

    if(y_angle < 5){
        y_angle = 5;
    }else if (y_angle > 175){
        y_angle = 175;
    }
    last_mouse_x_position = xpos;
    last_mouse_y_position = ypos;

}

int init_opengl(){

    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
//	 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//	 glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
//	 glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context

    window = glfwCreateWindow( screen_width, screen_height, "Marching Cubes", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    // Dark blue background
    glClearColor(0.2f, 0.2f, 0.3f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Marching cubes
//////////////////////////////////////////////////////////////////////////////
typedef float (*scalar_field_3d)(float, float, float);

float sphere(float x, float y, float z) {
    return (float)x*x + (float)y*y + (float)z*z;
}

float wave(float x, float y, float z) {
    return y - glm::sin(x)*cos(z);
}

float cone(float x, float y, float z) {
    return x*x - y*y - z*z - z;
}

float shape1(float x, float y, float z){
    return glm::pow(10, -(z*( x + y))) + glm::pow(10, -(-z *(x + y))) + glm::pow(10, -(z* (y + z)));
}

float ripple(float x, float y, float z) {
    return glm::sin(5*(x*x+z*z))/3 - y;
}

std::vector<float> marching_cubes(scalar_field_3d f, float isoval, float min_value[3], float max_value[3], float stepsize[3]) {

    int x_iterations = floor((max_value[0] - min_value[0]) / stepsize[0]);
    int y_iterations = floor((max_value[1] - min_value[1]) / stepsize[1]);
    int z_iterations = floor((max_value[2] - min_value[2]) / stepsize[2]);
    int total_iterations = x_iterations * y_iterations * z_iterations;
//    printf("X: %d\nY: %d\nZ: %d\n", x_iterations, y_iterations, z_iterations);

//    Yes, this is crazy inefficient, but I went for the extra marks, not beautiful code.
//    Looping three times like this looks cool on the screen

//    Points

    if(marching_x == x_iterations && marching_y == y_iterations && marching_z == z_iterations){
        return mesh_verts;
    }

    float tlf, trf, brf, blf;
    float tlb, trb, brb, blb;
    float current_x, current_y, current_z;
    float x_offset, y_offset, z_offset;
    unsigned int num_of_tris_processed = 0;
    unsigned int which = 0;
    unsigned int index_of_vert;
    int* verts;
    for(int i = marching_x; i <= x_iterations; i++){
        for(int j = marching_y; j <= y_iterations; j++){
            for(int k = marching_z; k <= z_iterations; k++){

                current_x = min_value[0] + (float)i * stepsize[0];
                current_y = min_value[1] + (float)j * stepsize[1];
                current_z = min_value[2] + (float)k * stepsize[2];

                tlf = (*f)(current_x, current_y+stepsize[1], current_z);
                trf = (*f)(current_x+stepsize[0], current_y+stepsize[1], current_z);
                brf = (*f)(current_x+stepsize[0], current_y, current_z);
                blf = (*f)(current_x, current_y, current_z);
                tlb = (*f)(current_x, current_y+stepsize[1], current_z+stepsize[2]);
                trb = (*f)(current_x+stepsize[0], current_y+stepsize[1], current_z+stepsize[2]);
                brb = (*f)(current_x+stepsize[0], current_y, current_z+stepsize[2]);
                blb = (*f)(current_x, current_y, current_z+stepsize[2]);

                which = 0;
                if (tlf < isoval) {
                    which |= TOP_LEFT_FRONT;
                }
                if (trf < isoval) {
                    which |= TOP_RIGHT_FRONT;
                }
                if (brf < isoval) {
                    which |= BOTTOM_RIGHT_FRONT;
                }
                if (blf < isoval) {
                    which |= BOTTOM_LEFT_FRONT;
                }
                if (tlb < isoval) {
                    which |= TOP_LEFT_BACK;
                }
                if (trb < isoval) {
                    which |= TOP_RIGHT_BACK;
                }
                if (brb < isoval) {
                    which |= BOTTOM_RIGHT_BACK;
                }
                if (blb < isoval) {
                    which |= BOTTOM_LEFT_BACK;
                }

                verts = marching_cubes_lut[which];

                int current_vert = verts[0];
                int number_of_verts = 0;
                while(current_vert != -1){
                    number_of_verts +=1;
                    current_vert = verts[number_of_verts];
                }

                for(int q = 0; q < number_of_verts; q++){
                        if(verts[q] != -1){

                            x_offset = stepsize[0] * vertTable[verts[q]][0];
                            y_offset = stepsize[1] * vertTable[verts[q]][1];
                            z_offset = stepsize[2] * vertTable[verts[q]][2];

                            index_of_vert = (unsigned int)mesh_verts.size();
                            mesh_verts.push_back(current_x + x_offset);
                            mesh_verts.push_back(current_y + y_offset);
                            mesh_verts.push_back(current_z + z_offset);

                            mesh_indices.push_back(glm::floor(index_of_vert/3));
                            num_of_tris_processed += 1;

//                            printf("%d\n", mesh_indices.size());

                        }

                    }


            }
        }

    }

    return mesh_verts;
}

std::vector<float> compute_normals(std::vector<float> vertices){

    std::vector<float> normals;

    for(int i = 0; i < vertices.size(); i+=9){

        auto Ax = vertices.at(i);
        auto Ay = vertices.at(i+1);
        auto Az = vertices.at(i+2);

        auto Bx = vertices.at(i+3);
        auto By = vertices.at(i+4);
        auto Bz = vertices.at(i+5);

        auto Cx = vertices.at(i+6);
        auto Cy = vertices.at(i+7);
        auto Cz = vertices.at(i+8);

        glm::vec3 A{Ax, Ay, Az};
        glm::vec3 B{Bx, By, Bz};
        glm::vec3 C{Cx, Cy, Cz};

        glm::vec3 normal = glm::normalize(glm::cross(B - A, C - A));

        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);
        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);
        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);
    }

    return normals;


}

void writePLY(std::vector<float> in_verts, std::vector<unsigned int> in_indices, std::vector<float> in_color){

    std::ofstream outfile;
    outfile.open("example.ply");

    outfile << "ply\n";
    outfile << "format ascii 1.0\n";
    outfile << "comment Automatically created by MarchingCubes.cpp \n";
    outfile << "element vertex "<< mesh_verts.size()/3 << "\n";
    outfile << "property float x\n";
    outfile << "property float y\n";
    outfile << "property float z\n";
    outfile << "element face "<< mesh_indices.size() << "\n";
    outfile << "property list uchar uint vertex_indices\n";
    outfile << "end_header\n";

//  Write vertices
    for(int i = 0; i < mesh_verts.size(); i++){

        char s[25];
        sprintf(s, "%06f", mesh_verts.at(i));
        outfile << s << " ";
        if((i+1) % 3 == 0){
            outfile << "\n";
        }
    }
//  Write faces
    for(int j = 0; j < mesh_indices.size(); j++){
        if(j % 3 == 0){
            outfile << "3 ";
        }
//        printf("I: %d\n", mesh_indices.at(j));
        outfile << mesh_indices.at(j) << " ";
        if((j+1) % 3 == 0){
            outfile << "\n";
        }
    }
    outfile.close();

}

//////////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[]) {

    printf("\n");
    if(init_opengl() != 0){
        return -1;
    }

    glm::mat4 M = glm::mat4(1.0f);
    glm::mat4 V;
	glm::mat4 MVP;
    glm::vec3 eye;
    glm::mat4 P = glm::perspective(glm::radians(45.0f), screen_width/screen_height, 0.001f, 1000.0f);

    glm::mat4 trans = glm::mat4(1.0f);
    glm::vec3 up = {0.0f, 1.0f, 0.0f};
    glm::vec3 center = {0.0f, 0.0f, 0.0f};
    glm::vec3 light_pos = {5.0f, 5.0f, 5.0f};
    glm::vec4 light_color = {1.0f, 0.5f, 1.0f, 0.9f};

////    Wave preset
//    scalar_field_3d function = wave;
//    float isoval = 1.0f;
//    float xmin = -5.0f;
//    float xmax = 5.0f;
//    float ymin = -2.0f;
//    float ymax = 2.0f;
//    float stepval = 0.04;
//    float min[3] = {xmin, ymin, xmin};
//    float max[3] = {xmax, ymax, xmax};
//    float step[3] = {stepval, stepval, stepval};


//    Cone preset
    scalar_field_3d function = wave;
    float isoval = -1.5f;
    float xmin = -5.0f;
    float xmax = 5.0f;
    float ymin = -5.0f;
    float ymax = 5.0f;
    float stepval = 0.04;
    float min[3] = {xmin, ymin, xmin};
    float max[3] = {xmax, ymax, xmax};
    float step[3] = {stepval, stepval, stepval};

    marching_cubes(function, isoval, min, max, step);
    mesh_normals = compute_normals(mesh_verts);
    Axes ax(glm::vec3(min[0], min[1], min[2]), glm::vec3(4.0f, 4.0f, 4.0f));
    Mesh mesh = Mesh(mesh_verts, mesh_indices, mesh_normals);
    printf("\nVerts: %d\n", mesh_verts.size());
//    std::thread inc_thread(inc_index);

	do{

        // Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		glLoadMatrixf(glm::value_ptr(P));
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();

//        Do user input
        if(glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS && radius > 0.1){
            radius-=0.2;
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
            radius+=0.2;
        }

        float h_outset = glm::sin(glm::radians(y_angle));
        eye_x = glm::sin(glm::radians(x_angle))*radius * h_outset;

        eye_y = glm::cos(glm::radians(y_angle))*radius;
        eye_z = glm::cos(glm::radians(x_angle))*radius * h_outset;
        eye = {eye_x, eye_y, eye_z};

        V = glm::lookAt(eye, center, up);
		glLoadMatrixf(glm::value_ptr(V));
//        ax.draw();
        draw_lines(min, max);
//        ax.draw();
        mesh.draw(light_pos, M, V, P, light_color, 64.0f);

		// Swap buffers and poll
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

    writePLY(mesh_verts, mesh_indices, mesh_normals);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}