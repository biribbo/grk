const float box[] = {
    // Front face
    // Points                          Colors
    0.25f,  0.25f,  0.25f, 1.0f,       1.0f, 0.0f, 0.0f, 1.0f, // Top right
    0.25f, -0.25f,  0.25f, 1.0f,       0.0f, 1.0f, 0.0f, 1.0f, // Bottom right
   -0.25f,  0.25f,  0.25f, 1.0f,       0.0f, 0.0f, 1.0f, 1.0f, // Top left

    0.25f, -0.25f,  0.25f, 1.0f,       0.0f, 1.0f, 0.0f, 1.0f, // Bottom right
   -0.25f, -0.25f,  0.25f, 1.0f,       1.0f, 1.0f, 0.0f, 1.0f, // Bottom left
   -0.25f,  0.25f,  0.25f, 1.0f,       0.0f, 0.0f, 1.0f, 1.0f, // Top left

   // Back face
   0.25f,  0.25f, -0.25f, 1.0f,       1.0f, 0.0f, 1.0f, 1.0f, // Top right
  -0.25f,  0.25f, -0.25f, 1.0f,       0.5f, 0.5f, 1.0f, 1.0f, // Top left
   0.25f, -0.25f, -0.25f, 1.0f,       0.5f, 0.5f, 0.5f, 1.0f, // Bottom right

   0.25f, -0.25f, -0.25f, 1.0f,       0.5f, 0.5f, 0.5f, 1.0f, // Bottom right
  -0.25f,  0.25f, -0.25f, 1.0f,       0.5f, 0.5f, 1.0f, 1.0f, // Top left
  -0.25f, -0.25f, -0.25f, 1.0f,       1.0f, 0.5f, 0.0f, 1.0f, // Bottom left

  // Left face
 -0.25f,  0.25f,  0.25f, 1.0f,       0.2f, 0.0f, 0.2f, 1.0f, // Top right
 -0.25f, -0.25f,  0.25f, 1.0f,       0.2f, 0.0f, 0.2f, 1.0f, // Bottom right
 -0.25f, -0.25f, -0.25f, 1.0f,       0.0f, 1.0f, 0.0f, 1.0f, // Bottom left

 -0.25f,  0.25f,  0.25f, 1.0f,       0.2f, 0.0f, 0.2f, 1.0f, // Top right
 -0.25f, -0.25f, -0.25f, 1.0f,       0.0f, 1.0f, 0.0f, 1.0f, // Bottom left
 -0.25f,  0.25f, -0.25f, 1.0f,       0.0f, 1.0f, 0.0f, 1.0f, // Top left

 // Right face
 0.25f,  0.25f,  0.25f, 1.0f,       1.0f, 0.2f, 0.0f, 1.0f, // Top left
 0.25f, -0.25f, -0.25f, 1.0f,       0.5f, 0.5f, 0.0f, 1.0f, // Bottom right
 0.25f, -0.25f,  0.25f, 1.0f,       0.5f, 0.5f, 0.0f, 1.0f, // Bottom left

 0.25f,  0.25f,  0.25f, 1.0f,       1.0f, 0.2f, 0.0f, 1.0f, // Top left
 0.25f,  0.25f, -0.25f, 1.0f,       0.5f, 0.5f, 0.0f, 1.0f, // Top right
 0.25f, -0.25f, -0.25f, 1.0f,       0.5f, 0.5f, 0.0f, 1.0f, // Bottom right

 // Top face
 0.25f,  0.25f, -0.25f, 1.0f,       0.0f, 1.0f, 0.0f, 1.0f, // Top right
 0.25f,  0.25f,  0.25f, 1.0f,       0.0f, 1.0f, 0.0f, 1.0f, // Top left
-0.25f,  0.25f,  0.25f, 1.0f,       1.0f, 0.0f, 0.0f, 1.0f, // Bottom left

 0.25f,  0.25f, -0.25f, 1.0f,       0.0f, 1.0f, 0.0f, 1.0f, // Top right
-0.25f,  0.25f,  0.25f, 1.0f,       1.0f, 0.0f, 0.0f, 1.0f, // Bottom left
-0.25f,  0.25f, -0.25f, 1.0f,       1.0f, 0.0f, 0.0f, 1.0f, // Bottom right

// Bottom face
0.25f, -0.25f, -0.25f, 1.0f,       1.0f, 1.0f, 1.0f, 1.0f, // Top right
-0.25f, -0.25f,  0.25f, 1.0f,       1.0f, 0.0f, 1.0f, 1.0f, // Bottom left
 0.25f, -0.25f,  0.25f, 1.0f,       1.0f, 1.0f, 1.0f, 1.0f, // Top left

 0.25f, -0.25f, -0.25f, 1.0f,       1.0f, 1.0f, 1.0f, 1.0f, // Top right
-0.25f, -0.25f, -0.25f, 1.0f,       1.0f, 0.0f, 1.0f, 1.0f, // Bottom right
-0.25f, -0.25f,  0.25f, 1.0f,       1.0f, 0.0f, 1.0f, 1.0f, // Bottom left
};
