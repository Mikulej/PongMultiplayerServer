#include "main.h"
constexpr int serverPort1 = 66671;
constexpr int serverPort2 = 66672;
constexpr char* serverIp = "127.0.0.1";
static bool clientReady1 = false, clientReady2 = false;
static int clientDirection1 = 5, clientDirection2 = 5; //8 = UP, 5 = NEUTRAL, 2 = DOWN
void EstableClientConnection(int clientID,std::shared_ptr<Socket> clientSocket){
    int serverPort = 0;
    if(clientID==1){
        serverPort = serverPort1;
    }
    else{
        serverPort = serverPort2;
    }
    clientSocket->Bind(serverIp);
    std::cout << "Listening for " << clientID << std::endl;
    clientSocket->Listen();
    clientSocket->Accept();
    std::cout << "Client " << clientID << " accepted" << std::endl;
    if(clientID==1){
        clientReady1 = true;
    }
    else{
        clientReady2 = true;
    }
    std::string receivedStr;
    while(true){
        receivedStr = clientSocket->Receive();
        receivedStr.c_str();
        clientDirection1 = receivedStr.c_str()[0] - '0';
        //std::cout << clientDirection1 << std::endl;
    }
    return;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
inline void processReceivedData(void);
inline void sendData(std::shared_ptr<Socket> clientSocket1,std::shared_ptr<Socket> clientSocket2);

// settings
int SCR_WIDTH = 1600;
int SCR_HEIGHT = 900;
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main()
{   
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    srand((unsigned)time(NULL));

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pong Multiplayer Server", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader("shader/vs.glsl", "shader/fs.glsl");
    glfwSwapInterval(1);
    ourShader.use(); 
    srand (static_cast <unsigned> (time(0)));
    Sprite::Initialize();
    Collider::Initialize();
    Collider::setRandomDirectionAt(0);

    //NETWORK
    std::shared_ptr<Socket> clientSocket1 = std::make_shared<Socket>(serverPort1);
    std::thread threadClient1(EstableClientConnection,1,clientSocket1);
    //std::thread threadClient2(EstableClientConnection,2);
    while(!clientReady1){

    }
    
    // while(!(clientReady1 && clientReady2)){
    //     //wait until both clients conntect to the server
    // }
    //Start a game, use threadClient1 and threadClient2 to receive input


    

    // Sprite::get(idplayer1).setScale(0.5,2.5);
    // int idplayer2 = Sprite::Add("box",-0.8f,0,0);
    // Sprite::get(idplayer2).setScale(0.5,2.5);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);
        processInput(window);
        processReceivedData();
        Collider::Collision();
        //sendData(clientSocket1,clientSocket1);

        // render
        // ------
       
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Sprite::RenderAll(ourShader);
        // render container


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    Sprite::DeleteTextures();
    threadClient1.join(); //stop main thread until t1 finishes its work
    //threadClient2.join();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
inline void processReceivedData(){
    switch (clientDirection1)
    {
        case 8:
        {
            Collider::setDirectionAt(0,1.0f,1);
            break;
        }
        case 2:
        {
            Collider::setDirectionAt(0,-1.0f,1);
            break;
        }
        default:{
            Collider::setDirectionAt(0.0f,0.0f,1);
            break;
        }
    }
    switch (clientDirection2)
    {
        case 8:
        {
            Collider::setDirectionAt(0,1.0f,2);
            break;
        }
        case 2:
        {
            Collider::setDirectionAt(0,-1.0f,2);
            break;
        }
        default:{
            Collider::setDirectionAt(0.0f,0.0f,2);
            break;
        }
    }
}
inline void sendData(std::shared_ptr<Socket> clientSocket1,std::shared_ptr<Socket> clientSocket2){
    while(true){
        clientSocket1->Send("10 10 10");
        //clientSocket2->Send("20 20 20");

    }
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // bool neutralInput = true;
    // if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    //     //send message
    //     s.Send("Q");
    // //     std::thread networkThread([&s](){
    // //         s.Send("Hej!");
    // //     });
    // }
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    //     Sprite::get(0).addPos(0,1.0f*deltaTime);
    //     neutralInput = false;
    //     s.Send("W");
    // }
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    //     Sprite::get(0).addPos(0,-1.0f*deltaTime);
    //     neutralInput = false;
    //     s.Send("S");
    // }
    // if(neutralInput){
    //     s.Send("N");
    // }
    // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    //     Sprite::get(1).addPos(0,-1.0f*deltaTime);
    // }
    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    //     Sprite::get(1).addPos(0,1.0f*deltaTime);
    // }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}