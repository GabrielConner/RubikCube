#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glm/common.hpp"
#include "glm/ext.hpp"

#include <cstdlib>
#include <fstream>
#include <chrono>
#include <map>
#include <stack>
#include <deque>
#include <algorithm>
#include <filesystem>
#include <queue>


#include "pPack/vector.h"
#include "pPack/shaderHandling.h"
#include "pPack/animateValue.h"

#include "rubikCube.hpp"
#include "helperClasses.hpp"
#include "cornerCube.hpp"

using namespace ::pPack;
using namespace ::pPack::vector;
using namespace ::pPack::shader_handling;
using namespace ::pPack::animate_value;


void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfwWindowResizedCallback(GLFWwindow* window, int width, int height);
void fillRotatingBuffer(bool* buffer, RubikActionSingle action);
void setVAO();
void fillVerticesBuffer(Vertex* vertices);
void fillRubikColorBuffer(Vector4* colors, CornerCube* cube);


namespace {
int screenWidth = 1280;
int screenHeight = 720;
float screenAspect = 1280.0f / 720.0f;

bool keyPressed[349];
bool keyHeld[349];
bool keyReleased[349];

glm::mat4 projection;

Vector3 cubePosition(0);
Vector3 cubeRotation(0);

float positionSpeedX = 0.f;
float positionSpeedY = 0.f;
float positionSpeedZ = 0.f;
float positionX = 0.f;
float positionY = 0.f;
float positionZ = 0.f;

float rotationSpeedX = 0.f;
float rotationSpeedY = 0.f;
float rotationX = 0.f;
float rotationY = 0.f;



constexpr float rotationAcceleration = 60.f;
constexpr float rotationDeceleration = 0.98f;
constexpr float rotationMin = 0.1f;
constexpr float rotationMax = 360.f;

constexpr float positionAcceleration = 3.f;
constexpr float positionDeceleration = 0.8f;
constexpr float positionMin = 0.01f;
constexpr float positionMax = 12.f;


constexpr float cubieOffset = 1.02f;
constexpr float cubieSize = 1.f;
constexpr float cubieGap = cubieOffset - cubieSize;


constexpr unsigned int rubikSize = 2;
constexpr unsigned int rubikRE = rubikSize - 1;
constexpr unsigned int rubikCE = (rubikSize - 1) * rubikSize;
constexpr unsigned int rubikAE = (rubikSize  * rubikSize) - 1;
constexpr bool rubikHasHalf = (rubikSize % 2);
constexpr unsigned int rubikHalf = rubikSize / 2;

constexpr float offsetHalf = cubieOffset / 2.f;
constexpr float rubikHalfShift = ((float)rubikSize / 2.f) - 0.5f;
constexpr unsigned int rubikFace = rubikSize * rubikSize;
constexpr unsigned int rubikCube = rubikSize * rubikSize * 6;

constexpr float cubieHalfSize = cubieSize / 2;
constexpr float rubikHalfShiftOffset = rubikHalfShift * cubieOffset;
constexpr float rubikHalfZFightOffset = ((float)rubikSize / 2) * cubieOffset - 0.05f;
constexpr float rubikHalfOffset = (float)rubikSize / 2 * cubieOffset;


constexpr unsigned int randomSteps = 7;

} // namespace



void GenerateHeuristicDatabases();


int main() {
  GenerateHeuristicDatabases();

  return 1;

  //std::string cDir;
  //std::string cPath;

  //{
  //  std::stringstream dayTimeSS;
  //  dayTimeSS << std::chrono::system_clock::now();
  //  cPath = dayTimeSS.str();
  //  cPath.erase(std::remove(cPath.begin(), cPath.end(), ' '), cPath.end());
  //  std::replace(cPath.begin(), cPath.end(), ':', '-');
  //  std::replace(cPath.begin(), cPath.end(), '.', '-');
  //  cDir = std::format("./SolvedCubes/{:}x{:}Cubes/", rubikSize, rubikSize);
  //  cPath = std::format("{:}{:}steps--{:}.txt", cDir, randomSteps, cPath);
  //}


  CornerCube rubik;
  //rubik.Rotate(4, 0, 0);
  //rubik.Rotate(1, 0, 0);
  //rubik.Rotate(4, 0, 0);
  //rubik.Rotate(1, 0, 0);
  //rubik.Rotate(4, 0, 0);
  //rubik.Rotate(1, 0, 0);
  //rubik.Rotate(4, 0, 0);
  

  //rubik.Rotate(4, 0, 0);
  //rubik.Rotate(3, 0, 0);
  //rubik.Rotate(2, 0, 0);
  //rubik.Rotate(5, 0, 0);
  //rubik.Rotate(1, 0, 0);
  //rubik.Rotate(0, 0, 0);
  //rubik.Rotate(1, 1, 0);
  //rubik.Rotate(5, 2, 0);

  std::queue<RubikActionSingle> randomQueue;
  std::deque<RubikActionSingle> actionList;
  std::stack<RubikActionSingle> actionHistoryList;

  {
    std::vector<RubikActionSingle> randomList = {
      //{4, 0, 0},
      //{1, 0, 0},
      //{4, 0, 0},
      //{1, 0, 0},
      //{4, 0, 0},
      //{1, 0, 0},
      //{4, 0, 0},

      //{4, 0, 0},
      //{3, 0, 0},
      //{2, 0, 0},
      //{5, 0, 0},
      //{1, 0, 0},
      //{0, 0, 0},
      //{1, 1, 0},
      //{5, 2, 0},
    };
    //actionList = rubik.Solve();
    actionList = {
      {4, 0, 0},
      {4, 1, 0},
      {4, 2, 0},
      {4, 2, 0},

      //{4, 0, 0},
      //{4, 1, 0},
      //{4, 2, 0},

      {1, 0, 0},
      {1, 1, 0},
      {1, 2, 0},
      {1, 2, 0},

      //{1, 0, 0},
      //{1, 1, 0},
      //{1, 2, 0},

      {3, 0, 0},
      {3, 1, 0},
      {3, 2, 0},
      {3, 2, 0},

      //{3, 0, 0},
      //{3, 1, 0},
      //{3, 2, 0},

      {5, 0, 0},
      {5, 1, 0},
      {5, 2, 0},
      {5, 2, 0},

      //{5, 0, 0},
      //{5, 1, 0},
      //{5, 2, 0},

      {2, 0, 0},
      {2, 1, 0},
      {2, 2, 0},
      {2, 2, 0},

      //{2, 0, 0},
      //{2, 1, 0},
      //{2, 2, 0},

      {0, 0, 0},
      {0, 1, 0},
      {0, 2, 0},
      {0, 2, 0},

      //{0, 0, 0},
      //{0, 1, 0},
      //{0, 2, 0},
    };


    //std::filesystem::create_directories(cDir);
    //std::ofstream sFile(cPath, std::ios::out | std::ios::trunc);

    //sFile << "Randomized steps : \n\n";
    //for (const auto& randStep : randomList) {
      //sFile << randStep << '\n';
    //  randomQueue.push(randStep);
    //}

    //sFile << "\n\n\nSteps to solve : \n\n";
    //while (!actionList.empty()) {
      //sFile << actionList.front() << '\n';
    //  actionHistoryList.push(actionList.front());
    //  actionList.pop_front();
    //}

    //while (!actionHistoryList.empty()) {
    //  actionList.push_front(actionHistoryList.top());
    //  actionHistoryList.pop();
    //}

    //sFile.flush();
    //sFile.close();

    //rubik.Reset();
  }



  Vector4* rubikColors = nullptr;

  ShaderHandler shader = 0;

  Vertex* vertices = nullptr;
  Vertex square = Vertex(Vector3(), -1, 6);
  Vertex* planes = nullptr;

  GLFWwindow* window = nullptr;

  glm::mat4 identity(1);
  glm::mat4 transform(1);
  glm::mat4 cubeTransform(1);

  glm::vec3 rotateAxis(0);

  AnimatedValue* animatedValue = nullptr;

  RubikActionSingle curAction;



  unsigned int VAO, VBO, SSBO, squareVBO, planesVBO;

  int planeIndex = 0;
  int rotatePlane = 0;
  int vertIndex = 0;
  int faceIndex = 0;

  float animationTime = 0.f;
  float deltaTime = 0.f;
  float curRotation = 0.f;
  float rubikTurnTime = 0.2f;

  bool *rotatingBuffer = nullptr;
  bool rotatingAll = false;
  bool reverseAnimation = false;
  bool randomized = true;

  const char* vertLocs[] = {"./shaders/shader.vert"};
  const char* geomLocs[] = {"./shaders/shader.geom"};
  const char* fragLocs[] = {"./shaders/shader.frag"};
  ShaderCreateInfo shaderCreateInfo[3] = {ShaderCreateInfo(vertLocs, 1, GL_VERTEX_SHADER), ShaderCreateInfo(geomLocs, 1, GL_GEOMETRY_SHADER), ShaderCreateInfo(fragLocs, 1, GL_FRAGMENT_SHADER)};







  if (!glfwInit())
    return 1;


  window = glfwCreateWindow(screenWidth, screenHeight, "RubrikCube", 0, 0);
  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, glfwKeyCallback);
  glfwSetWindowSizeCallback(window, glfwWindowResizedCallback);

  if (!gladLoadGL())
    return 1;
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);


  shader = ShaderHandler::CreateShader("Shader", shaderCreateInfo, 3);
  shader.Active();




  rubikColors = new Vector4[rubikCube];
  fillRubikColorBuffer(rubikColors, &rubik);


  rotatingBuffer = new bool[rubikCube] {0};


  planes = new Vertex[rubikRE * 3];
  for (int x = 0; x < rubikRE; x++) {
    planes[planeIndex] = Vertex(Vector3(-rubikHalfShiftOffset + cubieHalfSize - rubikHalfOffset + (x * cubieOffset), 0, 0), -1, 3);
    planeIndex++;
  }
  for (int y = 0; y < rubikRE; y++) {
    planes[planeIndex] = Vertex(Vector3(0, -rubikHalfShiftOffset + cubieHalfSize - rubikHalfOffset + (y * cubieOffset), 0), -1, 5);
    planeIndex++;
  }
  for (int z = 0; z < rubikRE; z++) {
    planes[planeIndex] = Vertex(Vector3(0, 0, -rubikHalfShiftOffset + cubieHalfSize - rubikHalfOffset + (z * cubieOffset)), -1, 4);
    planeIndex++;
  }

  glCreateBuffers(1, &planesVBO);
  glNamedBufferData(planesVBO, (size_t)rubikRE * 3 * sizeof(Vertex), planes, GL_STATIC_DRAW);
  delete[](planes);
  planes = nullptr;


  glCreateBuffers(1, &squareVBO);
  glNamedBufferData(squareVBO, sizeof(Vertex), &square, GL_STATIC_DRAW);




  vertices = new Vertex[rubikCube];
  fillVerticesBuffer(vertices);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, rubikCube * sizeof(Vertex), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &SSBO);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, rubikCube * sizeof(Vector4), rubikColors, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);



  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  delete[](vertices);
  vertices = nullptr;




  projection = glm::perspective(glm::radians(75.f), screenAspect, 0.001f, 5000.0f);

  positionZ = -rubikHalfShiftOffset - cubieHalfSize - 5;

  shader.SetMat4("projection", glm::value_ptr(projection));
  shader.SetMat4("transform", glm::value_ptr(identity));
  shader.SetMat4("cubeTransform", glm::value_ptr(cubeTransform));
  shader.SetFloat("cubieHalfSize", cubieHalfSize);





  glClearColor(0.8, 0.8, 0.8, 1.0);
  std::chrono::time_point prevTime = std::chrono::steady_clock::now();
  while (!glfwWindowShouldClose(window)) {
    std::chrono::time_point cur = std::chrono::steady_clock::now();
    deltaTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(cur - prevTime).count() / 1000.f;
    prevTime = std::chrono::steady_clock::now();


    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.SetMat4("projection", glm::value_ptr(projection));



    // update section
    // -------------

    if (abs(positionSpeedX) < positionMax) {
      if (keyHeld[GLFW_KEY_A])
        positionSpeedX += positionAcceleration * deltaTime;
      else if (keyHeld[GLFW_KEY_D])
        positionSpeedX -= positionAcceleration * deltaTime;
      else
        positionSpeedX *= positionDeceleration * (1.f - deltaTime);
    } else if (abs(positionSpeedX) > positionMin)
      positionSpeedX *= positionDeceleration * (1.f - deltaTime);
    else
      positionSpeedX = 0;

    if (abs(positionSpeedY) < positionMax) {
      if (keyHeld[GLFW_KEY_SPACE])
        positionSpeedY += positionAcceleration * deltaTime;
      else if (keyHeld[GLFW_KEY_LEFT_CONTROL])
        positionSpeedY -= positionAcceleration * deltaTime;
      else if (abs(positionSpeedY) > positionMin)
        positionSpeedY *= positionDeceleration * (1.f - deltaTime);

    } else if (abs(positionSpeedY) > positionMin)
      positionSpeedY *= positionDeceleration * (1.f - deltaTime);
    else
      positionSpeedY = 0;

    if (abs(positionSpeedZ) < positionMax) {
      if (keyHeld[GLFW_KEY_W])
        positionSpeedZ += positionAcceleration * deltaTime;
      else if (keyHeld[GLFW_KEY_S])
        positionSpeedZ -= positionAcceleration * deltaTime;
      else if (abs(positionSpeedZ) > positionMin)
        positionSpeedZ *= positionDeceleration * (1.f - deltaTime);

    } else if (abs(positionSpeedZ) > positionMin)
      positionSpeedZ *= positionDeceleration * (1.f - deltaTime);
    else
      positionSpeedZ = 0;


    if (abs(rotationSpeedX) < rotationMax) {
      if (keyHeld[GLFW_KEY_UP])
        rotationSpeedX += rotationAcceleration * deltaTime;
      else if (keyHeld[GLFW_KEY_DOWN])
        rotationSpeedX -= rotationAcceleration * deltaTime;
      else if (abs(rotationSpeedX) > rotationMin)
        rotationSpeedX *= rotationDeceleration * (1.f - deltaTime);

    } else if (abs(rotationSpeedX) > rotationMin)
      rotationSpeedX *= rotationDeceleration * (1.f - deltaTime);
    else
      rotationSpeedX = 0;


    if (abs(rotationSpeedY) < rotationMax) {
      if (keyHeld[GLFW_KEY_RIGHT])
        rotationSpeedY += rotationAcceleration * deltaTime;
      else if (keyHeld[GLFW_KEY_LEFT])
        rotationSpeedY -= rotationAcceleration * deltaTime;
      else if (abs(rotationSpeedY) > rotationMin)
        rotationSpeedY *= rotationDeceleration * (1.f - deltaTime);

    } else if (abs(rotationSpeedY) > rotationMin)
      rotationSpeedY *= rotationDeceleration * (1.f - deltaTime);
    else
      rotationSpeedY = 0;


    positionX -= positionSpeedX * deltaTime;
    positionY += positionSpeedY * deltaTime;
    positionZ -= positionSpeedZ * deltaTime;

    rotationX -= rotationSpeedX * deltaTime;
    rotationY += rotationSpeedY * deltaTime;

    rotationX = fmodf(rotationX, 360.f);
    rotationY = fmodf(rotationY, 360.f);

    cubeTransform = glm::mat4(1);

    cubeTransform = glm::translate(cubeTransform, glm::vec3(positionX, positionY, positionZ));

    cubeTransform = glm::rotate(cubeTransform, glm::radians(rotationY), glm::vec3(0, 1, 0));
    cubeTransform = glm::rotate(cubeTransform, glm::radians(rotationX), glm::vec3(1, 0, 0));

    shader.SetMat4("cubeTransform", glm::value_ptr(cubeTransform));


    // Continuous running
    if (keyPressed[GLFW_KEY_ENTER]) {
      if (randomized)
        std::cout << "Animation state is now : " << (rotatingAll ? "continuous\n\n" : "single\n\n");
      rotatingAll = !rotatingAll;
    }
    if (keyPressed[GLFW_KEY_BACKSLASH]) {
      reverseAnimation = !reverseAnimation;
      std::cout << "Animation list is now : " << (reverseAnimation ? "reverse list\n\n" : "action list\n\n");
    }

    if (keyPressed[GLFW_KEY_O] && rubikTurnTime > 0.f) {
      rubikTurnTime -= 0.05f;
      if (rubikTurnTime < 0.05f)
        rubikTurnTime = 0.f;
      std::cout << "Rubik turn time is now : " << rubikTurnTime << "\n\n";
    }
    if (keyPressed[GLFW_KEY_P]) {
      rubikTurnTime += 0.05f;
      std::cout << "Rubik turn time is now : " << rubikTurnTime << "\n\n";
    }

    // Start animation
    if ((keyPressed[GLFW_KEY_RIGHT_SHIFT] || rotatingAll) && animatedValue == nullptr) {
      if (randomized && ((!reverseAnimation && actionList.empty()) || (reverseAnimation && actionHistoryList.empty()))) {
        std::cout << "Nothing left in : " << (reverseAnimation ? "reverse list\n" : "action list\n");
        reverseAnimation = !reverseAnimation;
        std::cout << "Action list is now : " << (reverseAnimation ? "reverse list\n" : "action list\n");
        if (rotatingAll)
          std::cout << "Animation state is now : single\n\n";
        rotatingAll = false;
      } else {

        // Do animation

        if (randomized) {
          if (!reverseAnimation) {
            curAction = actionList.front();

            actionHistoryList.push({curAction.face, curAction.ReversedDirection(), curAction.layer});
            actionList.pop_front();
          } else {
            curAction = actionHistoryList.top();
            actionList.push_front({curAction.face, curAction.ReversedDirection(), curAction.layer});
            actionHistoryList.pop();
          }
        } else {
          curAction = randomQueue.front();
          randomQueue.pop();
          if (randomQueue.empty()) {
            rotatingAll = false;
            randomized = true;
            std::cout << "Randomized\n\n";
          }
        }

        fillRotatingBuffer(rotatingBuffer, curAction);
        rubik.Rotate(curAction.face, curAction.direction/*, curAction.layer*/);

        if (curAction.face == 0) {
          rotateAxis = glm::vec3(0, 0, -1);
          rotatePlane = 2 * rubikRE + curAction.layer;

        } else if (curAction.face == 4) {
          rotateAxis = glm::vec3(0, 0, 1);
          rotatePlane = 3 * rubikRE - 1 - curAction.layer;

        } else if (curAction.face == 1) {
          rotateAxis = glm::vec3(-1, 0, 0);
          rotatePlane = curAction.layer;

        } else if (curAction.face == 3) {
          rotateAxis = glm::vec3(1, 0, 0);
          rotatePlane = rubikRE - 1 - curAction.layer;

        } else if (curAction.face == 5) {
          rotateAxis = glm::vec3(0, 1, 0);
          rotatePlane = 2 * rubikRE - 1 - curAction.layer;

        } else {
          rotateAxis = glm::vec3(0, -1, 0);
          rotatePlane = rubikRE + curAction.layer;
        }


        float fAngle = 0;
        if (curAction.direction == 0)
          fAngle = -90.f;
        else if (curAction.direction == 1)
          fAngle = 90.f;
        else
          fAngle = 180.f;

        animationTime = getAnimationTime();

        animatedValue = new AnimatedValue(
          AnimatedPtrValueType(&curRotation),
          AnimatedValueType(0.f),
          AnimatedValueType(fAngle),
          animationTime,
          animationTime + rubikTurnTime,
          0,
          ANIMATE_VALUE_TYPE_FLOAT
        );

        addAnimation(CreateAnimationInfo(animatedValue, false));
      }
    }


    // Update animation
    if (animatedValue != nullptr) {
      animateValueUpdate(deltaTime);
      transform = glm::mat4(1);
      if (animatedValue->completed) {
        delete(animatedValue);
        animatedValue = nullptr;

        fillRubikColorBuffer(rubikColors, &rubik);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, rubikCube * sizeof(Vector4), rubikColors);
      } else
        transform = glm::rotate(transform, glm::radians(curRotation), rotateAxis);
    }



    // -------------

    shader.SetFloat("cubieHalfSize", cubieHalfSize);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    setVAO();


    unsigned int offset = 0;
    unsigned int count = 0;
    bool rotating = false;
    for (unsigned int i = 0; i < rubikCube; i++) {
      if (rotatingBuffer[i]) {
        if (!rotating && count > 0) {
          shader.SetMat4("transform", glm::value_ptr(identity));
          glDrawArrays(GL_POINTS, offset, count);
          offset += count;
          count = 0;
        }
        count++;
        rotating = true;
      } else {
        if (rotating && count > 0) {
          shader.SetMat4("transform", glm::value_ptr(transform));
          glDrawArrays(GL_POINTS, offset, count);
          offset += count;
          count = 0;
        }
        count++;
        rotating = false;
      }
    }


    if (rotating && count > 0) {
      shader.SetMat4("transform", glm::value_ptr(transform));
      glDrawArrays(GL_POINTS, offset, count);
    } else if (count > 0) {
      shader.SetMat4("transform", glm::value_ptr(identity));
      glDrawArrays(GL_POINTS, offset, count);
    }


    if (animatedValue != nullptr) {
      shader.SetFloat("cubieHalfSize", rubikHalfOffset);

      glBindBuffer(GL_ARRAY_BUFFER, planesVBO);
      setVAO();
      shader.SetMat4("transform", glm::value_ptr(transform));
      glDrawArrays(GL_POINTS, rotatePlane, 1);
      shader.SetMat4("transform", glm::value_ptr(identity));
      glDrawArrays(GL_POINTS, rotatePlane, 1);
    }


    for (int i = 0; i < 349; i++) {
      keyPressed[i] = false;
      keyReleased[i] = false;
    }


    glfwSwapBuffers(window);
  }

  if (animatedValue != nullptr)
    delete(animatedValue);



  delete[](rotatingBuffer);
  delete[](rubikColors);

  glfwDestroyWindow(window);
  glfwTerminate();
}



void setVAO() {
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)(sizeof(Vector3)));
  glVertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)(sizeof(Vector3) + sizeof(unsigned int)));
}

void fillRotatingBuffer(bool* buffer, RubikActionSingle action) {
  if (!buffer)
    return;

  bool* back = buffer;
  bool* left = buffer + rubikFace;
  bool* down = left + rubikFace;
  bool* right = down + rubikFace;
  bool* front = right + rubikFace;
  bool* up = front + rubikFace;


  for (unsigned int i = 0; i < 6 * rubikFace; i++) {
    buffer[i] = false;
  }


  // Back
  if (action.face == 0) {
    if (action.layer == 0) {
      for (unsigned int i = 0; i < rubikFace; i++)
        back[i] = true;
    }
    for (unsigned int i = 0; i < rubikSize; i++) {
      up[i + (action.layer * rubikSize)] = true;
      left[(i * rubikSize) + action.layer] = true;
      right[(i * rubikSize) + rubikRE - action.layer] = true;
      down[rubikCE + i - (action.layer * rubikSize)] = true;
    }


    // Left
  } else if (action.face == 1) {
    if (action.layer == 0) {
      for (unsigned int i = 0; i < rubikFace; i++)
        left[i] = true;
    }
    for (unsigned int i = 0; i < rubikSize; i++) {
      up[(i * rubikSize) + action.layer] = true;
      back[(i * rubikSize) + action.layer] = true;
      front[(i * rubikSize) + action.layer] = true;
      down[(i * rubikSize) + action.layer] = true;
    }


    // Down
  } else if (action.face == 2) {
    if (action.layer == 0) {
      for (unsigned int i = 0; i < rubikFace; i++)
        down[i] = true;
    }
    for (unsigned int i = 0; i < rubikSize; i++) {
      front[rubikCE + i - (action.layer * rubikSize)] = true;
      right[rubikCE + i - (action.layer * rubikSize)] = true;
      left[rubikCE + i - (action.layer * rubikSize)] = true;
      back[rubikCE + i - (action.layer * rubikSize)] = true;
    }


    // Right
  } else if (action.face == 3) {
    if (action.layer == 0) {
      for (unsigned int i = 0; i < rubikFace; i++)
        right[i] = true;
    }
    for (unsigned int i = 0; i < rubikSize; i++) {
      front[(i * rubikSize) + rubikRE - action.layer] = true;
      up[(i * rubikSize) + rubikRE - action.layer] = true;
      back[(i * rubikSize) + rubikRE - action.layer] = true;
      down[(i * rubikSize) + rubikRE - action.layer] = true;
    }


    // Front
  } else if (action.face == 4) {
    if (action.layer == 0) {
      for (unsigned int i = 0; i < rubikFace; i++)
        front[i] = true;
    }
    for (unsigned int i = 0; i < rubikSize; i++) {
      up[rubikCE + i - (action.layer * rubikSize)] = true;
      right[(i * rubikSize) + action.layer] = true;
      down[i + (action.layer * rubikSize)] = true;
      left[rubikRE + (i * rubikSize) - action.layer] = true;
    }


    // Up
  } else if (action.face == 5) {
    if (action.layer == 0) {
      for (unsigned int i = 0; i < rubikFace; i++)
        up[i] = true;
    }
    for (unsigned int i = 0; i < rubikSize; i++) {
      back[i + (action.layer * rubikSize)] = true;
      right[i + (action.layer * rubikSize)] = true;
      front[i + (action.layer * rubikSize)] = true;
      left[i + (action.layer * rubikSize)] = true;
    }
  }
}


void fillRubikColorBuffer(Vector4* colors, CornerCube* cube) {
  if (!colors || !cube)
    return;

  for (int i = 0; i < rubikCube; i++)
    switch (cube->cubiePlanes[i]) {
    case 0:
      colors[i] = Vector4(1, 1, 0, 1);
      break;
    case 1:
      colors[i] = Vector4(1, 0, 0, 1);
      break;
    case 2:
      colors[i] = Vector4(0, 0, 1, 1);
      break;
    case 3:
      colors[i] = Vector4(1, 0.4, 0, 1);
      break;
    case 4:
      colors[i] = Vector4(1, 1, 1, 1);
      break;
    case 5:
      colors[i] = Vector4(0, 1, 0, 1);
      break;
    }
}


void fillVerticesBuffer(Vertex* vertices) {
  if (!vertices)
    return;

  size_t vertIndex = 0;

  // Back
  for (int y = 0; y < rubikSize; y++) {
    for (int x = rubikSize - 1; x >= 0; x--) {
      vertices[vertIndex] = Vertex(Vector3(rubikHalfShiftOffset - (x * cubieOffset), rubikHalfShiftOffset - (y * cubieOffset), -rubikHalfShiftOffset - cubieGap), vertIndex, 0);
      vertIndex++;
    }
  }

  // Left
  for (int y = 0; y < rubikSize; y++) {
    for (int x = 0; x < rubikSize; x++) {
      vertices[vertIndex] = Vertex(Vector3(-rubikHalfShiftOffset - cubieGap, rubikHalfShiftOffset - (y * cubieOffset), -rubikHalfShiftOffset + (x * cubieOffset)), vertIndex, 1);
      vertIndex++;
    }
  }

  // Down
  for (int y = 0; y < rubikSize; y++) {
    for (int x = 0; x < rubikSize; x++) {
      vertices[vertIndex] = Vertex(Vector3(-rubikHalfShiftOffset + (x * cubieOffset), -rubikHalfShiftOffset - cubieGap, rubikHalfShiftOffset - (y * cubieOffset)), vertIndex, 2);
      vertIndex++;
    }
  }

  // Right
  for (int y = 0; y < rubikSize; y++) {
    for (int x = 0; x < rubikSize; x++) {
      vertices[vertIndex] = Vertex(Vector3(rubikHalfShiftOffset + cubieGap, rubikHalfShiftOffset - (y * cubieOffset), rubikHalfShiftOffset - (x * cubieOffset)), vertIndex, 3);
      vertIndex++;
    }
  }

  // Front
  for (int y = 0; y < rubikSize; y++) {
    for (int x = 0; x < rubikSize; x++) {
      vertices[vertIndex] = Vertex(Vector3(-rubikHalfShiftOffset + (x * cubieOffset), rubikHalfShiftOffset - (y * cubieOffset), rubikHalfShiftOffset + cubieGap), vertIndex, 4);
      vertIndex++;
    }
  }

  // Up
  for (int y = rubikSize - 1; y >= 0; y--) {
    for (int x = 0; x < rubikSize; x++) {
      vertices[vertIndex] = Vertex(Vector3(-rubikHalfShiftOffset + (x * cubieOffset), rubikHalfShiftOffset + cubieGap, rubikHalfShiftOffset - (y * cubieOffset)), vertIndex, 5);
      vertIndex++;
    }
  }
}


void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    keyPressed[key] = true;
    keyHeld[key] = true;
    keyReleased[key] = false;
  } else if (action == GLFW_RELEASE) {
    keyPressed[key] = false;
    keyHeld[key] = false;
    keyReleased[key] = true;
  }
}


void glfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
  screenWidth = width;
  screenHeight = height;
  screenAspect = (float)width / height;
  glViewport(0, 0, width, height);

  projection = glm::perspective(glm::radians(75.f), screenAspect, 0.001f, 5000.0f);
}