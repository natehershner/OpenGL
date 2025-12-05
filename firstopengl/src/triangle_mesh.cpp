#include "triangle_mesh.h"
#include "glad/glad.h"
#include <vector>

TriangleMesh::TriangleMesh() {
    std::vector<float> positions = {
	// x, y, z
        -1.0f, -1.0f, 0.0f, //Bottom left
         1.0f, -1.0f, 0.0f, //Bottom right
        -1.0f, 1.0f, 0.0f,  //Top left
         1.0f, 1.0f, 0.0f   //Top right
    };

    std::vector<int> colorIndices = {0, 1, 2, 3};
    std::vector<int> elementIndices = {0, 1, 2, 2, 1, 3};
   
    //Resize for position and color
    VBOs.resize(2);

    element_count = 6;
    std::size_t bufferSize;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Add position vector to vertex buffer
    bufferSize = positions.size() * sizeof(float);

    glGenBuffers(1, &VBOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, positions.data(), GL_STATIC_DRAW);
    //attr num, num elems in attr, normalize data, stride in bytes, offset to beginning
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    //Add color vector to vertex buffer
    bufferSize = colorIndices.size() * sizeof(int);

    glGenBuffers(1, &VBOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, colorIndices.data(), GL_STATIC_DRAW);
    glVertexAttribIPointer(1, 1, GL_INT, sizeof(int), (void*) 0);
    glEnableVertexAttribArray(1);

    //Add element vector to element buffer
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    bufferSize = elementIndices.size() * sizeof(int);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, elementIndices.data(), GL_STATIC_DRAW);

}

void TriangleMesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, element_count, GL_UNSIGNED_INT, 0);
}

TriangleMesh::~TriangleMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(VBOs.size(), VBOs.data());
    glDeleteBuffers(1, &EBO);
}
