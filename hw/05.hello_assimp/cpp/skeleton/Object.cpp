#include "Object.h"

void Object::init_buffer_objects()
{
    // Generate and bind the Vertex Buffer Object (VBO) for positions
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);

    // Pass vertex data
    glBufferData(GL_ARRAY_BUFFER, pmesh_->mNumVertices * sizeof(aiVector3D), pmesh_->mVertices, GL_STATIC_DRAW);

    // Check if color data exists
    is_color = pmesh_->mColors[0] != NULL;
    if (is_color)
    {
        // Generate and bind the Vertex Buffer Object (VBO) for colors
        glGenBuffers(1, &color_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);

        // Pass color data
        glBufferData(GL_ARRAY_BUFFER, pmesh_->mNumVertices * sizeof(aiColor4D), pmesh_->mColors[0], GL_STATIC_DRAW);
    }

    // Generate the index buffers for faces
    faces.resize(pmesh_->mNumFaces);
    for (unsigned int i = 0; i < pmesh_->mNumFaces; ++i)
    {
        glGenBuffers(1, &(faces[i].index_buffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces[i].index_buffer);

        // Pass index data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, pmesh_->mFaces[i].mNumIndices * sizeof(unsigned int), pmesh_->mFaces[i].mIndices, GL_STATIC_DRAW);

        faces[i].num_indices = pmesh_->mFaces[i].mNumIndices;
    }
}

glm::mat4 Object::get_model_matrix() const
{
    glm::mat4 mat_model = glm::mat4(1.0f);

    // Apply transformation
    glm::mat4 translate = glm::translate(mat_model, vec_translate_);
    glm::mat4 rotate = translate * glm::mat4_cast(quat_rotate_);
    glm::mat4 scale = glm::scale(rotate, vec_scale_);

    return scale; // Return the resultant model matrix
}

void Object::draw(int loc_a_position, int loc_a_color)
{
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glEnableVertexAttribArray(loc_a_position);
    glVertexAttribPointer(loc_a_position, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if (is_color)
    {
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
        glEnableVertexAttribArray(loc_a_color);
        glVertexAttribPointer(loc_a_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // Draw each face
    for (auto &face : faces)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face.index_buffer);
        glDrawElements(GL_TRIANGLES, face.num_indices, GL_UNSIGNED_INT, 0);
    }
}

void Object::print_info()
{
    std::cout << "print mesh info" << std::endl;

    std::cout << "num vertices " << pmesh_->mNumVertices << std::endl;
    for (unsigned int i = 0; i < pmesh_->mNumVertices; ++i)
    {
        aiVector3D vertex = pmesh_->mVertices[i];
        std::cout << "  vertex  (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;

        if (pmesh_->mColors[0] != NULL)
        {
            aiColor4D color = pmesh_->mColors[0][i];
            std::cout << "  color  (" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")" << std::endl;
        }
    }
}