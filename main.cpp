#include <bits/stdc++.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 100;
using namespace std;
void logSDLError(ostream& os, const string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(cout, "SDL_Init", true);

    window = SDL_CreateWindow("BTL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	SDL_Texture *texture = nullptr;
	SDL_Surface *loadedImage = IMG_Load(file.c_str());
	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
//		if (texture == nullptr){
//			logSDLError(std::cout, "CreateTextureFromSurface");
//		}
	}
//	else {
//		logSDLError(std::cout, "LoadBMP");
//	}
	return texture;
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
    dst.w = w;
    dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}
void waitUntilKeyPressed();

void waitUntilKeyPressed(SDL_Renderer* renderer)
{
    SDL_Texture *img[21];
    for (int i = 1; i <= 9; i++) {
        char c = ('0' + i);
        string s = "0";
        s += c;
        s += ".png";
        img[i] = loadTexture(s, renderer);
    }
    for (int i = 0; i <= 9; i++) {
        char c = ('0' + i);
        string s = "1";
        s += c;
        s += ".png";
        img[10+i] = loadTexture(s, renderer);
    }
    int mat[10][10];
    int ok[10][10];
    int n = 6;
    SDL_Event e;
    SDL_Texture *meo= loadTexture("Meo.jpg", renderer);
    SDL_Texture *bg = loadTexture("bg0.jpg", renderer);
    SDL_Texture *win = loadTexture("win.png", renderer);
    SDL_Texture *bg1 = loadTexture("bg1.jpg", renderer);
    SDL_Texture *start = loadTexture("start1.png", renderer);
    renderTexture(bg, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    renderTexture(start, renderer, 150, 250, 270, 100);
    bool dx[100];
    vector < int > r;
    SDL_RenderPresent(renderer);
    while (true) {
        if (SDL_WaitEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                break;
            SDL_RenderClear(renderer);
            renderTexture(bg, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            renderTexture(start, renderer, 150, 250, 270, 100);
            SDL_RenderPresent(renderer);

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if(e.button.x >= 150 && e.button.x <= 320 && e.button.y >= 250 && e.button.y <= 350) {
                    int tt=0;
                    memset(ok, 0, sizeof(ok));
                    pair<int,int> pos[3];
                    memset(dx, 0, sizeof(dx));
                    srand(time(NULL));
                    r.clear();
                    while(r.size()<36) {
                        int gt = rand()%18+1;
                        if (!dx[gt]) {
                            dx[gt] = 1;
                            r.push_back(gt);
                            r.push_back(gt);
                        }
                    }
                    for(int i = 1;i <= 6; i++)
                        for (int j = 1; j <= 6; j++) {
                            int gt = rand() % r.size();
                            mat[i][j] = r[gt];
                            swap(r[gt], r[r.size()-1]);
                            r.pop_back();
                        }
                    for(int i = 1;i <= 6; i++) {
                        for (int j = 1; j <= 6; j++)
                            cout << mat[j][i] <<" ";
                        cout << endl;

                    }

                    while(1) {
                        SDL_RenderClear(renderer);
                        renderTexture(bg1, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                        int cnt = 0;
                        for (int i = 1; i <= 6; i++)
                            for (int j = 1; j <= 6; j++) {
                                int x = mat[i][j];
                                if (ok[i][j] == 0)
                                    renderTexture(meo, renderer, (i-1)*TILE_SIZE, (j-1)*TILE_SIZE, TILE_SIZE, TILE_SIZE);
                                if (ok[i][j] == 1)
                                    renderTexture(img[x], renderer, (i-1)*TILE_SIZE, (j-1)*TILE_SIZE, TILE_SIZE, TILE_SIZE);
                                if (ok[i][j] == 2)
                                    cnt++;
                            }
                        SDL_RenderPresent(renderer);
                        if (cnt == n*n) {
                            SDL_RenderClear(renderer);
                            renderTexture(win, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                            SDL_RenderPresent(renderer);
                            break;
                        }
                        if(tt==2) {
                            SDL_Delay(500);
                            tt = 0;
                            int X1=pos[1].first, Y1=pos[1].second, X2=pos[2].first, Y2=pos[2].second;
                            if (mat[X1][Y1] == mat[X2][Y2]) {
                                ok[X1][Y1] = 2;
                                ok[X2][Y2] = 2;
                            }
                            else {
                                ok[X1][Y1] = 0;
                                ok[X2][Y2] = 0;
                            }

                        }
                        if (SDL_WaitEvent(&e) != 0) {
                            if (e.type == SDL_MOUSEBUTTONDOWN) {
                                int X = e.button.x/TILE_SIZE+1;
                                int Y = e.button.y/TILE_SIZE+1;
                                if (ok[X][Y] == 2)
                                    continue;
                                tt++;
                                ok[X][Y] = 1;
                                pos[tt] = {X,Y};
                            }
                            if(e.type == SDL_QUIT)
                                return;
                        }
                    }
                }

            }

            if(e.type == SDL_QUIT)
                return;
        }

    }
}

int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);

    waitUntilKeyPressed(renderer);
    quitSDL(window, renderer);
    return 0;
}
