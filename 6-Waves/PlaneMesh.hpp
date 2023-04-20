// Include standard headers
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Include GLFW and GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
using namespace glm;


GLuint LoadShaders(const char * vertex_file_path, const char* fragment_file_path, const char* tesselation_control_path, const char* tesselation_evaluation_path, const char* geometry_path){

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint TesselationControlShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
    GLuint TesselationEvaluationShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
    GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
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
    }else{
        printf("Impossible to open %s. Are you in the right directory? Don't forget to read the FAQ !\n", fragment_file_path);
        getchar();
        return 0;
    }

    // Read the Tesselation Control Shader code from the file
    std::string TesselationControlShaderCode;
    std::ifstream TesselationControlStream(tesselation_control_path, std::ios::in);
    if(TesselationControlStream.is_open()){
        std::stringstream sstr;
        sstr << TesselationControlStream.rdbuf();
        TesselationControlShaderCode = sstr.str();
        TesselationControlStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory? Don't forget to read the FAQ !\n", tesselation_control_path);
        getchar();
        return 0;
    }

    // Read the Tesselation Evaluation Shader code from the file
    std::string TesselationEvaluationShaderCode;
    std::ifstream TesselationEvaluationStream(tesselation_evaluation_path, std::ios::in);
    if(TesselationEvaluationStream.is_open()){
        std::stringstream sstr;
        sstr << TesselationEvaluationStream.rdbuf();
        TesselationEvaluationShaderCode = sstr.str();
        TesselationEvaluationStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory? Don't forget to read the FAQ !\n", tesselation_evaluation_path);
        getchar();
        return 0;
    }

    // Read the Geometry Shader code from the file
    std::string GeometryShaderCode;
    std::ifstream GeometryStream(geometry_path, std::ios::in);
    if(GeometryStream.is_open()){
        std::stringstream sstr;
        sstr << GeometryStream.rdbuf();
        GeometryShaderCode = sstr.str();
        GeometryStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory? Don't forget to read the FAQ !\n", geometry_path);
        getchar();
        return 0;
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

    // Compile Tesselation Control Shader
    printf("Compiling shader : %s\n", tesselation_control_path);
    char const * TesselationControlSourcePointer = TesselationControlShaderCode.c_str();
    glShaderSource(TesselationControlShaderID, 1, &TesselationControlSourcePointer , NULL);
    glCompileShader(TesselationControlShaderID);
    // Check Fragment Shader
    glGetShaderiv(TesselationControlShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(TesselationControlShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> TesselationControlShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(TesselationControlShaderID, InfoLogLength, NULL, &TesselationControlShaderErrorMessage[0]);
        printf("%s\n", &TesselationControlShaderErrorMessage[0]);
    }

    // Compile Tesselation Evaluation Shader
    printf("Compiling shader : %s\n", tesselation_evaluation_path);
    char const * TesselationEvaluationSourcePointer = TesselationEvaluationShaderCode.c_str();
    glShaderSource(TesselationEvaluationShaderID, 1, &TesselationEvaluationSourcePointer , NULL);
    glCompileShader(TesselationEvaluationShaderID);
    // Check Tesselation Evaluation Shader
    glGetShaderiv(TesselationEvaluationShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(TesselationEvaluationShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> TesselationEvaluationShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(TesselationEvaluationShaderID, InfoLogLength, NULL, &TesselationEvaluationShaderErrorMessage[0]);
        printf("%s\n", &TesselationEvaluationShaderErrorMessage[0]);
    }

    // Compile Geometry Shader
    printf("Compiling shader : %s\n", geometry_path);
    char const * GeometrySourcePointer = GeometryShaderCode.c_str();
    glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer , NULL);
    glCompileShader(GeometryShaderID);
    // Check Geometry Shader
    glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> GeometryShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
        printf("%s\n", &GeometryShaderErrorMessage[0]);
    }


    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, TesselationControlShaderID);
    glAttachShader(ProgramID, TesselationEvaluationShaderID);
    glAttachShader(ProgramID, GeometryShaderID);
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
    glDetachShader(ProgramID, TesselationControlShaderID);
    glDetachShader(ProgramID, TesselationEvaluationShaderID);
    glDetachShader(ProgramID, GeometryShaderID);
    glDetachShader(ProgramID, FragmentShaderID);


    glDeleteShader(VertexShaderID);
    glDeleteShader(TesselationControlShaderID);
    glDeleteShader(TesselationEvaluationShaderID);
    glDeleteShader(GeometryShaderID);
    glDeleteShader(FragmentShaderID);


    return ProgramID;
}

void loadBMP(const char* image_path, unsigned char** data, unsigned int* width, unsigned int* height) {

    // printf("Reading image %s\n", imagepath);

    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    // Actual RGB data

    // Open the file
    FILE * file = fopen(image_path,"rb");
    if (!file){
        printf("%s could not be opened. Are you in the right directory?\n", image_path);
        getchar();
        return;
    }

    // Read the header, i.e. the 54 first bytes

    // If less than 54 bytes are read, problem
    if ( fread(header, 1, 54, file)!=54 ){
        printf("Not a correct BMP file\n");
        fclose(file);
        return;
    }
    // A BMP files always begins with "BM"
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        fclose(file);
        return;
    }
    // Make sure this is a 24bpp file
    if ( *(int*)&(header[0x1E])!=0  ) {
    	printf("Not a correct BMP file\n");
	    fclose(file);
	    return;
	}
    if ( *(int*)&(header[0x1C])!=24 ) {
    	printf("Not a correct BMP file\n");
	    fclose(file);
	    return;
	}

    // Read the information about the image
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    *width      = *(int*)&(header[0x12]);
    *height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=(*width)* (*height)*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    *data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(*data,1,imageSize,file);

    fprintf(stderr, "Done reading!\n");

    // Everything is in memory now, the file can be closed.
    fclose (file);

}


class PlaneMesh {

private:

	std::vector<float> verts;
	std::vector<float> normals;
	std::vector<int> indices;

    GLuint VAO;
	GLuint positionsVBO;
    GLuint normalsVBO;
	GLuint EBO;

	GLuint tex0ID;
	GLuint tex1ID;
	GLuint programID;


    void planeMeshQuads(float min, float max, float stepsize) {

        // The following coordinate system works as if (min, 0, min) is the origin
        // And then builds up the mesh from that origin down (in z)
        // and then to the right (in x).
        // So, one "row" of the mesh's vertices have a fixed x and increasing z

        //manually create a first column of vertices
        float x = min;
        float y = 0;
        for (float z = min; z <= max; z += stepsize) {
            verts.push_back(x);
            verts.push_back(y);
            verts.push_back(z);
            normals.push_back(0);
            normals.push_back(1);
            normals.push_back(0);
        }

        for (float x = min+stepsize; x <= max; x += stepsize) {
            for (float z = min; z <= max; z += stepsize) {
                verts.push_back(x);
                verts.push_back(y);
                verts.push_back(z);
                normals.push_back(0);
                normals.push_back(1);
                normals.push_back(0);
            }
        }

        int nCols = (max-min)/stepsize + 1;
        int i = 0, j = 0;
        for (float x = min; x < max; x += stepsize) {
            j = 0;
            for (float z = min; z < max; z += stepsize) {
                indices.push_back(i*nCols + j);
                indices.push_back(i*nCols + j + 1);
                indices.push_back((i+1)*nCols + j + 1);
                indices.push_back((i+1)*nCols + j);
                ++j;
            }
            ++i;
        }
    }


public:

    PlaneMesh(float min, float max, float stepsize) {

//      Generate quads
        planeMeshQuads(min, max, stepsize);

        glm::vec4 modelColor = glm::vec4(0, 1.0f, 1.0f, 1.0f);
        // Water texture
		unsigned char* waterData;
		GLuint width, height;
        std::string textureFile0 = "./Assets/water.bmp";
		loadBMP(textureFile0.c_str(), &waterData, &width, &height);

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &tex0ID);
        glBindTexture(GL_TEXTURE_2D, tex0ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, waterData);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Displacement texture
        unsigned char* displacementData;
        GLuint width1, height1;
        std::string textureFile1 = "Assets/displacement-map1.bmp";
		loadBMP(textureFile1.c_str(), &displacementData, &width1, &height1);

        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &tex1ID);
        glBindTexture(GL_TEXTURE_2D, tex1ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width1, height1, 0, GL_RED, GL_UNSIGNED_BYTE, displacementData);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        programID = LoadShaders(
                "./Shaders/WaterShader.vert",
                "./Shaders/WaterShader.frag",
                "./Shaders/WaterShader.tesc",
                "./Shaders/WaterShader.tese",
                "./Shaders/WaterShader.geom"
        );


        //gen and fill buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &positionsVBO);
        glGenBuffers(1, &normalsVBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // set vertex positions
        glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                0,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                        // array buffer offset
        );

        // set vertex normals
        glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
        );

//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

//        glBindVertexArray(0);

	}

	void draw(glm::vec3 lightPos, glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec4 color, float time) {

        glm::mat4 MVP = P * V * M;

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex0ID);
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex1ID);

        glUseProgram(programID);


//      Generate uniforms
        // Textures
        glUniform1i(glGetUniformLocation(programID, "waterTexture"), 0);
        glUniform1i(glGetUniformLocation(programID, "displacementTexture"), 1);

        glUniform1f(glGetUniformLocation(programID, "outerTess"), 10);
        glUniform1f(glGetUniformLocation(programID, "innerTess"), 10);

        glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &M[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

        glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
        glUniform1f(glGetUniformLocation(programID, "time"), time);
        glUniform1f(glGetUniformLocation(programID, "alpha"), 3);
        glUniform4f(glGetUniformLocation(programID, "modelcolor"), color.x, color.y, color.z, color.w);

        glUniform2f(glGetUniformLocation(programID, "texOffset"), 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(programID, "texScale"), 3.6f);


        glBindVertexArray(VAO);

        glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, (void*)0);

//        Unbind texture
//		glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
		glUseProgram(0);


	}


};
