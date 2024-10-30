#define GLFW_EXPOSE_NATIVE_WIN32
#include "memory.h"
#include "datatypes.h"
#include "bone.h"
#include "helper.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#define M_PI 3.14159265358979323846

// drawcircle funckia tavistvis
void drawCircle(float centerX, float centerY, float radius, int numSegments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(centerX + x, centerY + y);
    }
    glEnd();
}

void DrawHealthBar(const Vec2& topLeft, float boxHeight, int health, float distance) {
    //max health
    float maxHealth = 100.0f;

    //bar height gamoitvleba current health * yutis height ita / max health(100)
    float healthBarHeight = (boxHeight * health) / maxHealth;

    float baseWidth = 4.5f; // bar height roca axlos aris player
    float maxDistance = 20.0f; // max distance playersa da chems shoris
    float minWidth = 2.0; // tu shorsaa player es value bars 

    // itvlis  widths ditances sashualebit
    float healthBarWidth = baseWidth - ((distance / maxDistance) * (baseWidth - minWidth));
    healthBarWidth = fmax(minWidth, healthBarWidth); // minwidth roar gacdes player

    // bottom position health barisas itvlis
    Vec2 barTopLeft = topLeft;
    barTopLeft.X -= healthBarWidth + 2.0f; // cota marcxniv mivwiot bar

    // empty bar is daxatva
    glColor3f(2.0f, 0.5f, 0.5f); // yle feri empty stvis
    glBegin(GL_QUADS);
    glVertex2f(barTopLeft.X, barTopLeft.Y); // zemota marcxena
    glVertex2f(barTopLeft.X + healthBarWidth, barTopLeft.Y); //zemota marjvena
    glVertex2f(barTopLeft.X + healthBarWidth, barTopLeft.Y + boxHeight); // qvemota marjvena
    glVertex2f(barTopLeft.X, barTopLeft.Y + boxHeight); // qvemota marcxena
    glEnd();

    // sicocxlis bar ramdenicaa daxators
    glColor3f((health / 100.0f), health / 100.0f, 0.0f); // yle feri
    glBegin(GL_QUADS);
    glVertex2f(barTopLeft.X, barTopLeft.Y + (boxHeight - healthBarHeight)); // marcxena zevita
    glVertex2f(barTopLeft.X + healthBarWidth, barTopLeft.Y + (boxHeight - healthBarHeight)); // marjvena zevbita
    glVertex2f(barTopLeft.X + healthBarWidth, barTopLeft.Y + boxHeight); // qvemota marjvena
    glVertex2f(barTopLeft.X, barTopLeft.Y + boxHeight); // qvemota marcxena
    glEnd();
}


int main() {




    matrix viewmatrix;
    offsets offsets;
    processes memory;
    // glfw damontajebis ponti
    if (!glfwInit()) {
        return -1;
    }

    int windowWidth = 1920;
    int windowHeight = 1080;

    // transparent da click through window shekmna
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);  // window decoration gakroba (shesazloa movashoro)

    // memory managment wave managment pontshi
    uintptr_t processid = memory.GetProcessID(L"cs2.exe");
    if (!processid) {
        return NULL;
    }
    uintptr_t gamemod = memory.GetModuleBaseAddress(processid, L"client.dll");
    if (gamemod == 0) {
        return NULL;
    }

    if (gamemod) {
        std::cout << "client.dll found" << std::endl;
    }
    HANDLE hprocess = memory.process();
    Vec2 lineorigin;
    lineorigin.X = windowWidth / 2.0f;
    lineorigin.Y = windowHeight;

    // opengl window shekmna
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "esp nigga", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();

    // Get HWND for transparency settings
    HWND hwnd = glfwGetWin32Window(window);
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST); // Add WS_EX_TOPMOST

    // Set the clear color (transparent)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);



    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // screen clear kovel  iteraciaze
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2d rendering istvis
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST);

        // viewmatrix wakitxvva (armjera ro nagdia da mushaobs)
        viewmatrix = memory.ReadMemory<matrix>(hprocess, gamemod + offsets.dwViewMatrix);

        // enttiy list start da tvit entity listis wakitxva
        uintptr_t entitylist = memory.ReadMemory<uintptr_t>(hprocess, gamemod + offsets.dwEntityList);
        uintptr_t listentry = memory.ReadMemory<uintptr_t>(hprocess, entitylist + 0x10);




        // local player || team sirobebi

        uintptr_t localPlayer = memory.ReadMemory<uintptr_t>(hprocess, gamemod + offsets.dwLocalPlayerController);
        if (!localPlayer) return 0;


        uintptr_t localPlayerPawn = memory.ReadMemory<uintptr_t>(hprocess, localPlayer + offsets.m_hPlayerPawn);
        if (!localPlayerPawn) return 0;

        uintptr_t localList_entry2 = memory.ReadMemory<uintptr_t>(hprocess, entitylist + 0x8 * ((localPlayerPawn & 0x7FFF) >> 9) + 16);
        if (!localList_entry2) return 0;


        uintptr_t localpCSPlayerPawn = memory.ReadMemory<uintptr_t>(hprocess, localList_entry2 + 120 * (localPlayerPawn & 0x1FF));
        if (!localpCSPlayerPawn) return 0;

        int localTeam = memory.ReadMemory<int>(hprocess, localPlayer + offsets.teamnum);
        if (!localTeam) return 0;

        Vec2 localcalc = memory.ReadMemory<Vec2>(hprocess, localpCSPlayerPawn + offsets.m_vOldOrigin);

        Vec3 localOrigin = memory.ReadMemory<Vec3>(hprocess, localpCSPlayerPawn + offsets.m_vOldOrigin);
        //local sruldeba





        // iteracia bozur entitiebze
        for (int i = 0; i < 64; i++) {
            if (!listentry) continue;



            uintptr_t currentcontroller = memory.ReadMemory<uintptr_t>(hprocess, listentry + i * 0x78);
            if (!currentcontroller) continue;

            int pawnhandle = memory.ReadMemory<int>(hprocess, currentcontroller + offsets.m_hPlayerPawn);
            if (pawnhandle == 0) continue;

            uintptr_t listentry2 = memory.ReadMemory<uintptr_t>(hprocess, entitylist + 0x8 * ((pawnhandle & 0x7FFF) >> 9) + 0x10);

            if (!listentry2) continue;

            //entity pawn is datreva 
            uintptr_t currentpawn = memory.ReadMemory<uintptr_t>(hprocess, listentry2 + 0x78 * (pawnhandle & 0x1FF));

            if (!currentpawn) continue;


            //scenenode datreva
            uintptr_t scenenode = memory.ReadMemory<uintptr_t>(hprocess, currentpawn + offsets.m_pGameSceneNode);
            uintptr_t bonearray = memory.ReadMemory<uintptr_t>(hprocess, scenenode + offsets.m_modelState + 0x80); // 0x80 dwbonematrix offset aris






            // vcheckavt tu entity cocxalia
            int health = memory.ReadMemory<int>(hprocess, currentpawn + offsets.m_iHealth);            
            if (health <= 0 || health > 100) continue;


            //team vcheckavt da colorebs vanawilebt
            int team = memory.ReadMemory<int>(hprocess, currentpawn + offsets.teamnum);
            if (team == localTeam) continue;

            //entity position ebis gageba
            Vec3 playerlocation = memory.ReadMemory<Vec3>(hprocess, currentpawn + offsets.m_vOldOrigin);
            Vec2 playercalc = memory.ReadMemory<Vec2>(hprocess, localpCSPlayerPawn + offsets.m_vOldOrigin);

            //shemowmeba tu entity aris localplayer tu aris ki vajmeinebt
            if (playerlocation.X == localOrigin.X &&
                playerlocation.Y == localOrigin.Y &&
                playerlocation.Z == localOrigin.Z) continue;

            // Head position calculation
            const float BODY_HEIGHT = 65.0f;  // head postiion value
            Vec3 headPosition = playerlocation;
            headPosition.Z += BODY_HEIGHT;  // head postiion gamotvla





            uintptr_t boneaddress = bonearray + 6 * 32;
            Vec3 boneposition = memory.ReadMemory<Vec3>(hprocess, boneaddress);




            //w2s funkciebi
            Vec2 footScreenCoords, headScreenCoords, ScreenCords, headbone;
            if (!W2S(playerlocation, footScreenCoords, viewmatrix.Vmatrix)) continue;
            if (!W2S(headPosition, headScreenCoords, viewmatrix.Vmatrix)) continue;
            if (!W2S(headPosition, ScreenCords, viewmatrix.Vmatrix)) continue;
            if (!W2S(boneposition, headbone, viewmatrix.Vmatrix)) continue;



            float headheight = (footScreenCoords.Y - headbone.Y) / 9;





            // box ganzomilebis gamotvla
            float boxHeight = footScreenCoords.Y - headScreenCoords.Y;
            float boxWidth = boxHeight / 2.0f;

            // marcxena marjvena zemota cornerebis gamotvla
            Vec2 boxTopLeft = headScreenCoords;
            boxTopLeft.X -= boxWidth / 2.0f;

            //bone esp 
            glColor3f(3.0f, 5.0f, 2.0f);
            glLineWidth(3.0f);
            drawCircle(headbone.X, headbone.Y, headheight, 20);
            for (int i = 0; i < sizeof(boneconnections) / sizeof(boneconnections[0]); i++) {
                int bone1 = boneconnections[i].bone1;
                int bone2 = boneconnections[i].bone2;

                //positionebis gageba bone ebis
                Vec3 vectorbone1 = memory.ReadMemory<Vec3>(hprocess, bonearray + bone1 * 32);
                Vec3 vectorbone2 = memory.ReadMemory<Vec3>(hprocess, bonearray + bone2 * 32);

                //ekranze gamosaxavat
                Vec2 screenBone1, screenBone2;
                if (!W2S(vectorbone1, screenBone1, viewmatrix.Vmatrix) ||
                    !W2S(vectorbone2, screenBone2, viewmatrix.Vmatrix)) {
                    continue;
                }
                //daxatva
                glBegin(GL_LINES);
                glVertex2f(screenBone1.X, screenBone1.Y);
                glVertex2f(screenBone2.X, screenBone2.Y);

                glEnd();

            }


            glLineWidth(2.0f);

            //health bar daxatva
            float distance = CalculateDistance(localcalc, playercalc);
            DrawHealthBar(boxTopLeft, boxHeight,health, distance);

            //line ebis daxatva entitebze
            glColor3f(0.0f, 0.0f, 0.0f);
            glBegin(GL_LINES);
            glVertex2f(lineorigin.X, lineorigin.Y);
            glVertex2f(headbone.X, headbone.Y);
            glEnd();
        }

        // Swap buffers
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}