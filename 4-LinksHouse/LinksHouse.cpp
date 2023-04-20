// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <vector>
#include <iostream>


/**
 * Given a file path imagepath, read the data in that bitmapped image
 * and return the raw bytes of color in the data pointer.
 * The width and height of the image are returned in the weight and height pointers,
 * respectively.
 *
 * usage:
 *
 * unsigned char* imageData;
 * unsigned int width, height;
 * loadARGB_BMP("mytexture.bmp", &imageData, &width, &height);
 *
 * Modified from https://github.com/opengl-tutorials/ogl.
 */
void loadARGB_BMP(const char* imagepath, unsigned char** data, unsigned int* width, unsigned int* height) {

//    printf("Reading image %s\n", imagepath);

    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    // Actual RGBA data

    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file){
        printf("%s could not be opened. Are you in the right directory?\n", imagepath);
        getchar();
        return;
    }

    // Read the header, i.e. the 54 first bytes

    // If less than 54 bytes are read, problem
    if ( fread(header, 1, 54, file)!=54 ){
        printf("Not a correct BMP file1\n");
        fclose(file);
        return;
    }

    // Read the information about the image
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    *width      = *(int*)&(header[0x12]);
    *height     = *(int*)&(header[0x16]);
    // A BMP files always begins with "BM"
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file2\n");
        fclose(file);
        return;
    }
    // Make sure this is a 32bpp file
    if ( *(int*)&(header[0x1E])!=3  ) {
        printf("Not a correct BMP file3\n");
        fclose(file);
        return;
    }
    // fprintf(stderr, "header[0x1c]: %d\n", *(int*)&(header[0x1c]));
    // if ( *(int*)&(header[0x1C])!=32 ) {
    //     printf("Not a correct BMP file4\n");
    //     fclose(file);
    //     return;
    // }

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=(*width)* (*height)*4; // 4 : one byte for each Red, Green, Blue, Alpha component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    *data = new unsigned char [imageSize];

    if (dataPos != 54) {
        fread(header, 1, dataPos - 54, file);
    }

    // Read the actual data from the file into the buffer
    fread(*data,1,imageSize,file);

    // Everything is in memory now, the file can be closed.
    fclose (file);
}

//////////////////////////////////////////////////////////////////////////////
// Helper Objects
//////////////////////////////////////////////////////////////////////////////
struct VertexData{

    float x;
    float y;
    float z;

    float nx;
    float ny;
    float nz;

    float r = 0;
    float g = 0;
    float b = 0;

    float u = 0;
    float v = 0;

};

struct TriData{

    int v1;
    int v2;
    int v3;

};

void readPLYFile(std::string fname, std::vector<VertexData>& vertices, std::vector<TriData>& faces){

//    printf("Reading contents...\n");

    FILE *fptr;
    long length;
    char *buf;

    fptr = fopen(fname.c_str(), "rb"); /* Open file for reading */
    if (!fptr) { /* Return NULL on failure */
//        printf("Could not open file! %s\n", strerror(errno));
        return;
    }

    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */

//  Code from https://stackoverflow.com/questions/10560505/how-to-read-line-by-line-after-i-read-a-text-into-a-buffer

    int number_of_vertices = 0;
    int number_of_faces = 0;
    std::string delimiter = " ";
    size_t pos;

    int lineCount = 0;

    char * line = strtok(strdup(buf), "\n");
//    Read header
    for(int i = 0; i < 14; i++){
        lineCount+=1;
//        printf("%s\n", line);
        line = strtok(NULL, "\n");
        if (i == 2){
//            printf("%s\n", line);
            std::string l = line;
            std::string token;
            pos = l.find(delimiter);
            token = l.substr(0, pos);
            l.erase(0, pos + delimiter.length());
            pos = l.find(delimiter);
            token = l.substr(0, pos);
            l.erase(0, pos + delimiter.length());
            pos = l.find(delimiter);
            token = l.substr(0, pos);
            int y = std::stoi(token);
            number_of_vertices = y;
            l.erase(0, pos + delimiter.length());

        }

        if (i == 11){
//            printf("%s\n", line);
            std::string l = line;
            std::string token;
            pos = l.find(delimiter);
            token = l.substr(0, pos);
            l.erase(0, pos + delimiter.length());
            pos = l.find(delimiter);
            token = l.substr(0, pos);
            l.erase(0, pos + delimiter.length());
            pos = l.find(delimiter);
            token = l.substr(0, pos);
            int y = std::stoi(token);
            number_of_faces = y;
            l.erase(0, pos + delimiter.length());

        }
    }
//    For testing
//    printf("Verts: %d\n", number_of_vertices);
//    printf("Tris: %d\n", number_of_faces);

//    Iterate the verticies
    for(int i = 0; i < number_of_vertices; i++){

        lineCount+=1;
        line = strtok(NULL, "\n");
        VertexData new_vert;
        std::string l = line;
        std::string token;

//        pos = l.find(delimiter);
//        token = l.substr(0, pos);
//            printf("====\n%s\n====\n", token.c_str());
//        l.erase(0, pos + delimiter.length());

        pos = l.find(delimiter);
        token = l.substr(0, pos);
        float x = std::stof(token.c_str());
        l.erase(0, pos + delimiter.length());

        //printf("Here\n");
        pos = l.find(delimiter);
        token = l.substr(0, pos);
        float y = std::stof(token);
        l.erase(0, pos + delimiter.length());
        pos = l.find(delimiter);
        token = l.substr(0, pos);
        float z = std::stof(token);
        l.erase(0, pos + delimiter.length());

        new_vert.x = x;
        new_vert.y = y;
        new_vert.z = z;

        pos = l.find(delimiter);
        token = l.substr(0, pos);
        float nx = std::stof(token);
        l.erase(0, pos + delimiter.length());

        pos = l.find(delimiter);
        token = l.substr(0, pos);
        float ny = std::stof(token);
        l.erase(0, pos + delimiter.length());

        pos = l.find(delimiter);
        token = l.substr(0, pos);
        float nz = std::stof(token);
        l.erase(0, pos + delimiter.length());

        new_vert.nx = nx;
        new_vert.ny = ny;
        new_vert.nz = nz;


        pos = l.find(delimiter);
        token = l.substr(0, pos);
        float u = std::stof(token);
        l.erase(0, pos + delimiter.length());

        new_vert.u = u;

        pos = l.find(delimiter);
        token = l.substr(0, pos);
        float v = std::stof(token);
        l.erase(0, pos + delimiter.length());

        new_vert.v = v;

        vertices.push_back(new_vert);

    }
    line = strtok(NULL, "\n");

//    printf("Almost Done reading contents: %d!\n", lineCount);
//    Iterate the indices
    for(int i = 0; i < number_of_faces; i++){
        lineCount +=1;
//        printf("Line %d: %s\n", lineCount, line);

        TriData new_tri;
//        Get all three indices
        std::string l = line;
        std::string token;

        pos = l.find(delimiter);
        l.erase(0, pos + delimiter.length());

        pos = l.find(delimiter);
        token = l.substr(0, pos);
        int one = std::stof(token);
        l.erase(0, pos + delimiter.length());

        pos = l.find(delimiter);
        token = l.substr(0, pos);
        int two = std::stof(token);
        l.erase(0, pos + delimiter.length());

        pos = l.find(delimiter);
        token = l.substr(0, pos);
        int three = std::stof(token);
        l.erase(0, pos + delimiter.length());

        new_tri.v1 = one;
        new_tri.v2 = two;
        new_tri.v3 = three;
//        printf("Values stored: %d %d %d\n", one, two, three);
        faces.push_back(new_tri);
        line = strtok(NULL, "\n");

    }

//    For testing
//    printf("Done reading contents: %d!\n", lineCount);

//    VertexData current = vertices.at(0);

//    for(int i = 0; i < verticies.size(); i++){
//
//        VertexData current = vertices.at(i);
//
//        printf("%d pos    - %f %f %f\n", i + 16, current.x, current.y, current.z);
//        printf("%d normal - : %f %f %f\n", i + 16, current.nx, current.ny, current.nz);
//        printf("%d uv     - %f %f\n", i + 16, current.u, current.v);
//
//    }

    size_t face_size = faces.size();
//    printf("SS: %zu", faces.size());
    for(std::size_t i = 0; i != face_size; ++i){
        TriData current = faces.at(i);
//        printf("%d Indexes - %d %d %d\n", i + 184, current.v1, current.v2, current.v3);
//        printf("%zu", i);
    }


    return;

}

class TexturedMesh{

private:

    std::string image_path;
    std::string ply_path;

    GLuint vertex_id;
    GLuint texture_id;
    GLuint vertex_indices_id;

    GLuint vao_id;
    GLuint program_id;

    GLuint matrix_id;

    std::vector<VertexData> verts;
    std::vector<TriData> tris;


public:

    TexturedMesh(std::string ply_path, std::string image_path){

//        printf("Creating texture...\n");
//        printf("Reading PLY...\n");

        readPLYFile(ply_path, verts, tris);

        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        std::string VertexShaderCode = "\
    	#version 330 core\n\
		layout(location = 0) in vec3 vertexPosition;\n\
		layout(location = 1) in vec2 uv;\n\
		out vec2 uv_out;\n\
		uniform mat4 MVP;\n\
		void main(){ \n\
			gl_Position =  MVP * vec4(vertexPosition,1);\n\
			uv_out = uv;\n\
		}\n";
        // Read the Fragment Shader code from the string
        std::string FragmentShaderCode = "\
		#version 330 core\n\
		in vec2 uv_out; \n\
		uniform sampler2D tex;\n\
		void main() {\n\
            vec4 texColor = texture(tex, uv_out);\
            if(texColor.a < 0.2)\n\
                discard;\n\
            gl_FragColor = texColor;\n\
			//gl_FragColor = texture(tex, uv_out);\n\
		}\n";
        char const * VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
        glCompileShader(VertexShaderID);

        // Compile Fragment Shader
        char const * FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
        glCompileShader(FragmentShaderID);

        program_id = glCreateProgram();
        glAttachShader(program_id, VertexShaderID);
        glAttachShader(program_id, FragmentShaderID);

        glLinkProgram(program_id);

        glDetachShader(program_id, VertexShaderID);
        glDetachShader(program_id, FragmentShaderID);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);
        //End of shader generation

        unsigned char* image_data;
        GLuint width, height;
        loadARGB_BMP(image_path.c_str(), &image_data, &width, &height);

        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

//      VAO INIT
        glGenVertexArrays(1, &vao_id);
        glBindVertexArray(vao_id);
        glGenBuffers(1, &vertex_id);
        glGenBuffers(1, &matrix_id);
        glGenBuffers(1, &vertex_indices_id);


        // 1st attribute buffer : vertices
        glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * verts.size(), verts.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) 0);

        // Faces
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_indices_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TriData) * tris.size(), tris.data(), GL_STATIC_DRAW);

        // 2nd attribute buffer : UV
        glBindBuffer(GL_ARRAY_BUFFER, matrix_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * verts.size(), verts.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)(offsetof(VertexData, u)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    void draw(glm::mat4 &MVP){

        GLuint m = glGetUniformLocation(program_id, "MVP");
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        glUseProgram(program_id);
        glUniformMatrix4fv(m, 1, GL_FALSE, value_ptr(MVP));

        glBindVertexArray(vao_id);

        glDrawElements(GL_TRIANGLES, tris.size() * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);


        glDisable(GL_TEXTURE_2D);
    }


};


//////////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[])
{
    
    ///////////////////////////////////////////////////////
    // INIT
    ///////////////////////////////////////////////////////


    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    float screenW = 1400;
    float screenH = 900;
    window = glfwCreateWindow( screenW, screenH, "Links House", NULL, NULL);
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
//    Set information about opengl
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.0,0.0,0.0,0.0);

//    Load all the assets into the scene
    std::string asset_directory = "LinksHouse/";
    TexturedMesh doorbg(asset_directory + "DoorBG.ply", asset_directory + "doorbg.bmp");
    TexturedMesh window_bg(asset_directory + "WindowBG.ply", asset_directory + "windowbg.bmp");
    TexturedMesh patio(asset_directory + "Patio.ply", asset_directory + "patio.bmp");
    TexturedMesh floor(asset_directory + "Floor.ply", asset_directory + "floor.bmp");
    TexturedMesh bottles(asset_directory + "Bottles.ply", asset_directory + "bottles.bmp");
    TexturedMesh curtians(asset_directory + "Curtains.ply", asset_directory + "curtains.bmp");
    TexturedMesh metal(asset_directory + "MetalObjects.ply", asset_directory + "metalobjects.bmp");
    TexturedMesh table(asset_directory + "Table.ply", asset_directory + "table.bmp");
    TexturedMesh walls(asset_directory + "Walls.ply", asset_directory + "walls.bmp");
    TexturedMesh wood(asset_directory + "WoodObjects.ply", asset_directory + "woodobjects.bmp");

//    Init all of our view variables
    glm::vec3 eye = {0.4f, 4.0f, 5.0f};
    float current_rotation_angle = 0;
    float x = 0.5;
    float y = 0.4;
    float z = 0.5;

//    Some of these are just used for testing
    glm::vec3 up = {0.0f, 1.0f, 0.0f};
    glm::vec3 center = {0.0f, 0.0f, 0.0f};
    glm::vec3 focal = {0.0f, 0.0f, -1.0f};

    glm::mat4 MVP;
//    Not actually sure if I need to enable this
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), screenW/screenH, 0.001f, 1000.0f);

    do{

//        Do user input stuffs
        if(glfwGetKey(window, GLFW_KEY_LEFT ) != GLFW_PRESS) {
            current_rotation_angle += 5;
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT ) != GLFW_PRESS){
            current_rotation_angle-=5;
        }
        if(glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS){
            x = eye.x + focal.x/20;
            z = eye.z + focal.z/20;
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
            x = eye.x - focal.x/20;
            z = eye.z - focal.z/20;
        }

//        This code is just for fun/testing
        if(glfwGetKey(window, GLFW_KEY_LEFT_BRACKET ) == GLFW_PRESS){
//            Go up
            y = eye.y + 0.02;
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET ) == GLFW_PRESS){
//            Go down
            y = eye.y - 0.02;
        }
        if(glfwGetKey(window, GLFW_KEY_H ) == GLFW_PRESS){
//            Go home
            eye = {0.4f, 4.0f, 5.0f};
            focal = {0.0f, 0.0f, -1.0f};
            current_rotation_angle = 0;
            x = 0.5;
            y = 0.4;
            z = 0.5;

        }

//        Manipulate our MVP based on the user input
//        Note dividing by 3 gives us 3 degrees per frame
        focal.x = glm::cos(current_rotation_angle/90);
        focal.z = glm::sin(current_rotation_angle/90);


        glLoadMatrixf(glm::value_ptr(Projection));
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();

        eye = {x, y, z};
        glm::mat4 V = glm::lookAt(eye, eye+focal, up);

//        Just for testing + fun
        if(glfwGetKey(window, GLFW_KEY_C ) == GLFW_PRESS){
            V = glm::lookAt(eye, center, up);
        }

        glm::mat4 M = glm::mat4(1.0f);
//        glm::mat4 MV = V * M;
//        V = glm::rotate(V,glm::radians(current_rotation_angle),glm::vec3(0.0f, 1.0f, 0.0f));
        MVP = Projection * V * M;

        glLoadMatrixf(glm::value_ptr(V));

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        Draw all of the assets
        bottles.draw(MVP);
        curtians.draw(MVP);
        doorbg.draw(MVP);
        floor.draw(MVP);
        metal.draw(MVP);
        patio.draw(MVP);
        table.draw(MVP);
        walls.draw(MVP);
        window_bg.draw(MVP);
        wood.draw(MVP);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();


    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

