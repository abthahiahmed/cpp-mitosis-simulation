#include <iostream>
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

using namespace std;
SDL_Window *win = SDL_CreateWindow("Mitosis", SDL_WINDOWPOS_CENTERED, 100, 400, 400, SDL_WINDOW_SHOWN);
SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
SDL_Event event;

int _mX, _mY;

float random(float low, float high){	
	return low + static_cast<float>(rand()) * static_cast<float>(high - low) / RAND_MAX;
}

struct Color{
	int r;
	int g;
	int b;
};
class Cell{
	
	public:
	float x, y, r;
	float tx, ty;
	float vx, vy;
	Color c;
	
	Cell(float x, float y, int r){
		
		this->x = x;
		this->y = y;
		this->r = r;
		
		this->tx = random(0, 400);
		this->ty = random(0, 400);
		
		this->vx = 0;
		this->vy = 0;
		
		this->c = {(int)random(150, 255), 0, (int)random(150, 255)};
		
		
	}
	
	void update(){
		float d = sqrt(pow(this->tx - this->x, 2) + pow(this->ty - this->y, 2));
		this->tx = random(0, 400);
		this->ty = random(0, 400);
		
//		cout<<"X:"<<this->tx<<" - Y:"<<this->ty<<endl;
		this->x += this->vx;
		this->y += this->vy;
		
		this->vx = 1 * ((this->tx - this->x) / d);
		this->vy = 1 * ((this->ty - this->y) / d);
		
	}
	
	bool clicked(){
		
		float dx = pow(this->x - _mX, 2);
		float dy = pow(this->y - _mY, 2);
		
		float dr = pow(this->r, 2);
		
		if (dx + dy <= dr && event.type == SDL_MOUSEBUTTONDOWN){
			
			return true;
			
		}
		return false;
		
	}
	
	auto getSubCell(){
		
		vector<Cell> cells;
		
		cells.push_back(Cell(this->x - this->r, this->y, this->r - 2));
		cells.push_back(Cell(this->x + this->r, this->y, this->r - 2));
	
		return cells;
		
	}
	
	void draw(){
		filledCircleRGBA(ren, this->x, this->y, this->r, this->c.r, this->c.g, this->c.b, 160);
	}
};

int main(int argc, char *argv[]){
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
	bool isRunning = true;
	
	vector<Cell> cells;
	
	cells.push_back(Cell(50, 50, 20));
	cells.push_back(Cell(200, 200, 30));
	
	while(isRunning){
		SDL_GetMouseState(&_mX, &_mY);
		SDL_PollEvent(&event);
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);
		for (int i = 0; i < (int)cells.size(); i++){
			
			if (cells[i].clicked()){
				
				cells.push_back(cells[i].getSubCell()[0]);
				cells.push_back(cells[i].getSubCell()[1]);
				cells.erase(cells.begin() + i);
		
			}
			cells[i].update();
			cells[i].draw();			
		}	
		SDL_RenderPresent(ren);
		SDL_Delay(1000 / 60); // FPS
		
		if (event.type == SDL_QUIT){
			isRunning = false;
		}
		cout<<(int)cells.size()<<endl;
	}
	
	
	return 0;
}

